#ifndef _START_LOGO_H_
#define _START_LOGO_H_

//=============================================================================
//
// スタートロゴヘッダー	[start_logo.h]
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
#define START_LOGO_POS_Y			(600.0f)		// 座標
#define START_LOGO_SIZE_X			(600.0f)		// サイズ
#define START_LOGO_SIZE_Y			(50.0f)			// サイズ
#define MAX_START_LOGO_TEXTURE		(1)				// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// タイトルBgクラス
//=============================================================================
class CStartLogo : public CScene2D
{
public:
	CStartLogo();			// コンストラクタ
	~CStartLogo();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);					// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理

	static CStartLogo * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// ポリゴン生成
	static HRESULT Load(void);													// テクスチャロード
	static void UnLoad(void);													// テクスチャアンロード
	void FlashPolygon(void);
private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_move;												// 移動量
	D3DXVECTOR3 m_size;												// サイズ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_START_LOGO_TEXTURE];	// テクスチャ情報のポインタ
	int m_nFlashFlame;												// 点滅のフレーム
	int m_nSubNumber;												// カラー減算
	int m_nSubNum;													// カラーの減算量
	bool m_bDisappearFlag;											// 消えたときのフラグ
};

#endif