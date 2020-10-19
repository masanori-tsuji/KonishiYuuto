#ifndef _GAME_H_
#define _GAME_H_
//=============================================================================
//
// タイトルゲームヘッダー [title.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayer;
class CEnemy;
class CLight;
class CLaser;
class CScore;
class CBoss;
class CRemain;
class CAlert;
class CIcon;
class CMessage;
class CTime;

//=============================================================================
// ゲームクラス
//=============================================================================
class CGame
{
public:
	//=====================================================
	// インクルード
	//=====================================================
	typedef enum
	{
		GAMESTATE_NONE = 0,			// 何もしていない状態
		GAMESTATE_NORMAL,			// 通常状態
		GAMESTATE_CLEAR,			// クリア状態
		GAMESTATE_END,				// ゲーム終了状態（画面遷移開始待ち状態）
		GAMESTATE_MAX
	}GAMESTATE;

	typedef enum
	{
		GAME_PHASE_NONE = 0,		// フェーズの列挙
		GAME_PHASE_FIRST,			// 第一フェーズ
		GAME_PHASE_MESSAGE,			// 幕間のメッセージ
		GAME_PHASE_SECOND,			// 第二フェーズ
		GAME_PHASE_THIRD,			// 第三フェーズ
		GAME_PHASE_FOUR,			// 第四フェーズ
		GAME_PHASE_BOSS,			// ボスフェーズ
		GAME_PHASE_MAX
	}GAME_PHASE;

	CGame();		// コンストラクタ
	~CGame();		// デストラクタ

	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetGame(void);

	static CScore *GetScore(void);			// スコアの情報
	static CBoss *GetBoss(void);			// ボスの情報
	static CRemain *GetRemain(void);		// 残機の情報
	static CPlayer *GetPlayer(void);		// プレイヤーの情報
private:
	static CScore *m_pScore;				// スコアのポインタ
	static CBoss *m_pBoss;					// ボスのポインタ
	static CRemain *m_pRemain;				// 残機のポインタ
	static CAlert *m_pAlert;				// 警告アラートのポインタ
	static CPlayer *m_pPlayer;				// プレイヤーのポインタ
	static CIcon *m_pIcon;					// プレイヤーのアイコン
	static CMessage *m_pMessage;			// メッセージのポインタ
	static CTime *m_pTime;					// タイムのポインタ
	static int m_nCntEnemy;					// エネミーの数
	bool m_bCurrentFlag;					// カウントのフラグ
	int m_nCntFlame;						// フレームのカウント
	int m_nCntEnemyPop;						// エネミーの出現カウント
	int m_nCntThroughFrame;					// エネミーの出るカウント
	int m_nCntThroughEnemy;					// エネミーの出現数カウント
	int m_nCntFollow;						// 追従の敵のカウント
	int m_nIconSoundFlame;					// アイコン音のフレーム
	bool m_bThroughFlag;					// カウントのフラグ
	bool m_bThroughSwitch;					// 入れ替わりのフラグ
	GAMESTATE m_gameState;					// ゲームの状態
	GAME_PHASE m_Phase;						// フェーズ
	int m_nCounterGameState;				// ゲームの状態カウンター
	int m_nGameCounter;						// 敵の出現カウンター
	bool m_bFirstPhaseEnd;					// 第一フェーズ終了
	bool m_bSecondPhaseEnd;					// 第二フェイズ終了
	bool m_bThirdPhaseEnd;					// 第三フェーズの終了
	bool m_bBossFlag;						// ボスのフラグ
};
#endif