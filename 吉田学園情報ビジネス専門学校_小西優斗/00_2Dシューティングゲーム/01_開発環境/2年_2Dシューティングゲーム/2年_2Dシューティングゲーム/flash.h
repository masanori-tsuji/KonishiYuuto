#ifndef _FLASH_H_
#define _FLASH_H_

//=============================================================================
//
// フラッシュクラスヘッダー [flash.h]
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
#define FLASH_SIZE_X			(27)			// サイズ
#define FLASH_SIZE_Y			(27)			// サイズ
#define MAX_FLASH_TEXTURE		(1)				// テクスチャの数
#define FLASH_LIFE				(5)				// ライフ
#define FLASH_NUM				(30)			// 数

//=============================================================================
// 前方宣言
//=============================================================================
class CEffect;

//=============================================================================
// フラッシュクラス
//=============================================================================
class CFlash : public CEffect
{
public:
	typedef enum
	{
		FLASH_TYPE_NONE = 0,	// 初期値
		FLASH_TYPE_TOP,			// 白い
		FLASH_TYPE_SHOT,		// 赤い
		FLASH_TYPE_MAX
	}FLASH_TYPE;

	typedef enum
	{
		FLASH_COLOR_NONE = 0,	// 初期値
		FLASH_COLOR_WHITE,		// 白いシールド
		FLASH_COLOR_BLACK,		// 赤いシールド
		FLASH_COLOR_MAX
	}FLASH_COLOR;

	CFlash();			// コンストラクタ
	~CFlash();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CFlash * Create(										// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, 
		FLASH_COLOR Ctype,  FLASH_TYPE Ftype , int nLife);

	void SetMove(D3DXVECTOR3 move);								// 移動量設定
	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード
	void SetColor(FLASH_COLOR Ctype);							// 色の設定
	void InitColor(void);										// 色の初期化
	void GetPlayerPos(void);									// プレイヤー情報の取得
	void GetLaserTop(void);										// レーザーの情報
	void SetFType(FLASH_TYPE Ftype);							// レーザーのタイプを設定

private:
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_move;											// 移動量
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// 頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FLASH_TEXTURE];	// テクスチャ情報のポインタ
	FLASH_COLOR m_Ctype;										// カラータイプ
	FLASH_TYPE m_Ftype;											// エフェクトタイプ
	int m_nLife;												// ライフ
	D3DXVECTOR3 m_TargetOld;									// 標的の古い座標
};

#endif