#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  while(from_server != 1) {
    printf("Tell the Server Something: ");
    char input[BUFFER_SIZE];
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input)-1] = 0;
    write(to_server, input, BUFFER_SIZE);
    char msg[BUFFER_SIZE];
    read(from_server, msg, BUFFER_SIZE);
    printf("The Server Said: %s\n", msg);
  }
}
