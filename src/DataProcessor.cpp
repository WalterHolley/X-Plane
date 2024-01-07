#define MAX_BUFFER 20480

#include "DataProcessor.h"


#include <iomanip>
#include <fstream>




//========PUBLIC METHODS===========//

DataProcessor::DataProcessor()
{
    _util = DataUtil();
    _client = UDPClient();
}

void DataProcessor::Start() {
    dataFrame df;
    try{
        _util.writeToLog("Connect and receive initial message");
        _client.open();

        //TODO: thread for receive functionality
        _util.writeToLog("Receive initial data");
        df = _client.receive();
        _util.writeToLog("Begin data transfer");
        _client.send(df);
        _util.writeToLog("Close connection");
        _client.close();


    }
    catch(exception& ex)
    {
        char* message;
        sprintf(message, "An exception occurred: %s", ex.what());
        _util.writeToLog(message);

    }

}

void DataProcessor::Stop() {

}



