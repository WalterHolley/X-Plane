//
// Created by Zero on 7/7/2024.
//

#define QUEUE_NAME "BBQUEUE"
#define BUFFER_LENGTH 256
#include<boost/interprocess/ipc/message_queue.hpp>
#include<string>
#include <fstream>

using namespace boost::interprocess;
using namespace std;

ofstream logFile;

struct bbmsg
{
    long msgType;
    char message[256];
};


int main(int argc, char* argv[])
{
    logFile.open("bbclient_log.txt", ios_base::app);
    BOOST_TRY
    {
        bbmsg message;
        unsigned int priority;

        message_queue mq(open_only, QUEUE_NAME);
        message_queue::size_type receivedSize;
        logFile << "Waiting for message" << endl;
        mq.receive(&message, sizeof(bbmsg), receivedSize, priority);
        logFile << message.message << endl;
    }
    BOOST_CATCH(interprocess_exception &ex)
    {
        logFile << "ERROR: " << ex.what() << endl;
    }BOOST_CATCH_END




    logFile << "end of program" << endl;
    logFile.flush();
    logFile.close();
}
