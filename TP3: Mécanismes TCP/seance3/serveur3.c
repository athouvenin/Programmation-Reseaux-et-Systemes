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

    struct sockaddr_in server_addr, client_addr, control_addr;
    

    int port= 5001;
    int port_c = 5002;
    int valid= 1;

    socklen_t alen= sizeof(client_addr);
    //socklen_t alenc= sizeof(cli_control_addr);

    char s_buffer[RCVSIZE];
    char c_buffer[RCVSIZE];

    char num_p[1000];
    sprintf(num_p, "%d", port);   //traduction d'un int en char
    char synack[RCVSIZE] = "SYN-ACK";
    char* SynAckPort = strcat(synack, num_p);
    printf("Converted to string : %s\n", SynAckPort);

    memset(s_buffer, 0, sizeof(s_buffer));
    memset(c_buffer, 0, sizeof(c_buffer));

    // (1) CREATE SOCKETS
    int server_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int control_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


    // (2) handle error
    if (server_desc < 0) {
        perror("Cannot create socket\n");
        return -1;
    }

    // (3) fonction pour réutiliser immédiatement les sockets
    setsockopt(server_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));
    setsockopt(control_desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

    // (4) faire le lien entre la socket et le couple (@IP, port) avec la structure sockaddr (coté serveur)
    server_addr.sin_family= AF_INET;
    server_addr.sin_port= htons(port);
    server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

    control_addr.sin_family= AF_INET;
    control_addr.sin_port= htons(port_c);
    control_addr.sin_addr.s_addr= htonl(INADDR_ANY);

    // (5) initialize socket

    if (bind(server_desc, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed\n");
        close(server_desc);
        return -1;
    } 

    if (bind(control_desc, (struct sockaddr*) &control_addr, sizeof(control_addr)) == -1) {
        perror("Bind failed\n");
        close(control_desc);
        return -1;
    } 
    printf("Listen done\n");

    // (6) CONTROL MESSSAGES
    //printf("Accepting\n");

    if (recvfrom(control_desc, c_buffer, sizeof(c_buffer), 0,
        (struct sockaddr*)&client_addr, &alen) < 0){
        printf("Couldn't receive\n");
        return -1;
    }


    // [1] Recevoir le SYN du client
    printf("Received message from IP: %s and port: %i\n",
        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
      
    if (strcmp(c_buffer,"SYN") == 0){
        printf("SYN from client: %s\n",c_buffer);
        memset(c_buffer,0,RCVSIZE);

        // [2] Respond "SYN-ACK" to client +  NUMERO PORT MESSAGES   
        int sen = sendto(control_desc, SynAckPort, strlen(SynAckPort), 0,
            (struct sockaddr*)&client_addr, alen);
        if (sen <0){
            perror("erreur d'envoie");
        }

        while(1){ 

            //[3] Reception du "ACK"
            if (recvfrom(control_desc, c_buffer, sizeof(c_buffer), 0,
                (struct sockaddr*)&client_addr, &alen) < 0){
                printf("Couldn't receive\n");
                return -1;
            } 
            //printf("contenu c_buffer: %s\n",c_buffer);  

            if (strcmp(c_buffer,"ACK") == 0){
                printf("ACK from client: %s\n",c_buffer);

                memset(c_buffer,0,RCVSIZE);

                

                //***************************
                printf("Accepting\n");

                while(1){
                    if (recvfrom(server_desc, c_buffer, sizeof(c_buffer), 0,
                        (struct sockaddr*)&client_addr, &alen) < 0){
                        printf("Couldn't receive\n");
                        return -1;
                    }
                    printf("Received message from IP: %s and port: %i\n",
                        inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    
                    printf("Msg from client: %s\n",c_buffer);
                    
                    memset(c_buffer,0,RCVSIZE);

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
        }
            
    }
    
    
    // Close the socket:
    close(server_desc);
    return 0;
}
