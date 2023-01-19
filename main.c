#include "klient.h"
#include "server.h"


int main(int argc, char * argv[]) {
    initscr();
    char * newargv[argc];
    newargv[0] = argv[0];
    for(int i = 2; i <= argc; i++) {
        newargv[i-1] = argv[i];
    }

    if(strcmp(argv[1],"server") == 0) {
        server_main(argc - 1, newargv);
    } else if(strcmp(argv[1],"klient") == 0) {
        klient_main(argc - 1, newargv);
    } else {
        fprintf(stderr, "zle zadané argumenty");
    }
    endwin();
}