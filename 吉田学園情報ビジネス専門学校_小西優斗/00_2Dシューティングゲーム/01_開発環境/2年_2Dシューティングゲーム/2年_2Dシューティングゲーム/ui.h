#ifndef _UI_H_
#define _UI_H_

//=============================================================================
//
// UIクラスヘッダー
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene2d.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define WALL_SIZE_X (350)		// 壁のサイズ
#define WALL_SIZE_Y (720)		// 壁のサイズ
#define WAKU_SIZE_X	(30)		// ワクのサイズ
#define WAKU_SIZE_Y	(720)		// ワクのサイズ
#define SHIP_SIZE_X (60)		// 自機のUIサイズ
#define SHIP_SIZE_Y (40)		// 自機のUIサイズ
#define MAX_UI_TEXTURE (3)		// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// BGクラス
//=============================================================================
class CUi : public CScene2D
{
public:
	typedef enum
	{
		UI_TYPE_NONE = 0,	// 初期値
		UI_TYPE_WALL,		// 壁
		UI_TYPE_WAKU,		// 枠
		UI_TYPE_REMAIN,		// ヒットポイント
		UI_TYPE_MAX
	}UI_TYPE;

	CUi();			// コンストラクタ
	~CUi();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CUi * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,		// ポリゴン生成
		TYPE type, UI_TYPE Utype);
	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード

	void SetType(UI_TYPE Utype);								// Uiタイプの設定

private:
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_move;											// 移動量
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_UI_TEXTURE];		// テクスチャ情報のポインタ
	UI_TYPE m_Utype;											// Uiの種類
};

#endif