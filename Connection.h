#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "MessagePacket.h"

#include <boost/asio.hpp>
#include <deque>

namespace neoplus {

	using message_ptr = boost::shared_ptr<google::protobuf::Message>;
	
	// forward declation
	class Connection;

	
	class Endpoint {
	public:
		virtual ~Endpoint();

		virtual void connected(Connection *connection) = 0;
		virtual void connecionFailed(Connection *connection, const boost::system::error_code &error) = 0;
	};

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

		std::deque<std::string> _requestQueue;
		char _receivedHeader[neoplus::PacketHeaderSize];

		void preClose();
		
		// request to server.
		void handleSendQueuedRequest(const boost::system::error_code &error);
		void sendQueuedRequest();
		void preSendRequest(message_ptr request);

		// response from server.
		void handleConnection(const boost::system::error_code &error);
		void ReadPacketHeader();
		
		Endpoint *_endpoint;

	};
}


#endif