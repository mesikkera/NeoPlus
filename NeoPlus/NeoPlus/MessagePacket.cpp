//
//  MessagePacket.cpp
//  NeoPlus
//
//  Created by mesikkera on 07/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#include "MessagePacket.hpp"

#include <stdint.h>

namespace neoplus {
    
    static inline void Int32ToBytes(int32_t n, char bytes[]) {
        bytes[0] = (n >> 24) & 0xff;
        bytes[1] = (n >> 16) & 0xff;
        bytes[2] = (n >> 8) & 0xff;
        bytes[3] = (n >> 0) & 0xff;
    }
    
    static inline int32_t Int32FromByte(const uint8_t bytes[]) {
        return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    }

    ::std::string PacketFromMessage(const ::google::protobuf::Message &message, PacketType type)
    {
        int size = message.ByteSize() + PacketHeaderSize;
        const int encoding = 0;
        char header[PacketHeaderSize];
        
        Int32ToBytes(size, header + offsetof(PacketHeader, size));
        Int32ToBytes(encoding, header + offsetof(PacketHeader, type));
        Int32ToBytes(type, header + offsetof(PacketHeader, type));
        Int32ToBytes(0, header + offsetof(PacketHeader, reserved));
        
        std::string packet(header, sizeof(header));
        message.AppendToString(&packet);
        
        return packet;
    }
    
    PacketHeader PacHeaderFromBytes(const void *data) {
        PacketHeader header;
        const uint8_t *bytes = static_cast<const uint8_t *>(data);
        
        header.size = Int32FromByte(bytes + offsetof(PacketHeader, size));
        header.encoding = Int32FromByte(bytes + offsetof(PacketHeader, encoding));
        header.type = Int32FromByte(bytes + offsetof(PacketHeader, type));
        header.reserved = Int32FromByte(bytes + offsetof(PacketHeader, reserved));
        
        return header;
    }

}    /* namespace neoplus */

