#define MAX_BUFFER 20480

#include "DataProcessor.h"
#include "DataUtil.h"

#include <fstream>


dataFrame df;


//========PUBLIC METHODS===========//

DataProcessor::DataProcessor(Logger* log)
{
    _started = false;
    _log = log;
    _client = new UDPClient(log);
}

void DataProcessor::Start()
{
    if(!_started)
    {
        try
        {
            if(!_client->connectionIsOpen())
            {
                _client->open();
            }

            if(_client->connectionIsOpen())
            {
                _log->info("Connected. Attempting to receive initial message");

                //TODO: thread for receive functionality
                df = _client->receive();
                _log->info("Received initial data");
                _log->info("Begin data transfer");
                _client->send(df);
                _log->info("Close connection");
                _client->close();
                _started = true;
            }


        }
        catch(std::exception& ex)
        {
            string message("An exception occurred during data processing: ");
            message.append(ex.what());
            _log->error(message);
            _started = false;
        }
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
    _started = false;
}

bool  DataProcessor::hasStarted()
{
    return _started;
}

DataProcessor::~DataProcessor()
{
    free(_client);
}

