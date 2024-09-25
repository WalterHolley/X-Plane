//
// Created by Zero on 6/25/2023.
//


#define XPLM200
#define XPLM210
#define XPLM300
#define XPLM400

#include<XPLM/XPLMProcessing.h>
#include<XPLM/XPLMMenus.h>
#include<fstream>
#include<sstream>
#include "include/DataUtil.h"
#include "include/Recorder.h"
#include "include/Logger.h"



Logger* _log;
DataUtil* _dataUtil;
Recorder* _recorder;
XPLMMenuID xplmMenuIdentifier;
int pluginSubMenuId;
const char* BASE_MENU_NAME = "BeigeBox";
const char* START_RECORDING = "Start Recording";
const char* STOP_RECORDING = "Stop Recording";
string TEST_DB = "NORTHWIND_AI";
ifstream json("datarefs.json");
bool record = false;
dataFrame flightData;



static void menuCallback(void* inMenuRef, void* inItemRef);
static float pollData(float timeSinceLastCall, float timeSinceLastFlightLoop, int count, void* refCon);

void cleanup()
{
    XPLMUnregisterFlightLoopCallback(pollData, NULL);
    free(_log);
    free(_dataUtil);
    free(_recorder);
}

void start()
{

    _log->debug("Start selected from menu");
    XPLMEnableMenuItem(xplmMenuIdentifier,1, 0);
    XPLMEnableMenuItem(xplmMenuIdentifier, 2, 1);
    record = true;

}

void stop()
{

    _log->debug("Stop selected from menu");

    XPLMEnableMenuItem(xplmMenuIdentifier,1, 1);
    XPLMEnableMenuItem(xplmMenuIdentifier,2, 0);
    record = false;

}


float pollData(float timeSinceLastCall, float timeSinceLastFlightLoop, int count, void* refCon)
{
    if(record)
    {
        _dataUtil->updateScenario(flightData);
        _recorder->write(flightData);
        _log->debug("BeigeBox: Recording completed");
    }
    else
    {
        _log->debug("BeigeBox: Recording Off");
    }

    return 1.0;
}


//***** X-PLANE plugin methods *****//
PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
    _log = new Logger();
    _dataUtil = new DataUtil(_log);
    _recorder = new Recorder(TEST_DB, _log);
    int result = 0;

    if(json.good())
    {
        string jsn;
        stringstream content;

        while(getline(json, jsn))
        {
            content << jsn;
        }
        jsn = content.str();
        flightData = _dataUtil->getScenarioData(jsn);
        _recorder->init(flightData);
        result = 1;
    }

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

    XPLMRegisterFlightLoopCallback(pollData, 1.0, NULL);


    return result;
}

PLUGIN_API void XPluginStop(void)
{
    cleanup();
}

PLUGIN_API int XPluginEnable(void)
{
    int result = 0;

        if(_log && _recorder && _dataUtil)
        {
            _log->info("Beigebox: Plugin enabled");
            result = 1;
        }

    return result;
}

PLUGIN_API void XPluginDisable(void)
{
    _log->info("BeigeBox: Plugin disabled");
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
