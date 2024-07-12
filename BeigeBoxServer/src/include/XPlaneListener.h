//
// Created by Zero on 7/11/2024.
//

#ifndef XPLANELISTENER_H
#define XPLANELISTENER_H


class XPlaneListener
{
    public:
        XPlaneListener();
    private:
        void acceptConnection();
        void start();
        void stop();
};


#endif //XPLANELISTENER_H
