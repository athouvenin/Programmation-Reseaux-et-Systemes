#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

  struct sockaddr_in adresse, client;
  struct sockaddr_in adresse2, client;
  //int port= 5001;
  int valid= 1;
  socklen_t alen= sizeof(client);
  char buffer[RCVSIZE];

  //create socket
  int server_desc = socket(AF_INET, SOCK_STREAM, 0);
  int serveur_s2 = socket(AF_INET, SOCK_DGRAM, 0);
  
  //declaration du set de descripteur
  FD_SET set_desc = FD_ZERO();

  //handle error
  if (server_desc < 0) {
    perror("Cannot create socket\n");
    return -1;
  }
  if (serveur_s2 < 0) {
    perror("Cannot create socket\n");
    return -1;
  }

  setsockopt(server_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(atoi(argv[1]));
  adresse.sin_addr.s_addr= htonl(INADDR_ANY);
  
  setsockopt(serveur_s2, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse2.sin_family= AF_INET;
  adresse2.sin_port= htons(atoi(argv[1]));
  adresse2.sin_addr.s_addr= htonl(INADDR_ANY);
  
  
  //printf("Val:%s\n", argv[0]);

  //initialize socket 1
  if (bind(server_desc, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
    perror("Bind failed\n");
    close(server_desc);
    return -1;
  }

  //initialize socket 2

  if (bind(serveur_s2, (struct sockaddr*) &adresse2, sizeof(adresse)) == -1) {
    perror("Bind failed\n");
    close(server_desc);
    return -1;
  }
  
  //listen to incoming clients (socket 1
  if (listen(server_desc, 0) < 0) {
    printf("Listen failed\n");
    return -1;
  }

  printf("Listen done\n");

  while (1) {

    pid_t c_pid = fork();
    printf("Valeur du process : %d\n", c_pid);
    
    //if(pid == 0){
    printf("Accepting\n");
    int client_desc = accept(server_desc, (struct sockaddr*)&client, &alen);

    printf("Value of accept is:%d\n", client_desc);
    
    if (c_pid == 0){
        printf("valeur socket crée par fct accept: %d\n" , client_desc);
        printf("valeur socket sur laquelle le serveur attend les connexions: %d\n" , server_desc);
    }    
       
    int msgSize = read(client_desc,buffer,RCVSIZE);

    while (msgSize > 0) {
    write(client_desc,buffer,msgSize);
    printf("%s",buffer);
    memset(buffer,0,RCVSIZE);
    msgSize = read(client_desc,buffer,RCVSIZE);
    }
    //}

    close(client_desc);

  }

close(server_desc);
return 0;
}
