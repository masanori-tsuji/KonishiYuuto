//=============================================================================
//
// アラートロゴクラス [warning.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//====================================================================
// インクルード
//====================================================================
#include "warning.h"
#include "renderer.h"
#include "manager.h"
#include "alert.h"

//====================================================================
// マクロ定義
//====================================================================
#define SCALE_DOWN_NUM		(0.1f)		// 縮小の値
#define WARNING_COLOR_NUM	(5)			// 色の変化
#define WARNING_COLOR_MAX	(150)		// カラー変更の最大
#define WARNING_COLOR_MIN	(0)			// カラー変更の最小
#define WARNING_CHANGE_TIME	(35)		// カラーを変えるフレーム

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CWarning::m_apTexture[MAX_WARNING_TEXTURE] = {};

//====================================================================
// ポリゴン生成
//====================================================================
CWarning * CWarning::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// インスタンス生成
	CWarning *pWarning = new CWarning;

	if (pWarning != NULL)
	{
		// 初期化処理
		pWarning->Init(pos, size, type);

		// テクスチャの設定
		pWarning->BindTexture(m_apTexture[0]);

		// サイズを代入
		pWarning->m_size = size;
	}
	return pWarning;
}

//====================================================================
// コンストラクタ
//====================================================================
CWarning::CWarning()
{
	m_fScale = 2.0f;
	m_fScaleNum = 0.0f;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nFlashFlame = 0;
	m_nSubNumber = 0;
	m_nSubNum = WARNING_COLOR_NUM;
}

//====================================================================
// デストラクタ
//====================================================================
CWarning::~CWarning()
{

}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CWarning::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	// 縮小量を設定
	m_fScaleNum = SCALE_DOWN_NUM;

	return S_OK;
}

//====================================================================
// 終了処理
//====================================================================
void CWarning::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//====================================================================
// 更新処理
//====================================================================
void CWarning::Update(void)
{
	// 更新処理
	CScene2D::Update();

	// 縮小
	ScaleDown();

	// 点滅させる
	FlashPolygon();
}

//====================================================================
// 描画処理
//====================================================================
void CWarning::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//====================================================================
// テクスチャロード
//====================================================================
HRESULT CWarning::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/WARNING.png",
		&m_apTexture[0]);

	return S_OK;
}

//====================================================================
// テクスチャアンロード
//====================================================================
void CWarning::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_WARNING_TEXTURE; nCount++)
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
// 縮小
//====================================================================
void CWarning::ScaleDown(void)
{
	if (m_fScale >= 1.0f)
	{
		// 縮小量を加算
		m_fScale -= m_fScaleNum;

		// サイズの設定
		D3DXVECTOR3 size = D3DXVECTOR3(m_size.x * m_fScale, m_size.y * m_fScale, 0.0f);
		SetSize2D(size);
	}
	else
	{
		m_fScale = 1.0f;
	}
}

//====================================================================
// ポリゴンの点灯
//====================================================================
void CWarning::FlashPolygon(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーの減算
	m_nSubNumber += m_nSubNum;

	// カラー変更の最大値
	if (m_nSubNumber >= WARNING_COLOR_MAX)
	{
		// 制限
		m_nSubNumber = WARNING_COLOR_MAX;

		// 値を反転
		m_nSubNum *= -1;
	}
	else if (m_nSubNumber <= WARNING_COLOR_MIN)
	{
		// カラーの最小値の制限
		m_nSubNumber = WARNING_COLOR_MIN;

		// フレームカウント
		m_nFlashFlame++;

		// 一定のフレーム以上になったら
		if (m_nFlashFlame >= WARNING_CHANGE_TIME)
		{
			m_nFlashFlame = 0;

			// 値を反転
			m_nSubNum *= -1;
		}
	}

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255 - m_nSubNumber, 255 - m_nSubNumber, 255);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 255 - m_nSubNumber, 255 - m_nSubNumber, 255);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 255 - m_nSubNumber, 255 - m_nSubNumber, 255);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 255 - m_nSubNumber, 255 - m_nSubNumber, 255);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}