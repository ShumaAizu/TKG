//=============================================================================
//
//	テクスチャクラス [texture.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CTexture::m_nNumAll = 0;		// テクスチャの総数

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTexture::CTexture()
{
	// メンバ変数のクリア
	memset(&m_apTexture[0], NULL, sizeof(LPDIRECT3DTEXTURE9) * MAX_TEXTURE);
	memset(&m_aFileName[0][0], 0, sizeof(char) * (MAX_TEXTURE * FILENAME_MAX));
	m_vpTexture.clear();
	m_vTexturePath.clear();
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTexture::~CTexture()
{

}

//*****************************************************************************
// 読み込み
//*****************************************************************************
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//*****************************************************************************
// 破棄
//*****************************************************************************
void CTexture::Unload(void)
{
	// すべてのテクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < m_vpTexture.size(); nCntTexture++)
	{
		if (m_vpTexture.at(nCntTexture) == NULL)
		{// 使われていなければ
			// スキップ
			continue;
		}

		// テクスチャの破棄
		m_vpTexture.at(nCntTexture)->Release();
		m_vpTexture.at(nCntTexture) = NULL;
	}

#if 0
	// すべてのテクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] == NULL)
		{// 使われていなければ
			// スキップ
			continue;
		}

		// テクスチャの破棄
		m_apTexture[nCntTexture]->Release();
		m_apTexture[nCntTexture] = NULL;
	}
#endif
}

//*****************************************************************************
// 登録
//*****************************************************************************
int CTexture::Register(const char* pTexturePath)
{
	// カウント変数
	int nCntTexture = 0;

	// パスチェック
	for (nCntTexture = 0; nCntTexture < m_vpTexture.size(); nCntTexture++)
	{
		if (pTexturePath == m_vTexturePath.at(nCntTexture))
		{// もしすでに読み込んだパスと一致していれば
			// その番号を返す
			return nCntTexture;
		}
	}

	// 返す番号を設定
	int nIdx = nCntTexture;

	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	LPDIRECT3DTEXTURE9 pTexture = NULL;

	HRESULT hr = D3DXCreateTextureFromFile(pDevice,
		pTexturePath,
		&pTexture);

	if (FAILED(hr))
	{// もしテクスチャの読み込みに失敗したら
		return -1;
	}

	// メンバに登録
	m_vpTexture.push_back(pTexture);

	// テクスチャのパスを登録
	m_vTexturePath.push_back(pTexturePath);

	// 番号を返す
	return nIdx;

#if 0
	// 返す番号の値
	int nTexIdx = -1;

	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (strcmp(pTexturePath, &m_aFileName[nCntTexture][0]) == 0)
		{// もしすでに読み込んだパスと一致していれば
			// その番号を返す
			return nCntTexture;
		}

		if (m_apTexture[nCntTexture] != NULL)
		{// すでに使っていたら
			// スキップ
			continue;
		}

		if (nTexIdx != -1)
		{// 番号が決まっていたらスキップ
			continue;
		}

		// 番号を登録
		nTexIdx = nCntTexture;
	}

	if (nTexIdx == -1)
	{// 空きがなかったら
		// 無効数値を返す
		return nTexIdx;
	}

	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	HRESULT hr = D3DXCreateTextureFromFile(pDevice,
		pTexturePath,
		&m_apTexture[nTexIdx]);

	if (FAILED(hr))
	{// もしテクスチャの読み込みに失敗したら
		return -1;
	}

	// テクスチャのパスを登録
	strcpy(&m_aFileName[nTexIdx][0], pTexturePath);

	// 登録したインデックスを返す
	return nTexIdx;
#endif
}

//*****************************************************************************
// アドレス取得
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTexture::GetAddress(const int nIdx)
{
	if (nIdx >= m_vpTexture.size() || nIdx < 0)
	{// 無効な値なら
		// NULLポインタを返す
		return NULL;
	}

	// 指定された番号のテクスチャポインタを返す
	return m_vpTexture.at(nIdx);

#if 0
	if (nIdx >= MAX_TEXTURE || nIdx < 0)
	{// 無効な値なら
		// NULLポインタを返す
		return NULL;
	}

	// 指定された番号のテクスチャポインタを返す
	return m_apTexture[nIdx];
#endif
}