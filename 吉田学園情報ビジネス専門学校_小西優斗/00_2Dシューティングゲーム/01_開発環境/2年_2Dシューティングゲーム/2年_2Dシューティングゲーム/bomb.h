#ifndef _BOMB_H_
#define _BOMB_H_

//=============================================================================
//
// ボムクラスヘッダー [bomb.h]
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
#define BOMB_SIZE_X				(50)		// 横サイズ
#define BOMB_SIZE_Y				(50)		// 縦サイズ
#define BOMB_SPARK_SIZE_X		(8)			// 横サイズ
#define BOMB_SPARK_SIZE_Y		(8)			// 縦サイズ
#define MAX_BOMB_TEXTURE		(1)			// テクスチャ数

#define BOMB_LIFE				(25)		// ボムの体力
#define BOMB_BASE_ANGLE			(90)		// 火花の向き
#define BOMB_NUM				(60)		// スパークの数
#define BOMB_SPPED				(5)			// スピード
#define BOMB_DISTANCE			(200)		// 発生距離
#define BOMB_ANGLE				(380)		// 角度

#define BOMB_SPARK_LIFE			(40)		// ボムの体力
#define BOMB_SPARK_BASE_ANGLE	(90)		// 火花の向き
#define BOMB_SPARK_NUM			(70)		// スパークの数
#define BOMB_SPARK_SPPED		(10)		// スピード
#define BOMB_SPARK_DISTANCE		(100)		// 発生距離
#define BOMB_SPARK_ANGLE		(361)		// 角度

//=============================================================================
// 前方宣言
//=============================================================================
class CEffect;

//=============================================================================
// BGクラス
//=============================================================================
class CBomb : public CEffect
{
public:
	typedef enum
	{
		BOMB_TYPE_NONE = 0,		// 初期値
		BOMB_TYPE_SHOCK,		// 衝撃派
		BOMB_TYPE_SPARK,		// 火花
		BOMB_TYPE_CENTER,		// 中心
		BOMB_TYPE_MAX			// 最大値
	}BOMB_TYPE;

	typedef enum
	{
		BOMB_COLOR_NONE = 0,		// 初期値
		BOMB_COLOR_WHITE,		// 衝撃派
		BOMB_COLOR_BLACK,		// 火花
		BOMB_COLOR_MAX			// 最大値
	}BOMB_COLOR;

	CBomb();			// コンストラクタ
	~CBomb();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CBomb * Create(										// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size,
		TYPE type, BOMB_TYPE Btype, BOMB_COLOR BColor,
		int nAngle, int nBaseAngle, int nDistance, int nLife);

	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード

	void InitColor(void);										// エフェクトのカラーを設定
private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_move;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// 頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BOMB_TEXTURE];	// テクスチャ情報のポインタ
	int m_nLife;												// 体力
	D3DXVECTOR3 m_TargetOld;									// 標的の古い座標
	BOMB_COLOR m_BColor;
};

#endif