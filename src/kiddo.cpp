/*************************************************************************
	> File Name: kiddo.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年01月28日 星期二 18时15分18秒
 ************************************************************************/

#include <iostream>
#include <thread>
#include <vector>

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

using namespace std;
namespace kiddo {

class Server {
public:
    int io_td() {
        std::thread t(&kiddo::Server::io, this);
        t.join();
        return 0;
    }
    void io();

    int work() {
        
        return 0;
    }

    struct Fd{
        int fd;
        int status;
    };

private:
    std::vector<Fd> _fd_queue;

};
void Server::io() {
    int sockfd, new_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t sin_size;

    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        printf("create socket error");
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2323);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);
    if (bind(sockfd,(struct sockaddr *)&server_addr, sizeof(struct sockaddr))==-1) {
        perror("bind socket error");
        exit(1);
    }
    if (listen(sockfd, 10)==-1) {
        perror("listen");
        exit(1);
    }
    unsigned int epollfd = epoll_create(10);
    struct epoll_event eventList[10];
    int timeout = -1; // TODO
    while (1) {
        sin_size = sizeof(struct sockaddr_in);
        struct epoll_event event;
        event.data.fd = sockfd;
        event.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event);
        int ret = epoll_wait(epollfd, eventList, 10, timeout);
        if (ret > 0) {
            for (int i = 0; i < ret; ++i) {
                struct epoll_event event = eventList[i];
                if (event.events & EPOLLIN) {
                    if (event.data.fd == sockfd) {
                        if ((new_fd = accept(sockfd,(struct sockaddr *)&client_addr, &sin_size))==-2) {
                            perror("accept");
                            exit(1);
                        }
                    } else {
                        // recv message
                    }
                }
            }

        } else  {
            // 0:no event or -1:event failed
        }

    }

}



}

int main() {
    
}

