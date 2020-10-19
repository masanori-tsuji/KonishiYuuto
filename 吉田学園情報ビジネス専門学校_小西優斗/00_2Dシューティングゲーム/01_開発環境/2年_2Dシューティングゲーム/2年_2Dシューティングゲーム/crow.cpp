//=============================================================================
//
// カラスのクラス [crow.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "crow.h"
#include "renderer.h"
#include "manager.h"
#include "alert.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CROW_FLAME			(1000)			// カラスの出現フレーム
#define CROW_RANDOM_POS_Y	(30)			// カラスの座標ランダム値

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CCrow::m_apTexture[MAX_CROW_TEXTURE] = {};

//=============================================================================
// ポリゴン生成
//=============================================================================
CCrow * CCrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, TYPE type)
{
	// インスタンス生成
	CCrow *pCrow = new CCrow;

	if (pCrow != NULL)
	{
		// カラスの座標設定
		D3DXVECTOR3 RandomPos;
		RandomPos.y = pos.y - rand() % CROW_RANDOM_POS_Y - rand() % CROW_RANDOM_POS_Y - rand() % CROW_RANDOM_POS_Y;

		// 初期化処理
		pCrow->Init(D3DXVECTOR3(pos.x, RandomPos.y, 0.0f), size, type);		// 初期化情報
		pCrow->BindTexture(m_apTexture[0]);									// テクスチャの設定
		pCrow->m_move = move;												// 移動量設定
		pCrow->m_size = size;												// サイズを代入
		pCrow->InitAnimation(7, 6, -1);										// アニメーション情報
	}

	return pCrow;
}

//====================================================================
// コンストラクタ
//====================================================================
CCrow::CCrow()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
}

//====================================================================
// デストラクタ
//====================================================================
CCrow::~CCrow()
{

}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CCrow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	return S_OK;
}

//====================================================================
// 終了処理
//====================================================================
void CCrow::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//====================================================================
// 更新処理
//====================================================================
void CCrow::Update(void)
{
	// 更新処理
	CScene2D::Update();

	// 座標を受け取る
	m_Pos = GetPosition();

	// 移動量代入
	m_Pos += m_move;

	// 座標を渡す
	SetPosition(m_Pos);

	// 画面外に行ったら
	if (0.0f > m_Pos.x && SCREEN_WIDTH < m_Pos.x&&
		0.0f > m_Pos.y && SCREEN_HEIGHT < m_Pos.y)
	{
		m_nCounter++;

		if (m_nCounter >= CROW_FLAME)
		{
			// 終了処理
			Uninit();
		}
	}
}

//====================================================================
// 描画処理
//====================================================================
void CCrow::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//====================================================================
// テクスチャロード
//====================================================================
HRESULT CCrow::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/crow.png",
		&m_apTexture[0]);

	return S_OK;
}

//====================================================================
// テクスチャアンロード
//====================================================================
void CCrow::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_CROW_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}