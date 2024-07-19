#pragma once

#define CPACKET_HEAD_SIZE sizeof(SPacketHead)
#define CPACKET_BODY_SIZE 2048
#define CPACKET_PACK_SIZE (CPACKET_HEAD_SIZE + CPACKET_BODY_SIZE + 1)

// 设计包的时候需要考虑内存对其的问题
// 设置字节对其的方式为1
#pragma pack(push, 1)

typedef struct SPacketHead
{
	WORD m_sHead;		// 固定头部 0xFEFF
	DWORD m_nLength;	// 整个包的长度
	WORD m_sCmd;		// 控制命令
	WORD m_sSum;		// 校验和

} SPacketHead;

typedef struct SPacket
{
	SPacket();
	SPacket(WORD sCmd, const BYTE* pData, size_t nSize);
	SPacket(const BYTE* pData, int& nSize);
	SPacket(const SPacket &packet);
	SPacket& operator=(const SPacket& packet);
	~SPacket();

	SPacketHead m_sHead;
	char m_szData[CPACKET_BODY_SIZE];
} SPacket;

// 恢复默认的字节对其方式
#pragma pack(pop)