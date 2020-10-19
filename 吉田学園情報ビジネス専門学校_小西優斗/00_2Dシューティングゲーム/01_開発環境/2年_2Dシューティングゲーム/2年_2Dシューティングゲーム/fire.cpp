//=============================================================================
//
// 花火最初のやつの処理 [fire.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "fire.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "bullet.h"
#include "time.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FIRE_MOVE_Y			(2.0f)			// 移動量
#define FIRE_DISTANCE		(200)			// 発生する距離

//=============================================================================
// static初期化処理
//=============================================================================
LPDIRECT3DTEXTURE9 CFire::m_apTexture[MAX_FIRE_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CFire::CFire()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_Ctype = FIRE_COLOR_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFire::~CFire()
{
}

//=============================================================================
// インスタンス生成
//=============================================================================
CFire * CFire::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
	TYPE type, FIRE_COLOR Ctype, int nLife)
{
	// メモリ確保
	CFire *pFire = new CFire;

	// 初期化処理
	pFire->Init(D3DXVECTOR3(												// 初期化処理
		pos.x - rand() % 120 + rand() % 120, pos.y, 0.0f), size, type);
	pFire->SetType(Ctype);													// 種類の設定
	pFire->SetFire(nLife, size);											// 弾の設定
	pFire->InitMove();														// 移動量設定
	pFire->m_nLife = nLife;													// 体力の代入
	pFire->BindTexture(m_apTexture[0]);										// テクスチャ設定

	return pFire;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFire::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFire::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFire::Update(void)
{
	// 2Dポリゴン更新処理
	CScene2D::Update();

	// 現在の位置を取得
	m_Pos = GetPosition();

	// 移動の更新
	m_Pos += m_move;

	// 飛距離のカウンター減算
	m_nLife--;

	// 座標を渡す
	SetPosition(m_Pos);

	// ライフが0になったら
	if (m_nLife <= 0)
	{
		// 色の種類
		switch (m_Ctype)
		{
			// 白いとき
		case FIRE_COLOR_WHITE:
			for (int nCount = 0; nCount < FIRE_WORKS_NUM; nCount++)
			{
				// 花火弾を出す
				CBullet::FireWorks(m_Pos,
					D3DXVECTOR3(FIRE_WORKS_SIZE_X, FIRE_WORKS_SIZE_Y, 0.0f),
					TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY,
					CBullet::BULLET_COLOR_WHITE, CBullet::BULLET_CATEGORY_FIREWORKS,
					FIRE_WORKS_LIFE);
			}
			break;

			// 黒いとき
		case FIRE_COLOR_BLACK:
			for (int nCount = 0; nCount < FIRE_WORKS_NUM; nCount++)
			{
				// 花火弾を出す
				CBullet::FireWorks(m_Pos,
					D3DXVECTOR3(FIRE_WORKS_SIZE_X, FIRE_WORKS_SIZE_Y, 0.0f),
					TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY,
					CBullet::BULLET_COLOR_BLACK, CBullet::BULLET_CATEGORY_FIREWORKS,
					FIRE_WORKS_LIFE);
			}
			break;
		}

		// 終了処理
		Uninit();

		return;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFire::Draw(void)
{
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
// 移動量の設定
//=============================================================================
void CFire::InitMove(void)
{
	m_move = D3DXVECTOR3(0.0f, FIRE_MOVE_Y, 0.0f);
}

//=============================================================================
// 弾の設定
//=============================================================================
void CFire::SetFire(int nLife, D3DXVECTOR3 size)
{
	m_nLife = nLife;
	m_Size = size;
}

//=============================================================================
// 弾の種類の設定
//=============================================================================
void CFire::SetType(FIRE_COLOR Ctype)
{
	m_Ctype = Ctype;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CFire::Load(void)
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
void CFire::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_FIRE_TEXTURE; nCount++)
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
// カラーの情報
//=============================================================================
CFire::FIRE_COLOR CFire::GetCtype(void)
{
	return m_Ctype;
}