#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <iostream>
#include <thread>
#include <unordered_map>

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

class Server;
using timer_fun_t = std::function<int(kiddo::Server*)>;
class TimerTask {
public:
    TimerTask(long time_interval, timer_fun_t fun):interval(time_interval),execute(fun){}

    long interval;
    timer_fun_t execute;
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

    void set_timer_fun(long time_interval, timer_fun_t& fun) {
        TimerTask tt(time_interval, fun);
        long task_start_time = time(NULL);
        _timer_queue.insert(std::make_pair(task_start_time, tt));
    }

private:
    // polling within IO thread
    int polling();

    void poll_cb();

    // working threads
    int working();

    void work_cb();

    // timing thread
    int timing();

    void time_cb();

    int listen_accept();

    virtual void initialize() {};
    virtual void finalize() {};

private:
    RingBuffer<int> _fd_queue;
    std::multimap<long, TimerTask> _timer_queue;
    std::condition_variable _has_timer_task;
    std::mutex _timer_mutex;

    unsigned int _epollfd;
    int _sockfd;
    const char* _host;
    int _port;

    work_fun_t _work_fun;

    static thread_local Request _request;
    static thread_local Response _response;

};
thread_local Request Server::_request;
thread_local Response Server::_response;

} // end of namespace
