//
// Created by Zero on 6/25/2023.
//


#define XPLM200
#define XPLM210
#define XPLM300
#define XPLM400

#include<XPLM/XPLMProcessing.h>
#include "include/PluginMenu.h"
#include "include/Logger.h"



Logger* _log;
DataProcessor* _dataProcessor;
PluginMenu* _menu;

void cleanup()
{

    free(_log);
    free(_dataProcessor);
    free(_menu);
}

//***** X-PLANE plugin methods *****//
PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
    _log = new Logger();
    _dataProcessor = new DataProcessor(*_log);

    _menu = new PluginMenu(*_log, *_dataProcessor);
    //basic plugin information
    strcpy(name, "BeigeBox");
    strcpy(sig, "com.avidata.recorder");
    strcpy(desc, "Sim Flight Event Recorder for varied data");


    //register callback
    return 1;
}

PLUGIN_API void XPluginStop(void)
{
    cleanup();
}

PLUGIN_API int XPluginEnable(void)
{
    int result = 0;

    try
    {

        if(_log && _menu)
        {
            _menu->init();
            _log->info("Log started. Plugin enabled");
            result = 1;
        }

    }
    catch(std::exception& ex)
    {
       string message("There was a problem enabling the plugin: ");
       message.append(ex.what());
       _log->error(message);
       cleanup();
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
