//
// Comm.cpp
// CPP Project
//
// Made for mid-project review code performance test
// 
// Created by Mitchell Oleson on 2/11/2016
//
// Made for Windows
//

#include "../Client_ServerCode/Client.h"

int main(int argc, char **argv) {
	// Validate the parameters
    if (argc != 3) {
        printf("Usage: %s <server-name> <port-number>\n", argv[0]);
        return 1;
    }

    // No print buffering
    setvbuf (stdout, NULL, _IONBF, 0);

    printf("Making client...\n");
    Client* winC = new Client(argv[1], argv[2]);
    printf("Client Success!\n\n\n");

	// Command Loop
	do {
        printf("Enter mode followed by a list of comma separated commands:\n");
        scanf("%s", winC->msgbuf);
        winC->client_send((const char *) &(winC->msgbuf));
        winC->client_receive();
        while(strcmp(winC->recvbuf, endMsg) != 0) {
            printf("Message Recieved: \"%s\"\n\n", winC->recvbuf);
            winC->client_receive();
        }
    } while (true);

	return 0;

}