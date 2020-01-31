#include <arpa/inet.h>

#include "server.h"
namespace kiddo {

void exit(int sig) {
    printf("rec signl:%d\n", sig);
    printf("close socket");
    shutdown(g_sockfd, SHUT_RDWR);
    close(g_sockfd);
    signal(sig, SIG_DFL);
    raise(sig);
}


Server::~Server() {
}


int Server::work() {
    int cpu_core_num = get_cpu_core_num();
    int thread_num = 2 * cpu_core_num + 1;
    for (int i = 0; i < thread_num; ++i) {
        std::thread td(&kiddo::Server::work_cb, this);
        td.detach();
    }
}
int Server::work_cb() {
    while (1) {
        int index = _fd_queue.get();
        if (index < 0) {
            continue;
        }
        int fd = _fd_queue[index];
        _fd_queue[index] = 0;
        if (fd == 0) {
            continue;
        }
        _fd_queue[index] = 0;
        memset(_request.data, 0, sizeof(_request.data));
        memset(_response.data, 0, sizeof(_response.data));

        char *recv_buff = _request.data;
        char *send_buff = _response.data;
        // recv 
        int num = recv(fd, recv_buff, sizeof(recv_buff), 0);
        if (recv_buff[0] == '\0') {
            continue;
        }
        // work for business logic
        if (_work_fun != NULL) {
            _work_fun(_request, _response);
        }
        // send
        printf("%s\n", send_buff);
        send(fd, send_buff, sizeof(send_buff), 0);
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
    return 0;
}
void Server::io_cb() {
    struct sockaddr_in server_addr;

    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error");
        perror("socket");
        return;
    }
    int reuse = 1;
    setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(int));

    g_sockfd = _sockfd;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    if (_host == nullptr) {
        server_addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        server_addr.sin_addr.s_addr = inet_addr(_host);
    }
    bzero(&(server_addr.sin_zero), 8);
    if (bind(_sockfd,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind socket error");
        return;
    }
    if (listen(_sockfd, 10) == -1) {
        perror("listen");
        return;
    }
    _epollfd = epoll_create(10);
    struct epoll_event event_list[10];
    while (1) {
        struct epoll_event event;
        event.data.fd = _sockfd;
        event.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
        epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &event);
        int ret = epoll_wait(_epollfd, event_list, 10, -1);
        if (ret > 0) {
            for (int i = 0; i < ret; ++i) {
                struct epoll_event event = event_list[i];
                if (event.events & EPOLLIN) {
                    if (event.data.fd == _sockfd) {
                        listen_accept();
                    } else {
                        _fd_queue.put(event.data.fd);
                    }
                }
            }

        } else  {
            // 0:no event or -1:event failed
        }
    }

}

} // end of namespace
