//=============================================================================
//
// ボムクラス [bomb.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "bomb.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CBomb::m_apTexture[MAX_BOMB_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CBomb * CBomb::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 size, TYPE type, BOMB_TYPE Btype, BOMB_COLOR BColor,
	int nAngle, int nBaseAngle, int nDistance, int nLife)
{
	// メモリ確保
	CBomb *pBomb = new CBomb;

	if (pBomb != NULL)
	{
		// ボムの種類
		switch (Btype)
		{
		case BOMB_TYPE_SHOCK:
		{// 衝撃派の時

			// 距離の設定
			int nDis = rand() % nDistance + rand() % nDistance;

			// ランダムで出現を決める
			D3DXVECTOR3 TargetPos = D3DXVECTOR3(pos.x + nDistance * cosf(D3DXToRadian(nAngle)),
				pos.y + nDistance * sinf(D3DXToRadian(nAngle)), 0.0f);

			// 初期化処理
			pBomb->Init(TargetPos, size, type);

			// 古い座標設定
			pBomb->m_TargetOld = pos;

			D3DXVECTOR3 move = D3DXVECTOR3(cosf(D3DXToRadian(nAngle)) * BOMB_SPPED,
				sinf(D3DXToRadian(nAngle)) * +BOMB_SPPED, 0.0f);

			// 移動量
			pBomb->m_move = move;

			// カラーの設定
			pBomb->m_BColor = BColor;

			// カラーの設定
			pBomb->InitColor();

			// テクスチャ設定
			pBomb->BindTexture(m_apTexture[0]);

			// ライフ
			pBomb->m_nLife = nLife;
		}
			break;

		case BOMB_TYPE_SPARK:
		{// 火花

			// 距離の設定
			int nDis = rand() % nDistance + rand() % nDistance;

			// ランダムで出現を決める
			D3DXVECTOR3 TargetPos = D3DXVECTOR3(pos.x + nDis * cosf(D3DXToRadian((rand() % nAngle - rand() % nAngle))),
				pos.y - nDis * sinf(D3DXToRadian((rand() % nAngle - rand() % nAngle))), 0.0f);

			// 初期化処理
			pBomb->Init(TargetPos, size, type);

			// 古い座標設定
			pBomb->m_TargetOld = pos;

			D3DXVECTOR3 move = D3DXVECTOR3(cosf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))) * BOMB_SPPED,
				sinf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))) * -BOMB_SPPED, 0.0f);

			// 移動量
			pBomb->m_move = move;

			// カラー初期化
			pBomb->InitColor();

			// テクスチャ設定
			pBomb->BindTexture(m_apTexture[0]);

			// ライフの処理
			pBomb->m_nLife = nLife - rand() % 20;
		}

			break;
		}
	}
	return pBomb;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CBomb::CBomb()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TargetOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBomb::~CBomb()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBomb::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, TYPE_EFFECT);
	m_Pos = pos;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBomb::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBomb::Update(void)
{
	// ライフ減算
	m_nLife--;

	// 2Dポリゴン更新処理
	CScene2D::Update();

	// 移動量更新
	m_Pos += m_move;

	// 座標を渡す
	SetPosition(m_Pos);

	// ライフが0になったら
	if (m_nLife <= 0)
	{
		// 終了処理
		Uninit();
		return;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBomb::Draw(void)
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
// カラーの設定
//=============================================================================
void CBomb::InitColor(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ボムの色
	switch (m_BColor)
	{
	case BOMB_COLOR_WHITE:
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(0, 100, 255, 255);	// 右下頂点の色	透明度255
		break;
	case BOMB_COLOR_BLACK:
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 右下頂点の色	透明度255
		break;
	default:
		break;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CBomb::Load(void)
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
void CBomb::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BOMB_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}