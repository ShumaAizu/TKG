//=============================================================================
//
//	カメラクラス [camera.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// カメラクラス
//*****************************************************************************
class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Set(void);
	virtual D3DVIEWPORT9 SetViewPort(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight);

	virtual D3DXVECTOR3 GetPositionV(void) { return m_posV; }
	virtual D3DXVECTOR3 GetPositionR(void) { return m_posR; }
	virtual D3DXVECTOR3 GetVectorU(void) { return m_vecU; }
	virtual void SetPositionV(D3DXVECTOR3 posV);
	virtual void SetPositionR(D3DXVECTOR3 posR);

	virtual D3DXVECTOR3 GetRotation(void) { return m_rot; }
	virtual void SetRotation(D3DXVECTOR3 rot);
	virtual void SetDistance(const float fDistance) { ; }

	void SetUse(bool isUse) { m_isUse = isUse; }

private:
	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXVECTOR3 m_rot;				// 向き
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			// ビューマトリックス
	D3DXVECTOR3 m_move;				// 移動量
	float m_fRadiusVertical;		// 縦軸距離(基本)
	float m_fRadiusHorizontal;		// 横軸距離
	bool m_isUse;					// 使用状態
};

#endif	// !_CAMERA_H_