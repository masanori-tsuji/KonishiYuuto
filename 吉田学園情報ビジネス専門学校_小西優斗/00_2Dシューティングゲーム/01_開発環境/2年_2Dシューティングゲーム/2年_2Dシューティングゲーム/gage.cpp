//=============================================================================
//
// ゲージクラス		[gage.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "gage.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_GAGE_NUM	(100)		// ゲージの最大値
#define FLASH_FLAME		(8)			// 点滅のフレーム

//=============================================================================
// static初期化処理
//=============================================================================
LPDIRECT3DTEXTURE9 CGage::m_apTexture[MAX_GAGE_TEXTURE] = {};	// テクスチャ情報のポインタ

//=============================================================================
// クリエイト
//=============================================================================
CGage * CGage::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, GAGE_TYPE Gtype)
{
	// メモリ確保
	CGage *pGage = new CGage;

	// 初期化処理
	pGage->Init(pos, size, type);			// 初期化情報
	pGage->SetInfo(pos, size, Gtype);		// 情報の設定
	pGage->m_nMaxLaserNum = MAX_GAGE_NUM;	// 最大量

	// ゲージの種類
	switch (Gtype)
	{
	case GAGE_TYPE_BLUE:
		// バーのメイン
		pGage->BindTexture(m_apTexture[0]);
		break;
	case GAGE_TYPE_FLAME:
		// バーの枠
		pGage->BindTexture(m_apTexture[1]);
		break;
	}

	return pGage;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CGage::CGage()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 座標
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_Gtype = GAGE_TYPE_NONE;					// ゲージタイプ
	m_nLaserNum = 0;							// レーザーの量
	m_nMaxLaserNum = 0;							// レーザーの最大量
	m_nFlashFlame = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGage::~CGage()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGage::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	CUi::Init(pos, size, type);

	// 本体
	if (m_Gtype == GAGE_TYPE_BLUE)
	{
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
		// 頂点情報を設定
		VERTEX_2D *pVtx;
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//HPが減った場合、画像の右側から描画をやめる
		pVtx[0].pos = D3DXVECTOR3(m_Pos.x - GAGE_SIZE_X / 2, m_Pos.y - GAGE_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_Pos.x + GAGE_SIZE_X / 2, m_Pos.y - GAGE_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_Pos.x - GAGE_SIZE_X / 2, m_Pos.y + GAGE_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_Pos.x + GAGE_SIZE_X / 2, m_Pos.y + GAGE_SIZE_Y / 2, 0.0f);

		pVtx[0].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 右下頂点の色	透明度255

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f - ((float)m_nLaserNum / (float)m_nMaxLaserNum));
		pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f - ((float)m_nLaserNum / (float)m_nMaxLaserNum));
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
	return E_NOTIMPL;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGage::Uninit(void)
{
	CUi::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGage::Update(void)
{
	// 更新処理
	CUi::Update();

	// メインのゲージ
	if (m_Gtype == GAGE_TYPE_BLUE)
	{
		// レンダラーの情報を受け取る
		CRenderer *pRenderer = NULL;
		pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

		//頂点情報へのポインタ
		VERTEX_2D *pVtx;

		//頂点バッファをロック
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = D3DXVECTOR3(m_Pos.x - GAGE_SIZE_X / 2, (m_Pos.y + GAGE_SIZE_Y / 2) - (GAGE_SIZE_Y *(((float)m_nLaserNum / m_nMaxLaserNum))), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((m_Pos.x + GAGE_SIZE_X / 2), (m_Pos.y + GAGE_SIZE_Y / 2) - (GAGE_SIZE_Y *(((float)m_nLaserNum / m_nMaxLaserNum))), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_Pos.x - GAGE_SIZE_X / 2, (m_Pos.y + GAGE_SIZE_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((m_Pos.x + GAGE_SIZE_X / 2), (m_Pos.y + GAGE_SIZE_Y / 2), 0.0f);

		// カラーの変更
		GetPlayerColor();

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f - ((float)m_nLaserNum / (float)m_nMaxLaserNum));
		pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f - ((float)m_nLaserNum / (float)m_nMaxLaserNum));
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGage::Draw(void)
{
	// 描画処理
	CUi::Draw();
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CGage::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/GageBar222.png",
		&m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/Gagebar111.png",
		&m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CGage::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_GAGE_TEXTURE; nCount++)
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
// ゲージのタイプを設定
//=============================================================================
void CGage::SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 size, GAGE_TYPE Gtype)
{
	m_Pos = pos;
	m_Size = size;
	m_Gtype = Gtype;
}

//=============================================================================
// ゲージ量の加算
//=============================================================================
void CGage::AddGage(int nNumber)
{
	if (m_nLaserNum <= MAX_GAGE_NUM)
	{
		// レーザーの量加算
		m_nLaserNum += nNumber;
	}
}

//=============================================================================
// ゲージ減算
//=============================================================================
void CGage::SubtractGame(int nNumber)
{
	// ゲージを減らす
	m_nLaserNum -= nNumber;

	if (m_nLaserNum <= 0)
	{
		m_nLaserNum = 0;
	}
}

//=============================================================================
// ゲージ量の加算
//=============================================================================
int CGage::GetLaserNum(void)
{
	return m_nLaserNum;
}

//=============================================================================
// プレイヤーの色を取得
//=============================================================================
void CGage::GetPlayerColor(void)
{
	m_nFlashFlame++;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	CPlayer *pPlayer = CGame::GetPlayer();

	// ゲージの色情報
	if (pPlayer->GetShildInfo() == true)
	{
		// 一定のフレームで点滅
		if (m_nFlashFlame % FLASH_FLAME == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(150, 150, 255, 255);	// 左上頂点の色	透明度255
			pVtx[1].col = D3DCOLOR_RGBA(150, 150, 255, 255);	// 右上頂点の色	透明度255
			pVtx[2].col = D3DCOLOR_RGBA(150, 150, 255, 255);	// 左下頂点の色	透明度255
			pVtx[3].col = D3DCOLOR_RGBA(150, 150, 255, 255);	// 右下頂点の色	透明度255
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 左上頂点の色	透明度255
			pVtx[1].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 右上頂点の色	透明度255
			pVtx[2].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 左下頂点の色	透明度255
			pVtx[3].col = D3DCOLOR_RGBA(50, 50, 255, 255);	// 右下頂点の色	透明度255
		}
	}
	else
	{
		// 一定のフレームで点滅
		if (m_nFlashFlame % FLASH_FLAME == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 150, 150, 255);	// 左上頂点の色	透明度255
			pVtx[1].col = D3DCOLOR_RGBA(255, 150, 150, 255);	// 右上頂点の色	透明度255
			pVtx[2].col = D3DCOLOR_RGBA(255, 150, 150, 255);	// 左下頂点の色	透明度255
			pVtx[3].col = D3DCOLOR_RGBA(255, 150, 150, 255);	// 右下頂点の色	透明度255
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 50, 50, 255);	// 左上頂点の色	透明度255
			pVtx[1].col = D3DCOLOR_RGBA(255, 50, 50, 255);	// 右上頂点の色	透明度255
			pVtx[2].col = D3DCOLOR_RGBA(255, 50, 50, 255);	// 左下頂点の色	透明度255
			pVtx[3].col = D3DCOLOR_RGBA(255, 50, 50, 255);	// 右下頂点の色	透明度255
		}
	}
}