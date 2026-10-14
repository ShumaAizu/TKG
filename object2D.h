//=============================================================================
//
//	オブジェクト2Dクラス [object2D.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INIT_POLYGONPOS		(D3DXVECTOR3((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 0.0f))	// ポリゴンの初期位置
#define POLYGON_WIDTH		(160.0f)																// ポリゴンの幅
#define POLYGON_HEIGHT		(90.0f)																	// ポリゴンの高さ

//*****************************************************************************
// オブジェクトクラスの定義
//*****************************************************************************
class CObject2D : public CObject
{
public:
	static CObject2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);
	static CObject2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const char* pTexFileName);
	static CObject2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeigh, const int nPriority);

	CObject2D(int nPriority = 3);
	virtual ~CObject2D();

	virtual HRESULT Init(void);
	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void);
	void SetSize(const float fWidth, const float fHeight);
	D3DXVECTOR2 GetSize(void);
	float GetLength(void) { return m_fLength; }
	D3DXCOLOR GetColor(void) { return m_col; }
	void SetColor(const D3DXCOLOR col);

	void SetIdxTexture(const int nIdx) { m_nIdxTexture = nIdx; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
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

#endif