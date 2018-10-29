/*
 *	MessagePacket.cpp
 */

#include <stdint.h>

struct PacketHeader {
	int32_t size;					// 패킷 전체 사이즈 (메세지 패킷 + 패킷 헤더)
	int32_t encoding;				// 패킷 데이터 인코딩 (헤더 제외)
	int32_t type;					// 메세지 타입
	int32_t reserved;
};