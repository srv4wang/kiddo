#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "common.h"

namespace kiddo {

class Server {
public:
    Server():_fd_size(0),_custom_index(0) {}
    int io_td() {
        std::thread t(&kiddo::Server::io, this);
        t.join();
        return 0;
    }
    void io();

    int work();

    int work_callback();

    int listen_accept();

    struct Fd{
        int fd;
        int status;
    };

private:
    Fd _fd_queue[100];
    int _fd_size;
    unsigned int _epollfd;
    int _sockfd;
    std::atomic<int> _custom_index; 

};

int Server::work() {
    int cpu_core_num = get_cpu_core_num();
    int thread_num = 2 * cpu_core_num + 1;
    for (int i = 0; i < thread_num; ++i) {
        std::thread td(&kiddo::Server::work_callback, this);
        td.join();
    }
}
int Server::work_callback() {
    while (1) {
        Fd fdd = _fd_queue[_custom_index];
        _custom_index++;
        if (_custom_index > 0) {
            _custom_index = _custom_index/100;
        }
    }
    return 0;
}

int Server::listen_accept() {
    int fd;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    if ((fd = accept(_sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -2) {
        perror("accept");
        return 1;
    }
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event);
    // re add socketfd
    //struct epoll_event event;
    event.data.fd = _sockfd;
    event.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event);
    return 0;
}
void Server::io() {
    struct sockaddr_in server_addr;

    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error");
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2323);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);
    if (bind(_sockfd,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind socket error");
        exit(1);
    }
    if (listen(_sockfd, 10) == -1) {
        perror("listen");
        exit(1);
    }
    _epollfd = epoll_create(10);
    struct epoll_event event_list[10];
    int timeout = -1; // TODO
    while (1) {
        struct epoll_event event;
        event.data.fd = _sockfd;
        event.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
        epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &event);
        int ret = epoll_wait(_epollfd, event_list, 10, timeout);
        if (ret > 0) {
            for (int i = 0; i < ret; ++i) {
                struct epoll_event event = event_list[i];
                if (event.events & EPOLLIN) {
                    if (event.data.fd == _sockfd) {
                        listen_accept();
                    } else {
                        Fd f;
                        f.fd = event.data.fd;
                        f.status = 0;
                        _fd_queue[i++] = f;
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
    kiddo::Server srv;
    srv.io_td();
    
}

