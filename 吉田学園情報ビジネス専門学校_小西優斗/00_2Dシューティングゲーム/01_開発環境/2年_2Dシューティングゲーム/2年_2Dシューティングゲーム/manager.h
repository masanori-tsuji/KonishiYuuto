#ifndef _MANAGER_H_
#define _MANAGER_H_
//=============================================================================
//
// マネージャー処理のヘッダー [manager.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CTitle;
class CRenderer;
class CInput;
class CKeyboard;
class CSound;
class CGame;
class CResult;
class CFade;
class CInputJoypad;

//=============================================================================
// マネージャークラス
//=============================================================================
class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,		// タイトル画面
		MODE_GAME,			// ゲーム画面
		MODE_RESULT,		// リザルト画面
		MODE_MAX			// 最大数
	}MODE;

	CManager();		// コンストラクタ
	~CManager();	// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	void SetFPS(int nCountFPS);										// デバッグ表示
	static CRenderer *GetRenderer(void);							// レンダラーの情報
	static CKeyboard *GetKeyboard(void);							// キーボードの情報
	static CSound *GetSound(void);									// サウンドの情報
	static CTitle *GetTitle(void);									// タイトルの情報
	static CResult *GetResult(void);								// リザルトの情報
	static CFade *GetFade(void);									// フェード情報
	static CInputJoypad *GetJoypad(void);							// ジョイパッドの取得処理
	static void SetMode(MODE mode);									// モードの設定
	static MODE GetMode(void);										// モードの情報

private:
	static CRenderer *m_pRenderer;			// レンダラーのポインタ
	static CInput *m_pInput;				// 入力処理のポインタ
	static CKeyboard *m_pKeyboard;			// キーボード処理のポインタ
	static CSound *m_pSound;				// サウンドのポインタ
	static MODE m_Mode;						// モードの情報
	static CTitle *m_pTitle;				// タイトルのポインタ
	static CGame *m_pGame;					// ゲームのポインタ
	static CResult *m_pResult;				// リザルト画面のポインタ
	static CFade *m_pFade;					// フェードのポインタ
	static CInputJoypad*m_pJoypad;			// ジョイパッドポインタ

};

#endif