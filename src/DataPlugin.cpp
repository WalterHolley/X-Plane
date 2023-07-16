//
// Created by Zero on 6/25/2023.
//
/*
#define XPLM XPLM400

#ifdef _WIN32
#define IBM 1
#endif

#ifdef __MACH__
#define APL apl
#endif

#ifdef __linux__
#define LIN
#endif
*/



#include <stdio.h>
#include <string.h>
#include <ctime>
#include <chrono>

#include "../SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include "../SDK/CHeaders/XPLM/XPLMUtilities.h"
#include "../SDK//CHeaders//XPLM//XPLMProcessing.h"




//Planned use APIs
//https://developer.x-plane.com/sdk/XPLMDataAccess/


FILE * logFile;
static XPLMDataRef latDataRef = NULL;
static XPLMDataRef lonDataRef = NULL;

float PollData()
{
    fprintf(logFile, "Polling Data \n");
    //Poll for data refs
    float lat = XPLMGetDataf(latDataRef);
    float lon = XPLMGetDataf(lonDataRef);

    fprintf(logFile, "lat: %f, lon: %f\n", lat, lon);
    //gather data

    //compile packet

    //send over network
    return 1.0;
}

PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
    char outFilePath[512];

    //basic plugin information
    strcpy(name, "BeigeBox Recorder");
    strcpy(sig, "com.avidata.recorder");
    strcpy(desc, "Sim Flight Data Recorder for varied data");


    //create path for log
    XPLMGetSystemPath(outFilePath);
    strcat(outFilePath, "TestValues.txt");
    logFile = fopen(outFilePath, "w");

    try
    {

    }
    catch (...)
    {

    }
    //init data ref
    lonDataRef = XPLMFindDataRef("sim/flightmodel/position/longititude");
    latDataRef = XPLMFindDataRef("sim/flightmodel/position/latitude");



    //register callback
    XPLMRegisterFlightLoopCallback((XPLMFlightLoop_f)PollData, 1.0, NULL);
    return 1;
}

PLUGIN_API void XPluginStop(void)
{
    //end network connection
    //destroy callback
    XPLMUnregisterFlightLoopCallback((XPLMFlightLoop_f)PollData, NULL);
    fclose(logFile);
}

PLUGIN_API int XPluginEnable(void)
{
    return 1;
}

PLUGIN_API void XPluginDisable(void)
{
    fflush(logFile);
}

PLUGIN_API void XPluginReceiveMessage(void)
{

}

#ifdef _WIN32
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}

#endif
