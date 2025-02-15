//
// Created by Zero on 7/7/2024.
//

#include <cstdio>
#include <fcntl.h>
#define LISTENER_QUEUE_NAME "/cwic_mq_reply"
#define REPLY_QUEUE_NAME "/cwic_mq_post"
#define BUFFER_LENGTH 256
#include <fstream>
#include <mqueue.h>
#include <string>

using namespace std;

ofstream logFile;
mqd_t listenerQueue;
mqd_t replyQueue;

struct bbmsg {
  unsigned int msgType;
  char message[BUFFER_LENGTH];
};

int main(int argc, char *argv[]) {
  logFile.open("bbclient_log.txt", ios_base::app);
  try {

    unsigned int priority = 0;
    listenerQueue = mq_open(LISTENER_QUEUE_NAME, O_RDONLY, 0666);
    replyQueue = mq_open(REPLY_QUEUE_NAME, O_NONBLOCK | O_WRONLY, 0666);

    do {
      bbmsg incoming;
      logFile << "Waiting for message" << endl;
      mq_receive(listenerQueue, (char *)&incoming, sizeof(bbmsg), 0);
      logFile << incoming.message << endl;
      bbmsg outgoing;
      sprintf(outgoing.message,
              "This is a test response from the underlying process");
      outgoing.msgType = 12;
      mq_send(replyQueue, (const char *)&outgoing, sizeof(bbmsg), 0);
      logFile << "Outgoing message: " + to_string(outgoing.msgType) + "|" +
                     std::string(outgoing.message)
              << endl;
    } while (true);

  } catch (exception &ex) {
    logFile << "ERROR: " << ex.what() << endl;
  }

  logFile << "end of program" << endl;
  logFile.flush();
  logFile.close();
}
