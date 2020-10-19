//=============================================================================
//
// 敵の光処理 [light.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "light.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CLight::m_apTexture[MAX_LIGHT_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CLight * CLight::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,
	D3DXVECTOR3 size, TYPE type, LIGHT_TYPE Ltype)
{
	// インスタンス生成
	CLight *pLight = new CLight;

	if (pLight != NULL)
	{
		// 初期化処理
		pLight->Init(pos, size, type);			// 初期化情報
		pLight->SetMove(move);					// 移動量
		pLight->BindTexture(m_apTexture[0]);	// テクスチャ設定
		pLight->SetColor(Ltype);				// 色の設定
		pLight->InitColor();					// 色の初期化
	}

	return pLight;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CLight::Load(void)
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
void CLight::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_LIGHT_TEXTURE; nCount++)
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
// カラーの設定
//=============================================================================
void CLight::SetColor(LIGHT_TYPE Ctype)
{
	m_Ltype = Ctype;
}

//=============================================================================
// カラー初期化
//=============================================================================
void CLight::InitColor(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の設定
	switch (m_Ltype)
	{
		// 白の時
	case LIGHT_TYPE_WHITE:
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 右下頂点の色	透明度255
		break;

		// 黒の時
	case LIGHT_TYPE_BLACK:
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 40, 255);	// 右下頂点の色	透明度255
		break;
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bScaleUp = false;
	m_Ltype = LIGHT_TYPE_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLight::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLight::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLight::Update(void)
{
	// 2Dポリゴン更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CLight::Draw(void)
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

//=============================================
// 弾の設定
//=============================================
void CLight::SetMove(D3DXVECTOR3 move)
{
	// 変数に代入
	m_move = move;
}