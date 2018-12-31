#include "pipe_networking.h"
#include <signal.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("SERVER: removing fifo\n");
    remove("/tmp/fifo");
    exit(0);
  }
}

void latin(char* msg) {
  int i = 0;
  for(i = 0; i < strlen(msg); i++) {
    msg[i] += 1;
  }
}

int main() {

  signal(SIGINT,sighandler);
  
  int to_client;
  int from_client;

  while(1) {
    from_client = server_handshake( &to_client );
    char input[BUFFER_SIZE];
    while(read(from_client, input, BUFFER_SIZE)){
      printf("The client said: %s\n", input);
      latin(input);
      write(to_client, input, BUFFER_SIZE);
    }
  }
}
