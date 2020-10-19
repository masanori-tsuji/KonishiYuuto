#ifndef _SURROUNDINGS_H_
#define _SURROUNDINGS_H_

//=============================================================================
//
// 取り巻きクラスヘッダー	[surroundings.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SURROUNDINGS_SIZE_X			(50)	// サイズ
#define SURROUNDINGS_SIZE_Y			(50)	// サイズ
#define MAX_SURROUNDINGS_TEXTURE	(1)		// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CEnemy;
class CHpbar;

//=============================================================================
// ボスクラス
//=============================================================================
class CSurroundings : public CEnemy
{
public:
	CSurroundings();			// コンストラクタ
	~CSurroundings();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);						// 初期化処理
	void Uninit(void);																// 終了処理
	void Update(void);																// 更新処理
	void Draw(void);																// 描画処理

	static CSurroundings * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// ポリゴン生成
	static HRESULT Load(void);														// テクスチャロード
	static void UnLoad(void);														// テクスチャアンロード

	void HitSurroundings(int nNumber);												// 体力を減らす
	void BulletShot(void);															// 弾を打つ処理

	int GetLife(void);
private:
	D3DXVECTOR3 m_Pos;													// 座標
	D3DXVECTOR3 m_Move;													// 移動
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_SURROUNDINGS_TEXTURE];	// テクスチャ情報のポインタ
	ENEMY_STATE m_State;												// エネミーの状態
	int m_nLife;														// ライフ
	int m_nStateCnt;													// エネミーの状態カウンター
	int m_nBulletFlame;													// フレームのカウント
	int m_nBulletCnt;													// 弾が出た回数
	int m_nFlashFlame;													// 点滅のフレーム
	int m_nSubNumber;													// カラー減算
	int m_nSubNum;														// カラーの減算量
	bool m_bColorFlag;													// 弾の色のフラグ
};

#endif