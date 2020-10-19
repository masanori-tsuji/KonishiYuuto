//=============================================================================
//
// マズルフラッシュ処理 [muzzle_flash.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "muzzle_flash.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MUZZLE_FLASH_ANGLE		(30)	// 角度
#define MUZZLE_FLASH_SPPED		(1)		// 速度
#define MUZZLE_FLASH_DISTANCE	(28)	// 発生距離

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CMuzzleFlash::m_apTexture[MAX_MUZZLE_FLASH_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CMuzzleFlash * CMuzzleFlash::Create(D3DXVECTOR3 pos, 
	D3DXVECTOR3 size, TYPE type, EFFECT_COLOR Ctype, int nLife)
{
	// インスタンス生成
	CMuzzleFlash *pMuzzleFlash = new CMuzzleFlash;

	if (pMuzzleFlash != NULL)
	{
		// 距離の設定
		int nDistance = rand() % MUZZLE_FLASH_DISTANCE + rand() % MUZZLE_FLASH_DISTANCE;

		// ランダムで出現を決める
		D3DXVECTOR3 TargetPos = D3DXVECTOR3(pos.x + nDistance * cosf(D3DXToRadian(90 + (rand() % MUZZLE_FLASH_ANGLE - rand() % MUZZLE_FLASH_ANGLE)) ),
			pos.y - nDistance * sinf(D3DXToRadian(90 + (rand() % MUZZLE_FLASH_ANGLE - rand() % MUZZLE_FLASH_ANGLE))), 0.0f);

		// 初期化処理
		pMuzzleFlash->Init(TargetPos, size, type);
		pMuzzleFlash->m_TargetOld = pos;

		// 移動量
		D3DXVECTOR3 move = D3DXVECTOR3(cosf(D3DXToRadian(90 + (rand() % MUZZLE_FLASH_ANGLE - rand() % MUZZLE_FLASH_ANGLE))) * MUZZLE_FLASH_SPPED,
			sinf(D3DXToRadian(90 + (rand() % MUZZLE_FLASH_ANGLE - rand() % MUZZLE_FLASH_ANGLE))) * -MUZZLE_FLASH_SPPED, 0.0f);
		
		// 距離の設定
		pMuzzleFlash->SetMove(move);

		// カラーの種類設定
		pMuzzleFlash->SetColor(Ctype);

		// カラー初期化
		pMuzzleFlash->InitColor();

		// テクスチャ設定
		pMuzzleFlash->BindTexture(m_apTexture[0]);

		// 体力の設定
		pMuzzleFlash->m_nLife = nLife;
	}

	return pMuzzleFlash;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CMuzzleFlash::Load(void)
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
void CMuzzleFlash::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_MUZZLE_FLASH_TEXTURE; nCount++)
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
CMuzzleFlash::CMuzzleFlash()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TargetOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Ctype = MUZZLE_FLASH_COLOR_NONE;
	m_Etype = MUZZLE_FLASH_TYPE_NONE;
	m_pVtxBuff = NULL;
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMuzzleFlash::~CMuzzleFlash()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMuzzleFlash::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);
	m_Pos = pos;

	return S_OK;
}

//=============================================================================
// カラーの設定
//=============================================================================
void CMuzzleFlash::InitColor(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の設定
	switch (m_Ctype)
	{
		// 白いとき
	case MUZZLE_FLASH_COLOR_WHITE:

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 右下頂点の色	透明度255
		break;

		// 黒いとき
	case MUZZLE_FLASH_COLOR_BLACK:
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 40, 40, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 40, 40, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 40, 40, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 40, 40, 255);	// 右下頂点の色	透明度255
		break;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void CMuzzleFlash::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMuzzleFlash::Update(void)
{
	// ライフ減算
	m_nLife--;

	// 2Dポリゴン更新処理
	CScene2D::Update();

	// 座標更新
	GetPlayerPos();

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
void CMuzzleFlash::Draw(void)
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
// プレイヤーの位置を取得
//=============================================================================
void CMuzzleFlash::GetPlayerPos(void)
{
	// 左右の座標更新用
	static int nNum = 0;

	// プレイヤーの情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	if (nNum % 2 == 0)
	{
		// ターゲットの情報確保
		D3DXVECTOR3 Target = D3DXVECTOR3(pPlayer->GetPosition().x - PLAYER_SHOT_POS_X, pPlayer->GetPosition().y - PLAYER_SHOT_POS_Y + 10, 0.0f);

		// 位置を更新
		D3DXVECTOR3 TargetMove = D3DXVECTOR3(Target.x - m_TargetOld.x, Target.y - m_TargetOld.y, 0.0f);

		// 移動量を足す
		m_Pos += TargetMove;

		nNum++;

		// 古い座標更新
		m_TargetOld = Target;
	}
	else
	{
		// ターゲットの情報確保
		D3DXVECTOR3 Target = D3DXVECTOR3(pPlayer->GetPosition().x + PLAYER_SHOT_POS_X, pPlayer->GetPosition().y - PLAYER_SHOT_POS_Y + 10, 0.0f);

		// 位置を更新
		D3DXVECTOR3 TargetMove = D3DXVECTOR3(Target.x - m_TargetOld.x, Target.y - m_TargetOld.y, 0.0f);

		// 移動量を足す
		m_Pos += TargetMove;

		nNum++;

		// 古い座標更新
		m_TargetOld = Target;
	}
}


//=============================================================================
// カラー設定
//=============================================================================
void CMuzzleFlash::SetColor(EFFECT_COLOR Ctype)
{
	m_Ctype = Ctype;
}

//=============================================
// 移動量設定
//=============================================
void CMuzzleFlash::SetMove(D3DXVECTOR3 move)
{
	// 移動量を設定
	m_move = move;
}