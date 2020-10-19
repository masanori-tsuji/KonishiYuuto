//=============================================================================
//
// ポリゴンの設定 [scene2d.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene.h"
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "player.h"
#include "bg.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCALE_UP_NUM		(0.05f)				// 拡大の値
#define SCALE_DOWN_NUM		(0.03f)				// 縮小の加算量
#define TRANS_PARENCY_DOWN	(30)				// 透明度減算

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D()
{
	m_pTexture = NULL;							// テクスチャのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファのポインタ
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ポリゴンのサイズ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ポリゴンの座標
	m_nCountAnim = 0;							// アニメーションのカウンター
	m_nCountAnimPattern = 0;					// アニメーションパターンのカウンタ
	m_nCounterAnim = 0;							// アニメーションの速さ
	m_nPatternAnim = 0;							// アニメーションの数
	m_nSpeedTexture = 0;						// テクスチャの移動速度
	m_fDivisionCounter = 0.0f;					// スクロールのカウンター
	m_fDivisionBaseNum = 0.0f;					// スクロールの移動量
	m_fScaleUp = 0.0f;							// 拡大の値
	m_fScaleDown = 1.0f;						// 縮小の値
	m_nEffectCntFlame = 0;
	m_nTransParencyNum = 255;
	m_nLoop = 1;								// ループするか
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// メンバ変数に値設定
	m_pos = pos;
	m_size = size;
	SetType(type);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		FVF_VERTEX_2D,														// 使用する頂点フォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		&m_pVtxBuff,														// 頂点バッファへのポインタ
		NULL)))																// NULLに設定
	{
		return E_FAIL;
	}

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

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
void CScene2D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// シーンリリース
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{
	// アニメーションの設定がされたとき
	if (m_nPatternAnim != 0)
	{
		// アニメーションを更新する
		UpdateAnimation();
	}
	else if (m_nSpeedTexture != 0)
	{
		// アニメーションを更新する
		UpdateScroll();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
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
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,
		NUM_POLYGON);				// プリミティブの数
}

//=============================================================================
// 位置を更新する関数
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズの設定
//=============================================================================
void CScene2D::SetSize2D(D3DXVECTOR3 size)
{
	m_size = size;

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 位置を返す関数
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
// テクスチャ設定関数
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// 初期化時の値代入
//=============================================================================
void CScene2D::SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;		// ポリゴンのサイズ
	m_size = size;		// ポリゴンの座標
}

//=============================================================================
// 座標代入
//=============================================================================
void CScene2D::SetP(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================
// アニメーション情報取得
//=============================================
void CScene2D::InitAnimation(int nCounterAnim, int nPatternAnim, int nLoop)
{
	m_nCounterAnim = nCounterAnim;
	m_nPatternAnim = nPatternAnim;
	m_nLoop = nLoop;

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================
// アニメーション更新関数
//=============================================
void CScene2D::UpdateAnimation(void)
{
	// 爆発のアニメーションカウントを進めて、パターンを切り替える
	m_nCountAnim++;
	// 頂点情報(テクスチャ座標)の更新
	if (m_nCountAnim >= m_nCounterAnim)	// 爆発の速さ
	{
		// アニメーションのカウントを0にする
		m_nCountAnim = 0;

		// アニメーションのパターンをカウントさせる
		m_nCountAnimPattern++;
	}

	// アニメーションが終わったら
	if (m_nCountAnimPattern >= m_nPatternAnim)
	{
		// 数値を戻しておく
		m_nCountAnimPattern = 0;

		if (m_nLoop == 0)
		{
			// 終了処理
			Uninit();
		}
	}
	else
	{
		// 頂点情報を設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標を更新
		pVtx[0].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 1.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================
// テクスチャのスクロール情報を取得
//=============================================
void CScene2D::InitScroll(int nSpeed, float fDivision)
{
	m_nSpeedTexture = nSpeed;
	m_fDivisionBaseNum = fDivision;
}

//=============================================================================
// ポリゴン拡大
//=============================================================================
bool CScene2D::ScaleUp(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2 * m_fScaleUp), m_pos.y - (m_size.y / 2 * m_fScaleUp), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2 * m_fScaleUp), m_pos.y - (m_size.y / 2 * m_fScaleUp), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2 * m_fScaleUp), m_pos.y + (m_size.y / 2 * m_fScaleUp), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2 * m_fScaleUp), m_pos.y + (m_size.y / 2 * m_fScaleUp), 0.0f);

	if (m_fScaleUp <= 1.0f)
	{
		// 拡大
		m_fScaleUp += SCALE_UP_NUM;
	}
	else
	{	// 拡大し終わったら
	//	m_bScaleUp = true;

		return true;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return false;
}

//=============================================================================
// ポリゴン縮小
//=============================================================================
bool CScene2D::ScaleDown(D3DXVECTOR3 pos, int nLife)
{
	m_nEffectCntFlame++;
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2 * m_fScaleDown), m_pos.y - (m_size.y / 2 * m_fScaleDown), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2 * m_fScaleDown), m_pos.y - (m_size.y / 2 * m_fScaleDown), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2 * m_fScaleDown), m_pos.y + (m_size.y / 2 * m_fScaleDown), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2 * m_fScaleDown), m_pos.y + (m_size.y / 2 * m_fScaleDown), 0.0f);

	if (m_nEffectCntFlame <= nLife)
	{
		m_fScaleDown -= SCALE_DOWN_NUM;
	}
	else
	{	// 拡大し終わったら
		//	m_bScaleUp = true;
		return true;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return false;
}

//=============================================================================
// 透明度
//=============================================================================
bool CScene2D::TransParency(D3DXVECTOR3 pos, int nLife)
{
	m_nEffectCntFlame++;
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2 * m_fScaleDown), m_pos.y - (m_size.y / 2 * m_fScaleDown), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2 * m_fScaleDown), m_pos.y - (m_size.y / 2 * m_fScaleDown), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2 * m_fScaleDown), m_pos.y + (m_size.y / 2 * m_fScaleDown), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2 * m_fScaleDown), m_pos.y + (m_size.y / 2 * m_fScaleDown), 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 100, 255, m_nTransParencyNum);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(0, 100, 255, m_nTransParencyNum);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(0, 100, 255, m_nTransParencyNum);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(0, 100, 255, m_nTransParencyNum);	// 右下頂点の色	透明度255


	if (m_nEffectCntFlame <= nLife)
	{
		m_nTransParencyNum -= TRANS_PARENCY_DOWN;
	}
	else
	{	// 拡大し終わったら
		return true;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return false;
}

//=============================================
// テクスチャのスクロール情報を更新
//=============================================
void CScene2D::UpdateScroll(void)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// カウンターを進める
	m_nCounterAnim++;
	// 頂点情報(テクスチャ座標)の更新
	if (m_nCounterAnim >= m_nSpeedTexture)		// 爆発の速さ
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標を更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f - m_fDivisionCounter);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f - m_fDivisionCounter);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f - m_fDivisionCounter);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f - m_fDivisionCounter);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		// アニメーションのパターンをカウントさせる
		m_fDivisionCounter += m_fDivisionBaseNum;
		m_nCounterAnim = 0;
	}
}

//=============================================================================
// バッファ情報
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
// サイズ情報
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize2D(void)
{
	return m_size;
}
