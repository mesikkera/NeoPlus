#include "Connection.h"

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>

using boost::asio::ip::tcp;

namespace neoplus {

	Connection::Connection(boost::asio::io_service& ios,
						   tcp::resolver::iterator iter,
						   Endpoint *endpoint) : _strand(ios), _socket(ios), _iterator(iter), _endpoint(endpoint) {
		boost::asio::async_connect(_socket, _iterator, _strand.wrap(boost::bind(&Connection::handleConnection, this,
																	boost::asio::placeholders::error)));
	}

	void Connection::close() {
		_strand.post(boost::bind(&Connection::preClose, this));
	}

	void Connection::preClose() {
		if (_socket.is_open()) {
			boost::system::error_code error;
			_socket.close(error);
		}
	}

	// Methods for handling request.
	void Connection::sendRequest(message_ptr request) {
		_strand.post(boost::bind(&Connection::preSendRequest, this, request));
	}

	void Connection::preSendRequest(message_ptr request) {
		bool idle = _requestQueue.empty();
		_requestQueue.push_back(neoplus::PacketFromMessage(*request, neoplus::RequestPacket));
		if(idle) {
			boost::asio::async_write(_socket, boost::asio::buffer(_requestQueue.front()), 
									 _strand.wrap(boost::bind(&Connection::handleSendQueuedRequest, this, boost::asio::placeholders::error)));
		}		

	}

	void Connection::handleSendQueuedRequest(const boost::system::error_code &error) {
		if (!error) {
			_requestQueue.pop_front(); 
			if(!_requestQueue.empty()) {
				// send request in queue
			}
		} else {
			preClose();
		}
	}

	void Connection::sendQueuedRequest() {
		boost::asio::async_write(_socket, boost::asio::buffer(_requestQueue.front()), 
								 _strand.wrap(boost::bind(&Connection::handleSendQueuedRequest, this, boost::asio::placeholders::error)));
	}

	// Methods for handling response.
	void Connection::handleConnect(const boost::system::error_code &error) {
		// 1. !error: endpoint --> connected
		// 						   start read packet header
		// 2. error: endpoint --> connection failed 
		// 						  sleep 
		//					      retry
		if(!error) {
			if(_endpoint) {
				_endpoint -> connected(this);
			} 
			// read packet
			readPacket();
		} else {
			if(_endpoint) {
				_endpoint -> connecionFailed(this, error);
			}
			_socket.close();
			boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
			boost::asio::async_connect(_socket, _iterator,
									   _strand.wrap(boost::bind(&Connection::handleConnect, this, boost::asio::placeholders::error)));
		}

	}

	void Connection::readPacket() {
		// socket
		// received packet header 
		// packet header size
		// _strand.wrap(read Packet header, this, boost::asio::placeholer::error)));
		// read Packet Header / read Packet Body ==> readingPacketHeader / readingPacketBody
		// readingPacketHeader(const boost::system::error_code& error);
		// readingPacketBody(const boost::system::error_code& error);
		// _strand.wrap(ReadingPacketHeader, this, boost::asio::placeholer::error));

		// boost::asio::async_read(AsyncReadStream &s, const MutableBufferSequence &buffers, ReadHandler &&handler)

		// _socket
		// boost::asio::buffer(_receivedHeaderBuffer, neoplus::PacketHeaderSize)
		// strand_.wrap(boost::bind(&Connection::handleReadHeader, this, boost::asio::placeholders::error)));
		boost::asio::async_read(_socket, 
								boost::asio::buffer(_receivedHeaderBuffer, neoplus::PacketHeaderSize),
			                    _strand.wrap(boost::bind(&Connection::readPacketHeader, this, boost::asio::placeholders::error)));

	}

	void Connection::readPacketHeader(const boost::system::error_code &error) {
		if (!error) { 
			_receivedHeader = neoplus::PacketHeaderFromBytes(_receivedHeaderBuffer);
			
			int32_t packetBodySize = _receivedHeader.size - neoplus::PacketHeaderSize;
			if(packetBodySize > 0 && _receivedHeader.encoding == 0) {
				char *buffer = new char[packetBodySize];
				boost::asio::async_read(_socket,
										boost::asio::buffer(buffer, packetBodySize),
										_strand.wrap(boost::bind(&Connection::readPacketBody)
											         this,
											         boost::asio::placeholders::error,
											         buffer,
											         static_cast<neoplus::PacketType>(_receivedHeader.type)))
			}

		} else {
			std::cerr << "Error Occurred in Connection::readPacketHeader(): " << error.message() << std::endl;
			preClose();
		}
	}

	void Connection::readPacketBody(const boost::system::error_code &error, neoplus::PacketType packetType) {
		if(!error) {
			if(packetType == neoplus::ResponsePacket) {
				// process Response Packet
				_endpoint -> processResponse(this);
			} else if (packetType == neoplus::NotificationPacket) {
				// process Notification Packet
				_endpoint -> processRequest(this);
			} else {
				// error message => unknown or wrong
				std::cerr << "Error Occurred in Connection::readPacketBody()::Unknown or Wrong packet type ==> " << _receivedHeader.type;
			}
			// read packet
		} else {
			std::cerr <<"Error Occurred in Connection::readPacketBody(): " << error.message() << std::endl;
			preClose();
		}
	}
} 