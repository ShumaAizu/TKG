//=============================================================================
//
//	オブジェクトXクラス [objectX.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"
#include <string>
#include <vector>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODELTEX			(16)			// 読み込むテクスチャの最大数

//*****************************************************************************
// オブジェクトXクラス
//*****************************************************************************
class CObjectX : public CObject
{
public:
	static CObjectX* Create(void);
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName);

	//CObjectX();
	CObjectX(int nPriority = 3);
	virtual ~CObjectX() override;

	virtual HRESULT Init(void) override;
	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName);
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	void Draw(void) override;

	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void);

	std::string GetFileName(void) { return m_FileName; }

private:
	void Debug(void);

	std::vector<int> m_vIdxTexture;					// テクスチャのインデックス
	LPD3DXMESH m_pMesh;								// メッシュへのポインタ
	LPD3DXBUFFER m_pBuffMat;						// マテリアルへのポインタ
	DWORD m_dwNumMat;								// マテリアルの数
	D3DXVECTOR3 m_vtxMin;							// 頂点位置の最小値
	D3DXVECTOR3 m_vtxMax;							// 頂点位置の最大値
	DWORD m_dwSizeFVF;								// 頂点フォーマットのサイズ
	int m_nNumVtx;									// 頂点数
	int m_nNumIdx;									// インデックス数
	std::string m_FileName;							// 保存用

	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_rot;								// 向き
	D3DXVECTOR3 m_move;								// 移動量
	D3DXVECTOR3 m_moveDest;							// 目的移動量
	D3DXVECTOR3 m_scl;								// 大きさ
	D3DXMATRIX m_mtxWorld;							// ワールドマトリックス
	D3DXMATRIX m_mtxRot;							// 回転マトリックス
	D3DXQUATERNION m_quat;							// クォータニオン
	D3DXVECTOR3 m_vecAxis;							// 回転軸
	D3DXVECTOR3 m_vecAxisOld;						// 保存用回転軸
	D3DXVECTOR3 m_vecAxisDest;						// 目的の回転軸
	D3DXVECTOR3 m_moveAxis;							// 移動ベクトル
	D3DXVECTOR3 m_moveAxisDest;						// 目的移動ベクトル
	float m_fRadius;								// 半径
	float m_fCircle;								// 円周
	float m_fMove;									// 移動量
	float m_fSpeed;									// 移動速度
	float m_fValueRot;								// 回転角(回転量)
	float m_fRotAttenuation;						// 回転減衰
	float m_fMoveKeyboard;							// キーボード移動量
	bool m_bJump;									// ジャンプ中か
};

#endif