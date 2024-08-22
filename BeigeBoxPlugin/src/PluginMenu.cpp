//
// Created by Zero on 8/15/2024.
//

#include "include/PluginMenu.h"

using namespace std;

int pluginSubMenuId;
const char* BASE_MENU_NAME = "BeigeBox";
const char* START_RECORDING = "Start Recording";
const char* STOP_RECORDING = "Stop Recording";
XPLMMenuID xplmMenuIdentifier;

static void menuCallback(void * inMenuRef, void * initemRef );


void PluginMenu::init()
{
     pluginSubMenuId = XPLMAppendMenuItem( XPLMFindPluginsMenu(), BASE_MENU_NAME, 0, 1);
     xplmMenuIdentifier = XPLMCreateMenu(BASE_MENU_NAME, XPLMFindPluginsMenu(), pluginSubMenuId, menuCallback, 0);
     XPLMAppendMenuItem(xplmMenuIdentifier, START_RECORDING, (void*) 1,1);
     XPLMAppendMenuItem(xplmMenuIdentifier, STOP_RECORDING, (void*) 2, 1);
}
void menCallback(void* menuRef, void* itemRef)
{

    switch((intptr_t) itemRef)
    {
        case 1:
            //start recording
            break;
        case 2:
            //stop recording
            break;
        default:
            break;

    }
}