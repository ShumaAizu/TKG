//=============================================================================
//
//	オブジェクト読み込みクラス [objectloader.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードヘッダー
//*****************************************************************************
#include "objectloader.h"
#include "objectX.h"
#include "player.h"
#include "item.h"
#include "field.h"
#include "wall.h"
#include <vector>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPACE		(2)			// 文中の空白

//=============================================================================
// コンストラクタ
//=============================================================================
CObjectLoader::CObjectLoader()
{
	// メンバ変数クリア
	m_fStream.clear();
	m_strIn.clear();
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectLoader::~CObjectLoader()
{

}

//=============================================================================
// 読み込み
//=============================================================================
HRESULT CObjectLoader::Load(const char* pFileName)
{
	// ファイルを開く
	m_fStream.open(pFileName, std::iostream::in);

	if (m_fStream.is_open() != true)
	{// ファイルが開けていなければ
		// 早期終了
		return E_FAIL;
	}

	// ローカル変数
	std::vector<std::string> aModelFileName;

	int nNumModel = 0;
	int nCntModel = 0;
	aModelFileName.clear();

	while (true)
	{
		// 一列読み込む
		GetLine();

		if (m_strIn.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}

		if (m_strIn.find("NUM_MODEL") != std::string::npos)
		{// モデル数読み込み
			m_strIn.erase(0, strlen("NUM_MODEL") + SPACE);
			nNumModel = LoadInt();

			// モデル数分のファイル名保存用変数
			aModelFileName.resize(nNumModel);
		}

		if (m_strIn.find("MODEL_FILENAME") != std::string::npos)
		{// モデルファイル名読み込み
			m_strIn.erase(0, strlen("MODEL_FILENAME") + SPACE);
			aModelFileName[nCntModel] = LoadStr();
			nCntModel++;
		}

		if (m_strIn.find("FIELDSET") != std::string::npos)
		{// 地面配置情報読み込み
			std::string textureFileName;
			D3DXVECTOR3 pos = {}, rot = {};
			float fWidth = 0.0f, fDepth = 0.0f;
			int nIdx = 0, nType = 0;
			while (true)
			{
				GetLine();

				if (m_strIn.find("TEXTURE_FILENAME") != std::string::npos)
				{
					m_strIn.erase(0, strlen("TEXTURE_FILENAME") + SPACE);
					textureFileName = LoadStr();
				}

				if (m_strIn.find("POS") != std::string::npos)
				{// 位置読み込み
					m_strIn.erase(0, strlen("POS") + SPACE);
					pos = LoadVec3();
				}

				if (m_strIn.find("ROT") != std::string::npos)
				{// 向き読み込み
					m_strIn.erase(0, strlen("ROT") + SPACE);
					rot = LoadVec3();
				}

				if (m_strIn.find("WIDTH") != std::string::npos)
				{// 幅読み込み
					m_strIn.erase(0, strlen("WIDTH") + SPACE);
					fWidth = LoadFloat();
				}

				if (m_strIn.find("DEPTH") != std::string::npos)
				{// 奥行き読み込み
					m_strIn.erase(0, strlen("DEPTH") + SPACE);
					fDepth = LoadFloat();
				}

				if (m_strIn.find("END_FIELDSET") != std::string::npos)
				{// 読み込み終了
					m_strIn.erase(0, strlen("END_FIELDSET") + SPACE);

					// 生成
					CField::Create(pos, rot, fWidth, fDepth, textureFileName.c_str());
					break;
				}
			}
		}

		if (m_strIn.find("WALLSET") != std::string::npos)
		{// 壁配置情報読み込み
			std::string textureFileName;
			D3DXVECTOR3 pos = {}, rot = {};
			float fWidth = 0.0f, fHeight = 0.0f;
			int nIdx = 0, nType = 0;
			while (true)
			{
				GetLine();

				if (m_strIn.find("TEXTURE_FILENAME") != std::string::npos)
				{
					m_strIn.erase(0, strlen("TEXTURE_FILENAME") + SPACE);
					textureFileName = LoadStr();
				}

				if (m_strIn.find("POS") != std::string::npos)
				{// 位置読み込み
					m_strIn.erase(0, strlen("POS") + SPACE);
					pos = LoadVec3();
				}

				if (m_strIn.find("ROT") != std::string::npos)
				{// 向き読み込み
					m_strIn.erase(0, strlen("ROT") + SPACE);
					rot = LoadVec3();
				}

				if (m_strIn.find("WIDTH") != std::string::npos)
				{// 幅読み込み
					m_strIn.erase(0, strlen("WIDTH") + SPACE);
					fWidth = LoadFloat();
				}

				if (m_strIn.find("HEIGHT") != std::string::npos)
				{// 高さ読み込み
					m_strIn.erase(0, strlen("HEIGHT") + SPACE);
					fHeight = LoadFloat();
				}

				if (m_strIn.find("END_WALLSET") != std::string::npos)
				{// 読み込み終了
					m_strIn.erase(0, strlen("END_WALLSET") + SPACE);

					// 生成
					CWall::Create(pos, rot, fWidth, fHeight, textureFileName.c_str());
					break;
				}
			}
		}

		if (m_strIn.find("PLAYERSET") != std::string::npos)
		{// プレイヤー情報読み込み
			D3DXVECTOR3 pos = {}, rot = {};
			float fRadius = 0.0f, fSpeed = 0.0f, fRotAttenuation = 0.0f;
			int nIdx = 0, nType = 0;
			while (true)
			{
				GetLine();

				if (m_strIn.find("POS") != std::string::npos)
				{// 位置読み込み
					m_strIn.erase(0, strlen("POS") + SPACE);
					pos = LoadVec3();
				}

				if (m_strIn.find("RADIUS") != std::string::npos)
				{// 半径読み込み
					m_strIn.erase(0, strlen("RADIUS") + SPACE);
					fRadius = LoadFloat();
				}

				if (m_strIn.find("SPEED") != std::string::npos)
				{// 回転速度読み込み
					m_strIn.erase(0, strlen("SPEED") + SPACE);
					fSpeed = LoadFloat();
				}

				if (m_strIn.find("ROTATTENUATION") != std::string::npos)
				{// 回転減衰読み込み
					m_strIn.erase(0, strlen("ROTATTENUATION") + SPACE);
					fRotAttenuation = LoadFloat();
				}

				if (m_strIn.find("END_PLAYERSET") != std::string::npos)
				{// 読み込み終了
					m_strIn.erase(0, strlen("END_PLAYERSET") + SPACE);

					// 生成
					CPlayer::Create(pos, fRadius, fSpeed, fRotAttenuation);
					break;
				}
			}
		}

		if (m_strIn.find("ITEMSET") != std::string::npos)
		{// アイテム情報読み込み
			std::string modelFileName;
			D3DXVECTOR3 pos = {}, rot = {};
			float fRadius = 0.0f;
			while (true)
			{
				GetLine();

				if (m_strIn.find("MODEL_FILENAME") != std::string::npos)
				{
					m_strIn.erase(0, strlen("MODEL_FILENAME") + SPACE);
					modelFileName = LoadStr();
				}

				if (m_strIn.find("POS") != std::string::npos)
				{// 位置読み込み
					m_strIn.erase(0, strlen("POS") + SPACE);
					pos = LoadVec3();
				}

				if (m_strIn.find("ROT") != std::string::npos)
				{// 位置読み込み
					m_strIn.erase(0, strlen("ROT") + SPACE);
					rot = LoadVec3();
				}

				if (m_strIn.find("RADIUS") != std::string::npos)
				{// 半径読み込み
					m_strIn.erase(0, strlen("RADIUS") + SPACE);
					fRadius = LoadFloat();
				}

				if (m_strIn.find("END_ITEMSET") != std::string::npos)
				{// 読み込み終了
					m_strIn.erase(0, strlen("END_ITEMSET") + SPACE);

					// 生成
					CItem::Create(pos, rot, fRadius, modelFileName.c_str());
					break;
				}
			}
		}
	}

	// ファイルを閉じる
	m_fStream.close();

	aModelFileName.clear();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CObjectLoader::Uninit(void)
{
	m_fStream.clear();
	m_strIn.clear();
}

//=============================================================================
// 文字列読み込み
//=============================================================================
void CObjectLoader::GetLine(void)
{
	// ローカル変数
	int nPosition;

	// ファイルストリームから一列読み込む
	std::getline(m_fStream, m_strIn);

	// 文字を検索
	nPosition = m_strIn.find('#');

	if (nPosition != std::string::npos)
	{// 存在したら以降の文字列を消去
		m_strIn.erase(nPosition);
	}

	while (true)
	{
		nPosition = m_strIn.find('\t');

		if (nPosition != std::string::npos)
		{// 存在していたら消去
			m_strIn.erase(nPosition, 1);
			continue;
		}

		nPosition = m_strIn.find('=');

		if (nPosition != std::string::npos)
		{// 存在していたら消去
			m_strIn.erase(nPosition, 1);
			continue;
		}

		break;
	}
}

//=============================================================================
// 文字列読み込み
//=============================================================================
std::string CObjectLoader::LoadStr(void)
{
	return m_strIn;
}

//=============================================================================
// int型読み込み
//=============================================================================
int CObjectLoader::LoadInt(void)
{
	// ローカル変数
	int nInt = 0;

	// 型変換
	const char* pBuff = m_strIn.c_str();

	// 読み込み
	sscanf(pBuff, "%d", &nInt);

	return nInt;
}

//=============================================================================
// float型読み込み
//=============================================================================
float CObjectLoader::LoadFloat(void)
{
	// ローカル変数
	float fFloat = 0;

	// 型変換
	const char* pBuff = m_strIn.c_str();

	// 読み込み
	sscanf(pBuff, "%f", &fFloat);

	return fFloat;
}

//=============================================================================
// Vector3型読み込み
//=============================================================================
D3DXVECTOR3 CObjectLoader::LoadVec3(void)
{
	// ローカル変数
	D3DXVECTOR3 vec3;

	// 型変換
	const char* pBuff = m_strIn.c_str();

	// 読み込み
	sscanf(pBuff, "%f %f %f", &vec3.x, &vec3.y, &vec3.z);

	return vec3;
}