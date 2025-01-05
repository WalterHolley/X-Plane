//
// Created by Zero on 7/7/2024.
//

#include <cstdio>
#define LISTENER_QUEUE_NAME "cwiq_mq_reply"
#define REPLY_QUEUE_NAME "cwiq_mq_post"
#define BUFFER_LENGTH 256
#include <boost/interprocess/ipc/message_queue.hpp>
#include <fstream>

using namespace boost::interprocess;
using namespace std;

ofstream logFile;

struct bbmsg {
  unsigned int msgType;
  char message[256];
};

int main(int argc, char *argv[]) {
  logFile.open("bbclient_log.txt", ios_base::app);
  BOOST_TRY {
    bbmsg message;
    bbmsg sendmessage;
    unsigned int priority;
    message_queue listen_mq(create_only, LISTENER_QUEUE_NAME, 100,
                            sizeof(bbmsg));
    message_queue reply_mq(create_only, REPLY_QUEUE_NAME, 100, sizeof(bbmsg));

    message_queue::size_type receivedSize;
    for (int i = 0; i < 10; i++) {
      logFile << "Waiting for message" << endl;
      listen_mq.receive(&message, sizeof(bbmsg), receivedSize, priority);
      logFile << message.message << endl;
      sendmessage.msgType = 12;
      strcat(sendmessage.message,
             "This is a test response from the underlying process");
      reply_mq.send(&sendmessage, sizeof(bbmsg), 0);
    }
  }
  BOOST_CATCH(interprocess_exception & ex) {
    logFile << "ERROR: " << ex.what() << endl;
  }
  BOOST_CATCH_END

  logFile << "end of program" << endl;
  logFile.flush();
  logFile.close();
}
