#ifndef _RANKING_H_
#define _RANKING_H_
//=============================================================================
//
// ランキングクラスヘッダー [ranking.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene.h"
#include "number.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_NUMBER		(8)				// ナンバーの桁数
#define MAX_RANKING		(5)				// ランキングの数
#define RANKING_POS_X	(950.0f)		// ランキングの横の座標
#define RANKING_POS_Y	(160.0f)		// ランキングの縦の座標
#define RANKING_SIZE_X	(55.0f)			// ランキングの横のサイズ
#define RANKING_SIZE_Y	(60.0f)			// ランキングの縦のサイズ

#define MY_RANKING_SIZE_X	(60.0f)		// ランキングの横のサイズ
#define MY_RANKING_SIZE_Y	(65.0f)		// ランキングの縦のサイズ

//=============================================================================
// 前置宣言
//=============================================================================
class CNumber;

//=============================================================================
// レンダラークラス
//=============================================================================
class CRanking : public CScene
{
public:
	CRanking();				// コンストラクタ
	~CRanking();			// デストラクタ

	// メンバ関数
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理
	static CRanking *Create(void);								// 生成
	void Save(void);											// ファイル出力
	static void Load(void);										// ファイル入力
	static void SetRanking(int nNumber);						// ランキングにスコアを与える
	void FlashPolygon(void);									// ポリゴンの点滅

private:
	CNumber *m_apRanking[MAX_RANKING][MAX_NUMBER];		// 桁数分
	CNumber *m_apMyRanking[MAX_NUMBER];					// 自分のスコア表示
	static int m_nRanking[MAX_RANKING];					// スコア
	int m_nNumRanking;									// ランキングの数
	static int m_nCurrentNum;							// 現在プレイしたスコア
	int m_nFlashFlame = 0;								// ランキングの点滅フレーム
	int m_nSubNumber;									// 点滅用変数
	int m_nSubNum;										// 点滅用変数
	static int m_nMyRank;								// 自分自身のスコア
};

#endif