//=============================================================================
//
//	ボールクラス [ball.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _BALL_H_
#define _BALL_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;			// モデルクラス
class CMotion;			// モーションクラス

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// ボールクラスの定義
//*****************************************************************************
class CBall : public CObject
{
public:
	static CBall* Create(void);
	static CBall* Create(const D3DXVECTOR3 pos, const float fRadius, const float fSpeed, const float fRotAttenuation);

	CBall(int nPriority = 3);
	virtual ~CBall();

	virtual HRESULT Init(void);
	virtual HRESULT Init(const D3DXVECTOR3 pos, const float fRadius, const float fSpeed, const float fRotAttenuation);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void);
	float GetSpeed(void) { return m_fSpeed; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

private:
	physx::PxRigidDynamic* m_pRigid_Dynamic;	// 動的剛体
	physx::PxShape* m_pSphere_Shape;			// 形状 (球)

	CModel* m_pModel;							// モデルへのポインタ
	CMotion* m_pMotion;							// モーションへのポインタ

	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 過去の位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_moveDest;						// 移動量
	D3DXVECTOR3 m_scl;							// 大きさ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXMATRIX m_mtxRot;						// 回転マトリックス
	D3DXQUATERNION m_quat;						// クォータニオン
	D3DXVECTOR3 m_vecAxis;						// 回転軸
	D3DXVECTOR3 m_vecAxisOld;					// 保存用回転軸
	D3DXVECTOR3 m_vecAxisDest;					// 目的の回転軸
	D3DXVECTOR3 m_moveAxis;						// 移動ベクトル
	D3DXVECTOR3 m_moveAxisDest;					// 目的移動ベクトル
	float m_fRadius;							// 半径
	float m_fCircle;							// 円周
	float m_fMove;								// 移動量
	float m_fSpeed;								// 移動速度
	float m_fValueRot;							// 回転角(回転量)
	float m_fRotAttenuation;					// 回転減衰
};

#endif