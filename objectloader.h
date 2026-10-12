//=============================================================================
//
//	オブジェクト読み込みクラス [objectloader.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OBJECTLOADER_H_
#define _OBJECTLOADER_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include <iostream>
#include <fstream>
#include <string>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// オブジェクト読み込みクラス
//*****************************************************************************
class CObjectLoader
{
public:
	CObjectLoader();
	~CObjectLoader();

	HRESULT Load(const char* pFileName);
	void Uninit(void);

	void GetLine(void);
	std::string LoadStr(void);
	int LoadInt(void);
	float LoadFloat(void);
	D3DXVECTOR3 LoadVec3(void);

private:
	std::fstream m_fStream;		// ファイルストリーム
	std::string m_strIn;		// 読み込んだ文字列
};

#endif	// !_OBJECTLOADER_H_