#ifndef _SHIELD_H_
#define _SHIELD_H_

//=============================================================================
//
// シールドクラスヘッダー	[shield.h]
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
#define SHIELD_SIZE_X			(94)	// サイズ
#define SHIELD_SIZE_Y			(94)	// サイズ
#define SHIELD_ANIM_SPEED		(5)		// アニメーションの速度
#define SHIELD_ANIM_PATTERN		(8)		// アニメーションのパターン
#define MAX_SHILD_TEXTURE		(2)		// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// BGクラス
//=============================================================================
class CShield : public CScene2D
{
public:
	typedef enum
	{
		SHIELD_TYPE_NONE = 0,	// 初期値
		SHIELD_TYPE_WHITE,		// 白いシールド
		SHIELD_TYPE_BLACK,		// 赤いシールド
		SHIELD_TYPE_MAX
	}SHIELD_TYPE;

	CShield();			// コンストラクタ
	~CShield();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CShield * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,	// ポリゴン生成
		D3DXVECTOR3 size, TYPE type, SHIELD_TYPE Stype);

	void SetMove(D3DXVECTOR3 move);								// 移動量設定
	bool GetbScale(void);										// バリアの大きさ情報
	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード
private:
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_move;											// 移動量
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_SHILD_TEXTURE];	// テクスチャ情報のポインタ
	bool m_bScaleUp;											// 拡大終わりフラグ
};

#endif