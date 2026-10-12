//=============================================================================
//
//	レンダラークラス [renderer.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

//*****************************************************************************
// インクルードガード
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_MULTITARGET		(2)		// マルチレンダリングターゲット用テクスチャ数

//*****************************************************************************
// レンダラークラスの定義
//*****************************************************************************
class CRenderer
{
public:

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFeedback(void);
	void UpdateFeedback(void);
	void DrawMT(int nTexIdx);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	void ChangeTarget(void);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_apTextureMT[0]; }

	void SetEnableFeedback(const bool isEnableFeedback) { m_isEnableFeedback = isEnableFeedback; }
	void ResetEnableFeedback(void);

	void SetTextureStageStateAlpha(const int nIdx, const DWORD Stage, const D3DTEXTUREOP Type, const DWORD Value);
	void ResetTextureStageStateAlpha(const int nNumIdx);
	void SetTextureStageStateColor(const int nIdx, const DWORD Stage, const D3DTEXTUREOP Type, const DWORD Value);
	void ResetTextureStageStateColor(const int nNumIdx);

private:
	LPDIRECT3D9 m_pD3D;						// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// Direct3Dデバイスへのポインタ

	LPDIRECT3DTEXTURE9 m_apTextureMT[NUM_MULTITARGET];	// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[NUM_MULTITARGET];	// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_pZBuffMT;						// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewportMT;							// テクスチャレンダリング用ビューポート

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;				// 頂点バッファへのポインタ
	float m_fPolygonMul;								// ポリゴンサイズの倍率

	bool m_isEnableFeedback;							// フィードバックエフェクトを使用するか
};

#endif