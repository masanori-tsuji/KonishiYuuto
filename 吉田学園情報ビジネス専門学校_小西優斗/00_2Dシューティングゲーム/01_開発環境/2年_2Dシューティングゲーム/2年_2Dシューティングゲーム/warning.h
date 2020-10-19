#ifndef _WARNING_H_
#define _WARNING_H_

//=============================================================================
//
// アラートロゴヘッダー	[warning.h]
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
#define WARNING_SIZE_X			(460.0f)	// サイズ
#define WARNING_SIZE_Y			(110.0f)	// サイズ
#define MAX_WARNING_TEXTURE		(1)			// テクスチャの数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// タイトルBgクラス
//=============================================================================
class CWarning : public CScene2D
{
public:
	CWarning();				// コンストラクタ
	~CWarning();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);					// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理

	static CWarning * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);		// ポリゴン生成
	static HRESULT Load(void);													// テクスチャロード
	static void UnLoad(void);													// テクスチャアンロード
	void ScaleDown(void);														// 縮小の処理
	void FlashPolygon(void);													// ポリゴンの点滅
private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_move;												// 移動量
	D3DXVECTOR3 m_size;												// サイズ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_WARNING_TEXTURE];		// テクスチャ情報のポインタ
	float m_fScale;													// 縮小用変数
	float m_fScaleNum;												// 縮小量
	int m_nFlashFlame;												// 点滅のフレーム
	int m_nSubNumber;												// カラー減算
	int m_nSubNum;													// カラーの減算量
};

#endif