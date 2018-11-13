//
//  MessagePacket.hpp
//  NeoPlus
//
//  Created by mesikkera on 13/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#ifndef MessagePacket_hpp
#define MessagePacket_hpp

#include <stdint.h>
#include <string>
#include <google/protobuf/message.h>
#include <google/protobuf/message_lite.h>

namespace neoplus {
    
    struct PacketHeader {
        int32_t size;               // 패킷 사이즈 (헤더 포함)
        int32_t encoding;           // 패킷 인코딩 (헤더 제외)
        int32_t type;               // 패킷 타입
        int32_t reserved;
    };
    
    enum { packetHeaderSize = sizeof(PacketHeader) };
    
    enum PacketType {
        requestPacket,
        responsePacket,
        notificationPacket,
        acknowledgementPacket
    };
    
    std::string PacketFromMessage(const ::google::protobuf::MessageLite &message, PacketType type);
    PacketHeader PacketHeaderFromBytes(const void *data);
}

#endif /* MessagePacket_hpp */
