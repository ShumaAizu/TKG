//=============================================================================
//
//	オブジェクトXクラス [objectX.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "object.h"
#include "objectX.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ROTATTENUATION		(0.08f)			// 回転減衰
#define SPEED				(0.025f)		// 速度

//=============================================================================
// 生成処理
//=============================================================================
CObjectX* CObjectX::Create(void)
{
#ifndef ENABLE_LINKLIST_OBJECT
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}
#endif // !ENABLE_LINKLIST_OBJECT

	// ローカル変数宣言
	CObjectX* pObjectX = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pObjectX = new CObjectX;

	// 初期化処理
	pObjectX->Init();

	// 生成したオブジェクトのポインタを返す
	return pObjectX;
}

//=============================================================================
// 生成処理
//=============================================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName)
{
#ifndef ENABLE_LINKLIST_OBJECT
	// 最大チェック
	if (GetNumAll() >= MAX_OBJECT)
	{// これ以上生成できなければ
		// NULLを返し終了
		return NULL;
	}
#endif // !ENABLE_LINKLIST_OBJECT

	// ローカル変数宣言
	CObjectX* pObjectX = NULL;		// ポインタ保存用

	// オブジェクト2Dのインスタンス生成
	pObjectX = new CObjectX;

	// 初期化処理
	pObjectX->Init(pos, rot, pFileName);

	// 生成したオブジェクトのポインタを返す
	return pObjectX;
}

//=============================================================================
// コンストラクタ (オーバーロード)
//=============================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	// 各変数クリア
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_dwNumMat = 0;
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_dwSizeFVF = 0;
	m_nNumVtx = 0;
	m_nNumIdx = 0;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxisOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0.0f;
	m_fCircle = 0.0f;
	m_fMove = 0.0f;
	m_fSpeed = 0.0f;
	m_fValueRot = 0.0f;
	m_fRotAttenuation = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectX::~CObjectX()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObjectX::Init(void)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	// 変数初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxis = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fRadius = 10.0f;
	m_fCircle = (D3DX_PI * 2.0f) * m_fRadius;
	m_fMove = 0.0f;
	m_fValueRot = 0.0f;
	m_fRotAttenuation = ROTATTENUATION;
	m_fSpeed = SPEED;
	D3DXMatrixIdentity(&m_mtxRot);

	BYTE* pVtxBuff;								// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data//MODEL//spikeball000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	if (m_pBuffMat == NULL)
	{
		return E_FAIL;
	}

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの登録
			m_vIdxTexture.push_back(pTexture->Register(pMat[nCntMat].pTextureFilename));
			continue;
		}

		m_vIdxTexture.push_back(-1);
	}

	// 頂点数を取得
	m_nNumVtx = m_pMesh->GetNumVertices();

	// インデックス数を取得
	m_nNumIdx = m_pMesh->GetNumFaces();

	// 頂点フォーマットのサイズを取得
	m_dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入

		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += m_dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObjectX::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pFileName)
{
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	// 変数初期化
	m_pos = pos;
	m_rot = rot;
	m_FileName = pFileName;
	m_vecAxis = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fRadius = 10.0f;
	m_fCircle = (D3DX_PI * 2.0f) * m_fRadius;
	m_fMove = 0.0f;
	m_fValueRot = 0.0f;
	m_fRotAttenuation = ROTATTENUATION;
	m_fSpeed = SPEED;
	D3DXMatrixIdentity(&m_mtxRot);

	BYTE* pVtxBuff;								// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	if (m_pBuffMat == NULL)
	{
		return E_FAIL;
	}

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの登録
			m_vIdxTexture.push_back(pTexture->Register(pMat[nCntMat].pTextureFilename));
			continue;
		}

		m_vIdxTexture.push_back(-1);
	}

	// 頂点数を取得
	m_nNumVtx = m_pMesh->GetNumVertices();

	// インデックス数を取得
	m_nNumIdx = m_pMesh->GetNumFaces();

	// 頂点フォーマットのサイズを取得
	m_dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入

		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += m_dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObjectX::Uninit(void)
{
	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// ファイル名の破棄
	m_FileName.clear();

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObjectX::Update(void)
{
#if 0
	// デバイス取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pInputJoypad = pManager->GetInputJoypad();
	CCamera* pCamera = pManager->GetCamera();

	static float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;

	D3DXVECTOR3 KeyboardMove = { 0.0f, 0.0f, 0.0f };
	static D3DXVECTOR3 KeyboardMoveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAxisOld = m_vecAxis;

	if (pInputKeyboard->GetPress(DIK_A) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_LEFT) == true)
	{// Aキーが押された
		KeyboardMove.x += -1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_D) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_RIGHT) == true)
	{// Dキーが押された
		KeyboardMove.x += 1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_UP) == true)
	{// Wキーが押された
		KeyboardMove.z += 1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_S) == true || pInputJoypad->GetPress(CInputJoypad::KEY::KEY_DOWN) == true)
	{// Sキーが押された
		KeyboardMove.z += -1.0f;
	}

	// デバッグ機能
#if _DEBUG
	Debug();
#endif

	// スティックの値をもらう(使っていたら優先)
	pInputJoypad->GetStickLeft(&KeyboardMove.x, &KeyboardMove.z);

	// 移動ベクトルを正規化
	D3DXVec3Normalize(&KeyboardMove, &KeyboardMove);

	// 移動総量
	m_fMoveKeyboard = SQRTF(KeyboardMove.x, KeyboardMove.z);

	if (m_fMoveKeyboard != 0.0f)
	{// 移動していたら
		KeyboardMoveOld = KeyboardMove;

		// 移動軸設定
		m_vecAxis.x = KeyboardMove.z;
		m_vecAxis.y = 0.0f;
		m_vecAxis.z = -KeyboardMove.x;

		fRotDest = atan2f(KeyboardMove.x, KeyboardMove.z) + pCamera->GetRotation().y;	// 目的地への向き

		// 目的向きに移動方向を合わせる
		KeyboardMove.x = sinf(fRotDest) * m_fMoveKeyboard;
		KeyboardMove.z = cosf(fRotDest) * m_fMoveKeyboard;

		D3DXVECTOR3 vecAxisDiff = m_vecAxisOld - m_vecAxis;

		if (fabsf(vecAxisDiff.x) > 1.5f || fabsf(vecAxisDiff.z) > 1.5f)
		{
			m_fValueRot *= -1.0f;
		}
	}

	// 移動量
	m_fValueRot += m_fMoveKeyboard * m_fSpeed;
	m_fMove = (m_fValueRot / (D3DX_PI * 2.0f)) * m_fCircle;

	// 移動量の更新
	m_move.x = (m_fMove * KeyboardMoveOld.x);
	m_move.z = (m_fMove * KeyboardMoveOld.z);

	CDebugProc::Print("前回の移動方向 = { %.2f %.2f %.2f }\n", KeyboardMoveOld.x, KeyboardMoveOld.y, KeyboardMoveOld.z);

	if ((pInputKeyboard->GetTrigger(DIK_SPACE) == true || pInputJoypad->GetTrigger(CInputJoypad::KEY::KEY_A) == true) && m_bJump == false)
	{// スペースキーが押されたかつジャンプ状態じゃない
		m_move.y = 5.0f;
		m_bJump = true;
	}

	m_move.y += -0.5f;		// 重力をかける
	m_pos += m_move;		// 位置を更新

	if (m_pos.y + -m_fRadius < 0.0f)
	{
		m_pos.y = m_fRadius;
		m_bJump = false;
	}

	// 慣性を更新
	m_fValueRot += (0.0f - m_fValueRot) * m_fRotAttenuation;

	CDebugProc::Print("*** モデル情報 ***\n");
	CDebugProc::Print("位置 = { %.2f %.2f %.2f }\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("移動量 = { %.2f %.2f %.2f }\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("回転軸 = { %.2f %.2f %.2f }\n", m_vecAxis.x, m_vecAxis.y, m_vecAxis.z);
	CDebugProc::Print("回転軸Old = { %.2f %.2f %.2f }\n", m_vecAxisOld.x, m_vecAxisOld.y, m_vecAxisOld.z);
	CDebugProc::Print("回転角 = { %.2f }\n", m_fValueRot);
	CDebugProc::Print("移動量 = { %.2f }\n", m_fMove);
	CDebugProc::Print("\n半径 : [← / →] (%.1f)\n", m_fRadius);
	CDebugProc::Print("回転速度 : [↑ / ↓] (%.3f)\n", m_fSpeed);
	CDebugProc::Print("回転減衰 : [Y / H] (%.2f)\n", m_fRotAttenuation);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CObjectX::Draw(void)
{
#if 1
	// デバイスの取得
	CManager * pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	D3DXMATERIAL mat;							// マテリアル保持用

	// ライトをオフにする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		mat = pMat[nCntMat];

		//mat.MatD3D.Diffuse = D3DXCOLOR(D3DXCOLOR(0.95f, 0.67f, 0.09f, 1.0f));

		// マテリアルの設定
		pDevice->SetMaterial(&mat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_vIdxTexture.at(nCntMat)));

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif

#if 0
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	D3DXMATERIAL mat;							// マテリアル保持用

	// 影の描画用変数
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DLIGHT9 light;			// ライトの情報
	D3DXVECTOR4 posLight;		// ライトの位置
	D3DXVECTOR3 pos, normal;	// 平面の点と法線ベクトル
	D3DXPLANE plane;			// 平面の情報

	// ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	// 平面情報を生成
	pos = D3DXVECTOR3(0.0f, 0.05f, 0.0f);		// 高さが0ならなんでもいい
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 描画
	D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
	D3DXMATRIX mtxParent;								// 親のマトリックス

	// ワールドマトリックスの初期化(デフォルトの値にする)
	D3DXMatrixIdentity(&mtxShadow);

	// シャドウマトリックスの生成
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// パーツのワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		mat = pMat[nCntMat];

		mat.MatD3D.Diffuse.r = 0.0f;
		mat.MatD3D.Diffuse.g = 0.0f;
		mat.MatD3D.Diffuse.b = 0.0f;

		// マテリアルの設定
		pDevice->SetMaterial(&mat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		// プレイヤー(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);;
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// ライトをオフにする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転軸における指定の回転角からクォータニオンを作成
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	// クォータニオンから回転マトリックスを作成
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	// 向きを反映
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		mat = pMat[nCntMat];

		// マテリアルの設定
		pDevice->SetMaterial(&mat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_vIdxTexture.at(nCntMat)));

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
#endif

#if 0
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	D3DVIEWPORT9 viewportDef;
	D3DXMATRIX mtxViewDef, mtxProjectionDef;
	CCamera* pCamera = static_cast<CCamera*>(pManager->GetFreeCamera());

	// 現在の各デバイス設定を取得 (一時保存)
	pDevice->GetRenderTarget(0, &pRenderDef);
	pDevice->GetDepthStencilSurface(&pZBuffDef);
	pDevice->GetViewport(&viewportDef);
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

	// レンダリングターゲットの変更
	pRenderer->ChangeTarget(D3DXVECTOR3(0.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), pCamera->GetVectorU());

	// レンダリングターゲット用テクスチャのクリア
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_pIdxTexture[nCntMat]));

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 保存していたデバイス設定を元に戻す
	pDevice->SetRenderTarget(0, pRenderDef);
	pDevice->SetDepthStencilSurface(pZBuffDef);
	pDevice->SetViewport(&viewportDef);
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewDef);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionDef);
#endif

#if 0
	// デバイスの取得
	CManager* pManager = CManager::GetInstance();
	CRenderer* pRenderer = pManager->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTexture* pTexture = pManager->GetTextrue();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	D3DVIEWPORT9 viewportDef;
	D3DXMATRIX mtxViewDef, mtxProjectionDef;
	CCamera* pCamera = static_cast<CCamera*>(pManager->GetFreeCamera());

	// 現在の各デバイス設定を取得 (一時保存)
	pDevice->GetRenderTarget(0, &pRenderDef);
	pDevice->GetDepthStencilSurface(&pZBuffDef);
	pDevice->GetViewport(&viewportDef);
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

	// レンダリングターゲットの変更
	pRenderer->ChangeTarget(D3DXVECTOR3(0.0f, 0.0f, -250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), pCamera->GetVectorU());

	// レンダリングターゲット用テクスチャのクリア
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_pIdxTexture[nCntMat]));

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 保存していたデバイス設定を元に戻す
	pDevice->SetRenderTarget(0, pRenderDef);
	pDevice->SetDepthStencilSurface(pZBuffDef);
	pDevice->SetViewport(&viewportDef);
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewDef);
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionDef);
#endif
}

//=============================================================================
// 位置指定処理
//=============================================================================
void CObjectX::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置変更
	m_pos = pos;
}

//=============================================================================
// 位置取得処理
//=============================================================================
D3DXVECTOR3 CObjectX::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// 向き指定処理
//=============================================================================
void CObjectX::SetRotation(const D3DXVECTOR3 rot)
{
	// 向き変更
	m_rot = rot;

	// 向きの範囲制限
	m_rot.z = AngleNormalize(m_rot.z);
}

//=============================================================================
// 向き取得処理
//=============================================================================
D3DXVECTOR3 CObjectX::GetRotation(void)
{
	return m_rot;
}

#if 0
//=============================================================================
// デバッグ処理
//=============================================================================
void CObjectX::Debug(void)
{
	// 入力デバイス取得
	CManager* pManager = CManager::GetInstance();
	CInputKeyboard* pInputKeyboard = pManager->GetInputKeyboard();
	CInputJoypad* pInputJoypad = pManager->GetInputJoypad();

	// 大きさ変更
	if (pInputKeyboard->GetPress(DIK_LEFT) == true && m_fRadius < 100.0f)
	{// 右入力で大きく
		m_scl = m_scl * 1.1f;
		m_fRadius = m_fRadius * 1.1f;
		m_fCircle = (D3DX_PI * 2) * m_fRadius;
	}

	if (pInputKeyboard->GetPress(DIK_RIGHT) == true && m_fRadius > 10.0f)
	{// 左入力で小さく
		m_scl = m_scl * 0.9f;
		m_fRadius = m_fRadius * 0.9f;
		m_fCircle = (D3DX_PI * 2) * m_fRadius;
	}

	// 回転速度変更
	if (pInputKeyboard->GetRepeat(DIK_UP) == true && m_fSpeed < 1.0f)
	{// 上入力で大きく
		m_fSpeed += 0.001f;
	}

	if (pInputKeyboard->GetRepeat(DIK_DOWN) == true && m_fSpeed > 0.001f)
	{// 下入力で小さく
		m_fSpeed += -0.001f;
	}

	// 回転減衰変更
	if (pInputKeyboard->GetRepeat(DIK_Y) == true && m_fRotAttenuation < 1.0f)
	{// Y入力で大きく
		m_fRotAttenuation += 0.01f;
	}

	if (pInputKeyboard->GetRepeat(DIK_H) == true && m_fRotAttenuation > 0.01f)
	{// H入力で小さく
		m_fRotAttenuation += -0.01f;
	}
}
#endif