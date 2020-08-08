// AudioServer.cpp: 定义应用程序的入口点。
//

#include "System.h"
#include "Server.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using namespace std;

int main()
{
    boost::asio::io_service io_service;
    const boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), System::instance()->server_port());
    Server server(io_service, endpoint);
    server.open();
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
    while (1)
    {
        usleep(1000);
    }
    return 0;
}
