#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//=============================================================================
//
// メッセージヘッダー	[message.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene2d.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MESSAGE_SIZE_X			(50.0f)		// サイズ
#define MESSAGE_SIZE_Y			(50.0f)		// サイズ
#define MAX_MESSAGE_TEXTURE		(1)			// テクスチャ数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// メッセージクラス
//=============================================================================
class CMessage : public CScene2D
{
public:
	CMessage();				// コンストラクタ
	~CMessage();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理

	static CMessage * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);	// ポリゴン生成
	static HRESULT Load(void);												// テクスチャロード
	static void UnLoad(void);												// テクスチャアンロード

	void ScaleUp(void);														// 拡大の処理
	void ScaleDown(void);													// 縮小の処理
	bool GetEndMessage(void);												// メッセージの終了フラグ情報
private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_move;												// 移動量
	D3DXVECTOR3 m_size;												// サイズ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_MESSAGE_TEXTURE];		// テクスチャ情報のポインタ
	float m_fScale;													// 縮小用変数
	float m_fScaleNum;												// 縮小量
	int m_nMessageCounter;											// メッセージの時間
	bool m_bEndMessage;												// メッセージの終了フラグ
};

#endif