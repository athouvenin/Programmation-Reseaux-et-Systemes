#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char *argv[]) {

    int client_desc = socket(AF_INET, SOCK_STREAM, 0);
    printf ("valeur descripteur: %d\n", client_desc);
}