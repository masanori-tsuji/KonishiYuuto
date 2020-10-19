#ifndef _HPBAR_H_
#define _HPBAR_H_
//=========================================================
//
// 体力バークラスヘッダー	[hpbar.h]
// Author : Konishi Yuuto
//
//=========================================================

//=========================================================
// インクルード
//=========================================================
#include "scene2d.h"

//=========================================================
// マクロ定義
//=========================================================
#define MAX_HPBAR_TEXTURE	(1)			// テクスチャ
#define BAR_SIZE_X			(500)		// サイズ
#define BAR_SIZE_Y			(20)		// サイズ

//=========================================================
// 前方宣言
//=========================================================
class CScene2D;

//=========================================================
// ボムクラス
//=========================================================
class CHpbar : public CScene2D
{
public:
	CHpbar();			// コンストラクタ
	~CHpbar();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理

	static HRESULT Load(void);												// テクスチャロード
	static void UnLoad(void);												// テクスチャアンロード
	static CHpbar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, 
		TYPE type, int nLife);												// ポリゴン生成

	void AddDamage(int nNumber);											// ダメージ処理
	void SetHp(int nNumber);												// 体力の回復
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_HPBAR_TEXTURE];	// テクスチャのポインタ
	D3DXVECTOR3 m_Pos;											// 座標
	int m_nMaxLife;												// 体力
	int m_nLife;												// 体力
};
#endif