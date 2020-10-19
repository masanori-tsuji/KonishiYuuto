#ifndef _CROW_H_
#define _CROW_H_

//=============================================================================
//
// カラスヘッダー	[crow.h]
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
#define CROW_POS_X			(-100.0f)	// 出現座標
#define CROW_POS_Y			(300.0f)	// 出現座標
#define CROW_MOVE_X			(5.0f)		// 移動量
#define CROW_MOVE_Y			(0.0f)		// 移動量
#define CROW_SIZE_X			(40.0f)		// サイズ
#define CROW_SIZE_Y			(40.0f)		// サイズ
#define MAX_CROW_TEXTURE	(1)			// テクスチャ

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// タイトルBgクラス
//=============================================================================
class CCrow : public CScene2D
{
public:
	CCrow();				// コンストラクタ
	~CCrow();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);					// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理

	static CCrow * Create(
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, TYPE type);		// ポリゴン生成
	static HRESULT Load(void);													// テクスチャロード
	static void UnLoad(void);													// テクスチャアンロード

private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_move;												// 移動量
	D3DXVECTOR3 m_size;												// サイズ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_CROW_TEXTURE];		// テクスチャ情報のポインタ
	int m_nCounter;													// 出現カウンター
};

#endif