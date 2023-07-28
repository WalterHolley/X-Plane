//
// Created by Zero on 6/25/2023.
//




#include <stdio.h>
#include <string.h>
#include <ctime>
#include <chrono>
#include "DataProcessor.hpp"
#include "../SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include "../SDK/CHeaders/XPLM/XPLMUtilities.h"
#include "../SDK//CHeaders//XPLM//XPLMProcessing.h"




//Planned use APIs
//https://developer.x-plane.com/sdk/XPLMDataAccess/


FILE * logFile;
DataProcessor dataProcessor;

//This is going away. Data processing needs to be moved
//and performed on a separate thread
float PollData()
{
    fprintf(logFile, "Polling Data \n");

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
    //TODO: Consider keeping log, but moving to resource folder
    XPLMGetSystemPath(outFilePath);
    strcat(outFilePath, "TestValues.txt");
    logFile = fopen(outFilePath, "w");

    //open data collection
    dataProcessor.Start();
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
    //close connection or send 'pause' notification?
    fflush(logFile);
}

PLUGIN_API void XPluginReceiveMessage(void)
{

}

//Windows DLL boilerplate
#ifdef _WIN32
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}

#endif
