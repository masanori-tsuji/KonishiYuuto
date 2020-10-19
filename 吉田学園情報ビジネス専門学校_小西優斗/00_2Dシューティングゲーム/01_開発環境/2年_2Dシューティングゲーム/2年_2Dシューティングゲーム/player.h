#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
// プレイヤークラスヘッダー [player.h]
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
#define DECISION_PLAYER_SIZE_X	(15)		// 当たり判定
#define DECISION_PLAYER_SIZE_Y	(15)		// 当たり判定
#define PLAYER_REMAIN_NUM		(2)			// 残機
#define PLAYER_SHOT_POS_X		(11.0f)		// 弾の出現位置
#define PLAYER_SHOT_POS_Y		(30.0f)		// 弾の出現位置

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;
class CShield;
class CGage;
class CLaser;
class CBombUi;

//=============================================================================
// プレイヤークラス
//=============================================================================
class CPlayer : public CScene2D
{
public:
	typedef enum
	{
		// プレイヤーの状態
		PLAYER_STATE_NONE = 0,		// 初期値
		PLAYER_STATE_NORMAL,		// 通常状態
		PLAYER_STATE_REVIVE,		// 復活時
		PLAYER_STATE_LASER,			// レーザー発射時
		PLAYER_STATE_MAX
	}PLAYER_STATE;

	CPlayer();			// コンストラクタ
	~CPlayer();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理

	static HRESULT Load(void);												// テクスチャのロード
	static void UnLoad(void);												// テクスチャのアンロード
	static CPlayer * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// ポリゴン生成

	void HitDamage(int nCount);												// ダメージ処理
	void PlayerControl(void);												// プレイヤー制御
	void Resurrection(void);												// 復活
	void UpdateState(void);													// プレイヤー状態更新
	bool BombCollision(void);												// ボムの当たり判定
	bool GetUseLaser(void);													// レーザーの情報
	bool GetShildInfo(void);												// シールドの情報
	CGage *GetGage(void);													// レーザーゲージ
	CLaser *GetLaser(void);													// レーザーの情報

private:
	D3DXVECTOR3 m_Pos;						// 座標
	D3DXVECTOR3 m_Move;						// 移動量
	PLAYER_STATE m_State;					// プレイヤーの状態
	int m_StateCount;						// プレイヤーの状態カウンター
	int m_BulletState;						// バレット間隔のカウンター
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャのポインタ
	int m_nLife;							// 自機の体力
	int m_nStock;							// ストック
	int m_nResurrectionCnt;					// 復活フレーム
	int m_nLaserFlame;						// レーザーのカウント
	int m_nLaserCounter;					// レーザーの切り替わりカウンター
	int m_nBombFlame;						// ボムの当たり判定フレーム
	int m_nBombNum;							// ボムに当たった敵のナンバー
	int m_nBombCount;						// ボムの数
	bool m_bShildInfo;						// シールドの情報
	bool m_bShildScaleInfo;					// シールドの拡大状況
	bool m_bPlayerDraw;						// プレイヤーの表示
	bool m_bUseLaser;						// レーザーを使っているか
	bool m_bPressInfo;						// 押したときの情報
	bool m_bBombUse;						// ボムを使ったとき
	CShield *m_pShield;						// シールドのポインタ
	CGage *m_pGage;							// ゲージのポインタ
	CLaser *m_pLaser;						// レーザーのポインタ
	CBombUi *m_pBombUi;						// ボムのUI
	TYPE m_BombType;						// ボムタイプ
};

#endif