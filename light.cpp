//=============================================================================
//
//	ライト処理 [light.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{
	// メンバ変数のクリア
	memset(&m_aLight[0], 0, sizeof(D3DLIGHT9) * NUM_LIGHT);
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLight::Init(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 vecDir;		// ライトの方向ベクトル

	int nCntLight;			// カウント用変数

	// ライトの情報をクリアする
	for (nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ライトの拡散光を設定
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライトの拡散光を設定
	m_aLight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

	// ライトの拡散光を設定
	m_aLight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_aLight[0].Direction = vecDir;

	vecDir = D3DXVECTOR3(-0.2f, 0.8f, -0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_aLight[1].Direction = vecDir;

	vecDir = D3DXVECTOR3(0.9f, 0.1f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_aLight[2].Direction = vecDir;

	// ライトを設定,有効にする
	for (nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void CLight::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CLight::Update(void)
{

}