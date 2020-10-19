#ifndef _LASER_H_
#define _LASER_H_
//=============================================================================
//
// レーザークラスヘッダー	[laser.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene2d.h"
#include "flash.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define LASER_SIZE_X			(95)		// サイズ
#define LASER_SIZE_Y			(10)		// サイズ
#define LASER_ANIM_SPEED		(5)			// アニメーションの速さ
#define LASER_ANIM_PATTERN		(8)			// アニメーションのパターン
#define MAX_LASER_TEXTURE		(1)			// テクスチャ数
#define MAX_FLASH_NUM			(64)		// 数
#define LASER_FLAME				(30)		// レーザーのフレーム

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;
class CFlash;

//=============================================================================
// BGクラス
//=============================================================================
class CLaser : public CScene2D
{
public:
	typedef enum
	{
		LASER_COLOR_NONE = 0,	// 初期値
		LASER_COLOR_WHITE,		// 白い
		LASER_COLOR_BLACK,		// 赤い
		LASER_COLOR_MAX
	}LASER_COLOR;

	CLaser();			// コンストラクタ
	~CLaser();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static CLaser * Create(											// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size,
		TYPE type, LASER_COLOR Ctype);

	void SetInfo(D3DXVECTOR3 move, D3DXVECTOR3 size);				// レーザーの情報設定
	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード
	void SetColor(LASER_COLOR Ctype);								// 変数の代入
	void InitColor(void);											// カラー設定
	void LaserUpdate(void);											// レーザーの処理
	void DisappearLaser(void);										// レーザーが消える処理
	bool Collision(void);											// レーザーが当たったとき
	void FlashCreate(D3DXVECTOR3 pos, CFlash::FLASH_TYPE Ftype);	// レーザーのフラッシュ
	D3DXVECTOR3 GetSize(void);										// サイズ情報
	D3DXVECTOR3 GetTop(void);										// 先端情報
private:
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_move;											// 移動量
	D3DXVECTOR3 m_Size;											// サイズ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// 頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_LASER_TEXTURE];	// テクスチャ情報のポインタ
	LASER_COLOR m_Ctype;										// カラータイプ
	bool m_bUse;												// レーザーを使っている
	D3DXVECTOR3 m_PolygonPos;									// ポリゴンの座標
	D3DXVECTOR3 m_PolygonSize;									// ポリゴンのサイズ
	D3DXVECTOR3 m_PolygonTop;									// ポリゴンの先端
};

#endif