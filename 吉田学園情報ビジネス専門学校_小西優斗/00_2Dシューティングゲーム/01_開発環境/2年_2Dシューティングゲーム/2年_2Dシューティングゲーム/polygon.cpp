//=====================================================
//
// ポリゴンクラス [polygon.cpp]
// Author:小西優斗
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "polygon.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//=====================================================
// static初期化処理
//=====================================================

//=====================================================
// コンストラクタ
//=====================================================
CPolygon::CPolygon()
{
	m_pVtxBuff = NULL;	// 頂点バッファのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=====================================================
// デストラクタ
//=====================================================
CPolygon::~CPolygon()
{
}

//=============================================================================
//ポリゴンクラスのクリエイト処理
//=============================================================================
CPolygon * CPolygon::Create( D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	//ポリゴンクラスのポインタ変数
	CPolygon *pPolygon = NULL;

	//メモリの確保
	pPolygon = new CPolygon;

	//メモリを確保できていたら
	if (pPolygon != NULL)
	{
		//初期化処理呼び出し
		pPolygon->Init(pos, size, type);
	}
	else
	{
		return NULL;
	}
	return pPolygon;
}


//=====================================================
// 初期化処理
//=====================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	// バッファサイズ
		D3DUSAGE_WRITEONLY,				// (固定)
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,				// (固定)
		&m_pVtxBuff,					// 変数名が変わると変更が必要
		NULL);

	//初期位置の設定
	m_pos = pos;

	//大きさの設定
	m_size = size;

	//テクスチャの設定
	//SetType(type);

	VERTEX_2D * pVtx;		// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定( Zの形に配置する)  / 四角形の頂点の位置
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

	// rhwの設定(値は1.0で固定)		/ あまり変えることはない
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPolygon::Uninit(void)
{
	// 頂点バッファの開放
 	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// リリース
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CPolygon::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CPolygon::Draw(void)
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
	pDevice->SetTexture(0, NULL);


	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,			// プリミティブの種類
		0,
		2);								// プリミティブの数
}

//=====================================================
// ポジションの設定
//=====================================================
void CPolygon::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// 色の設定
//=====================================================
void CPolygon::SetColor(D3DXCOLOR color)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データをロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//カラーの設定
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

}

//=====================================================
// バッファ情報
//=====================================================
LPDIRECT3DVERTEXBUFFER9 CPolygon::GetVtxBuff(void)
{
	return m_pVtxBuff;
}