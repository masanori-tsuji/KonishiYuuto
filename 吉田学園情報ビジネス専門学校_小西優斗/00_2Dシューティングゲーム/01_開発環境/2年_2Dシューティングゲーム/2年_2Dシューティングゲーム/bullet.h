#ifndef _BULLET_H_
#define _BULLET_H_
//=============================================================================
//
// バレットクラスヘッダー [bullet.h]
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
#define MAX_BULLET (512)				// 弾の最大数
#define BULLET_SIZE_X (25)				// 弾の横幅
#define BULLET_SIZE_Y (25)				// 弾の縦幅
#define COUNTER_BULLET_SIZE_X (20)		// カウンター時の弾
#define COUNTER_BULLET_SIZE_Y (20)		// カウンター時の弾
#define FIRE_WORKS_SIZE_X	(20)		// 花火のサイズ
#define FIRE_WORKS_SIZE_Y	(20)		// 花火のサイズ
#define MAX_BULLET_TEXTURE (4)			// テクスチャの最大数

#define FIRE_WORKS_NUM	(15)			// バレットの数
#define FIRE_WORKS_LIFE	(120)			// バレットの体力

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// バレットクラス
//=============================================================================
class CBullet : public CScene2D
{
public:
	typedef enum
	{
		BULLET_TYPE_NONE = 0,	// 初期値
		BULLET_TYPE_PLAYER,		// 自機の弾
		BULLET_TYPE_ENEMY,		// 敵の弾
		BULLET_TYPE_BOSS,		// ボスの弾
		BULLET_TYPE_MAX			// 最大数
	}BULLET_TYPE;

	typedef enum
	{
		BULLET_COLOR_NONE = 0,	// 初期値
		BULLET_COLOR_WHITE,		// 白い弾
		BULLET_COLOR_BLACK,		// 黒い弾
		BULLET_COLOR_MAX
	}BULLET_COLOR;

	typedef enum
	{
		BULLET_CATEGORY_NONE = 0,	// 初期値
		BULLET_CATEGORY_NORMAL,		// 通常
		BULLET_CATEGORY_CURRENT,	// 追従弾
		BULLET_CATEGORY_COUNTER,	// カウンター弾
		BULLET_CATEGORY_FIREWORKS,	// 花火弾
		BULLET_CATEGORY_MAX			// 最大数
	}BULLET_CATEGORY;

	CBullet();			// コンストラクタ
	~CBullet();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);						// 初期化処理
	void Uninit(void);																// 終了処理
	void Update(void);																// 更新処理
	void Draw(void);																// 描画処理
	void SetBullet(D3DXVECTOR3 move, int nLife, D3DXVECTOR3 size);							// 弾の情報を設定
	void SetType(BULLET_TYPE type, BULLET_COLOR Ctype);										// タイプ設定
	void SetCategory(BULLET_CATEGORY Bcate);												// カテゴリ設定
	static HRESULT Load(void);																// テクスチャロード
	static void UnLoad(void);																// テクスチャアンロード
	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size);										// 当たり判定
	BULLET_COLOR GetCtype(void);															// 色のタイプ情報
	void DecreaseLife(int nNumber);															// ライフを減らす
	void FlashPolygon(void);																// ポリゴンの点滅	
	D3DXVECTOR3 GetSize(void);																// サイズの情報
	void FireWorksUpdate(void);																// 花火更新処理

	static CBullet * Create(																// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size,
		TYPE type, BULLET_TYPE Btype, BULLET_COLOR Ctype,
		int nLife);	

	static CBullet *CounterAttack(D3DXVECTOR3 pos, D3DXVECTOR3 size,						// カウンター弾
		TYPE type, BULLET_TYPE Btype, BULLET_COLOR Ctype, BULLET_CATEGORY Bcate,
		int nLife, int nBulletNum);

	static CBullet *CurrentAttack(D3DXVECTOR3 pos, D3DXVECTOR3 Ppos, D3DXVECTOR3 size,		// プレイヤーの現在地に弾を打つ
		TYPE type, BULLET_TYPE Btype, BULLET_COLOR Ctype,
		int nLife);

	static CBullet *FireWorks(D3DXVECTOR3 pos, D3DXVECTOR3 size,							// 花火型バレット
		TYPE type, BULLET_TYPE Btype, BULLET_COLOR Ctype, BULLET_CATEGORY Bcate, int nLife);

private:
	D3DXVECTOR3 m_Pos;												// 位置
	D3DXVECTOR3 m_move;												// 移動
	D3DXVECTOR3 m_Size;												// サイズ
	int m_nLife;													// 制限距離
	D3DXVECTOR3 m_Target;											// ターゲットの場所
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BULLET_TEXTURE];		// テクスチャ情報のポインタ
	BULLET_TYPE m_type;												// バレットタイプ
	BULLET_COLOR m_Ctype;											// バレットカラー
	BULLET_CATEGORY m_Bcategory;									// バレットの種類
	int m_nSubColor;
	int m_nSubCol;
};

#endif