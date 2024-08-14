//
// Created by Zero on 6/25/2023.
//


#define XPLM200
#define XPLM210
#define XPLM300
#define XPLM400

#include<XPLM/XPLMProcessing.h>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/DataProcessor.h"
#include "include/Logger.h"



//Planned use APIs
//https://developer.x-plane.com/sdk/XPLMDataAccess/

DataProcessor* dataProcessor;
Logger* _log;

float PollData()
{
    float result = 0.0;
    if(dataProcessor->hasFailed())
    {
        //do nothing
        result = -1.0;
    }
    else if(dataProcessor->hasStarted())
    {

        //open data collection
        _log->info("Retrieving data");
        dataProcessor->get();
        result = 1.0;
    }
    else
    {
        _log->info("Initing data collection process.");
        dataProcessor->Start();
        result  = 1.0;
    }

    return result;
}

void cleanup()
{
    free(dataProcessor);
    free(_log);
}

//***** X-PLANE plugin methods *****//
PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
    _log = new Logger();
    dataProcessor = new DataProcessor(_log);
    //basic plugin information
    strcpy(name, "BeigeBox");
    strcpy(sig, "com.avidata.recorder");
    strcpy(desc, "Sim Flight Event Recorder for varied data");


    //register callback
    XPLMRegisterFlightLoopCallback((XPLMFlightLoop_f)PollData, -1, NULL);
    return 1;
}

PLUGIN_API void XPluginStop(void)
{
    //end network connection
    if(dataProcessor->hasStarted())
    {
        dataProcessor->Stop();
    }

    //destroy callback
    XPLMUnregisterFlightLoopCallback((XPLMFlightLoop_f)PollData, NULL);
    cleanup();




}

PLUGIN_API int XPluginEnable(void)
{
    int result = 0;

    try
    {
        if(_log && dataProcessor)
        {
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

    if(dataProcessor && !dataProcessor->hasFailed())
    {
        dataProcessor->Stop();
    }
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
