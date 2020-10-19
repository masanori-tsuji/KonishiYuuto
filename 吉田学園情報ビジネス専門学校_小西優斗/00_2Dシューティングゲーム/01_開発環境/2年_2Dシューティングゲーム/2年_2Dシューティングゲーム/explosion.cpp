//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "explosion.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_apTexture[MAX_EXPLOSION_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CExplosion::CExplosion()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Ctype = COLOR_TYPE_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	// 座標代入
	m_Pos = pos;

	// 爆発の種類
	switch (m_Ctype)
	{
	case COLOR_TYPE_WHITE:
	case COLOR_TYPE_BLACK:
		// アニメーション設定
		InitAnimation(EXPLOSION_ANIM_SPEED, EXPLOSION_ANIM_PATTERN, 0);
		break;

	case COLOR_TYPE_BOSS:
		// アニメーション設定
		InitAnimation(BOSS_EXPLOSION_ANIM_SPEED, BOSS_EXPLOSION_ANIM_PATTERN, 0);
		break;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CExplosion::Update(void)
{
	// 現在の位置を取得
	m_Pos = GetPosition();

	m_Pos += m_move;

	// 座標を渡す
	SetPosition(m_Pos);

	// 2Dポリゴン更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	// 2Dポリゴン描画処理
	CScene2D::Draw();
}

//=============================================================================
// インスタンス生成
//=============================================================================
CExplosion * CExplosion::Create(D3DXVECTOR3 pos,  D3DXVECTOR3 move, D3DXVECTOR3 size, TYPE type, COLOR_TYPE Ctype)
{
	// メモリ確保
	CExplosion *pExplosion = new CExplosion;

	// 初期化処理
	pExplosion->m_Ctype = Ctype;				// タイプ設定
	pExplosion->Init(pos, size, type);			// 初期化処理
	pExplosion->SetExplosion(move, Ctype);		// 移動量設定

	// 色の処理
	switch (Ctype)
	{
		// 白いとき
	case COLOR_TYPE_WHITE:
		// テクスチャ設定
		pExplosion->BindTexture(m_apTexture[0]);
		break;

		// 黒いとき
	case COLOR_TYPE_BLACK:
		// テクスチャ設定
		pExplosion->BindTexture(m_apTexture[1]);
		break;

		// ボスの時
	case COLOR_TYPE_BOSS:
		// テクスチャ設定
		pExplosion->BindTexture(m_apTexture[2]);
		break;
	}

	return pExplosion;
}

//=============================================================================
// 弾の設定
//=============================================================================
void CExplosion::SetExplosion(D3DXVECTOR3 move, COLOR_TYPE Ctype)
{
	m_move = move;
	m_Ctype = Ctype;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CExplosion::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/Explosion002.png",
		&m_apTexture[0]);		

	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/Explosion001.png",
		&m_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/Explosion100.png",
		&m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CExplosion::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_EXPLOSION_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}