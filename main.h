//=============================================================================
//
//	TKG [main.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MAIN_H_			// このマクロ定義がされていなかったら
#define _MAIN_H_			// 2重インクルード防止のマクロを定義する

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)        // メモリリーク感知newマクロ
#endif

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float AngleNormalize(float fAngle);
float DegreeToRadian(float fDegree);
int RadianToDegree(float fRadian);
float NumCirculation(float fNum, float fMin, float fMax);
void ToggleFullscreen(HWND hWnd);

#endif