#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include<SearchEngine.h>
#define TRUE   1
#define FALSE  0
#define MAX_PENDING_REQUESTS 20


class TcpSocketServer
{
public:
    TcpSocketServer(int p);
    virtual ~TcpSocketServer();
    void init();
    void startDaemon();

private:
    SearchEngine en;
    int opt = TRUE;
    int master_socket, addrlen, new_socket, client_socket[30],
        max_clients = 30, activity, valread, sd;

    int PORT;

    int max_sd;
    struct sockaddr_in address;

    char buffer[1025];

    fd_set readfds;

    char * message = "ECHO Daemon v1.0 \r\n";
};

#endif // TCPSOCKETSERVER_H
