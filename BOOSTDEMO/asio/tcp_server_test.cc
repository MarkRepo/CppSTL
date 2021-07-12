#include <gtest/gtest.h>

#include <string>
#include <boost/asio.hpp>
#include <ctime>
#include <iostream>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
  using namespace std;
  time_t now = time(0);
  return ctime(&now) + std::string(" markfqwu");
}

TEST(TCPSERVER, server) {
  try {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));
    for(;;) {
      tcp::socket socket(io_context);
      acceptor.accept(socket);
      std::string msg = make_daytime_string();
      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(msg), ignored_error);
    }
  }catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}