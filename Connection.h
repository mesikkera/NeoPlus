#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "MessagePacket.h"

#include <boost/asio.hpp>

class Connection {
private:
	boost::asio::ip::tcp::socket socket;

	// 응답 처리(from Server)

	// 요청 처리(to Server)

public:
	Connection();
};

#endif