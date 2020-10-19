#ifndef _MUZZLE_FLASH_H_
#define _MUZZLE_FLASH_H_

//=============================================================================
//
// マズルフラッシュクラスヘッダー [muzzle_flash.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MUZZLE_FLASH_SIZE_X				(18)		// サイズ
#define MUZZLE_FLASH_SIZE_Y				(18)		// サイズ
#define MAX_MUZZLE_FLASH_TEXTURE		(1)			// テクスチャ数
#define MUZZLE_FLASH_LIFE				(8)			// ライフ
#define MUZZLE_FLASH_NUM				(30)		// 数

//=============================================================================
// 前方宣言
//=============================================================================
class CEffect;

//=============================================================================
// BGクラス
//=============================================================================
class CMuzzleFlash : public CEffect
{
public:
	typedef enum
	{
		MUZZLE_FLASH_TYPE_NONE = 0,		// 初期値
		MUZZLE_FLASH_TYPE_BULLET,		// 弾
		MUZZLE_FLASH_TYPE_ENEMY,		// 敵
		MUZZLE_FLASH_TYPE_MAX
	}EFFECT_TYPE;

	typedef enum
	{
		MUZZLE_FLASH_COLOR_NONE = 0,	// 初期値
		MUZZLE_FLASH_COLOR_WHITE,		// 白いシールド
		MUZZLE_FLASH_COLOR_BLACK,		// 赤いシールド
		MUZZLE_FLASH_COLOR_MAX
	}EFFECT_COLOR;

	CMuzzleFlash();				// コンストラクタ
	~CMuzzleFlash();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CMuzzleFlash * Create(								// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size,
		TYPE type, EFFECT_COLOR Ctype, int nLife);

	void SetMove(D3DXVECTOR3 move);								// 移動量設定
	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード
	void SetColor(EFFECT_COLOR Ctype);							// カラー設定
	void InitColor(void);										// カラー初期化処理
	void GetPlayerPos(void);									// プレイヤー座標取得
private:
	D3DXVECTOR3 m_Pos;													// 座標
	D3DXVECTOR3 m_move;													// 移動量
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;									// 頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_MUZZLE_FLASH_TEXTURE];	// テクスチャ情報のポインタ
	EFFECT_COLOR m_Ctype;												// カラータイプ
	EFFECT_TYPE m_Etype;												// エフェクトタイプ
	int m_nLife;														// 体力　
	D3DXVECTOR3 m_TargetOld;											// 標的の古い座標
};

#endif