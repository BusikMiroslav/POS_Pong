all: klient server

clean:
	rm -f klient server

klient: k_s_definitions.c k_s_definitions.h klient.c klient.h pong.c pong.h
	gcc	$^ -o $@ -lncurses -pthread

server: k_s_definitions.c k_s_definitions.h server.c server.h pong.c pong.h
	gcc $^ -o $@ -lncurses -pthread


