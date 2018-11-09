//
//  MessagePacket.hpp
//  NeoPlus
//
//  Created by mesikkera on 07/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#ifndef MessagePacket_hpp
#define MessagePacket_hpp

#include <stdint.h>
#include <string>
#include <google/protobuf/message.h>

namespace neoplus {
    
    struct PacketHeader {
        int32_t size;
        int32_t encoding;
        int32_t type;
        int32_t reserved;
    };

    enum {PacketHeaderSize = sizeof(PacketHeader)};

    enum PacketType {
        RequestPacket,
        ResponsePacket,
        NotificationPacket,
        AcknowledgementPacket
    };
    
    ::std::string PacketFromMessage(const ::google::protobuf::Message &message, PacketType type);
    PacketHeader PacketHeaderFromBytes(const void *data);

}   /* namespace neoplus */

#endif /* MessagePacket_hpp */
