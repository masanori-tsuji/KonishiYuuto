#ifndef _RENDERER_H_
#define _RENDERER_H_
//=============================================================================
//
// レンダリングヘッダー [renderer.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCREEN_WIDTH (1280)		// ウインドウの幅	1280
#define SCREEN_HEIGHT (720)		// ウインドウの高さ	720

// 頂点フォーマット(頂点座標[2D]/頂点カラー/テクスチャ座標)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//=============================================================================
// レンダラークラス
//=============================================================================
class CRenderer
{
public:
	CRenderer();		// コンストラクタ
	~CRenderer();		// デストラクタ

	HRESULT Init(HWND hWnd, bool bWindow);	// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理

	void SetFPS(int nCountFPS);				// FPSの設定
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// デバイスの情報受け取り
private:
	// メンバ変数
	void DrawFPS(void);					// デバッグ表示
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Deviceオブジェクト(描画に必要)
	LPD3DXFONT m_pFont;					// フォントへのポインタ
	int m_nCountFPS;					// FPSのポインタ
};
#endif