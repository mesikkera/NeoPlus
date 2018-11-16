#ifndef MESSAGEPACKET_H_
#define MESSAGEPACKET_H_

#include <stdint.h>
#include <string>
// #include <google/protobuf/message.h>
#include <google/protobuf/message_lite.h>
namespace neoplus {

	struct PacketHeader {
 		int32_t size;				// 패킷 사이즈(헤더 포함)
		int32_t encoding;			// 패킷 데이터 인코딩(헤더 제외)
		int32_t type;				// 패킷 타입 (request, response, notification, acknowledgement)
		int32_t reserved;
	};

	enum { PacketHeaderSize = sizeof(PacketHeader) };

	enum PacketType {
		RequestPacket, 
		ResponsePacket,
		NotificationPacket,
		AcknowledgementPacket
	};

	std::string PacketFromMessage(const google::protobuf::Message &message, PacketType type);
	PacketHeader PacketHeaderFromBytes(const void *data);
}
#endif	// MESSAGEPACKET_H_