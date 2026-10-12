//=============================================================================
//
//	テクスチャクラス [texture.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include <vector>
#include <string>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE			(16)		// テクスチャの最大数

//*****************************************************************************
// テクスチャクラス
//*****************************************************************************
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT Load(void);
	void Unload(void);

	int Register(const char* pTexturePath);
	LPDIRECT3DTEXTURE9 GetAddress(const int nIdx);

private:
	std::vector<LPDIRECT3DTEXTURE9> m_vpTexture;	// テクスチャへのポインタ
	std::vector<std::string> m_vTexturePath;		// テクスチャのパス

	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];	// テクスチャへのポインタ
	char m_aFileName[MAX_TEXTURE][FILENAME_MAX];	// テクスチャのパス
	static int m_nNumAll;							// テクスチャの総数
};

#endif	// _TEXTURE_H_