#ifndef _SHOCK_H_
#define _SHOCK_H_
//=============================================================================
//
// 衝撃クラスヘッダー [shock.h]
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
#define MAX_SHOCK			(256)		// 衝撃の最大数
#define SHOCK_SIZE_X		(70)		// 衝撃の横幅
#define SHOCK_SIZE_Y		(70)		// 衝撃の縦幅
#define SHOCK_ANIM_SPEED	(5)			// 衝撃の速さ
#define SHOCK_ANIM_PATTERN	(8)			// 衝撃テクスチャのパターン数
#define MAX_SHOCK_TEXTURE	(2)			// テクスチャの最大数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// 爆発クラス
//=============================================================================
class CShock : public CScene2D
{
public:
	typedef enum
	{
		COLOR_TYPE_NONE = 0,
		COLOR_TYPE_WHITE,
		COLOR_TYPE_BLACK,
		COLOR_TYPE_MAX
	}COLOR_TYPE;

	CShock();			// コンストラクタ
	~CShock();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);			// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理

	static CShock * Create(												// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, COLOR_TYPE Ctype);

	void SetShock(COLOR_TYPE Ctype);									// 移動量設定
	static HRESULT Load(void);											// テクスチャロード
	static void UnLoad(void);											// テクスチャアンロード
	void SizeUp(void);													// サイズアップ
	void UpdateInvisible(void);											// 透明になる処理
private:
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_move;											// 移動
	D3DXVECTOR3 m_size;											// サイズ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_SHOCK_TEXTURE];	// テクスチャ情報のポインタ
	COLOR_TYPE m_Ctype;
	float m_fScaleNumber;										// スケールの拡大量
	float m_fScaleNum;											// スケールの拡大量
	int m_nSubAlpha;											// 透明度の減算用
	int m_nSubAlphaNum;											// 透明度減算値
};

#endif