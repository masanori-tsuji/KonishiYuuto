//=====================================================
//
// 警報クラス [alert.cpp]
// Author : Konishi Yuuto
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "alert.h"
#include "manager.h"
#include "renderer.h"
#include "warning.h"
#include "sound.h"

//=====================================================
// マクロ定義
//=====================================================
#define ALERT_END_FLAME			(300)			// アラートの終了フレーム
#define ALERTLOGO_START_FLAME	(50)			// ロゴを出すフレーム
#define SUB_COLOR_NUM			(15)			// カラーの増減量

//=====================================================
// ポリゴン生成
//=====================================================
CAlert * CAlert::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// メモリ確保
	CAlert *pAlert = new CAlert;

	if (pAlert != NULL)
	{
		// 初期化
		pAlert->Init(pos, size, type);		// 初期化処理
		pAlert->InitColor();				// カラーの設定
		pAlert->SetType(type);				// タイプの設定
	}

	// サウンド設定
	CSound *pSound = NULL;
	pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_WARNING);

	return pAlert;
}

//=====================================================
// コンストラクタ
//=====================================================
CAlert::CAlert()
{
	m_nSubNumber = 255;
	m_nSubNum = SUB_COLOR_NUM;
	m_nAlertFlame = 0;
	m_bAlertFlag = false;
	m_pWarning = NULL;
}

//=====================================================
// デストラクタ
//=====================================================
CAlert::~CAlert()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CAlert::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type)
{
	// 初期化処理
	CPolygon::Init(pos, move, type);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CAlert::Uninit(void)
{
	// 終了処理
	CPolygon::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CAlert::Update(void)
{
	// 更新処理
	CPolygon::Update();

	// 光の点滅
	FlashPolygon();
}

//=====================================================
// 描画処理
//=====================================================
void CAlert::Draw(void)
{
	// 描画処理
	CPolygon::Draw();
}

//=====================================================
// カラーの初期化
//=====================================================
void CAlert::InitColor(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=====================================================
// ポリゴンの点滅
//=====================================================
void CAlert::FlashPolygon(void)
{
	// フレームを加算
	m_nAlertFlame++;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーを減算
	m_nSubNumber -= m_nSubNum;

	if (m_nSubNumber <= 0)
	{
		// 0になったら切り替え
		m_nSubNumber = 0;
		m_nSubNum *= -1;
	}
	else if (m_nSubNumber >= 255)
	{
		// 255になったら切り替え
		m_nSubNumber = 255;
		m_nSubNum *= -1;
	}
	
	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255 - m_nSubNumber, 0, 0, 100);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255 - m_nSubNumber, 0, 0, 100);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255 - m_nSubNumber, 0, 0, 100);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255 - m_nSubNumber, 0, 0, 100);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// アラートロゴの生成
	CreateAlertLogo();

	// 一定のフレームで終了する
	if (m_nAlertFlame >= ALERT_END_FLAME)
	{
		m_bAlertFlag = true;

		// ロゴの終了
		m_pWarning->Uninit();
	}
}

//=====================================================
// アラートロゴの生成
//=====================================================
void CAlert::CreateAlertLogo(void)
{
	// アラートロゴの出現
	if (m_nAlertFlame >= ALERTLOGO_START_FLAME)
	{
		if (m_pWarning == NULL)
		{
			// アラートロゴ生成
			m_pWarning = CWarning::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3(WARNING_SIZE_X, WARNING_SIZE_Y, 0.0f), TYPE_WARNING);
		}
	}
}

//=====================================================
// アラートの情報
//=====================================================
bool CAlert::GetAlertFlag(void)
{
	return m_bAlertFlag;
}