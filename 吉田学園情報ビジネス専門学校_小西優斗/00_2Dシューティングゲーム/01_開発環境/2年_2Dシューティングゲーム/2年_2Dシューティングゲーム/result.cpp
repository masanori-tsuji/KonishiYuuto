//=============================================================================
//
// リザルトの処理 [result.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//==================================================================
// インクルード
//==================================================================
#include "result.h"
#include "resultbg.h"
#include "renderer.h"
#include "keyboard.h"
#include "manager.h"
#include "fade.h"
#include "ranking.h"
#include "joypad.h"
#include "sound.h"

//==================================================================
// static初期化処理
//==================================================================
CResultBg *CResult::m_pResultBg = NULL;

//==================================================================
// コンストラクタ
//==================================================================
CResult::CResult()
{

}

//==================================================================
// デストラクタ
//==================================================================
CResult::~CResult()
{

}

//==================================================================
// 初期化処理
//==================================================================
HRESULT CResult::Init()
{
	// リザルト生成
	m_pResultBg = CResultBg::Create(
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CScene::TYPE_BG);

	// ランキング生成
	CRanking::Create();

	// サウンド設定
	CSound *pSound = NULL;
	pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//==================================================================
// 終了処理
//==================================================================
void CResult::Uninit(void)
{
	// 終了処理
	m_pResultBg->Uninit();
}

//==================================================================
// 更新処理
//==================================================================
void CResult::Update(void)
{
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
		// サウンド設定
		CSound *pSound = NULL;
		pSound = CManager::GetSound();
		pSound->Stop(CSound::SOUND_LABEL_RESULT);

		// フェード移行
		CFade *pFade = CManager::GetFade();
		pFade->SetFade(CManager::MODE_TITLE);

		return;
	}
}

//==================================================================
// 描画処理
//==================================================================
void CResult::Draw(void)
{

}

//==================================================================
// タイトル背景の情報
//==================================================================
CResultBg * CResult::GetResultBg(void)
{
	return m_pResultBg;
}