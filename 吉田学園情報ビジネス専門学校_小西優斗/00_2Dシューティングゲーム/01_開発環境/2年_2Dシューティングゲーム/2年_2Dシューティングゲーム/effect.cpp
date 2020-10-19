//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EFFECT_LIFE			(4)			// エフェクトの体力

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[MAX_EFFECT_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, 
	D3DXVECTOR3 size, TYPE type, EFFECT_COLOR Ctype)
{
	// インスタンス生成
	CEffect *pEffect = new CEffect;

	if (pEffect != NULL)
	{
		// 初期化処理
		pEffect->Init(pos, size, type);			// 初期化情報
		pEffect->SetMove(move);					// 移動量
		pEffect->SetColor(Ctype);				// 色の種類
		pEffect->InitColor();					// 色の初期化
		pEffect->BindTexture(m_apTexture[0]);	// テクスチャ設定
	}
	
	return pEffect;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CEffect::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/effect000.jpg",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CEffect::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_EFFECT_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Ctype = EFFECT_COLOR_NONE;
	m_Etype = EFFECT_TYPE_NONE;
	m_bScaleDown = false;
	m_pVtxBuff = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);
	m_Pos = pos;

	return S_OK;
}

//=============================================================================
// カラーの設定
//=============================================================================
void CEffect::InitColor(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の種類
	switch (m_Ctype)
	{
		// 白い弾の時
	case EFFECT_COLOR_WHITE:

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 右下頂点の色	透明度255
		break;

	case EFFECT_COLOR_BLACK:

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 右下頂点の色	透明度255
		break;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	// 2Dポリゴン更新処理
	CScene2D::Update();

	// 小さくしていく
   	m_bScaleDown = ScaleDown(m_Pos, EFFECT_LIFE);

	// 小さくなりきったら
	if (m_bScaleDown == true)
	{
		// 終了処理
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 加算合成を行う
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// aデスティネーションカラー

	// 2Dポリゴン描画処理
	CScene2D::Draw();

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// aデスティネーションカラー
}

//=============================================================================
// カラー設定
//=============================================================================
void CEffect::SetColor(EFFECT_COLOR Ctype)
{
	m_Ctype = Ctype;
}

//=============================================
// 弾の設定
//=============================================
void CEffect::SetMove(D3DXVECTOR3 move)
{
	// 変数に代入
	m_move = move;
}