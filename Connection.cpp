#include "Connection.h"

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>

using boost::asio::ip::tcp;

namespace neoplus {

	Connection::Connection(boost::asio::io_service& ios,
						   tcp::resolver::iterator iter) : strand_(ios), socket_(ios), iterator_(iter) {
		boost::asio::async_connect(socket_, iterator_, strand_.wrap(boost::bind(&Connection::handleConnect, this,
																	boost::asio::placeholders::error)));
	}

	// close connection
	void Connection::close() {
		strand_.post(boost::bind(&Connection::closeSocket, this));
	}

	void Connection::closeSocket() {
		if (socket_.is_open()) {
			boost::system::error_code error;
			socket_.close(error);
		}
	}

	// Methods for handling request.
	void Connection::sendRequest(message_ptr request) {
		strand_.post(boost::bind(&Connection::preSendRequest, this, request));
	}

	// preprocess send request
	void Connection::preSendRequest(message_ptr request) {
		bool idle = requestQueue_.empty();
		requestQueue_.push_back(neoplus::PacketFromMessage(*request, neoplus::RequestPacket));
		if(idle) {
			boost::asio::async_write(socket_, boost::asio::buffer(requestQueue_.front()), 
									 strand_.wrap(boost::bind(&Connection::handleSendQueuedRequest, this, boost::asio::placeholders::error)));
		}		
	}

	void Connection::handleSendQueuedRequest(const boost::system::error_code &error) {
		if (!error) {
			requestQueue_.pop_front(); 
			if(!requestQueue_.empty()) {
				sendQueuedRequest();
			}
		} else {
			closeSocket();
		}
	}

	void Connection::sendQueuedRequest() {
		boost::asio::async_write(socket_, boost::asio::buffer(requestQueue_.front()), 
								 strand_.wrap(boost::bind(&Connection::handleSendQueuedRequest, this, boost::asio::placeholders::error)));
	}

	// Methods for handling response.
	void Connection::handleConnect(const boost::system::error_code &error) {
		if(!error) {
			if(endpoint_) {
				endpoint_ -> connected(this);
			} 
			readPacket();
		} else {
			if(endpoint_) {
				endpoint_ -> connecionFailed(this, error);
			}
			socket_.close();
			boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
			boost::asio::async_connect(socket_, iterator_,
									   strand_.wrap(boost::bind(&Connection::handleConnect, this, boost::asio::placeholders::error)));
		}

	}

	void Connection::readPacket() {
		boost::asio::async_read(socket_, 
								boost::asio::buffer(receivedHeader_Buffer, neoplus::PacketHeaderSize),
			                    strand_.wrap(boost::bind(&Connection::readPacketHeader, this, boost::asio::placeholders::error)));
	}

	void Connection::readPacketHeader(const boost::system::error_code &error) {
		if (!error) { 
			receivedHeader_ = neoplus::PacketHeaderFromBytes(receivedHeader_Buffer);
			
			int32_t packetBodySize = receivedHeader_.size - neoplus::PacketHeaderSize;
			if(packetBodySize > 0 && receivedHeader_.encoding == 0) {
				char *buffer = new char[packetBodySize];
				boost::asio::async_read(socket_, boost::asio::buffer(buffer, packetBodySize),
										strand_.wrap(boost::bind(&Connection::readPacketBody,
											         this,
											         boost::asio::placeholders::error,
											         buffer,
											         static_cast<neoplus::PacketType>(receivedHeader_.type))));
			}

		} else {
			std::cerr << "Error Occurred in Connection::readPacketHeader(): " << error.message() << std::endl;
			closeSocket();
		}
	}

	void Connection::readPacketBody(const boost::system::error_code &error, const char *buffer, neoplus::PacketType packetType) {
		if(!error) {
			if(packetType == neoplus::ResponsePacket) {
				// process Response Packet
				endpoint_ -> processResponse(this);
			} else if (packetType == neoplus::NotificationPacket) {
				// process Notification Packet
				endpoint_ -> processRequest(this);
			} else {
				// error message => unknown or wrong
				std::cerr << "Error Occurred in Connection::readPacketBody()::Unknown or Wrong packet type ==> " << receivedHeader_.type;
			}
			// read packet
		} else {
			std::cerr <<"Error Occurred in Connection::readPacketBody(): " << error.message() << std::endl;
			closeSocket();
		}
	}
} 