//=====================================================
//
// 3Dゲーム キー入力処理 [keyboard.cpp]
// Author : Konishi Yuuto
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "input.h"
#include "keyboard.h"

//=====================================================
// コンストラクタ
//=====================================================
CKeyboard::CKeyboard()
{
	// キー情報クリア
	memset(m_aState, 0, sizeof(m_aState));
	memset(m_aStateTrigger, 0, sizeof(m_aStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));

}

//=====================================================
// デストラクタ
//=====================================================
CKeyboard::~CKeyboard()
{

}

//=====================================================
// キーボードの初期化
//=====================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	return S_OK;
}

//=====================================================
// キーボードの終了処理
//=====================================================
void CKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=====================================================
// キーボードの更新処理
//=====================================================
void CKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	int nCntKey;		// ローカル変数

	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// トリガー処理
			m_aStateTrigger[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey])&~aKeyState[nCntKey];
			// キープレス情報を保存
			m_aState[nCntKey] = aKeyState[nCntKey];
		}
	}

	// キーボードの更新処理
	else
	{
		// キーボードへのアクセス権を取得
		m_pDevice->Acquire();
	}
}

//=====================================================
// キーボードのプレス状態を取得
//=====================================================
bool CKeyboard::GetPress(int nKey)
{
	return (m_aState[nKey] & 0x80) ? true : false;
}
//=====================================================
// キーボードのトリガー
//=====================================================
bool CKeyboard::GetTrigger(int nKey)
{
	return(m_aStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//キーボードのリリース情報の取得
//=============================================================================
bool CKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}