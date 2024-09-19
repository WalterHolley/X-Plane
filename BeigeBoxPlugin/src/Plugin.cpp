//
// Created by Zero on 6/25/2023.
//


#define XPLM200
#define XPLM210
#define XPLM300
#define XPLM400

#include<XPLM/XPLMProcessing.h>
#include<XPLM/XPLMMenus.h>
#include "include/DataProcessor.h"
#include "include/Logger.h"



Logger* _log;
DataProcessor* _dataProcessor;
XPLMMenuID xplmMenuIdentifier;
int pluginSubMenuId;
const char* BASE_MENU_NAME = "BeigeBox";
const char* START_RECORDING = "Start Recording";
const char* STOP_RECORDING = "Stop Recording";

static void menuCallback(void* inMenuRef, void* inItemRef);

void cleanup()
{
    free(_log);
    free(_dataProcessor);
}

void start()
{

    if(_dataProcessor->hasInited())
    {
        _log->debug("Start selected from menu");
        _dataProcessor->start();
        XPLMEnableMenuItem(xplmMenuIdentifier,1, 0);
        XPLMEnableMenuItem(xplmMenuIdentifier, 2, 1);
    }

}

void stop()
{
    if(_dataProcessor->hasInited())
    {
        _log->debug("Stop selected from menu");
        _dataProcessor->stop();
        XPLMEnableMenuItem(xplmMenuIdentifier,1, 1);
        XPLMEnableMenuItem(xplmMenuIdentifier,2, 0);
    }
}





//***** X-PLANE plugin methods *****//
PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
    _log = new Logger();
    _dataProcessor = new DataProcessor(_log);
    _dataProcessor->init();
    //basic plugin information
    strcpy(name, "BeigeBox");
    strcpy(sig, "com.avidata.recorder");
    strcpy(desc, "Sim Flight Event Recorder for varied data");

    //menu setup
    pluginSubMenuId = XPLMAppendMenuItem( XPLMFindPluginsMenu(), BASE_MENU_NAME, 0, 1);
    xplmMenuIdentifier = XPLMCreateMenu(BASE_MENU_NAME, XPLMFindPluginsMenu(), pluginSubMenuId, menuCallback, 0);
    XPLMAppendMenuItem(xplmMenuIdentifier, START_RECORDING, (void*) 1,1);
    XPLMAppendMenuItem(xplmMenuIdentifier, STOP_RECORDING, (void*) 2, 1);
    XPLMEnableMenuItem(xplmMenuIdentifier, 1, 0);

    return 1;
}

PLUGIN_API void XPluginStop(void)
{
    cleanup();
}

PLUGIN_API int XPluginEnable(void)
{
    int result = 0;

        if(_log)
        {
            _log->info("Log started. Plugin enabled");
            result = 1;
        }

    return result;
}

PLUGIN_API void XPluginDisable(void)
{
    _log->info("Plugin disabled");
}

PLUGIN_API void XPluginReceiveMessage(void)
{

}

void menuCallback(void* menuRef, void* itemRef)
{
    _log->debug("Menu item selected");
    switch((intptr_t) itemRef)
    {
        case 1:
            _log->info("MENU:  BBPlugin->Start Selected");
            //start recording
            start();
            break;
        case 2:
            //stop recording
            _log->info("MENU:  BBPlugin->Stop Selected");
            stop();
            break;
        default:
            _log->error("MENU: unknown menu item selected");
            break;

    }
}



//Windows DLL boilerplate
#ifdef IBM
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    DWORD ul_reason_for_call;
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

#endif
