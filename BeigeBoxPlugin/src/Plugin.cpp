// Copyright (c) 2024 Walter Holley III. All Rights Reserved.

#include <fcntl.h>
#define XPLM200
#define XPLM210
#define XPLM300
#define XPLM301
#define XPLM302
#define XPLM303
#define MAX_MSGS 150
#ifdef IBM
#include <windows.h>
#endif

#include "include/DataUtil.h"
#include "include/Logger.h"
#include "include/MQClient.h"
#include "include/Recorder.h"
#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>
#include <XPLM/XPLMMenus.h>
#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMUtilities.h>
#include <thread>

Logger *_log;
DataUtil *_dataUtil;
Recorder *_recorder;
MQClient *_mq;
XPLMMenuID xplmMenuIdentifier;
XPLMCreateWindow_t pluginWindow;
int pluginSubMenuId;
const char *BASE_MENU_NAME = "BeigeBox";
const char *START_RECORDING = "Start Recording";
const char *STOP_RECORDING = "Stop Recording";
string TEST_DB = "NORTHWIND_AI";
bool record = false;
dataFrame flightData;
vector<bbmsg> consolemsgs = {};
pid_t clientPID;

static void menuCallback(void *inMenuRef, void *inItemRef);
static float pollData(float timeSinceLastCall, float timeSinceLastFlightLoop,
                      int count, void *refCon);
static void defineWindow();
static XPLMWindowID messageWindow;

// plugin window methods
void draw(XPLMWindowID in_windowId, void *refCon);
XPLMCursorStatus dumbCursorHandler(XPLMWindowID in_window_id, int x, int y,
                                   void *in_refcon) {
  return xplm_CursorDefault;
}
void dumbKeyHandler(XPLMWindowID in_windowId, char key, XPLMKeyFlags keyFlag,
                    char virtual_key, void *in_refcon, int losing_focus) {};
int dumbMWheelHandler(XPLMWindowID in_window_id, int x, int y, int wheel,
                      int clicks, void *in_refcon) {
  return 0;
}
int dumbMouseHandler(XPLMWindowID in_window_id, int x, int y,
                     XPLMMouseStatus in_status, void *in_refcon) {
  return 0;
}

void cleanup() {
  XPLMUnregisterFlightLoopCallback(pollData, NULL);
  XPLMDestroyWindow(messageWindow);
  if (_mq) {
    _mq->close();
  }

  free(_log);
  free(_mq);
}

void startClient() {
  char *xplPath;
  char *destination;
  sprintf(destination, "./Resources/plugins/BeigeBox/lin_x64/bbclient");
  XPLMGetSystemPath(xplPath);
#ifdef IBM
  // ShellExecute(NULL, "open", "F:\\X-Plane
  // 12\\Resources\\plugins\\BeigeBox\\win_x64\\bbclient.exe", NULL, NULL,
  // SW_SHOWDEFAULT);
  STARTUPINFO info = {sizeof(info)};
  PROCESS_INFORMATION processInformation;
  if (CreateProcess(
          NULL,
          "F:\\X-Plane 12\\Resources\\plugins\\BeigeBox\\win_x64\\bbclient.exe",
          NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInformation)) {
    WaitForSingleObject(processInformation.hProcess, INFINITE);
    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);
  }
#endif
#ifdef LIN
  // strcat(xplPath, "Resources/plugins/Beigebox/lin_x64/bbclient");
  // sprintf(destination, "%s", xplPath);
  char *args[] = {destination, NULL};
  _log->debug(args[0]);
  if (!clientPID) {
    clientPID = fork();

    if (clientPID == 0) {
      execv(args[0], args);
    }
  }
#endif
}
void start() {
  bbmsg msg;
  msg.msgType = INIT;
  strcat(msg.message, "Plugin started.  Hello!");
  _log->debug("Start selected from menu");
  if (_mq->init()) {
    // start client process
    thread t(startClient);
    t.detach();
    _log->info("Client detached");
    _mq->send(msg);
    messageWindow = XPLMCreateWindowEx(&pluginWindow);
    XPLMSetWindowPositioningMode(messageWindow, xplm_WindowPositionFree, -1);
    XPLMSetWindowResizingLimits(messageWindow, 200, 200, 500, 500);
    XPLMSetWindowGravity(messageWindow, 0, 1, 0, 1);
    XPLMSetWindowTitle(messageWindow, "BeigeBox Message Viewer");
    XPLMEnableMenuItem(xplmMenuIdentifier, 0, 0);
    XPLMEnableMenuItem(xplmMenuIdentifier, 1, 1);
    record = true;
  }
}

void stop() {

  _log->debug("Stop selected from menu");

  XPLMEnableMenuItem(xplmMenuIdentifier, 0, 1);
  XPLMEnableMenuItem(xplmMenuIdentifier, 1, 0);
  record = false;
}

float pollData(float timeSinceLastCall, float timeSinceLastFlightLoop,
               int count, void *refCon) {
  if (record) {
    vector<bbmsg> messages = _mq->receive();
    consolemsgs.insert(consolemsgs.end(), messages.begin(), messages.end());
    bbmsg confirm;
    confirm.msgType = MESSAGE;
    sprintf(confirm.message, "Queue has been checked for messages");
    _mq->send(confirm);
    if (consolemsgs.size() > MAX_MSGS) {
      int delta = consolemsgs.size() - MAX_MSGS;
      auto end = consolemsgs.begin() + delta - 1;
      consolemsgs.erase(consolemsgs.begin(), end);
    }
    draw(messageWindow, &refCon);
  }

  return 1.0;
}

//***** X-PLANE plugin methods *****//
PLUGIN_API int XPluginStart(char *name, char *sig, char *desc) {
  _log = new Logger();
  _mq = new MQClient(_log);
  int desktopScreenBounds[4];

  // Screen bounds
  XPLMGetScreenBoundsGlobal(&desktopScreenBounds[0], &desktopScreenBounds[2],
                            &desktopScreenBounds[1], &desktopScreenBounds[3]);
  // basic plugin information
  strcpy(name, "BeigeBox");
  strcpy(sig, "com.avidata.recorder");
  strcpy(desc, "Sim Flight Event Recorder for varied data");

  // message window defininition
  pluginWindow.structSize = sizeof(pluginWindow);
  pluginWindow.left = desktopScreenBounds[0] + 50;
  pluginWindow.right = desktopScreenBounds[0] + 350;
  pluginWindow.top = desktopScreenBounds[1] + 450;
  pluginWindow.bottom = desktopScreenBounds[1] + 50;
  pluginWindow.visible = 1;
  pluginWindow.drawWindowFunc = draw;
  pluginWindow.handleMouseClickFunc = dumbMouseHandler;
  pluginWindow.handleRightClickFunc = NULL;
  pluginWindow.handleMouseWheelFunc = dumbMWheelHandler;
  pluginWindow.handleKeyFunc = dumbKeyHandler;
  pluginWindow.handleCursorFunc = dumbCursorHandler;
  pluginWindow.refcon = NULL;
  pluginWindow.layer = xplm_WindowLayerFloatingWindows;
  pluginWindow.handleRightClickFunc = dumbMouseHandler;
  pluginWindow.decorateAsFloatingWindow = 1;

  // menu setup
  pluginSubMenuId =
      XPLMAppendMenuItem(XPLMFindPluginsMenu(), BASE_MENU_NAME, 0, 1);
  xplmMenuIdentifier = XPLMCreateMenu(BASE_MENU_NAME, XPLMFindPluginsMenu(),
                                      pluginSubMenuId, menuCallback, 0);
  XPLMAppendMenuItem(xplmMenuIdentifier, START_RECORDING, (void *)1, 1);
  XPLMAppendMenuItem(xplmMenuIdentifier, STOP_RECORDING, (void *)2, 1);
  XPLMEnableMenuItem(xplmMenuIdentifier, 1, 0);

  XPLMRegisterFlightLoopCallback(pollData, 1.0, NULL);

  return 1;
}

PLUGIN_API void XPluginStop(void) { cleanup(); }

PLUGIN_API int XPluginEnable(void) {
  int result = 0;

  if (_log && _mq) {
    _log->info("Beigebox: Plugin enabled");
    result = 1;
  }

  return result;
}

PLUGIN_API void XPluginDisable(void) {
  _log->info("BeigeBox: Plugin disabled");
}

PLUGIN_API void XPluginReceiveMessage(void) {}

void menuCallback(void *menuRef, void *itemRef) {
  _log->debug("Menu item selected");
  switch ((intptr_t)itemRef) {
  case 1:
    _log->info("MENU:  BBPlugin->Start Selected");
    // start recording
    start();
    break;
  case 2:
    // stop recording
    _log->info("MENU:  BBPlugin->Stop Selected");
    stop();
    break;
  default:
    _log->error("MENU: unknown menu item selected");
    break;
  }
}

void draw(XPLMWindowID in_windowId, void *in_refcon) {
  int charHeight;
  int left, right, top, bottom;
  XPLMSetGraphicsState(0, 0, 0, 0, 1, 1, 0);
  char textBuffer[288];
  // font and window settings
  XPLMGetFontDimensions(xplmFont_Proportional, NULL, &charHeight, NULL);
  XPLMGetWindowGeometry(in_windowId, &left, &top, &right, &bottom);
  std::string message_type_string[] = {"ERROR",
                                       "BUTTON1",
                                       "BUTTON2",
                                       "BUTTON3",
                                       "INIT",
                                       "RUN",
                                       "END",
                                       "FAIL",
                                       "RESET",
                                       "PING",
                                       "GETSIMSOCKET",
                                       "GETSERVERSOCKET",
                                       "MESSAGE",
                                       "BUFFERSIZEERROR",
                                       "BUFFERSTRINGERROR",
                                       "PLANNINGFAIL"};

  // colors
  float clr_white[] = {1.0, 1.0, 1.0};
  float clr_green[] = {0.08, 0.9, 0.05};
  float clr_red[] = {0.9, 0.16, 0.05};

  // draw messages
  {
    int y = bottom;

    for (int iter = consolemsgs.size() - 1; iter >= 0; iter--) {
      bbmsg msg = consolemsgs.at(iter);
      sprintf(textBuffer, "[%s]: %s", message_type_string[msg.msgType].c_str(),
              msg.message);
      switch (msg.msgType) {
      case ERROR:
        XPLMDrawString(clr_red, left, y, textBuffer, NULL,
                       xplmFont_Proportional);
        break;
      case BUTTON1:
      case BUTTON2:
      case BUTTON3:
      case INIT:
      case RUN:
      case END:
        XPLMDrawString(clr_green, left, y, textBuffer, NULL,
                       xplmFont_Proportional);
        break;
      case FAIL:
        XPLMDrawString(clr_red, left, y, textBuffer, NULL,
                       xplmFont_Proportional);
        break;
      case RESET:
      case PING:
      case GETSIMSOCKET:
      case GETSERVERSOCKET:
      case MESSAGE:
        XPLMDrawString(clr_white, left, y, textBuffer, NULL,
                       xplmFont_Proportional);
        break;
      case BUFFERSIZEERROR:
      case BUFFERSTRINGERROR:
      case PLANNINGFAIL:
        XPLMDrawString(clr_red, left, y, textBuffer, NULL,
                       xplmFont_Proportional);
      default:
        break;
        // log and ignore
      };

      y += 1.5 * charHeight;
    }
  }
}

// Windows DLL boilerplate
#ifdef IBM

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
  DWORD ul_reason_for_call;
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

#endif
