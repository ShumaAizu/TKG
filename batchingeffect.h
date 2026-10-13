//=============================================================================
//
//	バッチ描画エフェクトクラス [batchingeffect.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _BATCHINGEFFECT_H_
#define _BATCHINGEFFECT_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"
#include <vector>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// バッチ描画エフェクトクラスの定義
//*****************************************************************************
class CBatchingEffect
{
public:
	//*************************************************************************
	// 種類
	//*************************************************************************
	enum class TYPE
	{
		TYPE_STAR = 0,		// 星
		TYPE_DUST,			// 塵
		TYPE_TRAJECTORY,	// 軌跡
		TYPE_MAX			// 総数
	};

	static CBatchingEffect* Create(const int nMax, const char* pTexFileName);

	CBatchingEffect();
	~CBatchingEffect();

	HRESULT Init(void);
	HRESULT Init(const int nMax, const char* pTexFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, TYPE type);
	void Reset(void);
	void SwapAndPop(int nIdx);

	D3DXVECTOR3 GetPosition(int nIdx) { return m_vPos.at(nIdx); }
	void SetPosition(D3DXVECTOR3 pos, int nIdx) { m_vPos.at(nIdx) = pos; }
	TYPE GetType(int nIdx) { return m_vType.at(nIdx); }
	float GetRadius(int nIdx) { return m_vRadius.at(nIdx); }

	int GetNum(void) { return m_nNum; } 

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	std::vector<D3DXMATRIX> m_vMtxWorld;	// ワールドマトリックス
	std::vector<D3DXVECTOR3> m_vPos;		// 位置
	std::vector<D3DXVECTOR3> m_vRot;		// 向き
	std::vector<D3DXCOLOR> m_vCol;			// 色
	std::vector<TYPE> m_vType;				// 種類
	std::vector<float> m_vRadius;			// 半径
	std::vector<bool> m_visDirty;			// 変更が行われたか
	int m_nIdxTexture;						// テクスチャのインデックス
	int m_nMax;								// 最大数
	int m_nNum;								// エフェクト使用数
};

#endif