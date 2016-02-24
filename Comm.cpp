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
    Client* cc = new Client(argv[1], argv[2]);
    printf("Client Success!\n\n\n");

    // Message holder
    char msg[DEFAULT_BUFLEN];

	// Command Loop
	do {
        printf("Enter message: ");
        scanf("%s", msg);
        cc->cSend((const char *) &msg);
        cc->cReceive();
        printf("Message Recieved: \"%s\"\n\n", c->recvbuf);
    } while (true);

	return 0;

}