#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void){
    int serveur_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length = sizeof(client_addr);
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create UDP socket:
    serveur_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(serveur_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind to the set port and IP:
    if(bind(serveur_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    

    printf("Listening for incoming messages...\n\n");
    
    while(1){
        /*
        // Receive client's message:
        if (recvfrom(serveur_desc, client_message, sizeof(client_message), 0,
            (struct sockaddr*)&client_addr, &client_struct_length) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
        printf("Received message from IP: %s and port: %i\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        printf("Msg from client: %s\n", client_message);
        
        // Respond to client:
        strcpy(server_message, client_message);
        
        if (sendto(serveur_desc, server_message, strlen(server_message), 0,
            (struct sockaddr*)&client_addr, client_struct_length) < 0){
            printf("Can't send\n");
            return -1;
            }
    
    }*/

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
    // Close the socket:
    close(serveur_desc);
    
    return 0;
}