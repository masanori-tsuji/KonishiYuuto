//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//==================================================================
// インクルード
//==================================================================
#include "title.h"
#include "titlebg.h"
#include "renderer.h"
#include "keyboard.h"
#include "manager.h"
#include "fade.h"
#include "start_logo.h"
#include "crow.h"
#include "manager.h"
#include "joypad.h"
#include "sound.h"

//==================================================================
// static初期化処理
//==================================================================
CTitleBg *CTitle::m_pTitleBg = NULL;

//==================================================================
// コンストラクタ
//==================================================================
CTitle::CTitle()
{
	m_nCrowCreateFlame = 499;
}

//==================================================================
// デストラクタ
//==================================================================
CTitle::~CTitle()
{

}

//==================================================================
// 初期化処理
//==================================================================
HRESULT CTitle::Init()
{
	// タイトル
	m_pTitleBg = CTitleBg::Create(
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CScene::TYPE_BG );

	// タイトルロゴ
	CStartLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, START_LOGO_POS_Y, 0.0f),
		D3DXVECTOR3(START_LOGO_SIZE_X, START_LOGO_SIZE_Y, 0.0f), CScene::TYPE_UI);

	// サウンド設定
	CSound *pSound = NULL;
	pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//==================================================================
// 終了処理
//==================================================================
void CTitle::Uninit(void)
{
	if (m_pTitleBg != NULL)
	{
		m_pTitleBg->Uninit();
	}
}

//==================================================================
// 更新処理
//==================================================================
void CTitle::Update(void)
{
	m_nCrowCreateFlame++;

	// 一定間隔で
	if (m_nCrowCreateFlame % 500 == 0)
	{
		for (int nCount = 0; nCount < 3; nCount++)
		{
			// カラス生成
			CCrow::Create(D3DXVECTOR3(CROW_POS_X -nCount*90.0f, CROW_POS_Y, 0.0f), D3DXVECTOR3(CROW_MOVE_X, CROW_MOVE_Y, 0.0f),
				D3DXVECTOR3(CROW_SIZE_X, CROW_SIZE_Y, 0.0f), CScene::TYPE_ENEMY);
		}
	}

	// キーボード更新
	CKeyboard *pKeyboard = CManager::GetKeyboard();
	
	// フェード情報
	CFade::FADE fade = CManager::GetFade()->Get();

	// Wキーを押したとき
	if (pKeyboard->GetTrigger(DIK_RETURN) && 
		fade == CFade::FADE_NONE ||
		CManager::GetJoypad()->GetJoystickTrigger(11, 0) && 
		fade == CFade::FADE_NONE)
	{

		CSound *pSound = NULL;
		pSound = CManager::GetSound();
		pSound->Stop(CSound::SOUND_LABEL_TITLE);

		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	}
}

//==================================================================
// 描画処理
//==================================================================
void CTitle::Draw(void)
{

}

//==================================================================
// タイトル背景の情報
//==================================================================
CTitleBg * CTitle::GetTitleBg(void)
{
	return m_pTitleBg;
}