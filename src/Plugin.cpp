//
// Created by Zero on 6/25/2023.
//


#define XPLM200
#define XPLM210
#define XPLM300
#define XPLM400

#include <string.h>
#include <chrono>
#include<XPLM/XPLMProcessing.h>
#include "DataProcessor.h"




//Planned use APIs
//https://developer.x-plane.com/sdk/XPLMDataAccess/

DataProcessor* dataProcessor;


float PollData()
{
    if(dataProcessor)
    {
        dataProcessor->get();
    }
    return 1.0;
}

//***** X-PLANE plugin methods *****//
PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{

    //basic plugin information
    strcpy(name, "BeigeBox");
    strcpy(sig, "com.avidata.recorder");
    strcpy(desc, "Sim Flight Event Recorder for varied data");




    //register callback
    XPLMRegisterFlightLoopCallback((XPLMFlightLoop_f)PollData, 1.0, NULL);
    return 1;
}

PLUGIN_API void XPluginStop(void)
{
    //end network connection
    //destroy callback
    XPLMUnregisterFlightLoopCallback((XPLMFlightLoop_f)PollData, NULL);
    dataProcessor->Stop();
    free(dataProcessor);

}

PLUGIN_API int XPluginEnable(void)
{
    if(dataProcessor == nullptr)
    {
        dataProcessor = new DataProcessor();
    }

    //open data collection
    dataProcessor->Start();
    return 1;
}

PLUGIN_API void XPluginDisable(void)
{
    if(dataProcessor)
    {
        dataProcessor->Stop();

    }
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
