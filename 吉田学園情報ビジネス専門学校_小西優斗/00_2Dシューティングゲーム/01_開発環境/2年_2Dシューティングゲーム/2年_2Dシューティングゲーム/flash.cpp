//=============================================================================
//
// フラッシュ処理 [flash.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "flash.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "laser.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FLASH_ANGLE		(360)			// 角度
#define FLASH_SPPED		(1.5f)			// 速度
#define FLASH_DISTANCE	(14)			// 距離

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CFlash::m_apTexture[MAX_FLASH_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CFlash * CFlash::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 size, TYPE type, FLASH_COLOR Ctype, FLASH_TYPE Ftype, int nLife)
{
	// インスタンス生成
	CFlash *pFlash = new CFlash;

	if (pFlash != NULL)
	{
		// 距離の設定
		int nDistance = rand() % FLASH_DISTANCE + rand() % FLASH_DISTANCE;

		// ランダムで出現を決める
		D3DXVECTOR3 TargetPos = D3DXVECTOR3(pos.x + nDistance * cosf(D3DXToRadian(90 + (rand() % FLASH_ANGLE - rand() % FLASH_ANGLE))),
			pos.y - 20.0f - nDistance * sinf(D3DXToRadian(90 + (rand() % FLASH_ANGLE - rand() % FLASH_ANGLE))), 0.0f);


		// 移動量
		D3DXVECTOR3 move = D3DXVECTOR3(cosf(D3DXToRadian(90 + (rand() % FLASH_ANGLE - rand() % FLASH_ANGLE))) * FLASH_SPPED,
			sinf(D3DXToRadian(90 + (rand() % FLASH_ANGLE - rand() % FLASH_ANGLE))) * -FLASH_SPPED, 0.0f);

		// 初期化処理
		pFlash->Init(TargetPos, size, type);	// 初期化情報
		pFlash->m_TargetOld = pos;				// 最初の座標
		pFlash->SetMove(move);					// 移動量
		pFlash->SetFType(Ftype);				// 種類
		pFlash->SetColor(Ctype);				// 色
		pFlash->InitColor();					// 色の初期化
		pFlash->BindTexture(m_apTexture[0]);	// テクスチャ設定
		pFlash->m_nLife = nLife;				// ライフ設定
	}

	return pFlash;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CFlash::Load(void)
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
void CFlash::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_FLASH_TEXTURE; nCount++)
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
CFlash::CFlash()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TargetOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Ctype = FLASH_COLOR_NONE;
	m_Ftype = FLASH_TYPE_NONE;
	m_pVtxBuff = NULL;
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFlash::~CFlash()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFlash::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);
	m_Pos = pos;

	return S_OK;
}

//=============================================================================
// カラーの設定
//=============================================================================
void CFlash::InitColor(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 色の種類
	switch (m_Ctype)
	{
		// 白の時
	case FLASH_COLOR_WHITE:

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(30, 30, 255, 255);	// 右下頂点の色	透明度255
		break;

		// 黒の時
	case FLASH_COLOR_BLACK:
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
void CFlash::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFlash::Update(void)
{
	// ライフ減算
	m_nLife--;

	// 2Dポリゴン更新処理
	CScene2D::Update();

	// 位置の種類
	switch (m_Ftype)
	{
		// 上の時
	case FLASH_TYPE_TOP:
		GetLaserTop();
		break;
		
		// 下の時
	case FLASH_TYPE_SHOT:
		// 座標更新
		GetPlayerPos();
	}

	// 移動量更新
	m_Pos += m_move;

	// 座標を渡す
	SetPosition(m_Pos);

	// ライフが0になったら
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
void CFlash::Draw(void)
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
void CFlash::GetPlayerPos(void)
{
	// 二つの座標を見るため
	static int nNum = 0;

	CPlayer *pPlayer = CGame::GetPlayer();

	if (nNum % 2 == 0)
	{
		// ターゲットの情報確保
		D3DXVECTOR3 Target = D3DXVECTOR3(pPlayer->GetPosition().x - PLAYER_SHOT_POS_X, pPlayer->GetPosition().y - PLAYER_SHOT_POS_Y + 10, 0.0f);

		// 位置を更新
		D3DXVECTOR3 TargetMove = D3DXVECTOR3(Target.x - m_TargetOld.x, Target.y - m_TargetOld.y, 0.0f);

		// 移動量を足す
		m_Pos += TargetMove;

		// カウントを進める
		nNum++;

		// 古い座標座標を更新
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

		// カウントを進める
		nNum++;

		// 古い座標座標を更新
		m_TargetOld = Target;
	}
}

//=============================================================================
// トップの情報
//=============================================================================
void CFlash::GetLaserTop(void)
{
	static int nNum = 0;

	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer->GetLaser() != NULL)
	{
		CLaser *pLaser = pPlayer->GetLaser();
		if (nNum % 2 == 0)
		{
			// ターゲットの情報確保
			D3DXVECTOR3 Top = D3DXVECTOR3(pLaser->GetTop().x - PLAYER_SHOT_POS_X, pLaser->GetTop().y - PLAYER_SHOT_POS_Y, 0.0f);

			// 位置を更新
			D3DXVECTOR3 TopMove = D3DXVECTOR3(Top.x - m_TargetOld.x, Top.y - m_TargetOld.y, 0.0f);

			// 移動量を足す
			m_Pos += TopMove;

			// カウントを進める
			nNum++;

			// 古い座標座標を更新
			m_TargetOld = Top;
		}
		else
		{
			// ターゲットの情報確保
			D3DXVECTOR3 Target = D3DXVECTOR3(pLaser->GetTop().x + PLAYER_SHOT_POS_X, pLaser->GetTop().y - PLAYER_SHOT_POS_Y, 0.0f);

			// 位置を更新
			D3DXVECTOR3 TargetMove = D3DXVECTOR3(Target.x - m_TargetOld.x, Target.y - m_TargetOld.y, 0.0f);

			// 移動量を足す
			m_Pos += TargetMove;

			// カウントを進める
			nNum++;

			// 古い座標座標を更新
			m_TargetOld = Target;
		}
	}
}

//=============================================================================
// タイプ設定
//=============================================================================
void CFlash::SetFType(FLASH_TYPE Ftype)
{
	m_Ftype = Ftype;
}

//=============================================================================
// カラー設定
//=============================================================================
void CFlash::SetColor(FLASH_COLOR Ctype)
{
	m_Ctype = Ctype;
}

//=============================================
// 移動量設定
//=============================================
void CFlash::SetMove(D3DXVECTOR3 move)
{
	// 移動量を設定
	m_move = move;
}