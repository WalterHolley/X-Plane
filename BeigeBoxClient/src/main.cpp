//
// Created by Zero on 7/7/2024.
//

#include <cstdio>
#include <fcntl.h>
#define LISTENER_QUEUE_NAME "/cwiq_mq_reply"
#define REPLY_QUEUE_NAME "/cwiq_mq_post"
#define BUFFER_LENGTH 256
#include <fstream>
#include <mqueue.h>
#include <string.h>

using namespace std;

ofstream logFile;
mqd_t listenerQueue;
mqd_t replyQueue;

struct bbmsg {
  unsigned int msgType;
  char message[256];
};

int main(int argc, char *argv[]) {
  logFile.open("bbclient_log.txt", ios_base::app);
  try {

    bbmsg message;
    bbmsg sendmessage;
    unsigned int priority = 0;
    listenerQueue = mq_open(LISTENER_QUEUE_NAME, O_RDONLY, 066);
    replyQueue = mq_open(REPLY_QUEUE_NAME, O_NONBLOCK | O_WRONLY, 066);

    for (int i = 0; i < 10; i++) {
      logFile << "Waiting for message" << endl;
      mq_receive(listenerQueue, (char *)&message, sizeof(bbmsg), 0);
      logFile << message.message << endl;
      sendmessage.msgType = 12;
      strcat(sendmessage.message,
             "This is a test response from the underlying process");
      mq_send(replyQueue, (const char *)&sendmessage, sizeof(bbmsg), 0);
    }
  } catch (exception &ex) {
    logFile << "ERROR: " << ex.what() << endl;
  }

  logFile << "end of program" << endl;
  logFile.flush();
  logFile.close();
}
