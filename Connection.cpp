#include "Connection.h"

#include <boost/bind.hpp>

using boost::asio::ip::tcp;

namespace neoplus {

	Connection::Connection(boost::asio::io_service& ios,
						   tcp::resolver::iterator iter) : _strand(ios), _socket(ios), _iterator(iter) {
		boost::asio::async_connect(_socket, _iterator, _strand);

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


	void Connection::sendRequest(message_ptr request) {
		_strand.post(boost::bind(&Connection::sendRequest, this, request));
	}

	void Connection::preSendRequest(message_ptr request) {
		bool idle = requestQueue.empty();
		requestQueue.push_back(neoplus::PacketFromMessage(*request, neoplus::RequestPacket));
		if(idle) {
			boost::asio::async_write(_socket, boost::asio::buffer(requestQueue.front()), 
									 _strand.wrap(boost::bind(&Connection::handleSendQueuedRequest, this, boost::asio::placeholders::error)));
		}		

	}

	void Connection::handleSendQueuedRequest(const boost::system::error_code &error) {
		if (!error) {
			requestQueue.pop_front();
			if(!requestQueue.empty()) {
				// send request in queue
			}
		} else {
			preClose();
		}
	}

	void Connection::sendQueuedRequest() {
		boost::asio::async_write(_socket, boost::asio::buffer(requestQueue.front()), 
								 _strand.wrap(boost::bind(&Connection::handleSendQueuedRequest, this, boost::asio::placeholders::error)));
	}

}