#ifndef _BOSS_H_
#define _BOSS_H_
//=============================================================================
//
// ボスクラスヘッダー	[boss.h]
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
#define BOSS_SIZE_X			(150)			// ボスのサイズ
#define BOSS_SIZE_Y			(150)			// ボスのサイズ
#define MAX_BOSS_TEXTURE	(1)				// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CEnemy;
class CHpbar;

//=============================================================================
// ボスクラス
//=============================================================================
class CBoss : public CEnemy
{
public:
	typedef enum
	{
		BOSS_PHASE_NONE = 0,	// 初期値
		BOSS_PHASE_FIRST,		// 第一フェイズ
		BOSS_PHASE_CHANGE_01,	// 幕間
		BOSS_PHASE_SECOND,		// 第二フェイズ
		BOSS_PHASE_CHANGE_02,	// 幕間
		BOSS_PHASE_THIRD,		// 第三フェイズ
		BOSS_PHASE_EXPLOSION,	// 爆発フェーズ
		BOSS_PHASE_MAX
	}BOSS_PHASE;

	CBoss();			// コンストラクタ
	~CBoss();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理
	static CBoss * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// ポリゴン生成
	static HRESULT Load(void);												// テクスチャロード
	static void UnLoad(void);												// テクスチャアンロード

	void AppearanceMove(void);												// 登場時の移動量
	void FirstUpdateMove(void);												// エネミーの移動
	void FirstBulletShot(void);												// 弾の発射
	void FirstPhases(void);													// 第一フェーズ
	void ChangePhases(void);												// チェンジフェーズ
	void ChangeUpdateMove(void);											// チェンジの更新
	void ChangeFlash(void);													// 点滅
	void InitSecondPhase(void);												// 第二フェイズ初期化
	void SecondUpdateMove(void);											// エネミーの移動
	void SecondBulletShot(void);											// 弾の発射
	void SecondPhases(void);												// 第二フェーズ
	void ThirdPhase(void);													// 第三フェーズ
	void InitThirdPhase(void);												// 第三フェーズボス情報初期化
	void ThirdUpdateMove(void);												// 第三フェーズの動き
	void ThirdBulletShot(void);												// 第三フェーズの弾の処理
	void HitBossDamage(int nNumber);										// ボスの体力を減らす
	void FhaseUpdate(void);													// ボスのフェイズ更新
	void Retire(void);														// 撤退の関数
	void ExplosionPhase(void);												// ボスの爆発フェーズ
	int GetLife(void);														// ボスの体力の情報
	bool GetColor(void);													// 弾のカラーの情報
	bool GetExplosion(void);												// ボスの爆発時の情報

private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_Move;												// 移動
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BOSS_TEXTURE];		// テクスチャ情報のポインタ
	ENEMY_STATE m_State;											// エネミーの状態
	BOSS_PHASE m_Fhase;												// フェーズ
	int m_nLife;													// ライフ
	int m_nStateCnt;												// エネミーの状態カウンター
	int m_nBulletFlame;												// フレームのカウント
	int m_nBulletCnt;												// 弾が出た回数
	int m_nBossPhases;												// ボスのフェイズ
	int m_nFlashFlame;												// 点滅のフレーム
	int m_nSubNumber;												// カラー減算
	int m_nSubNum;													// カラーの減算量
	int m_nPhaseFlame;												// フェーズのフレーム
	int m_nRetireCount;												// 撤退のカウント
	int m_nExplosionFlame;											// 爆発しているカウント
	int m_nExplosionCount;											// 爆発のカウント
	int m_nEndCount;												// 終わるフレーム
	bool m_bColorFlag;												// 弾の色のフラグ
	bool m_bExplosion;												// ボスの爆発
	bool m_bBossEnd;												// ボスの終了処理
	CHpbar *pHpbar;													// HPバー
};

#endif