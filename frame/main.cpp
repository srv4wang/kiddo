#include "server.h"

int main() {
    //signal(SIGINT, kiddo::exit);
    //signal(SIGQUIT, kiddo::exit);
    kiddo::Server srv("127.0.0.1");
    kiddo::work_fun_t echo = [](kiddo::Request& req, kiddo::Response& res){memcpy(res.data, req.data, sizeof(res.data));return 1;};
    kiddo::timer_fun_t timer = [](kiddo::Server*){std::cout<<time(NULL)<<std::endl;return 1;};
    srv.set_work_fun(echo);
    srv.set_timer_fun(10, timer);
    srv.run();
}
