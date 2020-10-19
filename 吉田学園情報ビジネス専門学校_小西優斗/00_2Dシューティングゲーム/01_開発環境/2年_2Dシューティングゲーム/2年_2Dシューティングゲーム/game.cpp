//=============================================================================
//
// ゲームモードの処理 [game.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "keyboard.h"
#include "sound.h"
#include "light.h"
#include "laser.h"
#include "score.h"
#include "ui.h"
#include "bg.h"
#include "player.h"
#include "boss.h"
#include "manager.h"
#include "fade.h"
#include "remain.h"
#include "alert.h"
#include "icon.h"
#include "message.h"
#include "ranking.h"
#include "time.h"
#include "tutorial.h"

//=============================================================================
// static初期化
//=============================================================================
CScore *CGame::m_pScore = NULL;
int CGame::m_nCntEnemy = 0;
CBoss *CGame::m_pBoss = NULL;
CRemain *CGame::m_pRemain = NULL;
CAlert *CGame::m_pAlert = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CIcon *CGame::m_pIcon = NULL;
CMessage *CGame::m_pMessage = NULL;
CTime *CGame::m_pTime = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_bCurrentFlag = false;
	m_nCntFlame = 0;
	m_nCntThroughFrame = 0;
	m_nCntThroughEnemy = 0;
	m_bThroughFlag = false;
	m_bThroughSwitch = false;
	m_gameState = GAMESTATE_NONE;
	m_Phase = GAME_PHASE_NONE;
	m_nCounterGameState = 0;
	m_nCntEnemyPop = 0;
	m_bBossFlag = false;
	m_nGameCounter = 0;
	m_bFirstPhaseEnd = false;
	m_bSecondPhaseEnd = false;
	m_bThirdPhaseEnd = false;
	m_nCntFollow = 0;
	m_nIconSoundFlame = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	// スコア初期化処理
	m_pScore = new CScore;
	if (m_pScore != NULL)
	{
		m_pScore->Init(D3DXVECTOR3(WALL_SIZE_X * 0.75f, 50.0f, 0.0f),
			D3DXVECTOR3(NUMBER_SIZE_X, NUMBER_SIZE_Y, 0.0f),
			CScene::TYPE_SCORE);
	}

	if (m_pRemain == NULL)
	{
		// 残機の初期化
		m_pRemain = new CRemain;
		if (m_pRemain != NULL)
		{
			m_pRemain->Init(D3DXVECTOR3(WALL_SIZE_X * 0.73f, 690.0f, 0.0f),
				D3DXVECTOR3(REMAIN_SIZE_X, REMAIN_SIZE_Y, 0.0f),
				CScene::TYPE_SCORE);
		}
	}

	// 背景を生成
	CBackground::Create(
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
		D3DXVECTOR3(BG_SIZE_X, WALL_SIZE_Y, 0.0f), CScene::TYPE_BG);

	// プレイヤーの生成
	if (m_pPlayer == NULL)
	{
		m_pPlayer = CPlayer::Create(
			D3DXVECTOR3(600.0f, 600.0f, 0.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f), CScene::TYPE_PLAYER);
	}

	// 壁の生成
	CUi::Create(
		D3DXVECTOR3(WALL_SIZE_X / 2, WALL_SIZE_Y / 2, 0.0f), 
		D3DXVECTOR3(WALL_SIZE_X, WALL_SIZE_Y, 0.0f),
		CScene::TYPE_UI, CUi::UI_TYPE_WALL);

	// 壁の生成
	CUi::Create(
		D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X / 2, WALL_SIZE_Y / 2, 0.0f), 
		D3DXVECTOR3(WALL_SIZE_X, WALL_SIZE_Y, 0.0f),
		CScene::TYPE_UI, CUi::UI_TYPE_WALL);

	// 枠の生成
	CUi::Create(
		D3DXVECTOR3(WALL_SIZE_X - 14.0f, WALL_SIZE_Y / 2, 0.0f),
		D3DXVECTOR3(WAKU_SIZE_X , WAKU_SIZE_Y, 0.0f),
		CScene::TYPE_UI, CUi::UI_TYPE_WAKU);

	// 枠の生成
	CUi::Create(
		D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X + 14.0f, WALL_SIZE_Y / 2, 0.0f),
		D3DXVECTOR3(WAKU_SIZE_X, WAKU_SIZE_Y, 0.0f),
		CScene::TYPE_UI, CUi::UI_TYPE_WAKU);

	// 残機の表示
	CUi::Create(
		D3DXVECTOR3(WALL_SIZE_X * 0.58f, 685.0f, 0.0f),
		D3DXVECTOR3(SHIP_SIZE_X, SHIP_SIZE_Y, 0.0f),
		CScene::TYPE_UI, CUi::UI_TYPE_REMAIN);

	CTutorial::Create(
		D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X / 2 + 10.0f, WALL_SIZE_Y / 2, 0.0f), 
		D3DXVECTOR3(TUTORIAL_SIZE_X, TUTORIAL_SIZE_Y, 0.0f), CScene::TYPE_UI);

	//// サウンド設定
	CSound *pSound = NULL;
	pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_GAME);

	m_gameState = GAMESTATE_NORMAL;		// 通常状態に設定
	m_Phase = GAME_PHASE_FIRST;
	m_nCounterGameState = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	if (m_pScore != NULL)
	{
		CRanking::SetRanking(m_pScore->GetScore());
		// スコア終了処理
		m_pScore = NULL;
	}

	// タイム
	if (m_pTime != NULL)
	{
		m_pTime = NULL;
	}

	if (m_pPlayer != NULL)
	{
		// プレイヤー終了処理
		m_pPlayer = NULL;
	}

	if (m_pBoss != NULL)
	{
		// ボス終了処理
		m_pBoss = NULL;
	}

	if (m_pRemain != NULL)
	{
		m_pRemain = NULL;
	}

	// サウンド設定
	CSound *pSound = NULL;
	pSound = CManager::GetSound();
	pSound->Stop(CSound::SOUND_LABEL_BOSS);

//	pSound->Stop(CSound::SOUND_LABEL_GAME);

	// エネミーの総数を0にする
	m_nCntEnemy = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	// ゲームの設定
	SetGame();

	switch (m_gameState)	// ゲーム状態
	{
	case GAMESTATE_NORMAL:	// 通常状態の時
		break;
	case GAMESTATE_CLEAR:
		m_nCounterGameState++;	// フレーム数を数える
		if (m_nCounterGameState >= 60)	// 1秒後(60フレーム)
		{
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		//	StopSound(SOUND_LABEL_BGM002);
			m_gameState = GAMESTATE_NONE;	// 何もしていない状態にする
			m_nCounterGameState = 0;		// 初期化をする
		}
		break;

	case GAMESTATE_END:		// ゲーム終了時
		m_nCounterGameState++;	// フレーム数を数える
		if (m_nCounterGameState >= 60)	// 1秒後(60フレーム)
		{
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			//	StopSound(SOUND_LABEL_BGM002);
			m_gameState = GAMESTATE_NONE;	// 何もしていない状態にする
			m_nCounterGameState = 0;		// 初期化をする
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{

}


//=============================================================================
// ゲームの設定
//=============================================================================
void CGame::SetGame(void)
{
	m_nGameCounter++;
	int nChange = 0;

	switch (m_Phase)
	{
	// 第一フェーズ
	case GAME_PHASE_FIRST:

		switch (m_nGameCounter)
		{
		case 40:
			// 最初の出現
			for (int nCount = 0; nCount < 5; nCount++)
			{
				CEnemy::Create(
					D3DXVECTOR3(WALL_SIZE_X + 50.0f, 0.0f - (nCount * 60), 0.0f),
					D3DXVECTOR3(0.0f, 1.5f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_NORMAL);
				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;

		case 140:

			// 二回目の出現
			for (int nCount = 0; nCount < 5; nCount++)
			{
				CEnemy::Create(
					D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 50.0f, 0.0f - (nCount * 60), 0.0f),
					D3DXVECTOR3(0.0f, 1.5f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_NORMAL);
				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;

		case 300:
			for (int nCount = 0; nCount < 5; nCount++)
			{
				CEnemy::Create(
					D3DXVECTOR3(WALL_SIZE_X + 50.0f, 0.0f - (nCount * 60), 0.0f),
					D3DXVECTOR3(0.0f, 1.5f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_NORMAL);
				CScene::CountEnemy();
				m_nCntEnemy++;

			}

			for (int nCount = 0; nCount < 5; nCount++)
			{
				CEnemy::Create(
					D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 50.0f, 0.0f - (nCount * 60), 0.0f),
					D3DXVECTOR3(0.0f, 1.5f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_NORMAL);
				CScene::CountEnemy();
				m_nCntEnemy++;

			}
			break;

		case 540:
			for (int nCntSnake = 0; nCntSnake < 7; nCntSnake++)
			{
				// ジグザグ出現
				for (int nCount = 0; nCount < 3; nCount++)
				{
					CEnemy::Create(
						D3DXVECTOR3(WALL_SIZE_X + 75.0f + (nCount * 75), 0.0f - (nCntSnake * 40), 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_SNAKE_SIZE_X, ENEMY_SNAKE_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_SNAKE);
					CScene::CountEnemy();
					m_nCntEnemy++;
				}
				for (int nCount = 0; nCount < 3; nCount++)
				{
					CEnemy::Create(
						D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 85.0f - (nCount * 75), 0.0f - (nCntSnake * 40), 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_SNAKE_SIZE_X, ENEMY_SNAKE_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_SNAKE);
					CScene::CountEnemy();
					m_nCntEnemy++;
				}

			}
			// 第一フェーズ終了
			m_bFirstPhaseEnd = true;
			break;

		default:
			break;
		}
		break;

		// ～幕間　メッセージ～
	case GAME_PHASE_MESSAGE:
		switch (m_nGameCounter)
		{
		case 40:
			if (m_pIcon == NULL)
			{
				m_pIcon = CIcon::Create(
					D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 85.0f, 70.0f, 0.0f),
					D3DXVECTOR3(ICON_SIZE_X, ICON_SIZE_Y, 0.0f), CScene::TYPE_UI);

				// サウンド
				CSound *pSound = NULL;
				pSound = CManager::GetSound();
				pSound->Play(CSound::SOUND_LABEL_ICON);
			}
			break;

		}
		break;

		// 第三フェーズ
	case GAME_PHASE_SECOND:
		switch (m_nGameCounter)
		{
		case 40:
			for (int nCount = 0; nCount < CIRCLE_ENEMY_NUM; nCount++)
			{
				CEnemy::Circle(D3DXVECTOR3(
					WALL_SIZE_X + CIRCLE_DISTANCE*cosf(D3DXToRadian(CIRCLE_ANGLE + (nCount * CIRCLE_MAX_ANGLE / CIRCLE_ENEMY_NUM))),
					CIRCLE_INITIAL_Y + CIRCLE_DISTANCE*sinf(D3DXToRadian(CIRCLE_ANGLE + (nCount * CIRCLE_MAX_ANGLE / CIRCLE_ENEMY_NUM))), 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_CIRCLE,
					CIRCLE_DISTANCE, CIRCLE_ANGLE + (float)(nCount * CIRCLE_MAX_ANGLE / CIRCLE_ENEMY_NUM));
				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;

		case 210:
			for (int nCount = 0; nCount < CIRCLE_ENEMY_NUM; nCount++)
			{
				CEnemy::Circle(D3DXVECTOR3(
					SCREEN_WIDTH - WALL_SIZE_X - CIRCLE_DISTANCE*cosf(D3DXToRadian(-CIRCLE_ANGLE + (nCount * CIRCLE_MAX_ANGLE / CIRCLE_ENEMY_NUM))),
					CIRCLE_INITIAL_Y + CIRCLE_DISTANCE*sinf(D3DXToRadian(-CIRCLE_ANGLE + (nCount * CIRCLE_MAX_ANGLE / CIRCLE_ENEMY_NUM))), 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_CIRCLE,
					-CIRCLE_DISTANCE, -(CIRCLE_ANGLE + (float)(nCount * CIRCLE_MAX_ANGLE / CIRCLE_ENEMY_NUM)));
				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;

		case 390:		//260
			nChange = 0;
			for (int nCount = 0; nCount < 3; nCount++)
			{
				if (nChange % 2 == 0)
				{
					CEnemy::Create(
						D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 85.0f - ((float)nCount * 50.0f), 0.0f, 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;
				}
				else
				{
					CEnemy::Create(
						D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 85.0f - ((float)nCount * 50.0f), 0.0f + (nCount * 60), 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;
				}
			}
			break;
		case 470:
			nChange = 0;
			for (int nCount = 0; nCount < 3; nCount++)
			{
				if (nChange % 2 == 0)
				{
					CEnemy::Create(
						D3DXVECTOR3(WALL_SIZE_X + 175.0f - ((float)nCount * 50.0f), 0.0f, 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;
				}
				else
				{
					CEnemy::Create(
						D3DXVECTOR3(WALL_SIZE_X + 175.0f - ((float)nCount * 50.0f), 0.0f + (nCount * 60), 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;

				}
			}
			break;
		case 620:
			nChange = 0;
			for (int nCount = 0; nCount < 3; nCount++)
			{
				if (nChange % 2 == 0)
				{
					CEnemy::Create(
						D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 85.0f - ((float)nCount * 50.0f), 0.0f, 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;
				}
				else
				{
					CEnemy::Create(
						D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 85.0f - ((float)nCount * 50.0f), 0.0f + (nCount * 60), 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;
				}
			}
			break;

		case 700:
			nChange = 0;
			for (int nCount = 0; nCount < 3; nCount++)
			{
				if (nChange % 2 == 0)
				{
					CEnemy::Create(
						D3DXVECTOR3(WALL_SIZE_X + 175.0f - ((float)nCount * 50.0f), 0.0f, 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;
				}
				else
				{
					CEnemy::Create(
						D3DXVECTOR3(WALL_SIZE_X + 175.0f - ((float)nCount * 50.0f), 0.0f + (nCount * 60), 0.0f),
						D3DXVECTOR3(0.0f, 1.5f, 0.0f),
						D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_CURRENT);
					m_nCntEnemy++;
					CScene::CountEnemy();
					nChange++;

				}
			}
			break;

		case 900:
			CEnemy::Create(
				D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 940:
			CEnemy::Create(
				D3DXVECTOR3(WALL_SIZE_X + 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 980:
			CEnemy::Create(
				D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1020:
			CEnemy::Create(
				D3DXVECTOR3(WALL_SIZE_X + 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1060:
			CEnemy::Create(
				D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1100:
			CEnemy::Create(
				D3DXVECTOR3(WALL_SIZE_X + 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1140:
			CEnemy::Create(
				D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1180:
			CEnemy::Create(
				D3DXVECTOR3(WALL_SIZE_X + 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1220:
			CEnemy::Create(
				D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1260:
			CEnemy::Create(
				D3DXVECTOR3(WALL_SIZE_X + 75.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 1.5f, 0.0f),
				D3DXVECTOR3(ENEMY_CURRENT_SIZE_X, ENEMY_CURRENT_SIZE_Y, 0.0f),
				CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_THROUGH);
			m_nCntEnemy++;
			CScene::CountEnemy();
			break;

		case 1400:
			for (int nCount = 0; nCount < 10; nCount++)
			{
				CEnemy::Rotation(
					D3DXVECTOR3(600.0f, 500.0f, 0.0f),
					D3DXVECTOR3(0.0f, 1.5f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_ROTATION,
					INSIDE_ROTATION_RADIUS, 0.0f + ((float)nCount*(360.0f / 10.0f)));
				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			for (int nCount = 0; nCount < 10; nCount++)
			{
				CEnemy::Rotation(
					D3DXVECTOR3(600.0f, 500.0f, 0.0f),
					D3DXVECTOR3(0.0f, 1.5f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_BLACK, CEnemy::ENEMY_TYPE_ROTATION,
					OUTSIDE_ROTATION_RADIUS, 0.0f + ((float)nCount*(360.0f / 10.0f)));
				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;

		case 1800:
			for (int nCount = 0; nCount < 3; nCount++)
			{
				CEnemy::Direction(
					D3DXVECTOR3(WALL_SIZE_X + 75.0f, -10.0f - nCount*100.0f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_DIRECTION, 0);

				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			for (int nCount = 0; nCount < 3; nCount++)
			{
				CEnemy::Direction(
					D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, -10.0f - nCount*100.0f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_DIRECTION, 1);

				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;

		case 1900:
			for (int nCount = 0; nCount < 3; nCount++)
			{
				CEnemy::Direction(
					D3DXVECTOR3(WALL_SIZE_X + 75.0f, -10.0f - nCount*100.0f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_DIRECTION, 0);

				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			for (int nCount = 0; nCount < 3; nCount++)
			{
				CEnemy::Direction(
					D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, -10.0f - nCount*100.0f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_DIRECTION, 1);

				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;

		case 2000:

			m_bSecondPhaseEnd = true;

			for (int nCount = 0; nCount < 3; nCount++)
			{
				CEnemy::Direction(
					D3DXVECTOR3(WALL_SIZE_X + 75.0f, -10.0f - nCount*100.0f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_DIRECTION, 0);

				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			for (int nCount = 0; nCount < 3; nCount++)
			{
				CEnemy::Direction(
					D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 75.0f, -10.0f - nCount*100.0f, 0.0f),
					D3DXVECTOR3(ENEMY_NORMAL_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
					CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_DIRECTION, 1);

				CScene::CountEnemy();
				m_nCntEnemy++;
			}
			break;
		}
		break;

		case GAME_PHASE_THIRD:
			if (m_nGameCounter >= 40)
			{
				if (m_nGameCounter % 5 == 0 && m_nCntFollow <= 15)
				{
					m_bThirdPhaseEnd = true;
					int nDistance = 450 + (rand() % 300);

					CEnemy::Follow(
						D3DXVECTOR3(WALL_SIZE_X + nDistance *cosf(D3DXToRadian(0+((m_nCntFollow * 2) * (90 / 30)))),
						(SCREEN_HEIGHT - 100.0f) - nDistance *sinf(D3DXToRadian(0 + ((m_nCntFollow * 2) * (90 / 30)))),
						0.0f),
						D3DXVECTOR3(ENEMY_FOLLOW_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_FOLLOW);

					m_nCntFollow++;
					CScene::CountEnemy();
					m_nCntEnemy++;

				}
				else if (m_nGameCounter % 5 == 0 && m_nCntFollow > 15 && m_nCntFollow <= 60)
				{
					int nDistance = 450 + (rand() % 300);

					int nAngle = rand() % 90;

					CEnemy::Follow(
						D3DXVECTOR3(WALL_SIZE_X + nDistance *cosf(D3DXToRadian(nAngle)),
							(SCREEN_HEIGHT-100.0f) - nDistance *sinf(D3DXToRadian(nAngle)),
							0.0f),
						D3DXVECTOR3(ENEMY_FOLLOW_SIZE_X, ENEMY_NORMAL_SIZE_Y, 0.0f),
						CScene::TYPE_ENEMY, CEnemy::COLOR_TYPE_WHITE, CEnemy::ENEMY_TYPE_FOLLOW);

					m_nCntFollow++;
					CScene::CountEnemy();
					m_nCntEnemy++;

				}
			}

			break;

		case GAME_PHASE_FOUR:

			break;

	case GAME_PHASE_BOSS:
		if (m_bBossFlag == false)
		{

			if (m_pAlert == NULL)
			{
				m_pAlert = CAlert::Create(
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(580.0f, 300.0f, 0.0f),
					CScene::TYPE_WARNING);

				// サウンド設定
				CSound *pSound = NULL;
				pSound = CManager::GetSound();

				pSound->Stop(CSound::SOUND_LABEL_GAME);

				pSound->Play(CSound::SOUND_LABEL_ARERT);

			}
		}

		if (m_bBossFlag == false && m_pAlert != NULL)
		{
			bool bAlert = m_pAlert->GetAlertFlag();

			if (bAlert == true)
			{
				if (m_pBoss == NULL)
				{

					// サウンド設定
					CSound *pSound = NULL;
					pSound = CManager::GetSound();
					pSound->Play(CSound::SOUND_LABEL_BOSS);

					m_pBoss = CBoss::Create(D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 100.0f, -450.0f, 0.0f),
						D3DXVECTOR3(BOSS_SIZE_X, BOSS_SIZE_Y, 0.0f),
						CScene::TYPE_BOSS);

					CScene::CountEnemy();
					m_nCntEnemy++;
					m_bBossFlag = true;

					// タイム
					if (m_pTime == NULL)
					{
						m_pTime = new CTime;

						if (m_pTime != NULL)
						{
							m_pTime->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 50.0f, 50.0f, 0.0f),
								D3DXVECTOR3(TIME_SIZE_X, TIME_SIZE_Y, 0.0f), CScene::TYPE_SCORE);
						}

					}

				}

				if (m_pAlert != NULL)
				{
					// アラート終了処理
					m_pAlert->Uninit();
					m_pAlert = NULL;

					// サウンド設定
					CSound *pSound = NULL;
					pSound = CManager::GetSound();
					pSound->Stop(CSound::SOUND_LABEL_ARERT);
				}
			}
		}
	}

	// 第一フェーズの終了
	if (m_bFirstPhaseEnd == true)
	{
		int nNumEnemy = CScene::GetEnemyNum();

		if (nNumEnemy == 0)
		{
			m_Phase = GAME_PHASE_MESSAGE;
			m_bFirstPhaseEnd = false;
			m_nGameCounter = 0;
		}
	}

	// 第二フェーズの終了
	if (m_bSecondPhaseEnd == true)
	{
		int nNumEnemy = CScene::GetEnemyNum();

		if (nNumEnemy == 0)
		{
			m_Phase = GAME_PHASE_THIRD;
			m_bSecondPhaseEnd = false;
			m_nGameCounter = 0;
		}
	}

	// 第二フェーズの終了
	if (m_bThirdPhaseEnd == true)
	{
		int nNumEnemy = CScene::GetEnemyNum();

		if (nNumEnemy == 0)
		{
			m_Phase = GAME_PHASE_BOSS;
			m_bThirdPhaseEnd = false;
			m_nGameCounter = 0;
		}
	}


	// 幕間MESSAGEアイコンの終了
	if (m_pIcon != NULL)
	{
		m_nIconSoundFlame++;

		if (m_nIconSoundFlame % 80 == 0)
		{
			// サウンド
			CSound *pSound = NULL;
			pSound = CManager::GetSound();
			pSound->Play(CSound::SOUND_LABEL_ICON);
		}
		if (m_pIcon->GetEndIcon() == true)
		{
			m_pIcon->Uninit();
			m_pIcon = NULL;

			// メッセージ表示
			if (m_pMessage == NULL)
			{
				m_pMessage = CMessage::Create(
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
					D3DXVECTOR3(520.0f, 280.0f, 0.0f),
					CScene::TYPE_WARNING);

				// サウンドストップ
				CSound *pSound = NULL;
				pSound = CManager::GetSound();
				pSound->Play(CSound::SOUND_LABEL_MESSAGE);
			}
			return;
		}
	}

	// 幕間メッセージ終了
	if (m_pMessage != NULL)
	{
		if (m_pMessage->GetEndMessage() == true)
		{
			m_pMessage->Uninit();
			m_pMessage = NULL;

			// 第二フェイズへ
			m_Phase = GAME_PHASE_SECOND;
			m_nGameCounter = 0;
			return;
		}
	}

	int nCnt = m_nCntEnemy;			// 71
}

//=============================================================================
// スコア情報を返す
//=============================================================================
CScore * CGame::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// ボスの情報を返す
//=============================================================================
CBoss * CGame::GetBoss(void)
{
	return m_pBoss;
}

//=============================================================================
// 残機の情報
//=============================================================================
CRemain * CGame::GetRemain(void)
{
	return m_pRemain;
}

//=============================================================================
// プレイヤーの情報
//=============================================================================
CPlayer * CGame::GetPlayer(void)
{
	return m_pPlayer;
}