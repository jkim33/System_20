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
    printf("SERVER: making fifo\n");
    mkfifo("/tmp/fifo", 0644);
    printf("SERVER: making upstream pipe\n");
    from_client = open("/tmp/fifo", O_RDONLY);
    int id = fork();
    if (!id) {
      from_client = server_handshake(from_client, &to_client);
      char input[BUFFER_SIZE];
      while(read(from_client, input, BUFFER_SIZE)){
	printf("S%d recieved: %s\n", getpid(), input);
	latin(input);
	write(to_client, input, BUFFER_SIZE);
      }
      exit(0);
    }
    else {
      remove("/tmp/fifo");
      close(from_client);
      close(to_client);
    }
  }
}
