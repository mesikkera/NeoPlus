#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "MessagePacket.h"

#include <boost/asio.hpp>

namespace neoplus {
	
	class Connection {
	public:
		Connection(boost::asio::io_service& io_service);
	};
}


#endif