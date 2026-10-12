//=============================================================================
//
//	ランキング [protocol_ranking.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _PROTOCOL_RANKING_H_
#define _PROTOCOL_RANKING_H_

//*****************************************************************************
// コマンドの種類
//*****************************************************************************
typedef enum COMMAND_TYPE
{
	COMMAND_TYPE_NONE = 0,			// なにもない
	COMMAND_TYPE_SET_RANKING,		// ランキング設定
	COMMAND_TYPE_GET_RANKING,		// ランキング取得
	COMMAND_TYPE_MAX				// 総数
}COMMAND_TYPE;

#endif // !_PROTOCOL_RANKING_H_