#define MAX_BUFFER 20480

#include "DataProcessor.h"
#include "DataUtil.h"

#include <fstream>


dataFrame df;


//========PUBLIC METHODS===========//

DataProcessor::DataProcessor(Logger* log)
{
    _log = log;
    _client = new UDPClient(log);
}

void DataProcessor::Start()
{

    try{
        _log->info("Connect and receive initial message");
        _client->open();

        //TODO: thread for receive functionality
        _log->info("Receive initial data");
        df = _client->receive();
        _log->info("Begin data transfer");
        _client->send(df);
        _log->info("Close connection");
        _client->close();


    }
    catch(std::exception& ex)
    {
        char* message;
        sprintf(message, "An exception occurred: %s", ex.what());
        _log->error(message);

    }

}

void DataProcessor::get()
{
    df = _client->receive();
    _client->send(df);
}


void DataProcessor::Stop()
{
    _client->close();
}

DataProcessor::~DataProcessor()
{
    free(_client);
}

