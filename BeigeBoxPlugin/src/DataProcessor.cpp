#define MAX_BUFFER 20480

#include "DataProcessor.h"
#include "DataUtil.h"

#include <fstream>


dataFrame df;


//========PUBLIC METHODS===========//

DataProcessor::DataProcessor(Logger* log)
{
    _started = false;
    _dpFailed = false;
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
                this->get();
                _log->info("Received initial data");
                _started = true;
            }
            else
            {
                _dpFailed = true;
            }


        }
        catch(std::exception& ex)
        {
            string message("An exception occurred during data processing: ");
            message.append(ex.what());
            _log->error(message);
            _started = false;
            _dpFailed = true;
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
    if(_client->connectionIsOpen())
    {
        _client->close();
        _started = false;
    }

}

bool  DataProcessor::hasStarted()
{
    return _started;
}
bool DataProcessor::hasFailed()
{
    return _dpFailed;
}

DataProcessor::~DataProcessor()
{
    free(_client);
}

