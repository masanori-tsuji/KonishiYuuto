//=============================================================================
//
// バッググラウンド処理 [bg.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "bg.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CBackground::m_apTexture[MAX_BG_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CBackground::CBackground()
{
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
	{
		m_apScene2D[nCount] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CBackground::~CBackground()
{
}

//=============================================================================
// インスタンス生成
//=============================================================================
CBackground * CBackground::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// メモリ確保
	CBackground *pBackground = new CBackground;

	// 初期化処理
	pBackground->Init(pos, size, type);

	return pBackground;
}

//=============================================================================
// テクスチャのロード
//=============================================================================
HRESULT CBackground::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/bg006.png",
		&m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/bg100.png",
		&m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/bg004.png",
		&m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// テクスチャのアンロード
//=============================================================================
void CBackground::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
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
// 初期化処理
//=============================================================================
HRESULT CBackground::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
	{
		// 使われていないとき
		if (m_apScene2D[nCount] == NULL)
		{
			// 初期化処理
			m_apScene2D[nCount] = new CScene2D;									// メモリ確保
			m_apScene2D[nCount]->InitScroll(2, 0.008f + 0.004f*(float)nCount);	// スクロール情報
			m_apScene2D[nCount]->Init(pos, size, type);							// 初期化情報
			m_apScene2D[nCount]->BindTexture(m_apTexture[nCount]);				// テクスチャ情報
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBackground::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
	{
		if (m_apScene2D[nCount] != NULL)
		{
			// 終了処理
			m_apScene2D[nCount]->Uninit();
			m_apScene2D[nCount] = NULL;
		}
	}

	// リリース
	Release();
}
 
//=============================================================================
// 更新処理
//=============================================================================
void CBackground::Update(void)
{
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
	{
		if (m_apScene2D[nCount] != NULL)
		{
			// 更新処理
			m_apScene2D[nCount]->Update();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBackground::Draw(void)
{

}