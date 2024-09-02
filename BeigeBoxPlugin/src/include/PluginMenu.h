//
// Created by Zero on 8/15/2024.
//

#ifndef PLUGINMENU_H
#define PLUGINMENU_H

#include <XPLM/XPLMMenus.h>
#include "Logger.h"
#include "DataProcessor.h"

class PluginMenu {
public:
    PluginMenu(Logger &logger, DataProcessor &processor);
    void init();
    ~PluginMenu();
private:
    bool _wasInited;
    static void start();
    static void stop();
    static void menuCallback(void * inMenuRef, void * initemRef );

};


#endif //PLUGINMENU_H
