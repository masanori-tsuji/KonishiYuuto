//=============================================================================
//
// ナンバーヘッダー [number.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "score.h"
#include "number.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCORE_ANIM_PATTERN	(8)			// アニメーションのパターン
#define NUMBER_NUM			(10)		// ナンバーの数

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber::m_apTexture[MAX_NUMBER_TEXTURE] = {};
LPDIRECT3DDEVICE9 CNumber::m_pDevice = NULL;

//=============================================================================
// インスタンス生成
//=============================================================================
CNumber * CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CScene::TYPE type, NUMBER_TYPE Ntype)
{
	// インスタンス生成
	CNumber *pNumber = new CNumber;

	if (pNumber != NULL)
	{
		// 初期化処理
		pNumber->Init(pos, size, type);
		pNumber->NumberInit(pos, size);
		pNumber->SetNType(Ntype);
	}
	return pNumber;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CNumber::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_apTexture[0] = pTexture;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CNumber::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/number.png",
		&m_apTexture[0]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/Number010.png",
		&m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CNumber::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_NUMBER_TEXTURE; nCount++)
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
CNumber::CNumber()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	m_pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,														// 頂点バッファの使用法　
		FVF_VERTEX_2D,															// 使用する頂点フォーマット
		D3DPOOL_MANAGED,														// リソースのバッファを保持するメモリクラスを指定
		&m_pVtxBuff,															// 頂点バッファへのポインタ
		NULL)))																	// NULLに設定
	{
		return E_FAIL;
	}

	SetType(type);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x / 2, pos.y - size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x / 2, pos.y - size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - size.x / 2, pos.y + size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x / 2, pos.y + size.y / 2, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右下頂点の色	透明度255

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
{
		// 頂点バッファをデバイスのデータストリームにバインド
		m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pDevice->SetFVF(FVF_VERTEX_2D);

		// ナンバーの種類
		switch (m_Ntype)
		{
			// スコア
		case NUMBER_TYPE_SCORE:
			// テクスチャの設定
			m_pDevice->SetTexture(0, m_apTexture[0]);
			break;

			// ランキング
		case NUMBER_TYPE_RANKING:
			// テクスチャの設定
			m_pDevice->SetTexture(0, m_apTexture[1]);
			break;

			// その他
		default:
			// テクスチャの設定
			m_pDevice->SetTexture(0, m_apTexture[0]);
			break;
		}

		// ポリゴンの描画
		m_pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,
			NUM_POLYGON);				// プリミティブの数
}

//=============================================================================
// ナンバーの加算
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)nNumber, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)nNumber + (1.0f / NUMBER_NUM), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)nNumber, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / NUMBER_NUM) * (float)nNumber + (1.0f / NUMBER_NUM), 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ナンバー情報の初期化
//=============================================================================
void CNumber::NumberInit(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_Pos = pos;
	m_Size = size;
}

//=============================================================================
// ナンバーの種類情報
//=============================================================================
void CNumber::SetNType(NUMBER_TYPE Ntype)
{
	m_Ntype = Ntype;
}

//=============================================================================
// バッファ情報
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CNumber::GetVtxBuff(void)
{
	return m_pVtxBuff;
}