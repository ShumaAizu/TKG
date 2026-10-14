//=============================================================================
//
//	ギミッククラス [gimmick.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;			// モデルクラス

//*****************************************************************************
// ギミッククラス
//*****************************************************************************
class CGimmick : public CObject
{
public:
	//*************************************************************************
	// 種類
	//*************************************************************************
	typedef enum TYPE
	{
		TYPE_NONE = 0,		// なにもなし
		TYPE_BUTTON,		// ボタン
		TYPE_MOVE,			// 移動
		TYPE_ROTATION,		// 回転
		TYPE_MAX			// 総数
	}TYPE;

	static CGimmick* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFile);

	CGimmick(int nPriority = 3);
	virtual ~CGimmick() override;

	virtual HRESULT Init(void) override;
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFile);
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }

	TYPE GetType(void) { return m_type; }

protected:
	TYPE m_type;						// 種類

private:
	CModel* m_pModel;					// モデルへのポインタ
	int m_nNumModel;					// モデルの総数

	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
};

#endif	// !_GIMMICK_H_