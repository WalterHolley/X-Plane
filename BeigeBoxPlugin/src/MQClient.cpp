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

                message_queue mq(create_only, QUEUE_NAME, 100, sizeof(bbmsg));
                mqInited = true;
                _log->info("MQCLIENT: Message Queue Inited: " + std::string(QUEUE_NAME));
            }
            BOOST_CATCH(interprocess_exception &ex)
            {
                _log->error("MQCLIENT: Could not initialize MQ client: " + std::string(ex.what()));
            }BOOST_CATCH_END
    }

    return mqInited;
}

bool MQClient::send(bbmsg &mqMessage)
{
    bool result = false;
    if(mqInited)
    {
        message_queue mq(open_only, QUEUE_NAME);
        mq.send(&mqMessage, sizeof(mqMessage), 0);
        result = true;
        std::string msg(mqMessage.message);
        _log->debug("MQCLIENT: Message sent: " + msg);
    }


    return result;
}

void MQClient::close()
{
    message_queue::remove(QUEUE_NAME);
    mqInited = false;
    _log->debug("MQCLIENT: MQ closed");
}

