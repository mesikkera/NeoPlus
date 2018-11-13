//
//  MessagePacket.cpp
//  NeoPlus
//
//  Created by mesikkera on 13/11/2018.
//  Copyright © 2018 mesikkera. All rights reserved.
//

#include "MessagePacket.hpp"

namespace neoplus {
    void Int32ToBytes(int32_t n, char bytes[]) {
        bytes[0] = (n >> 24) & 0xff;
        bytes[1] = (n >> 16) & 0xff;
        bytes[2] = (n >> 8) & 0xff;
        bytes[3] = (n >> 0) & 0xff;
    }
    
    int32_t Int32FromBytes(const uint8_t bytes[]) {
        return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    }

    std::string PacketFromMessage(const ::google::protobuf::MessageLite &message, PacketType type) {
        // int size = message.ByteSize() + packetHeaderSize;
        int size = (int)message.ByteSizeLong() + packetHeaderSize;
        char header[packetHeaderSize];
        const int encoding = 0;
        
        Int32ToBytes(size, header + offsetof(PacketHeader, size));
        Int32ToBytes(encoding, header + offsetof(PacketHeader, encoding));
        Int32ToBytes(type, header + offsetof(PacketHeader, type));
        Int32ToBytes(0, header + offsetof(PacketHeader, reserved));
        std::string packet(header, sizeof(header));
        message.AppendToString(&packet);
        return packet;
    }
    
    PacketHeader PacketHeaderFromBytes(const void *data) {
        PacketHeader header;
        const uint8_t *bytes = static_cast<const uint8_t *>(data);
        
        header.size = Int32FromBytes(bytes + offsetof(PacketHeader, size));
        header.encoding = Int32FromBytes(bytes + offsetof(PacketHeader, encoding));
        header.type = Int32FromBytes(bytes + offsetof(PacketHeader, type));
        header.reserved = Int32FromBytes(bytes + offsetof(PacketHeader, reserved));
        
        return header;
    }
}
