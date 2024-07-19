#include "pch.h"
#include "Packet.h"

SPacket::SPacket()
{
	m_sHead.m_sHead = 0xFEFF;
	m_sHead.m_nLength = 0;
	m_sHead.m_sCmd = 0;
	m_sHead.m_sSum = 0;
}

SPacket::SPacket(WORD sCmd, const BYTE* pData, size_t nSize)
{
	m_sHead.m_sHead = 0xFEFF;
	m_sHead.m_nLength = nSize;
	m_sHead.m_sCmd = sCmd;

	if (nSize > 0)
	{
		memcpy(m_szData, pData, nSize);
		m_szData[nSize] = '\0';
		m_sHead.m_sSum = 0;
		for (size_t i = 0; i < nSize; i++)
		{
			m_sHead.m_sSum += BYTE(pData[i] & 0xFF);
		}
	}
	else
	{
		m_szData[0] = '\0';
		m_sHead.m_nLength = 0;
		m_sHead.m_sSum = 0;
	}
}

SPacket::SPacket(const BYTE* pData, int& nSize)
{
	size_t i = 0;
	for (; i + CPACKET_HEAD_SIZE < nSize; i++)
	{
		WORD head = *(WORD*)(pData + i);
		if (head == 0xFEFF)
		{
			break;
		}
	}

	// 判断是否找到起点
	if (i + CPACKET_HEAD_SIZE >= nSize)
	{
		nSize = 0;
		return;
	}

	m_sHead.m_sHead = 0xFEFF;
	m_sHead.m_nLength = *(DWORD*)(pData + i + 2);
	// 对包的长度进行判断
	if (i + m_sHead.m_nLength > nSize)
	{
		nSize = 0;
		m_sHead.m_nLength = 0;
		return;
	}

	// 数据拷贝
	m_sHead.m_sCmd = *(WORD*)(pData + i + 6);
	memcpy(m_szData, pData + CPACKET_HEAD_SIZE, m_sHead.m_nLength);
	m_szData[m_sHead.m_nLength] = '\0';
	
	// 检验校验码
	m_sHead.m_sSum = 0;
	for (int i = 0; i < m_sHead.m_nLength; i++)
	{
		m_sHead.m_sSum += BYTE(m_szData[i]) & 0xFF;
	}

	WORD sum = *(WORD*)(pData + 8);
	if (m_sHead.m_sSum != sum)
	{
		nSize = 0;
		m_sHead.m_nLength = 0;
	}
}

SPacket::SPacket(const SPacket& packet)
{
	m_sHead.m_sHead = packet.m_sHead.m_sHead;
	m_sHead.m_nLength = packet.m_sHead.m_nLength;
	m_sHead.m_sCmd = packet.m_sHead.m_sCmd;
	m_sHead.m_sSum = packet.m_sHead.m_sSum;
	memcpy(m_szData, packet.m_szData, packet.m_sHead.m_nLength);
	m_szData[m_sHead.m_nLength] = '\0';
}

SPacket& SPacket::operator=(const SPacket& packet)
{
	// 其实也是设置自己，然后把自己进行返回
	if (this != &packet)
	{
		m_sHead.m_sHead = packet.m_sHead.m_sHead;
		m_sHead.m_nLength = packet.m_sHead.m_nLength;
		m_sHead.m_sCmd = packet.m_sHead.m_sCmd;
		m_sHead.m_sSum = packet.m_sHead.m_sSum;
		memcpy(m_szData, packet.m_szData, packet.m_sHead.m_nLength);
		m_szData[m_sHead.m_nLength] = '\0';
	}

	return *this;
}

SPacket::~SPacket()
{

}
