#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include "sync_client.h"
#include <QLineEdit>

using namespace boost::asio;


io_service service;
boost::recursive_mutex cs;

ip::tcp::endpoint ep = ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 8001);
std::unique_ptr<talk_to_svr> client = std::make_unique<talk_to_svr>("0");

//std::unique_ptr<QLineEdit> inputMessage;
