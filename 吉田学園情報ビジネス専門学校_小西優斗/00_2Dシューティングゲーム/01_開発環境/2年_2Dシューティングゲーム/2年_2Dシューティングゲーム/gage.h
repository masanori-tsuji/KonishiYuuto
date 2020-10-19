#ifndef _GAGE_H_
#define _GAGE_H_
//=============================================================================
//
// ゲージクラスヘッダー		[gage.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene2d.h"
#include "ui.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GAGE_SIZE_X			(10)			// ゲージのサイズ
#define GAGE_SIZE_Y			(300)			// ゲージのサイズ
#define MAX_GAGE_TEXTURE	(3)				// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CUi;

//=============================================================================
// BGクラス
//=============================================================================
class CGage : public CUi
{
public:
	typedef enum
	{
		GAGE_TYPE_NONE = 0,		// 初期値
		GAGE_TYPE_BLUE,			// 青いゲージ
		GAGE_TYPE_FLAME,		// 枠
		GAGE_TYPE_MAX
	}GAGE_TYPE;

	typedef enum
	{
		GAGE_COLOR_NONE = 0,	// 初期値
		GAGE_COLOR_WHITE,		// 白
		GAGE_COLOR_BLACK,		// 赤
		GAGE_COLOR_MAX
	}GAGE_COLOR;

	CGage();			// コンストラクタ
	~CGage();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CGage * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, GAGE_TYPE Gtype);
	static HRESULT Load(void);
	static void UnLoad(void);

	void SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 size, GAGE_TYPE Gtype);	// ゲージの情報設定
	void AddGage(int nNumber);											// ゲージ量加算
	void SubtractGame(int nNumber);										// ゲージ減算
	int GetLaserNum(void);												// レーザーの量
	void GetPlayerColor(void);											// プレイヤーの色を取得

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_GAGE_TEXTURE];	// テクスチャ情報のポインタ
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_Move;											// 移動量
	D3DXVECTOR3 m_Size;											// サイズ
	GAGE_TYPE m_Gtype;											// ゲージタイプ
	GAGE_COLOR m_Ctype;											// ゲージのカラー
	int m_nLaserNum;											// レーザーの量
	int m_nMaxLaserNum;											// レーザーゲージのマックス
	int m_nFlashFlame;											// フレーム
};

#endif