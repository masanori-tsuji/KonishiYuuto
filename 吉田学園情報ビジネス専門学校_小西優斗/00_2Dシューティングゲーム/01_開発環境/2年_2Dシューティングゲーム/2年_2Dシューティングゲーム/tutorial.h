#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//=============================================================================
//
// チュートリアルクラスヘッダー
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
#define MAX_TUTORIAL_TEXTURE		(1)			// テクスチャの数
#define TUTORIAL_SIZE_X			(300.0f)		// サイズ
#define TUTORIAL_SIZE_Y			(400.0f)		// サイズ

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// タイトルBgクラス
//=============================================================================
class CTutorial : public CScene2D
{
public:
	CTutorial();			// コンストラクタ
	~CTutorial();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CTutorial * Create(									// ポリゴンの生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);
	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード

private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_move;												// 移動量
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TUTORIAL_TEXTURE];	// テクスチャ情報のポインタ
};

#endif