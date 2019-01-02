#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int up, int *to_client) {
  remove("/tmp/fifo");
  char input[BUFFER_SIZE];
  char* msg = malloc(HANDSHAKE_BUFFER_SIZE);
  read(up, msg, HANDSHAKE_BUFFER_SIZE);
  printf("SERVER: message recieved\n");

  printf("SERVER: making downstream pipe\n");
  int down = open(msg, O_WRONLY);
  write(down, msg, HANDSHAKE_BUFFER_SIZE);
  printf("SERVER: message sent\n");

  read(up, msg, HANDSHAKE_BUFFER_SIZE);
  if (strncmp(ACK, msg, HANDSHAKE_BUFFER_SIZE) == 0) {
    printf("SERVER: connection established\n");
  }
 
  free(msg);
  
  *to_client = down;
  return up;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char* priv = malloc(HANDSHAKE_BUFFER_SIZE);
  sprintf(priv, "/tmp/%d", getpid());
  printf("CLIENT: making priv\n");
  mkfifo(priv, 0644);
  
  printf("CLIENT: making upstream pipe\n");
  int up = open("/tmp/fifo", O_WRONLY);
  if (up == -1) {
    printf("Failed to connect to server\n");
    return 1;
  }
  write(up, priv, HANDSHAKE_BUFFER_SIZE);
  printf("CLIENT: message sent\n");

  printf("CLIENT: making downstream pipe\n");
  int down = open(priv, O_RDONLY);
  char* msg = malloc(HANDSHAKE_BUFFER_SIZE);
  read(down, msg, HANDSHAKE_BUFFER_SIZE);
  if (strncmp(priv, msg, HANDSHAKE_BUFFER_SIZE) == 0) {
    printf("CLIENT: message recieved. connection established\n");
  }
  
  remove(priv);

  printf("CLIENT: sending connection message\n");
  write(up, ACK, HANDSHAKE_BUFFER_SIZE);

  free(msg);
  
  *to_server = up;
  return down;
}
