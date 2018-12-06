//
//  Session.h
//  NeoPlus
//
//  Created by mesikkera on 29/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#ifndef SESSION_H_
#define SESSION_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "MessagePacket.h"

namespace neoplus {
	
 	using message_ptr = boost::shared_ptr<::google::protobuf::Message>;
 	
	class Session : public boost::enable_shared_from_this<Session> {
	public:
		Session(boost::asio::io_service &ios);
		~Session();

	private:

	};
}

#endif