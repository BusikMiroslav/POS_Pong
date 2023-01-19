#include "server.h"


int server_main(int argc, char** argv) {
    if (argc < 2) {
        printError("Sever je nutne spustit s nasledujucimi argumentmi: port.");
    }
    int port = atoi(argv[1]);
	if (port <= 0) {
		printError("Port musi byt cele cislo vacsie ako 0.");
	}

    //vytvorenie TCP socketu <sys/socket.h>
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printError("Chyba - socket.");        
    }
    
    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;         //internetove sockety
    serverAddress.sin_addr.s_addr = INADDR_ANY; //prijimame spojenia z celeho internetu
    serverAddress.sin_port = htons(port);       //nastavenie portu
    
    //prepojenie adresy servera so socketom <sys/socket.h>
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - bind.");
    }
    
    //server bude prijimat nove spojenia cez socket serverSocket <sys/socket.h>
    listen(serverSocket, 10);
    //server caka na pripojenie klienta <sys/socket.h>
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket1 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    int clientSocket2 = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    //uzavretie pasivneho socketu <unistd.h>
    close(serverSocket);
    if (clientSocket1 < 0 || clientSocket2 < 0) {
        printError("Chyba - accept.");
    }

    printf("Klient sa pripojil na server.\n");

    char buff[2] = { 0 };
    buff[0] = 1;
    write(clientSocket1, buff, 2);
    buff[0] = 2;
    write(clientSocket2, buff, 2);


    pthread_mutex_t mut;
    pthread_mutex_init(&mut, NULL);
    ThrData data = { 20,100,1,0,15,15,0,0,clientSocket2, clientSocket1, 1,0, 0, &mut};
    pthread_t server;
    pthread_create(&server, NULL, hraj, &data);
    pthread_t hrac_jedna;
    pthread_t hrac_dva;
    pthread_create(&hrac_jedna, NULL, prepocetPozicii, &data);
    sleep(1);
    pthread_mutex_lock(data.mutex);
    data.jePrvy = 2;
    pthread_mutex_unlock(data.mutex);
    pthread_create(&hrac_dva, NULL, prepocetPozicii, &data);

    pthread_join(server, NULL);
    pthread_join(hrac_jedna, NULL);
    pthread_join(hrac_dva, NULL);

    printf("Klient ukoncil komunikaciu.\n");
    
    //uzavretie socketu klienta <unistd.h>
    close(clientSocket1);
    close(clientSocket2);
    
    return (EXIT_SUCCESS);
}