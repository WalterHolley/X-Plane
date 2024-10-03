//
// Created by Zero on 7/7/2024.
//

#define MQ_NAME "BBQUEUE"
#include<boost/interprocess/ipc/message_queue.hpp>
#include<string>

using namespace boost::interprocess;
using namespace std;

int main(int argc, char* argv[])
{
    message_queue mq(open_only, MQ_NAME);
    message_queue::size_type receivedSize;
    string message;
    unsigned int priority;
    mq.receive(&message, size(message), receivedSize, priority);

}
