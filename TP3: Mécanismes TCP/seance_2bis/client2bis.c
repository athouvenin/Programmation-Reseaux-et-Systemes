#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

    struct sockaddr_in adresse;
    int port = 5001;
    int valid = 1;
    char s_buffer[RCVSIZE];
    char c_buffer[RCVSIZE];
    char syn[RCVSIZE] = "SYN";
    char ack[RCVSIZE] = "ACK";

    int server_struct_length = sizeof(adresse);

    memset(s_buffer, 0, sizeof(s_buffer));
    //memset(c_buffer, 0, sizeof(c_buffer));

    // (1) create socket
    int client_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // (2) handle error
    if (client_desc < 0) {
        perror("cannot create socket\n");
        return -1;
    }

    // (3) fonction pour réutiliser immédiatement la socket
    setsockopt(client_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

    // (4) faire le lien entre la socket et le couple (@IP, port) avec la structure sockaddr (coté serveur)s
    adresse.sin_family= AF_INET;
    adresse.sin_port= htons(port);
    adresse.sin_addr.s_addr= htonl(INADDR_ANY);


    // (6) recevoir et transmettre des messages

    // [1] Envoie du "SYN" au serveur
    sendto(client_desc, syn, strlen(syn), 0,
            (struct sockaddr*)&adresse, server_struct_length);
            
    //int cont = 1;
    while(1){ 

        // [2] Afficher le "SYN-ACK" du serveur

        if(recvfrom(client_desc, s_buffer, sizeof(s_buffer), 0,
            (struct sockaddr*)&adresse, &server_struct_length) < 0){
            printf("Error while receiving server's msg\n");
            return -1;
        }

        if (strcmp(s_buffer,"SYN-ACK") == 0){
            printf("Response from server: %s\n",s_buffer);

            /*// [3] Renvoie du "ACK" au serveur
            sendto(client_desc, ack, strlen(ack), 0,
            (struct sockaddr*)&adresse, server_struct_length);*/

            //***********************************************
            //fgets(c_buffer, RCVSIZE, stdin);   
            if(sendto(client_desc, c_buffer, strlen(c_buffer), 0,
                (struct sockaddr*)&adresse, server_struct_length) < 0){
                printf("Unable to send message\n");
                return -1;
            }
            
            memset(s_buffer,0,RCVSIZE);

            // Receive the server's response:
            if(recvfrom(client_desc, s_buffer, sizeof(s_buffer), 0,
                (struct sockaddr*)&adresse, &server_struct_length) < 0){
                printf("Error while receiving server's msg\n");
                return -1;
            }
            
            printf("Server's response: %s\n", s_buffer);
        }
    }
    // Close the socket:
    close(client_desc);
    
    return 0;
}
