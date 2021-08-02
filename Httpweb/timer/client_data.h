#ifndef TEST2_CLIENT_DATA_H
#define TEST2_CLIENT_DATA_H

#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 64

//class util_timer;

class tw_timer;
//class heap_timer;
struct client_data {
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    union {
        tw_timer *timer;
        //        util_timer *timer;
        //        heap_timer*timer3;
    };
};

#endif //TEST2_CLIENT_DATA_H


