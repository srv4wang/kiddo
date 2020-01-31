#pragma once
#include <atomic>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <netinet/in.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "ring_buffer.h"

namespace kiddo {

int g_sockfd = 0;

struct Request {
    char data[4096];
};
struct Response {
    char data[4096];
};

using work_fun_t = std::function<int(Request&, Response&)>;

class Server {
public:
    Server(const char* host, int port=2605):
            _epollfd(0), _sockfd(0),_host(host), _port(port), _work_fun(nullptr) {
    }
    Server(int port):Server(nullptr, port) {}
    ~Server();
    int io_td() {
        std::thread t(&kiddo::Server::io_cb, this);
        t.join();
        return 0;
    }
    void io_cb();

    int work();

    int work_cb();

    int listen_accept();

    void set_work_fun(work_fun_t& fun) {
        _work_fun = fun;
    }

private:
    RingBuffer<int> _fd_queue;
    unsigned int _epollfd;
    int _sockfd;
    const char* _host;
    int _port;

    static thread_local Request _request;
    static thread_local Response _response;

    work_fun_t _work_fun;
};
thread_local Request Server::_request;
thread_local Response Server::_response;

} // end of namespace
