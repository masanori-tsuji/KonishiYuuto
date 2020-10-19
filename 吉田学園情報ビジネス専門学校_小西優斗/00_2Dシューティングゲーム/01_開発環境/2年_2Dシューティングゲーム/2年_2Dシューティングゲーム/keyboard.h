//=====================================================
//
// キーボード [keyboard.h]
// Author : Konishi Yuuto
//
//=====================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//=====================================================
// インクルードファイル
//=====================================================
#include "main.h"
#include "input.h"

//=====================================================
// マクロ定義
//=====================================================
#define NUM_KEY_MAX (256)			// キー最大数

//=====================================================
// 前方宣言
//=====================================================
class CInput;

//=====================================================
// インプットクラス
//=====================================================
class CKeyboard : public CInput
{
public:
	CKeyboard();		// コンストラクタ
	~CKeyboard();		// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	bool GetPress(int nKey);					// 長押しの処理
	bool GetTrigger(int nKey);				// 押された処理
	bool GetRelease(int nKey);

private:
	 BYTE m_aState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
	 BYTE m_aStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報
	 BYTE m_aKeyStateRelease[NUM_KEY_MAX];	//キーボードのリリース情報

};

#endif
