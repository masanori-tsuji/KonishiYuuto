//=============================================================================
//
// 取り巻きクラス	[surroundings.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "surroundings.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "ui.h"
#include "fade.h"
#include "hpbar.h"
#include "fire.h"
#include "spark.h"
#include "game.h"
#include "boss.h"
#include "player.h"
#include "explosion.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SURROUNDINGS_LIFE				(500)		// 取り巻きの体力
#define SURROUNDINGS_BULLET_INTERVAL	(30)		// バレット間隔

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CSurroundings::m_apTexture[MAX_SURROUNDINGS_TEXTURE] = {};

//=============================================================================
// ポリゴン生成
//=============================================================================
CSurroundings * CSurroundings::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// メモリ確保
	CSurroundings *pSurroundings = new CSurroundings;

	if (pSurroundings != NULL)
	{
		// 初期化処理
		pSurroundings->Init(pos, size, type);

		// テクスチャ設定
		pSurroundings->BindTexture(m_apTexture[0]);

		// ボスステータス設定
		pSurroundings->SetPos(pos);					// 座標
		pSurroundings->SetSize(size);				// サイズ設定
		pSurroundings->m_nLife = SURROUNDINGS_LIFE;	// ライフ設定
	}

	return pSurroundings;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CSurroundings::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/enemy_q.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CSurroundings::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_SURROUNDINGS_TEXTURE; nCount++)
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
CSurroundings::CSurroundings()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_nBulletFlame = 0;
	m_nBulletCnt = 0;
	m_bColorFlag = true;
	m_State = ENEMY_STATE_NORMAL;
	m_nStateCnt = 0;
	m_nFlashFlame = 0;
	m_nSubNumber = 0;

}

//=============================================================================
// デストラクタ
//=============================================================================
CSurroundings::~CSurroundings()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSurroundings::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);
	m_Pos = pos;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSurroundings::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSurroundings::Update(void)
{
	// 更新処理
	CScene2D::Update();
	// 座標更新
	m_Pos = GetPosition();
	// 移動の更新
	m_Pos += m_Move;

	switch (m_State)
	{
	case ENEMY_STATE_NORMAL:
		break;
	case ENEMY_STATE_DAMAGE:
		m_nStateCnt++;
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
		// 頂点情報を設定
		VERTEX_2D *pVtx;
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(120, 120, 120, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(120, 120, 120, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(120, 120, 120, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(120, 120, 120, 255);	// 右下頂点の色	透明度255

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		if (m_nStateCnt >= 5)
		{
			// 状態を戻す
			m_State = ENEMY_STATE_NORMAL;

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左上頂点の色	透明度255
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右上頂点の色	透明度255
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左下頂点の色	透明度255
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右下頂点の色	透明度255

			// カウンターを戻す
			m_nStateCnt = 0;
		}
		break;
	}

	// 座標を渡す
	SetPosition(m_Pos);
	SetPos(m_Pos);

	// 弾を打つ処理
	BulletShot();

	// 当たり判定
	Collision();

	// ボスの情報取得
	CBoss *pBoss = CGame::GetBoss();
	
	if (pBoss->GetExplosion() == true)
	{
		m_nLife = 0;

		// 爆発生成
		CExplosion::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			D3DXVECTOR3(SURROUNDINGS_EXPLOSION_SIZE_X, SURROUNDINGS_EXPLOSION_SIZE_Y, 0.0f),
			TYPE_EXPLOSION, CExplosion::COLOR_TYPE_BOSS);
	}

	// 体力が0になったら
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
void CSurroundings::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//=============================================================================
// 弾を打つ処理
//=============================================================================
void CSurroundings::BulletShot(void)
{
	m_nBulletFlame++;

	if (m_nBulletFlame >= SURROUNDINGS_BULLET_INTERVAL)
	{
		// ボスの情報受け取り
		CBoss *pBoss = CGame::GetBoss();
		bool bColor = pBoss->GetColor();

		// 白色だったら
		if (bColor == true)
		{
			// プレイヤーの情報取得
			CPlayer *pPlayer = CGame::GetPlayer();
			D3DXVECTOR3 Target = pPlayer->GetPosition();

			D3DXVECTOR3 RandomPos = D3DXVECTOR3(Target.x - rand() % 50 + rand() % 50 - rand() % 50 + rand() % 50, Target.y, 0.0f);

			// 自機取得弾を発射
			CBullet::CurrentAttack(D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f),
				RandomPos, D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_BLACK,
				1000);
		}
		else
		{
			// プレイヤーの情報取得
			CPlayer *pPlayer = CGame::GetPlayer();
			D3DXVECTOR3 Target = pPlayer->GetPosition();

			D3DXVECTOR3 RandomPos = D3DXVECTOR3(Target.x - rand() % 50 + rand() % 50 - rand() % 50 + rand() % 50, Target.y, 0.0f);

			// 自機取得弾を発射
			CBullet::CurrentAttack(D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f),
				RandomPos, D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_WHITE,
				1000);
		}

		// 値初期化
		m_nBulletFlame = 0;
	}
}

//=============================================================================
// ダメージ
//=============================================================================
void CSurroundings::HitSurroundings(int nNumber)
{
	m_nLife -= nNumber;
}

//=============================================================================
// ライフの情報
//=============================================================================
int CSurroundings::GetLife(void)
{
	return m_nLife;
}
