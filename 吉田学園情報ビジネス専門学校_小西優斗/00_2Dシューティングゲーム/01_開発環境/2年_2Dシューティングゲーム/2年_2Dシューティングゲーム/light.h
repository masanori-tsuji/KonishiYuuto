#ifndef _LIGHT_H_
#define _LIGHT_H_

//=============================================================================
//
// ライトクラスヘッダー		[light.h]
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
#define LIGHT_SIZE_X			(80)		// サイズ
#define LIGHT_SIZE_Y			(80)		// サイズ
#define LIGHT_ANIM_SPEED		(5)			// アニメーションのスピード
#define LIGHT_ANIM_PATTERN		(8)			// アニメーションのパターン
#define MAX_LIGHT_TEXTURE		(1)			// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// BGクラス
//=============================================================================
class CLight : public CScene2D
{
public:
	typedef enum
	{
		LIGHT_TYPE_NONE = 0,	// 初期値
		LIGHT_TYPE_WHITE,		// 白いシールド
		LIGHT_TYPE_BLACK,		// 赤いシールド
		LIGHT_TYPE_MAX
	}LIGHT_TYPE;

	CLight();					// コンストラクタ
	~CLight();					// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CLight * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,	// ポリゴンのサイズ
		D3DXVECTOR3 size, TYPE type, LIGHT_TYPE Ltype);

	void SetMove(D3DXVECTOR3 move);								// 移動量設定
	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード
	void SetColor(LIGHT_TYPE Ctype);							// カラーの設定
	void InitColor(void);										// カラーの初期化

private:
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_move;											// 移動量
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_LIGHT_TEXTURE];	// テクスチャ情報のポインタ
	LIGHT_TYPE m_Ltype;											// ライトの種類
	bool m_bScaleUp;											// 拡大終わりフラグ
};

#endif