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

    char listenmsg[256];
    char replymsg[256];
    unsigned int priority = 0;
    listenerQueue = mq_open(LISTENER_QUEUE_NAME, O_RDONLY, 0666);
    replyQueue = mq_open(REPLY_QUEUE_NAME, O_NONBLOCK | O_WRONLY, 0666);

    do {
      logFile << "Waiting for message" << endl;
      mq_receive(listenerQueue, listenmsg, sizeof(listenmsg), 0);
      logFile << listenmsg << endl;
      sprintf(replymsg, "This is a test response from the underlying process");
      mq_send(replyQueue, replymsg, sizeof(replymsg), 0);
    } while (true);

  } catch (exception &ex) {
    logFile << "ERROR: " << ex.what() << endl;
  }

  logFile << "end of program" << endl;
  logFile.flush();
  logFile.close();
}
