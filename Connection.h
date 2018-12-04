//
//  Connection.h
//  NeoPlus
//
//  Created by mesikkera on 23/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "MessagePacket.h"

#include <boost/asio.hpp>
#include <deque>

namespace neoplus {

	using message_ptr = boost::shared_ptr<google::protobuf::Message>;
	
	// forward declation
	class Connection;

	// interface for delegate of Endpoint
	class Endpoint {
	public:
		virtual ~Endpoint();

		virtual void connected(Connection *connection) = 0;
		virtual void connecionFailed(Connection *connection, const boost::system::error_code &error) = 0;

		virtual void processResponse(Connection *connection) = 0;
		virtual void processRequest(Connection *connection) = 0;
	};

	class Connection {
	public:
		Connection(boost::asio::io_service& ios,
				   boost::asio::ip::tcp::resolver::iterator iter);
		
		void close();
		void sendRequest(message_ptr request);

	private:
		Connection(const Connection&);
		void operator=(const Connection&);

		boost::asio::io_service::strand _strand;
		boost::asio::ip::tcp::socket _socket;
		boost::asio::ip::tcp::resolver::iterator _iterator;

		std::deque<std::string> _requestQueue;
		char _receivedHeaderBuffer[neoplus::PacketHeaderSize];
		neoplus::PacketHeader _receivedHeader;

		void preClose();
		 
		// request to server.
		void handleSendQueuedRequest(const boost::system::error_code &error);
		void sendQueuedRequest();
		void preSendRequest(message_ptr request);

		// response from server.
		void handleConnect(const boost::system::error_code &error);
		void readPacket();
		void readPacketHeader(const boost::system::error_code &error);
		void readPacketBody(const boost::system::error_code &error, neoplus::PacketType packetType);

		Endpoint *_endpoint;

	};
}


#endif