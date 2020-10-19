#ifndef _POLYGON_H_
#define _POLYGON_H_
//=====================================================
//
// ポリゴンクラスヘッダー [polygon.h]
// Author : Konishi Yuuto
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "main.h"
#include "manager.h"
#include "scene.h"

//=====================================================
// 前方宣言
//=====================================================
class CScene;

//=====================================================
// クラスヘッダー
//=====================================================
class CPolygon : public CScene
{
public:
	//=============================================================================
	// 構造体定義
	//=============================================================================
	typedef struct
	{
		D3DXVECTOR3 pos;	// 頂点座標
		float rhw;			// 座標
		D3DCOLOR col;		// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
	}VERTEX_2D;

	CPolygon();										// コンストラクタ
	virtual ~CPolygon();							// デストラクタ

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);			// 初期化処理
	virtual void Uninit(void);													// 終了処理
	virtual void Update(void);													// 更新処理
	virtual void Draw(void);													// 描画処理

	static CPolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);		// ポリゴン生成

	void SetPos(D3DXVECTOR3 pos);												// 座標設定
	void SetColor(D3DXCOLOR color);												// 色の設定
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);									// バッファ情報

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	D3DXVECTOR3 m_size;						// ポリゴンのサイズ
	D3DXVECTOR3 m_pos;						// ライフの位置
	TYPE m_type;							// タイプ
};

#endif