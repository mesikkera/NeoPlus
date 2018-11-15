#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "MessagePacket.h"

#include <boost/asio.hpp>

namespace neoplus {

using message_ptr = boost::shared_ptr<google::protobuf::Message>;
	
	class Connection {
	public:
		Connection(boost::asio::io_service& ios,
				   boost::asio::ip::tcp::resolver::iterator iter);
		
		void close();
		void sendRequest(message_ptr request);
		
	private:
		boost::asio::io_service::strand _strand;
		boost::asio::ip::tcp::socket _socket;
		boost::asio::ip::tcp::resolver::iterator _iterator;

		void closeSocket();

	};
}


#endif