//=============================================================================
//
//	オブジェクトクラス [object.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PRIORITY_MAX	(8)			// 優先順位の最大
#define	MAX_OBJECT		(256)		// オブジェクトの最大数

#define ENABLE_LINKLIST_OBJECT		// 双方向リストでのオブジェクト管理
//#undef ENABLE_LINKLIST_OBJECT

//*****************************************************************************
// オブジェクトクラスの定義
//*****************************************************************************
class CObject
{
public:
	//*************************************************************************
	// 種類
	//*************************************************************************
	typedef enum TYPE
	{
		TYPE_NONE = 0,		// なにもない
		TYPE_UI,			// UI
		TYPE_PLAYER,		// プレイヤー
		TYPE_ITEM,			// アイテム
		TYPE_FIELD,			// フィールド
		TYPE_WALL,			// ウォール
		TYPE_GIMMICK,		// ギミック
		TYPE_MAX			// 最大
	}TYPE;

	CObject(int nPriority = 3);
	virtual ~CObject();


	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static int GetNumAll(void) { return m_nNumAll; }
	static CObject* GetObject(int nPriority, int nIdx) { return m_apObject[nPriority][nIdx]; }
	static CObject* GetTop(const int nPriority) { return m_apTop[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

	int GetID(void) { return m_nID; }
	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }

	void Delete(void);

	static bool GetFreeCamera(void) { return m_isFreeCamera; }
	static void SetFreeCamera(bool isFreeCamera) { m_isFreeCamera = isFreeCamera; }
	
protected:
	void Release(void);

private:
	static CObject* m_apObject[PRIORITY_MAX][MAX_OBJECT];	// オブジェクトへのポインタ

	static CObject* m_apTop[PRIORITY_MAX];					// 先頭のオブジェクトへのポインタ
	static CObject* m_apCur[PRIORITY_MAX];					// 最後尾のオブジェクトへのポインタ
	CObject* m_pPrev;										// 前のオブジェクトへのポインタ
	CObject* m_pNext;										// 次のオブジェクトへのポインタ

	static int m_nNumAll;									// オブジェクトの総数
	int m_nPriority;										// 優先順位の位置
	int m_nID;												// 自分自身のID
	TYPE m_type;											// 種類
	bool m_isDeath;											// 死亡フラグ

	static bool m_isFreeCamera;								// 仮置きカメラモード
};

#endif // !_OBJECT_H_