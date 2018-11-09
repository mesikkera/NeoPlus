//
//  Connection.hpp
//  NeoPlus
//
//  Created by mesikkera on 09/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#ifndef Connection_hpp
#define Connection_hpp

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "MessagePacket.hpp"

namespace neoplus {
    // typedef boost::shared_ptr<::google::protobuf::Message> MessagePtr;
    using MessagePtr = boost::shared_ptr<::google::protobuf::Message>;

    class Connection {
    private:
        boost::asio::ip::tcp::socket socket;
        boost::asio::io_service::strand strand;
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator;
        
        
    // Methods
    private:
        // response from server.
        
        // request to server.
    public:
        Connection();
        
    };
 
}

#endif /* Connection_hpp */
