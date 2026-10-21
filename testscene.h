//=============================================================================
//
//	テスト画面クラス [testscene.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _TESTSCENE_H_
#define _TESTSCENE_H_

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "manager.h"
#include <vector>

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;				// プレイヤークラス
class CObjectLoader;		// オブジェクトローダークラス
class CUI;					// UIクラス
class CScore;				// スコアクラス
class CRotationGimmick;		// 回転ギミッククラス

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// テスト画面クラス
//*****************************************************************************
class CTestScene : public CScene
{
public:

	CTestScene(int nPriority = 0);
	~CTestScene() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	CPlayer* GetPlayer(void) const { return m_pPlayer; } 
	CScore* GetScore(void) const { return m_pScore; }

private:
	std::vector<CRotationGimmick*> m_vpRotGimmick;		// 回転ギミックへのポインタ
	CPlayer* m_pPlayer;									// プレイヤーへのポインタ
	CObjectLoader* m_pObjectLoader;						// オブジェクトローダーへのポインタ
	CUI* m_pTutorial;									// チュートリアルUIへのポインタ
	CScore* m_pScore;									// スコアへのポインタ
	int m_nCntTutorial;									// チュートリアルUIの消失カウント
	bool m_isPause;										// ポーズ状態
};

#endif	// !_GAME_H_