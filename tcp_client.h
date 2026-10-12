//=============================================================================
//
//	クライアントクラス [tcp_client.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include <stdio.h>
#include <string.h>

//*****************************************************************************
// クライアントクラスの定義
//*****************************************************************************
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	bool Init(void);
	bool Init(SOCKET sock, char* pIP, u_short sPort);
	void Uninit(void);
	bool Connect(const char* pIPAddress, u_short sPortID);
	template<class T> int Send(const T* pSendData);
	template<class T> int Send(const T* pSendData, const int nNumAry);
	template<class T> int Recv(T* pRecvData);
	template<class T> int Recv(T* pRecvData, const int nNumAry);

	const char* GetIP(void) { return m_pIP; }
	u_short GetPort(void) { return m_sPort; }

private:
	SOCKET m_sock;		// 通信用ソケット
	const char* m_pIP;	// IPアドレス
	u_short m_sPort;	// ポート番号
};

//=============================================================================
// データの送信処理
//=============================================================================
template<class T> int CTcpClient::Send(const T* pSendData)
{
	// ソケットが有効かどうか
	if (m_sock == INVALID_SOCKET)
	{
		return -1;
	}

	// データが存在するかどうか
	if (pSendData == NULL)
	{
		return -1;
	}

	// データ送信用ポインタ
	unsigned char* pSendPointer = NULL;

	// データサイズ
	int nDataSize = sizeof(T);

	// メモリ確保
	pSendPointer = new unsigned char[nDataSize];

	if (pSendPointer == NULL)
	{// メモリの確保に失敗したなら
		printf("\n*---< メモリの確保に失敗しました >---*\n");
		return -1;
	}

	// データをコピー
	memcpy(pSendPointer, pSendData, nDataSize);

	// データを送信する
	int nRecvByte = send(m_sock, (const char*)pSendPointer, nDataSize, 0);

	if (pSendPointer != NULL)
	{// データ送信用メモリの開放
		delete[] pSendPointer;
		pSendPointer = NULL;
	}

	return nRecvByte;
}

//=============================================================================
// データの送信処理 (配列数指定)
//=============================================================================
template<class T> int CTcpClient::Send(const T* pSendData, const int nNumAry)
{
	// ソケットが有効かどうか
	if (m_sock == INVALID_SOCKET)
	{
		return -1;
	}

	// データが存在するかどうか
	if (pSendData == NULL)
	{
		return -1;
	}

	// データ数が指定されているか
	if (nNumAry < 0)
	{
		return -1;
	}

	// データ送信用ポインタ
	unsigned char* pSendPointer = NULL;

	// データサイズ
	int nDataSize = sizeof(T) * nNumAry;

	// メモリ確保
	pSendPointer = new unsigned char[nDataSize];

	if (pSendPointer == NULL)
	{// メモリの確保に失敗したなら
		printf("\n*---< メモリの確保に失敗しました >---*\n");
		return -1;
	}

	// データをコピー
	memcpy(pSendPointer, pSendData, nDataSize);

	// データを送信する
	int nRecvByte = send(m_sock, (const char*)pSendPointer, nDataSize, 0);

	if (pSendPointer != NULL)
	{// データ送信用メモリの開放
		delete[] pSendPointer;
		pSendPointer = NULL;
	}

	return nRecvByte;
}

//=============================================================================
// データの受信処理
//=============================================================================
template<class T> int CTcpClient::Recv(T* pRecvData)
{
	// ソケットが有効かどうか
	if (m_sock == INVALID_SOCKET)
	{
		return -1;
	}

	// データが存在するかどうか
	if (pRecvData == NULL)
	{
		return -1;
	}

	// データ受信用ポインタ
	unsigned char* pRecvPointer = NULL;

	// データサイズ
	int nDataSize = sizeof(T);

	// メモリ確保
	pRecvPointer = new unsigned char[nDataSize];

	// データ受信
	int nRecvByte = recv(m_sock, (char*)pRecvPointer, nDataSize, 0);

	if (nRecvByte <= 0)
	{// 切断されたなら
		// 終了処理
		Uninit();
	}
	else
	{// 何かしらデータが入っていれば
		memcpy(pRecvData, pRecvPointer, nDataSize);
	}

	if (pRecvPointer != NULL)
	{// データ受信用メモリの開放
		delete[] pRecvPointer;
		pRecvPointer = NULL;
	}

	return nRecvByte;
}

//=============================================================================
// データの受信処理 (配列数指定)
//=============================================================================
template<class T> int CTcpClient::Recv(T* pRecvData, const int nNumAry)
{
	// ソケットが有効かどうか
	if (m_sock == INVALID_SOCKET)
	{
		return -1;
	}

	// データが存在するかどうか
	if (pRecvData == NULL)
	{
		return -1;
	}

	// データ数が指定されているか
	if (nNumAry < 0)
	{
		return -1;
	}

	// データ受信用ポインタ
	unsigned char* pRecvPointer = NULL;

	// データサイズ
	int nDataSize = sizeof(T) * nNumAry;

	// メモリ確保
	pRecvPointer = new unsigned char[nDataSize];

	// データ受信
	int nRecvByte = recv(m_sock, (char*)pRecvPointer, nDataSize, 0);

	if (nRecvByte <= 0)
	{// 切断されたなら
		// 終了処理
		Uninit();
	}
	else
	{// 何かしらデータが入っていれば
		memcpy(pRecvData, pRecvPointer, nDataSize);
	}

	if (pRecvPointer != NULL)
	{// データ受信用メモリの開放
		delete[] pRecvPointer;
		pRecvPointer = NULL;
	}

	return nRecvByte;
}

#endif // !_TCP_CLIENT_H_