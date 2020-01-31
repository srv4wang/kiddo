#include "server.h"
int main() {
    //signal(SIGINT, kiddo::exit);
    //signal(SIGQUIT, kiddo::exit);
    kiddo::Server srv;
    kiddo::work_fun_t echo = [](kiddo::Request& req, kiddo::Response& res){memcpy(res.data, req.data, sizeof(res.data));return 1;};
    srv.set_work_fun(echo);
    srv.run();
}
