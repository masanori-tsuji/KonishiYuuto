//=============================================================================
//
// 衝撃クラス処理 [shock.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "shock.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCALE_UP_NUM	(0.08f)
#define SUB_ALPHA_NUM	(6)

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CShock::m_apTexture[MAX_SHOCK_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CShock * CShock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, COLOR_TYPE Ctype)
{
	// インスタンス生成
	CShock *pShock = new CShock;

	// 初期化処理
	pShock->Init(pos, size, type);

	// 移動量設定
	pShock->SetShock(Ctype);

	switch (Ctype)
	{
	case COLOR_TYPE_WHITE:
		// テクスチャ
		pShock->BindTexture(m_apTexture[0]);
		break;

	case COLOR_TYPE_BLACK:
		// テクスチャ
		pShock->BindTexture(m_apTexture[1]);
		break;
	}
	return pShock;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CShock::CShock()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0);
	m_Ctype = COLOR_TYPE_NONE;
	m_fScaleNumber = 0.0f;
	m_fScaleNum = 0.0f;
	m_nSubAlpha = 0;
	m_nSubAlphaNum = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CShock::~CShock()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	m_Pos = pos;
	m_size = size;
	m_fScaleNum = SCALE_UP_NUM;
	m_nSubAlphaNum = SUB_ALPHA_NUM;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShock::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShock::Update(void)
{
	// 現在の位置を取得
	m_Pos = GetPosition();

	// 2Dポリゴン更新処理
	CScene2D::Update();

	// スケールアップ
	SizeUp();
}

//=============================================================================
// 描画処理
//=============================================================================
void CShock::Draw(void)
{
	// 2Dポリゴン描画処理
	CScene2D::Draw();
}

//=============================================================================
// 弾の設定
//=============================================================================
void CShock::SetShock(COLOR_TYPE Ctype)
{
	m_Ctype = Ctype;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CShock::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/shock003.png",
		&m_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/shock002.png",
		&m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CShock::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_SHOCK_TEXTURE; nCount++)
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
// スケールアップ
//=============================================================================
void CShock::SizeUp(void)
{
	// 拡大量加算
	m_fScaleNumber += m_fScaleNum;

	// サイズの設定
	D3DXVECTOR3 size = D3DXVECTOR3(m_size.x * m_fScaleNumber, m_size.y * m_fScaleNumber, 0.0f);

	// 透明にする処理
	UpdateInvisible();

	if (m_fScaleNumber >= 2.0)
	{
		m_fScaleNumber = 2.0f;

		// 終了処理
		Uninit();
		return;
	}
	// サイズを渡す
	SetSize2D(size);
}

//=============================================================================
// 透明度減算
//=============================================================================
void CShock::UpdateInvisible(void)
{
	// 透明度の引く値を加算
	m_nSubAlpha += m_nSubAlphaNum;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubAlpha);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubAlpha);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubAlpha);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubAlpha);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

}