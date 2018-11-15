#include "Connection.h"

#include <boost/bind.hpp>

using boost::asio::ip::tcp;

namespace neoplus {

	Connection::Connection(boost::asio::io_service& ios,
						   tcp::resolver::iterator iter) : _strand(ios), _socket(ios), _iterator(iter) {
		boost::asio::async_connect(_socket, _iterator, _strand);

	}

	// TODO: closing의 적절한 이름 다시 생각해볼것.
	// closeSocket()
	void Connection::closeSocket() {
		if (_socket.is_open()) {
			boost::system::error_code error;
			_socket.close(error);
		}
	}

	void Connection::close() {
		_strand.post(boost::bind(&Connection::closeSocket, this));
	}

	void Connection::sendRequest(message_ptr request) {
		_strand.post(boost::bind(&Connection::closeSocket, this, request));
	}
}