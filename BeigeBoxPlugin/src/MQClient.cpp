//
// Created by Zero on 9/27/2024.
//

#include "include/MQClient.h"


#define BUFFER_LENGTH 256
#define QUEUE_NAME "BBQUEUE"


MQClient::MQClient(Logger *log)
{
    _log = log;
    mqInited = false;
}


bool MQClient::init()
{
    if(!mqInited)
    {
        BOOST_TRY{
                message_queue::remove(QUEUE_NAME);

                message_queue mq(create_only, QUEUE_NAME, 100, BUFFER_LENGTH);
                mqInited = true;
            }
            BOOST_CATCH(interprocess_exception &ex)
            {
                _log->error("MQCLIENT: Could not initialize MQ client: " + std::string(ex.what()));
            }BOOST_CATCH_END
    }

    return mqInited;
}

bool MQClient::send(std::string message)
{
    bool result = false;

    message_queue mq(open_only, QUEUE_NAME);
    mq.send(&message, size(message), 0);
    result = true;
    _log->debug("MQCLIENT: Message sent: " + message);

    return result;
}

void MQClient::close()
{
    message_queue::remove(QUEUE_NAME);
    mqInited = false;
    _log->debug("MQCLIENT: MQ closed");
}

