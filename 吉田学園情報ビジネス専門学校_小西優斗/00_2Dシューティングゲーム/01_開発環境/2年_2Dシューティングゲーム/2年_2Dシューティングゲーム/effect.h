#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
//
// エフェクトクラスヘッダー
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
#define EFFECT_SIZE_X			(30)	// サイズ
#define EFFECT_SIZE_Y			(30)	// サイズ
#define EFFECT_ANIM_SPEED		(5)		// アニメーションのスピード
#define EFFECT_ANIM_PATTERN		(8)		// アニメーションのパターン
#define MAX_EFFECT_TEXTURE		(1)		// テクスチャ数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// BGクラス
//=============================================================================
class CEffect : public CScene2D
{
public:
	typedef enum
	{
		EFFECT_TYPE_NONE = 0,	// 初期値
		EFFECT_TYPE_BULLET,		// 弾
		EFFECT_TYPE_ENEMY,		// 敵
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	typedef enum
	{
		EFFECT_COLOR_NONE = 0,	// 初期値
		EFFECT_COLOR_WHITE,		// 白いシールド
		EFFECT_COLOR_BLACK,		// 赤いシールド
		EFFECT_COLOR_MAX
	}EFFECT_COLOR;

	CEffect();			// コンストラクタ
	~CEffect();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static CEffect * Create(										// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, 
		TYPE type, EFFECT_COLOR Ctype);

	void SetMove(D3DXVECTOR3 move);									// 移動量設定
	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード
	void SetColor(EFFECT_COLOR Ctype);								// 色の設定
	void InitColor(void);											// 色の初期化

private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_move;												// 移動量
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_EFFECT_TEXTURE];		// テクスチャ情報のポインタ
	EFFECT_COLOR m_Ctype;											// カラータイプ
	EFFECT_TYPE m_Etype;											// エフェクトタイプ
	bool m_bScaleDown;												// 拡大終わりフラグ
};

#endif