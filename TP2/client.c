#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

  struct sockaddr_in adresse;
  //int port = 5001;
  int valid = 1;
  char msg[RCVSIZE];
  char blanmsg[RCVSIZE];

  //create socket
  int server_desc = socket(AF_INET, SOCK_STREAM, 0);

  // handle error
  if (server_desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  setsockopt(server_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(atoi(argv[2]));
  adresse.sin_addr.s_addr= htonl(INADDR_LOOPBACK);

  // connect
  
  int rc = connect(server_desc, (struct sockaddr*)&adresse, sizeof(adresse));
  printf("value of connect is: %d\n", rc);
  
 
    
  // handle error
  if (rc < 0) {
    perror("connect failed\n");
    return -1;
  }


  int cont= 1;
  while (cont) {
    fgets(msg, RCVSIZE, stdin);
    int sss= write(server_desc,msg,strlen(msg));
    printf("the value of sent is:%d\n", sss);
    read(server_desc, blanmsg, RCVSIZE);
    printf("%s",blanmsg);
    memset(blanmsg,0,RCVSIZE);
    if (strcmp(msg,"stop\n") == 0) {
      cont= 0; 
    }
  }

close(server_desc);
return 0;
}

