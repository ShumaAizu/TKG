//=============================================================================
//
//	モーションクラス [motion.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "motion.h"
#include "model.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMotion::CMotion()
{
	// メンバ変数のクリア
	memset(&m_aInfo, 0, sizeof(INFO) * MAX_MOTION);
	m_nNumAll = 0;
	m_ppModel = NULL;
	m_nNumModel = 0;
	m_nType = 0;
	m_isLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nCounter = 0;
	m_isFinish = false;
	m_nTypeBlend = 0;
	m_isBlend = false;
	m_isLoopBlend = false;
	m_nNumKeyBlend = 0;
	m_nKeyBlend = 0;
	m_nCounterMotionBlend = 0;
	m_nFrameBlend = 0;
	m_nCounterBlend = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// 情報設定
//=============================================================================
void CMotion::SetInfo(INFO info)
{
	// 今の最後列に情報を格納して総数を増やす
	m_aInfo[m_nNumAll] = info;
	m_nNumAll++;
}

//=============================================================================
// モデル設定
//=============================================================================
void CMotion::SetModel(CModel** ppModel , int nNumModel)
{
	// 受け取ったモデルへの先頭アドレスと総数を設定
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}

//=============================================================================
// 設定
//=============================================================================
void CMotion::Set(int nType, bool isBrend)
{
	if (isBrend == true)
	{
		m_isBlend = true;
		m_nFrameBlend = 40;
		m_nCounterBlend = 0;
		
		m_nTypeBlend = nType;
		m_isLoopBlend = m_aInfo[nType].isLoop;
		m_nNumKeyBlend = m_aInfo[nType].nNumKey;
		m_nKeyBlend = 0;
		m_nCounterMotionBlend = 0;
		m_isFinish = false;
	}
	else
	{
		m_nNumKey = m_aInfo[nType].nNumKey;
		m_nType = nType;
		m_isLoop = m_aInfo[nType].isLoop;
		m_nKey = 0;
		m_nCounter = 0;
		m_isFinish = false;

		KEY_INFO* pKeyInfo = &m_aInfo[nType].aKeyInfo[0];

		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++, pKeyInfo++)
		{// 全パーツの初期化
			D3DXVECTOR3 pos = m_ppModel[nCntModel]->GetPosition();
			D3DXVECTOR3 rot = m_ppModel[nCntModel]->GetRotation();

			pos.x = pKeyInfo->aKey[0].fPosX;
			pos.y = pKeyInfo->aKey[0].fPosY;
			pos.z = pKeyInfo->aKey[0].fPosZ;

			rot.x = pKeyInfo->aKey[0].fRotX;
			rot.y = pKeyInfo->aKey[0].fRotY;
			rot.z = pKeyInfo->aKey[0].fRotZ;

			m_ppModel[nCntModel]->SetPosition(pos);
			m_ppModel[nCntModel]->SetRotation(rot);
		}
	}
}

//=============================================================================
// 更新
//=============================================================================
void CMotion::Update(void)
{
	// ローカル変数
	float fPosX, fPosY, fPosZ, fRotX, fRotY, fRotZ;
	float fDiffKey;

	// 全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		KEY DiffKey = {};
		KEY RateKey = {};

		// 現在のキー
		KEY* pKey = &m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel];

		// 次のキー
		KEY* pKeyNext = &m_aInfo[m_nType].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel];

		// 対象モデル
		D3DXVECTOR3 pos = m_ppModel[nCntModel]->GetPosition();
		D3DXVECTOR3 rot = m_ppModel[nCntModel]->GetRotation();

		// 最大フレームと現在のフレームの比率
		float fRateKey = (float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;

		if (m_isBlend == true)
		{// ブレンドあり
			// 現在の値格納用
			float fPosXCurrent, fRotXCurrent,	// 位置・向きX
				fPosYCurrent, fRotYCurrent, 	// 位置・向きY
				fPosZCurrent, fRotZCurrent;		// 位置・向きZ

			// ブレンド用
			float fPosXBlend, fRotXBlend,
				fPosYBlend, fRotYBlend,
				fPosZBlend, fRotZBlend;

			float fDiffKeyBlend, fDiffBlend;	// 差分

			// ブレンドモーションの現在のキー
			KEY* pKeyBlend = &m_aInfo[m_nTypeBlend].aKeyInfo[m_nKeyBlend].aKey[nCntModel];

			// ブレンドモーションの次のキー
			KEY* pKeyNextBlend = &m_aInfo[m_nTypeBlend].aKeyInfo[(m_nKeyBlend + 1) % m_nNumKeyBlend].aKey[nCntModel];

			// ブレンドモーションの最大フレームと現在のフレームの比率
			float fRateKeyBlend = (float)m_nCounterMotionBlend / (float)m_aInfo[m_nTypeBlend].aKeyInfo[m_nKeyBlend].nFrame;

			// モーションブレンドの最大フレームと現在のフレームの比率
			float fRateBlend = (float)m_nCounterBlend / (float)m_nFrameBlend;

			// 座標差分
			// X座標
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;
			fPosXCurrent = pKey->fPosX + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosX - pKeyBlend->fPosX;
			fPosXBlend = pKeyBlend->fPosX + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosXBlend - fPosXCurrent;

			fPosX = fPosXCurrent + (fDiffBlend * fRateBlend);

			// Y座標
			fDiffKey = pKeyNext->fPosY - pKey->fPosY;
			fPosYCurrent = pKey->fPosY + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosY - pKeyBlend->fPosY;
			fPosYBlend = pKeyBlend->fPosY + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosYBlend - fPosYCurrent;

			fPosY = fPosYCurrent + (fDiffBlend * fRateBlend);

			// Z座標
			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;
			fPosZCurrent = pKey->fPosZ + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosZ - pKeyBlend->fPosZ;
			fPosZBlend = pKeyBlend->fPosZ + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosZBlend - fPosZCurrent;

			fPosZ = fPosZCurrent + (fDiffBlend * fRateBlend);

			// 向き差分
			// X軸
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;
			fRotXCurrent = pKey->fRotX + (fDiffKey * fRateKey);
			fRotXCurrent = AngleNormalize(fRotXCurrent);

			fDiffKeyBlend = pKeyNextBlend->fRotX - pKeyBlend->fRotX;
			fRotXBlend = pKeyBlend->fRotX + (fDiffKeyBlend * fRateKeyBlend);
			fRotXBlend = AngleNormalize(fRotXBlend);

			fDiffBlend = fRotXBlend - fRotXCurrent;
			fDiffBlend = AngleNormalize(fDiffBlend);

			fRotX = fRotXCurrent + (fDiffBlend * fRateBlend);

			fRotX = AngleNormalize(fRotX);

			// Y軸
			fDiffKey = pKeyNext->fRotY - pKey->fRotY;
			fRotYCurrent = pKey->fRotY + (fDiffKey * fRateKey);
			fRotYCurrent = AngleNormalize(fRotYCurrent);

			fDiffKeyBlend = pKeyNextBlend->fRotY - pKeyBlend->fRotY;
			fRotYBlend = pKeyBlend->fRotY + (fDiffKeyBlend * fRateKeyBlend);
			fRotYBlend = AngleNormalize(fRotYBlend);

			fDiffBlend = fRotYBlend - fRotYCurrent;
			fDiffBlend = AngleNormalize(fDiffBlend);

			fRotY = fRotYCurrent + (fDiffBlend * fRateBlend);

			fRotY = AngleNormalize(fRotY);

			// Z軸
			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;
			fRotZCurrent = pKey->fRotZ + (fDiffKey * fRateKey);
			fRotZCurrent = AngleNormalize(fRotZCurrent);

			fDiffKeyBlend = pKeyNextBlend->fRotZ - pKeyBlend->fRotZ;
			fRotZBlend = pKeyBlend->fRotZ + (fDiffKeyBlend * fRateKeyBlend);
			fRotZBlend = AngleNormalize(fRotZBlend);

			fDiffBlend = fRotZBlend - fRotZCurrent;
			fDiffBlend = AngleNormalize(fDiffBlend);

			fRotZ = fRotZCurrent + (fDiffBlend * fRateBlend);

			fRotY = AngleNormalize(fRotY);
		}
		else
		{
			// 座標差分
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;
			fPosX = pKey->fPosX + (fDiffKey * fRateKey);

			fDiffKey = pKeyNext->fPosY - pKey->fPosY;
			fPosY = pKey->fPosY + (fDiffKey * fRateKey);

			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;
			fPosZ = pKey->fPosZ + (fDiffKey * fRateKey);

			// 向き差分
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;
			fDiffKey = AngleNormalize(fDiffKey);

			fRotX = AngleNormalize(pKey->fRotX + (fDiffKey * fRateKey));

			fDiffKey = pKeyNext->fRotY - pKey->fRotY;
			fDiffKey = AngleNormalize(fDiffKey);
			fRotY = AngleNormalize(pKey->fRotY + (fDiffKey * fRateKey));

			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;
			fDiffKey = AngleNormalize(fDiffKey);
			fRotZ = AngleNormalize(pKey->fRotZ + (fDiffKey * fRateKey));
		}

		// オフセット位置と向きを取得
		D3DXVECTOR3 posOffSet = m_ppModel[nCntModel]->GetPositionOffset();
		D3DXVECTOR3 rotOffSet = m_ppModel[nCntModel]->GetRotationOffset();

		// パーツの向きと位置を設定
		pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);
		rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);

		// オフセットを加算
		pos += posOffSet;
		rot += rotOffSet;

		// 位置と向きを設定
		m_ppModel[nCntModel]->SetPosition(pos);
		m_ppModel[nCntModel]->SetRotation(rot);
	}

	if (m_isBlend == true)
	{// ブレンドあり
		// ブレンドモーションのカウンターを更新
		m_nCounterMotionBlend++;

		if (m_nCounterMotionBlend >= m_aInfo[m_nTypeBlend].aKeyInfo[m_nKeyBlend].nFrame)
		{
			// キーを進める
			m_nCounterMotionBlend = 0;	// カウンターリセット
			if ((m_nKeyBlend = (m_nKeyBlend + 1) % m_nNumKeyBlend) == 0 && m_isLoopBlend == false)
			{// キーが終着点まで来たかつループ状態じゃなければ
				m_isFinish = true;				// 終了フラグ
			}
		}

		// ブレンドカウンターを更新
		m_nCounterBlend++;
		if (m_nCounterBlend >= m_nFrameBlend)
		{// カウンターが回り切ったら
			m_nCounterBlend = 0;					// カウンターリセット
			m_nType = m_nTypeBlend;					// 次のモーションに切り替える
			m_nCounter = m_nCounterMotionBlend;		// モーションカウンターをブレンドと同期
			m_nNumKey = m_nNumKeyBlend;				// キーの総数をブレンドと同期
			m_nKey = m_nKeyBlend;					// キー番号をブレンドと同期
			m_isLoop = m_isLoopBlend;				// ループ状態をブレンドと同期
			m_isBlend = false;						// ブレンド状態を終了
		}
	}
	else
	{// ブレンドなし
		m_nCounter++;

		if (m_nCounter >= m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		{
			// キーを進める
			m_nCounter = 0;	// カウンターリセット
			m_nKey = (m_nKey + 1) % m_nNumKey;

			if (m_nKey == m_nNumKey - 1 && m_isLoop == false)
			{// 最後のキーに到達していたら
				m_isFinish = true;					// 終了フラグ
			}
		}
	}
}

//=============================================================================
// 現在の種類取得
//=============================================================================
int CMotion::GetType(void)
{
	return m_nType;
}

//=============================================================================
// 終了状態取得
//=============================================================================
bool CMotion::IsFinish(void)
{
	return m_isFinish;
}