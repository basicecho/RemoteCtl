#pragma once

#define CPACKET_HEAD_SIZE sizeof(SPacketHead)
#define CPACKET_BODY_SIZE 2048
#define CPACKET_PACK_SIZE (CPACKET_HEAD_SIZE + CPACKET_BODY_SIZE + 1)

// ��ư���ʱ����Ҫ�����ڴ���������
// �����ֽڶ���ķ�ʽΪ1
#pragma pack(push, 1)

typedef struct SPacketHead
{
	WORD m_sHead;		// �̶�ͷ�� 0xFEFF
	DWORD m_nLength;	// �������ĳ���
	WORD m_sCmd;		// ��������
	WORD m_sSum;		// У���

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

// �ָ�Ĭ�ϵ��ֽڶ��䷽ʽ
#pragma pack(pop)