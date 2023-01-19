#include "klient.h"

int klient_main(int argc, char *argv[]) {
    if (argc < 3) {
        printError("Klienta je nutne spustit s nasledujucimi argumentmi: adresa port.");
    }
    
    //ziskanie adresy a portu servera <netdb.h>
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        printError("Server neexistuje.");
    }
    int port = atoi(argv[2]);
	if (port <= 0) {
		printError("Port musi byt cele cislo vacsie ako 0.");
	}
    
    //vytvorenie socketu <sys/socket.h>
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printError("Chyba - socket.");        
    }
    
    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);

    if (connect(sock,(struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - connect.");        
    }

    printf("Spojenie so serverom bolo nadviazane.\n");

    char buff[2] = { 0 };
    read(sock, buff, strlen(buff)+1);

    char buffer[2] = { 0 };
    KlientData klient_data = { sock, buff[0] };
    pthread_t klient;
    pthread_create(&klient, NULL, zobrazHru, &klient_data);

    while (buffer[0] != 'Q' || buffer[0] != 'q') {
        buffer[0] = getchar();

        if(buffer[0] != -1) {
            write(sock, buffer, strlen(buffer)+1);
        }
    }

    pthread_join(klient, NULL);

    //uzavretie socketu <unistd.h>
    close(sock);
    printf("Spojenie so serverom bolo ukoncene.\n");
    
    return (EXIT_SUCCESS);
}