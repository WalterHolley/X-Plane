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
static DataProcessor dataProcessor;
static Logger logger;



PluginMenu::PluginMenu(Logger &log, DataProcessor &processor)
{
    logger = log;
    dataProcessor = processor;
}

void PluginMenu::init()
{

    if(!_wasInited)
    {
        pluginSubMenuId = XPLMAppendMenuItem( XPLMFindPluginsMenu(), BASE_MENU_NAME, 0, 1);
        xplmMenuIdentifier = XPLMCreateMenu(BASE_MENU_NAME, XPLMFindPluginsMenu(), pluginSubMenuId, PluginMenu::menuCallback, 0);
        XPLMAppendMenuItem(xplmMenuIdentifier, START_RECORDING, (void*) 1,1);
        XPLMAppendMenuItem(xplmMenuIdentifier, STOP_RECORDING, (void*) 2, 1);
        XPLMEnableMenuItem(xplmMenuIdentifier, 2, 0);
        _wasInited = true;
        logger.info("Menu Inited");
    }

}

PluginMenu::~PluginMenu() {
    XPLMDestroyMenu(xplmMenuIdentifier);
}

void PluginMenu::start()
{

    if(dataProcessor.hasInited())
    {
        logger.debug("Start selected from menu");
        dataProcessor.start();
        XPLMEnableMenuItem(xplmMenuIdentifier,1, 0);
        XPLMEnableMenuItem(xplmMenuIdentifier, 2, 1);
    }

}

void PluginMenu::stop()
{
    if(dataProcessor.hasInited())
    {
        logger.debug("Stop selected from menu");
        dataProcessor.stop();
        XPLMEnableMenuItem(xplmMenuIdentifier,1, 1);
        XPLMEnableMenuItem(xplmMenuIdentifier,2, 0);
    }
}

void PluginMenu::menuCallback(void* menuRef, void* itemRef)
{

    switch((intptr_t) itemRef)
    {
        case 1:
            //start recording
            start();
            break;
        case 2:
            //stop recording
            stop();
            break;
        default:
            break;

    }
}