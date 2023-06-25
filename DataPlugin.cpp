//
// Created by Zero on 6/25/2023.
//

#include <stdio.h>
#include <string.h>
#include "SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include "SDK/CHeaders/XPLM/XPLMPlugin.h"

//Planned use APIs
//https://developer.x-plane.com/sdk/XPLMDataAccess/
//https://developer.x-plane.com/sdk/XPLMWeather/

//TODO: Research dataref paths
//https://developer.x-plane.com/datarefs/

PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{

}

PLUGIN_API void XPluginStop(void)
{

}

PLUGIN_API int XPluginEnable(void)
{
    return 1;
}

PLUGIN_API void XPluginDisable(void)
{
}