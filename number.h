//=============================================================================
//
//	ナンバークラス [number.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// ナンバークラス
//*****************************************************************************
class CNumber
{
public:
	static HRESULT Load(void);
	static void Unload(void);

	static CNumber* Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight);

	CNumber();
	~CNumber();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void);
	void SetSize(const float fWidth, const float fHeight);
	D3DXVECTOR2 GetSize(void);
	float GetLength(void) { return m_fLength; }
	void SetNum(const int nNumber);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	int m_nIdxTexture;		// テクスチャのインデックス

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXCOLOR m_col;		// 色
	float m_fWidth;			// 幅
	float m_fHeight;		// 高さ
	float m_fAngle;			// 角度
	float m_fLength;		// 長さ
};

#endif	// !_NUMBER_H_