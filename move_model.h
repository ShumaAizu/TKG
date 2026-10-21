//=============================================================================
//
//	移動モデル処理 [move_model.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _MOVE_MODEL_H_
#define _MOVE_MODEL_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 移動モデルクラス
//*****************************************************************************
class CMoveModel : public CModel
{
public:
	static CMoveModel* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const char* pModelFile);

	CMoveModel();
	~CMoveModel() override;

	HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const char* pModelFile);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;
};

#endif