#ifndef _SPARK_H_
#define _SPARK_H_

//=============================================================================
//
// 火花クラスヘッダー [spark.h]
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
#define SPARK_SIZE_X			(5)			// 横サイズ
#define SPARK_SIZE_Y			(5)			// 縦サイズ
#define MAX_SPARK_TEXTURE		(1)			// テクスチャ数

#define SPARK_LIFE				(30)		// スパークの体力
#define SPARK_BASE_ANGLE		(90)		// 火花の向き
#define SPARK_NUM				(50)		// スパークの数
#define SPARK_SPPED				(3)			// スピード
#define SPARK_DISTANCE			(5)			// 発生距離
#define SPARK_ANGLE				(360)		// 角度

#define SPARK_BOSS_NUM			(2)			// スパークの数
#define SPARK_BASE_BOSS_ANGLE	(-90)		// 火花の向き
#define SPARK_BOSS_SPPED		(3)			// スピード
#define SPARK_BOSS_DISTANCE		(5)			// 発生距離
#define SPARK_BOSS_ANGLE		(60)		// 角度

#define SPARK_CHANGE_LIFE		(17)		// スパークの体力
#define SPARK_CHANGE_NUM		(2)			// スパークの数
#define SPARK_CHANGE_SPPED		(5)			// スピード
#define SPARK_CHANGE_DISTANCE	(20)		// 発生距離
#define SPARK_CHANGE_ANGLE		(20)		// 角度

//=============================================================================
// 前方宣言
//=============================================================================
class CEffect;

//=============================================================================
// BGクラス
//=============================================================================
class CSpark : public CEffect
{
public:
	CSpark();			// コンストラクタ
	~CSpark();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CSpark * Create(										// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size,
		TYPE type, int nAngle,  int nBaseAngle, int nDistance, int nLife);
	static HRESULT Load(void);									// テクスチャロード
	static void UnLoad(void);									// テクスチャアンロード

	void InitColor(void);										// エフェクトのカラーを設定
	void SetMove(D3DXVECTOR3 move);								// 移動の設定

private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_move;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_SPARK_TEXTURE];		// テクスチャ情報のポインタ
	int m_nLife;							// 体力
	D3DXVECTOR3 m_TargetOld;				// 標的の古い座標
};

#endif