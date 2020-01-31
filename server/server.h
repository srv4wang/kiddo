#pragma once
#include <atomic>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "common.h"
#include "ring_buffer.h"
#include "singleton.h"

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
    Server(int port=2605):Server(nullptr, port) {}

    virtual ~Server();

    int run();

    void set_work_fun(work_fun_t& fun) {
        _work_fun = fun;
    }

private:
    // polling within IO thread
    int polling();

    void poll_cb();

    // working threads
    int working();

    void work_cb();

    int listen_accept();

    virtual void initialize() {};
    virtual void finalize() {};

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
