//=============================================================================
//
// スパーククラス [spark.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "spark.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CSpark::m_apTexture[MAX_SPARK_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CSpark * CSpark::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 size, TYPE type, int nAngle, int nBaseAngle, int nDistance, int nLife)
{
	// インスタンス生成
	CSpark *pSpark = new CSpark;

	if (pSpark != NULL)
	{
		// 距離の設定
		int nDis = rand() % nDistance + rand() % nDistance;

		// ランダムで出現を決める
		D3DXVECTOR3 TargetPos = D3DXVECTOR3(pos.x + nDis * cosf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))),
			pos.y - nDis * sinf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))), 0.0f);

		// 初期化処理
		pSpark->Init(TargetPos, size, type);
		pSpark->m_TargetOld = pos;

		// 移動量
		D3DXVECTOR3 move = D3DXVECTOR3(cosf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))) * SPARK_SPPED,
			sinf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))) * -SPARK_SPPED, 0.0f);
		
		// 移動量
		pSpark->SetMove(move);

		// カラー初期化
		pSpark->InitColor();

		// テクスチャ設定
		pSpark->BindTexture(m_apTexture[0]);
		pSpark->m_nLife = nLife - rand() % 20;
	}
	return pSpark;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CSpark::Load(void)
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
void CSpark::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_SPARK_TEXTURE; nCount++)
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
CSpark::CSpark()
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
CSpark::~CSpark()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpark::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);
	m_Pos = pos;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSpark::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSpark::Update(void)
{
	// ライフ減算
	m_nLife--;

	// 2Dポリゴン更新処理
	CScene2D::Update();

	// 移動量更新
	m_Pos += m_move;

	// 座標を渡す
	SetPosition(m_Pos);

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
void CSpark::Draw(void)
{
	// 描画処理
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 加算合成を行う
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// aデスティネーションカラー

	// 2Dポリゴン描画処理
	CScene2D::Draw();

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// aデスティネーションカラー
}

//=============================================================================
// カラーの設定
//=============================================================================
void CSpark::InitColor(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================
// 移動量設定
//=============================================
void CSpark::SetMove(D3DXVECTOR3 move)
{
	// 移動量を設定
	m_move = move;
}