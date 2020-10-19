//=========================================================
//
// 体力バークラスヘッダー	[hpber.cpp]
// Author : Konishi Yuuto
//
//=========================================================

//=========================================================
// インクルード
//=========================================================
#include "hpbar.h"
#include "manager.h"
#include "renderer.h"
#include "boss.h"
#include "game.h"

//=========================================================
// static初期化
//=========================================================
LPDIRECT3DTEXTURE9 CHpbar::m_apTexture[MAX_HPBAR_TEXTURE] = {};

//=========================================================
// ポリゴン生成
//=========================================================
CHpbar * CHpbar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, int nLife)
{
	// メモリ確保
	CHpbar *pHpbar = new CHpbar;

	pHpbar->m_nMaxLife = nLife;				// 体力の設定
	pHpbar->m_nLife = nLife;				// ライフの設定
	pHpbar->SetType(type);					// 種類の設定
	pHpbar->BindTexture(m_apTexture[0]);	// テクスチャの設定
	pHpbar->Init(pos, size, type);			// 初期化

	return pHpbar;
}

//=========================================================
// コンストラクタ
//=========================================================
CHpbar::CHpbar()
{
	m_pVtxBuff = NULL;
	m_nLife = 0;
	m_nMaxLife = 0;
}

//=========================================================
// デストラクタ
//=========================================================
CHpbar::~CHpbar()
{
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CHpbar::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	m_Pos = pos;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

	//HPが減った場合、画像の右側から描画をやめる
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - BAR_SIZE_X / 2, m_Pos.y - BAR_SIZE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + BAR_SIZE_X / 2, m_Pos.y - BAR_SIZE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - BAR_SIZE_X / 2, m_Pos.y + BAR_SIZE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + BAR_SIZE_X / 2, m_Pos.y + BAR_SIZE_Y / 2, 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(((float)m_nLife) * (1.0f / m_nMaxLife), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((float)m_nLife) * (1.0f / m_nMaxLife), 1.0f);

	//頂点データをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CHpbar::Uninit(void)
{

}

//=========================================================
// 更新処理
//=========================================================
void CHpbar::Update(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//HPが減った場合、画像の右側から描画をやめる

	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - BAR_SIZE_X / 2, m_Pos.y - BAR_SIZE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((m_Pos.x - BAR_SIZE_X / 2) + (BAR_SIZE_X * (((float)m_nLife / m_nMaxLife))), m_Pos.y - BAR_SIZE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - BAR_SIZE_X / 2, m_Pos.y + BAR_SIZE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((m_Pos.x - BAR_SIZE_X / 2) + (BAR_SIZE_X  * (((float)m_nLife / m_nMaxLife))) , m_Pos.y + BAR_SIZE_Y / 2, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(((float)m_nLife / m_nMaxLife), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((float)m_nLife / m_nMaxLife), 1.0f);

	//HPに応じてカラーを変える（満タンから半分まで緑）
	if (m_nLife <= m_nMaxLife&&m_nLife > m_nMaxLife / 2)
	{
		pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	}

	//HPに応じてカラーを変える（半分から4分の1まで黄色）
	if (m_nLife <= m_nMaxLife / 2 && m_nLife > m_nMaxLife / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
	}

	//HPに応じてカラーを変える（4分の1からは赤色）
	if (m_nLife <= m_nMaxLife / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

//=========================================================
// 描画処理
//=========================================================
void CHpbar::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, m_apTexture[0]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,
		NUM_POLYGON);				// プリミティブの数}
}

//=========================================================
// テクスチャロード
//=========================================================
HRESULT CHpbar::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/3Dgame_HPbar001.png",
		&m_apTexture[0]);

	return S_OK;
}

//=========================================================
// テクスチャアンロード
//=========================================================
void CHpbar::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_HPBAR_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=========================================================
// ダメージ判定
//=========================================================
void CHpbar::AddDamage(int nNumber)
{
	m_nLife -= nNumber;
}

//=========================================================
// ライフの回復
//=========================================================
void CHpbar::SetHp(int nNumber)
{
	// ライフを減らしていく
	m_nMaxLife = nNumber;

	// ライフの回復
	if (m_nLife <= m_nMaxLife)
	{
		m_nLife += 3;
		if (m_nLife >= m_nMaxLife)
		{
			m_nLife = m_nMaxLife;
		}
	}
}