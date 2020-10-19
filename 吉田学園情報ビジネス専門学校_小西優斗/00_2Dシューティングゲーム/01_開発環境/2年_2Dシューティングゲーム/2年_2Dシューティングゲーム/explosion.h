#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
//=============================================================================
//
// 爆発クラスヘッダー [explosion.h]
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
#define MAX_EXPLOSION								(256)		// 爆発の最大数
#define EXPLOSION_SIZE_X							(70)		// 爆発の横幅
#define EXPLOSION_SIZE_Y							(70)		// 爆発の縦幅
#define EXPLOSION_ANIM_SPEED						(5)			// 爆発の速さ
#define EXPLOSION_ANIM_PATTERN						(8)			// 爆発テクスチャのパターン数
#define MAX_EXPLOSION_TEXTURE						(3)			// テクスチャの最大数

#define BOSS_EXPLOSION_SIZE_X						(150.0f)	// 爆発の横幅
#define BOSS_EXPLOSION_SIZE_Y						(150.0f)	// 爆発の縦幅
#define BOSS_EXPLOSION_ANIM_SPEED					(2)			// ボスの爆発の速さ
#define BOSS_EXPLOSION_ANIM_PATTERN					(15)		// ボスの爆発テクスチャのパターン数

#define SURROUNDINGS_EXPLOSION_SIZE_X				(50.0f)		// 取り巻きの爆発
#define SURROUNDINGS_EXPLOSION_SIZE_Y				(50.0f)		// 取り巻きの爆発

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// 爆発クラス
//=============================================================================
class CExplosion : public CScene2D
{
public:
	typedef enum
	{
		COLOR_TYPE_NONE = 0,		// 初期値
		COLOR_TYPE_WHITE,			// 白
		COLOR_TYPE_BLACK,			// 黒
		COLOR_TYPE_BOSS,			// ボス
		COLOR_TYPE_MAX
	}COLOR_TYPE;

	CExplosion();			// コンストラクタ
	~CExplosion();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理

	static CExplosion * Create(D3DXVECTOR3 pos,  D3DXVECTOR3 move,	
		D3DXVECTOR3 size, TYPE type, COLOR_TYPE Ctype);						// ポリゴン生成

	void SetExplosion(D3DXVECTOR3 move, COLOR_TYPE Ctype);					// 移動量設定
	static HRESULT Load(void);												// テクスチャロード
	static void UnLoad(void);												// テクスチャアンロード

private:
	D3DXVECTOR3 m_Pos;													// 座標
	D3DXVECTOR3 m_move;													// 移動
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_EXPLOSION_TEXTURE];		// テクスチャ情報のポインタ
	COLOR_TYPE m_Ctype;													// カラータイプ
};

#endif