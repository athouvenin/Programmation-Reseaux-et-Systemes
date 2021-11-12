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

int main(int argc, char *argv[]){
    
    /*int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length = sizeof(client_addr);*/
    
    // déclaration des variables
    struct sockaddr_in server_addr, client_addr;
    int port= 5001;
    int valid= 1;
    socklen_t alen= sizeof(client_addr);
    char s_buffer[RCVSIZE];
    char c_buffer[RCVSIZE];
    char synack[RCVSIZE] = "SYN-ACK";

    memset(s_buffer, 0, sizeof(s_buffer));
    memset(c_buffer, 0, sizeof(c_buffer));

    // (1) create socket
    int server_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

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
    server_addr.sin_family= AF_INET;
    server_addr.sin_port= htons(port);
    server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

    // (5) initialize socket
    if (bind(server_desc, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed\n");
        close(server_desc);
        return -1;
    }   
    printf("Listen done\n");

    // (6) Receive client's message:

    while(1){

        //printf("Accepting\n");

        if (recvfrom(server_desc, c_buffer, sizeof(c_buffer), 0,
            (struct sockaddr*)&client_addr, &alen) < 0){
            printf("Couldn't receive\n");
            return -1;
        }

        printf("Received message from IP: %s and port: %i\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        //printf("contenu c_buffer: %s\n",c_buffer);
        
        //Afficher le "SYN" du client
        if (strcmp(c_buffer,"SYN") == 0){
            printf("Msg from client: %s\n",c_buffer);
        
            memset(s_buffer,0,RCVSIZE);

            //Respond "SYN-ACK" to client
            sendto(server_desc, synack, strlen(synack), 0,
                (struct sockaddr*)&client_addr, alen);



            // Respond to client:
            fgets(s_buffer, RCVSIZE, stdin);
            //strcpy(s_buffer, c_buffer);
            
            if (sendto(server_desc, s_buffer, strlen(s_buffer), 0,
                (struct sockaddr*)&client_addr, alen) < 0){
                printf("Can't send\n");
                return -1;
            }
        }
    }
    
    // Close the socket:
    close(server_desc);
    return 0;
}
