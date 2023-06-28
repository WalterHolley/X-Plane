//
// Created by Zero on 6/25/2023.
//

#include <stdio.h>
#include <string.h>
#include "SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include "SDK/CHeaders/XPLM/XPLMPlugin.h"
#include "SDK/CHeaders/XPLM/XPLMProcessing.h"

//Planned use APIs
//https://developer.x-plane.com/sdk/XPLMDataAccess/
//https://developer.x-plane.com/sdk/XPLMWeather/

//TODO: Research dataref paths
//https://developer.x-plane.com/datarefs/

PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
    //basic plugin information
    strcpy(name, "BeigeBox Recorder");
    strcpy(sig, "com.avidata.recorder");
    strcpy("Sim Flight Data Recorder for varied data");

    //start network connection

    //register callback

}

PLUGIN_API void XPluginStop(void)
{
    //end network connection
    //destroy callback
}

PLUGIN_API int XPluginEnable(void)
{
    return 1;
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API void XPluginReceiveMessage(void)
{

}

void PollData()
{
    //Poll for data refs

    //gather data

    //compile packet

    //send over network
}