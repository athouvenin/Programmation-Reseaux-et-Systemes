#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

  // déclaration des variables
  struct sockaddr_in adresse, client;
  int port= 5001;
  int valid= 1;
  socklen_t alen= sizeof(client);
  char buffer[RCVSIZE];

  // (1) create socket
  int server_desc = socket(AF_INET, SOCK_STREAM, 0);

  // afficher valeur descripteur
  // printf ("valeur descripteur: %d\n", server_desc);

  // (2) handle error
  if (server_desc < 0) {
    perror("Cannot create socket\n");
    return -1;
  }

  // (3) fonction pour réutiliser immédiatement la socket
  setsockopt(server_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  // (4) faire le lien entre la socket et le couple (@IP, port) avec la structure sockaddr (coté serveur)
  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port);
  adresse.sin_addr.s_addr= htonl(INADDR_ANY);

  // (5) initialize socket
  if (bind(server_desc, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
    perror("Bind failed\n");
    close(server_desc);
    return -1;
  }

  // (6) déclarer la fonction en passive : listen to incoming clients
  if (listen(server_desc, 0) < 0) {
    printf("Listen failed\n");
    return -1;
  }

  printf("Listen done\n");

  // (7) boucle infinie en attente de connexion
  while (1) {

    printf("Accepting\n");
    int client_desc = accept(server_desc, (struct sockaddr*)&client, &alen);

    //printf("Value of accept is:%d\n", client_desc);
    
    // (8) recevoir et transmettre des messages
    int msgSize = read(client_desc,buffer,RCVSIZE);
    printf("le contenu du buffer : %s\n",buffer);

    while (msgSize > 0) {
      fgets(buffer, RCVSIZE, stdin);
      write(client_desc,buffer,msgSize);
      memset(buffer,0,RCVSIZE);
      msgSize = read(client_desc,buffer,RCVSIZE);
      printf("le contenu du buffer boucle: %s\n",buffer);
    }

    close(client_desc);

  }

close(server_desc);
return 0;
}
