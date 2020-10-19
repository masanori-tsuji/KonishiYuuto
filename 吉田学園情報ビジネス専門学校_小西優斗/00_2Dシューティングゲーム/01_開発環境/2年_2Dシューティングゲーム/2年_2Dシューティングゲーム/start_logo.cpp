//=============================================================================
//
// スタートロゴクラス [start_logo.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//====================================================================
// インクルード
//====================================================================
#include "start_logo.h"
#include "renderer.h"
#include "manager.h"
#include "alert.h"

//====================================================================
// マクロ定義
//====================================================================
#define START_LOGO_COLOR_NUM	(5)			// 色の変化
#define START_LOGO_COLOR_MAX	(150)		// カラー変更の最大
#define START_LOGO_COLOR_MIN	(0)			// カラー変更の最小
#define START_LOGO_CHANGE_TIME	(35)		// カラーを変えるフレーム

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CStartLogo::m_apTexture[MAX_START_LOGO_TEXTURE] = {};

//====================================================================
// ポリゴン生成
//====================================================================
CStartLogo * CStartLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// インスタンス生成
	CStartLogo *pStartLogo = new CStartLogo;

	if (pStartLogo != NULL)
	{
		// 初期化処理
		pStartLogo->Init(pos, size, type);
		// テクスチャの設定
		pStartLogo->BindTexture(m_apTexture[0]);

		// サイズを代入
		pStartLogo->m_size = size;
	}
	return pStartLogo;
}

//====================================================================
// コンストラクタ
//====================================================================
CStartLogo::CStartLogo()
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nFlashFlame = 0;
	m_nSubNumber = 0;
	m_nSubNum = START_LOGO_COLOR_NUM;
	m_bDisappearFlag = false;
}

//====================================================================
// デストラクタ
//====================================================================
CStartLogo::~CStartLogo()
{

}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CStartLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	return S_OK;
}

//====================================================================
// 終了処理
//====================================================================
void CStartLogo::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//====================================================================
// 更新処理
//====================================================================
void CStartLogo::Update(void)
{
	// 更新処理
	CScene2D::Update();

	// 点滅させる
	FlashPolygon();
}

//====================================================================
// 描画処理
//====================================================================
void CStartLogo::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//====================================================================
// テクスチャロード
//====================================================================
HRESULT CStartLogo::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/StartLogo.png",
		&m_apTexture[0]);

	return S_OK;
}

//====================================================================
// テクスチャアンロード
//====================================================================
void CStartLogo::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_START_LOGO_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//====================================================================
// ポリゴンの点灯
//====================================================================
void CStartLogo::FlashPolygon(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーの減算
	m_nSubNumber += m_nSubNum;

	// 上限に行ったら
	if (m_nSubNumber >= 255)
	{
		m_nSubNumber = 255;
		m_bDisappearFlag = true;

	}
	else if (m_nSubNumber <= 0)
	{
		m_nSubNumber = 0;
		m_nSubNum *= -1;
	}

	if (m_bDisappearFlag == true)
	{
		m_nFlashFlame++;

		if (m_nFlashFlame >= 20)
		{
			m_nFlashFlame = 0;
			m_nSubNum *= -1;
			m_bDisappearFlag = false;
		}
	}

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}