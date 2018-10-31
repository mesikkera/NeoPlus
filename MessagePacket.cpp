/*
 *	MessagePacket.cpp
 */

#include <stdint.h>
#include <string>
#include <google/protobuf/message.h>

struct PacketHeader {
	int32_t size;					// 패킷 전체 사이즈 (메세지 패킷 + 패킷 헤더)
	int32_t encoding;				// 패킷 데이터 인코딩 (헤더 제외)
	int32_t type;					// 메세지 타입
	int32_t reserved;
};

enum { PacketHeaderSize = sizeof(PacketHeader) };

enum PacketType {
	RequestPacket,
	ResponsePacket, 
	NotificationPacket,
	AcknowledgementPacket
};

static inline void Int32ToBytes(int32_t n, char bytes[]) {
	bytes[0] = (n >> 24) & 0xff;
	bytes[1] = (n >> 16) & 0xff;
	bytes[2] = (n >> 8) & 0xff;
	bytes[3] = (n >> 0) & 0xff;
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

// TODO: PacketHeaderFromBytes
// TODO: PacketFromMessage
// TODO: google protocol buffer


