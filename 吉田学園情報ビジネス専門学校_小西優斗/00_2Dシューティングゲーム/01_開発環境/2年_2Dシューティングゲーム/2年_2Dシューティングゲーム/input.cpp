//=====================================================
//
// 3Dゲーム キー入力処理 [input.cpp]
// Author:小西優斗
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "input.h"

//=====================================================
// static初期化処理
//=====================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=====================================================
// コンストラクタ
//=====================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=====================================================
// デストラクタ
//=====================================================
CInput::~CInput()
{

}

//=====================================================
// 初期化処理（仮想関数）
//=====================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		// 入力デバイス(キーボード)の作成
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}
		// データフォーマットを設定
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			return E_FAIL;
		}
		// 協調モードを設定(フォアグラウンド＆非排他モード)
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		// キーボードへのアクセス権を獲得(入力制御開始)
		m_pDevice->Acquire();
	}
	return S_OK;
}

//=====================================================
// 終了処理（仮想関数）
//=====================================================
void CInput::Uninit(void)
{
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		// キーボードへのアクセス権を開放(入力制御終了)
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}