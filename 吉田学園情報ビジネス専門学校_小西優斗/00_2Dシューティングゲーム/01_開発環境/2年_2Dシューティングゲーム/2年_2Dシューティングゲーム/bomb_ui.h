#ifndef _BOMB_UI_H_
#define _BOMB_UI_H_
//=============================================================================
//
// ボムのUIクラスヘッダー	[bomb_ui.h]
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
#define MAX_BOMB_UI_TEXTURE		(1)
#define BOMB_UI_SIZE_X			(25.0f)
#define BOMB_UI_SIZE_Y			(45.0f)

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// タイトルBgクラス
//=============================================================================
class CBombUi : public CScene2D
{
public:
	CBombUi();			// コンストラクタ
	~CBombUi();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CBombUi * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);
	static HRESULT Load(void);
	static void UnLoad(void);

private:
	D3DXVECTOR3 m_move;
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BOMB_UI_TEXTURE];		// テクスチャ情報のポインタ
	D3DXVECTOR3 m_Pos;
};

#endif