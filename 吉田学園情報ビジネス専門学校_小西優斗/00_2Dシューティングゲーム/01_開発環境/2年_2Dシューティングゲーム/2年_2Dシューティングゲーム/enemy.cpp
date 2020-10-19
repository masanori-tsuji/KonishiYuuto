//=============================================================================
//
// エネミーの処理　[enemy.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "ui.h"
#include "explosion.h"
#include "light.h"
#include "player.h"
#include "effect.h"
#include "score.h"
#include "game.h"
#include "shock.h"
#include "spark.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ENEMY_NORMAL_LIFE			(4)					// 通常のエネミーのライフ
#define ENEMY_OUTSIDE_FLAME			(120)				// 敵が画面外にいれるフレーム
#define ENEMY_SCORE					(200)				// エネミーのスコア
#define ENEMY_STATE_COUNT			(1)					// エネミーの透明化フレーム

#define BULLET_INTERVAL				(35.0f)				// 発射間隔基盤
#define BULLET_RANDOM_INTERVAL		(50)				// 発射間隔のランダム
#define CONTINUOUS_INTERVAL			(7)					// 自機取得弾の間隔
#define TARGET_POS_AMPLITUDE		(1)					// 標的の座標振れ幅

#define NORMAL_COUNTER_BULLET_NUM	(5)					// 敵の反撃弾

#define OBLIQUE_POS_Y				(30.0f)				// 加速のフラグ位置
#define OBLIQUE_MOVE_Y				(3.0f)				// 縦の速さ
#define OBLIQUE_ACCELERATION_X		(0.04f)				// X座標の加速量
#define OBLIQUE_ACCELERATION_Y		(0.003f)			// Y座標の加速量
#define OBLIQUE_MOVE_LIMIT			(7.0f)				// 加速の制限

#define CURRENT_ENEMY_LIFE			(8)					// 自機取得弾を打つ敵の体力
#define CURRENT_NUM_BULLET			(9)					// 自機取得弾の数
#define CURRENT_COUNTER_BULLET_NUM	(10)				// 自機取得弾を打つ敵のカウンター弾の数
#define CURRENT_MOVE				(1.5f)				// 自機取得敵の速さ
#define CURRENT_BULLET_AMPLITUDE_X	(5)					// 自機取得弾の横の振れ幅
#define CURRENT_BULLET_AMPLITUDE_Y	(5)					// 自機取得弾の縦の振れ幅
#define CURRENT_FLAG_POS_Y			(50.0f)				// 減速フラグ位置
#define CURRENT_MOVE_Y				(7.0f)				// 自機取得弾を打つ敵の移動量
#define CURRENT_ACCELERATION_SPEED	(0.4f)				// 自機取得弾を打つ敵の加速量
#define CURRENT_DECELERATION_SPEED	(0.25f)				// 自機取得弾を打つ敵の減速量
#define CURRENT_RETURN_FLAME		(200)				// 自機取得弾を打つ敵の戻っていくフレーム
#define CURRENT_LAPSE_FLAME	(CURRENT_RETURN_FLAME+50)	// 自機取得弾を打つ敵の敵の消滅のフレーム

#define SNAKE_LIFE						(3)				// ジグザグ移動敵の体力
#define SNAKE_MOVE_X					(0.7f)			// ジグザグ移動のX移動量
#define SNAKE_MOVE_Y					(1.5f)			// ジグザグ移動のY移動量
#define SNAKE_SWITCH_TIME				(100)			// X移動切り替えの時間

#define COUNTINUE_BULLET_INTERVAL		(5)				// 連続弾の間隔
#define THROUGH_ENEMY_LIFE				(7)				// 真っすぐ弾を打つ敵の体力
#define THROUGH_BULLET_NUM				(30)			// 真っすぐ弾を打つ敵の弾数
#define THROUGH_MOVE_X					(0.0f)			// 真っすぐ弾を打つ敵の移動量
#define THROUGH_MOVE_Y					(2.0f)			// 真っすぐ弾を打つ敵の移動量
#define THROUGH_ACCELERATION_X			(0.05f)			// 真っすぐ弾を打つ敵の加速量
#define THROUGH_ACCELERATION_Y			(0.04f)			// 真っすぐ弾を打つ敵の加速量
#define THROUGH_DECELERATION_X			(0.05f)			// 真っすぐ弾を打つ敵の減速量
#define THROUGH_DECELERATION_Y			(0.04f)			// 真っすぐ弾を打つ敵の減速量
#define THROUGH_STOP_TIME				(70)			// 真っすぐ弾を打つ敵の静止時間
#define THROUGH_BULLET_SPEED_Y			(8.0f)			// 真っすぐ弾を打つ敵の弾の早さ
#define THROUGH_COUNTER_BULLET_NUM		(5)				// 真っすぐ弾を打つ敵のカウンター弾の数

#define ROTATION_LIFE					(5)				// 回る敵の体力
#define ROTATION_COUNTER_BULLET_NUM		(5)				// 回る敵のカウンター弾の数
#define ROTATION_SCALE_UP				(0.04f)			// 回る敵の拡大

#define CIRCLE_COUNTER_BULLET_NUM		(5)				// サークル配置の敵のカウンターバレット数
#define CIRCLE_MOVE_Y					(4.0f)			// サークル配置の敵のY軸の移動量
#define CIRCLE_LIFE						(3)				// サークル配置の敵の体力
#define CIRCLE_FLAME					(60)			// サークル配置の敵の移動変更フレーム

#define DIRECTION_MOVE_X				(7.0f)			// 方向転換をする敵の移動量
#define DIRECTION_MOVE_Y				(8.0f)			// 方向転換をする敵の移動量
#define DIRECTION_LIFE					(1)				// 方向転換をする敵の体力
#define DIRECTION_COUNTER_BULLET_NUM	(5)				// 方向転換する敵のカウンター弾数

#define FOLLOW_MOVE_X					(4.0f)			// 追従する敵の移動量
#define FOLLOW_MOVE_Y					(4.0f)			// 追従する敵の移動量
#define FOLLOW_LIFE						(1)				// 追従する敵の体力
#define FOLLOW_COUNTER_BULLET_NUM		(5)				// 追従する敵のカウンター弾数
#define FOLLOW_MOVE_TIME				(50)			//  追従する敵動く時間

#define WARNING_COLOR_MAX				(255)			// 透明度変更値の最大数
#define COLOR_NUM						(4)				// 透明度

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[MAX_ENEMY_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_fBulletFlame = 0.0f;
	m_Ctype = COLOR_TYPE_NONE;
	m_Etype = ENEMY_TYPE_NONE;
	m_State = ENEMY_STATE_NORMAL;
	m_nCountinueInter = 0;
	m_nBulletNum = 0;
	m_nStateCnt = 0;
	m_nCounterBulletNum = 0;
	m_bSnakeSwitch = false;
	m_bTwo_Sides = false;
	m_nFirstBulletFrame = 0;
	m_nThroughStopCnt = 0;
	m_pLight = NULL;
	m_fRadius = 0.0f;
	m_fAddRadius = 0.7f;
	m_fTheta = 0.0f;
	m_fOmega = 0.2f;
	m_nOutsideScreenCnt = 0;
	m_fThroughBulletCnt = 0.0f;
	m_fScale = 0.0f;
	m_fScaleNum = 0.0f;
	m_bRotationScale = false;
	m_nDirection = 0;
	m_bFlashFlag = false;
	m_nSubNumber = 255;
	m_nSubNum = 0;
	m_bFollowFlag = false;
	m_nEnemyCounter = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// インスタンス生成
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, 
	TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype)
{
	// 初期化処理
	CEnemy *pEnemy = new CEnemy;
	if (pEnemy != NULL)
	{
		// 初期化処理
		pEnemy->Init(pos, size, type);			// 初期化情報
		pEnemy->SetSize(size);					// サイズ設定
		pEnemy->SetEnemyType(Ctype, Etype);		// エネミーの種類
		pEnemy->SetMove();						// 移動量設定
		pEnemy->SetLife();						// ライフ初期化
		pEnemy->SetCounter();					// カウンター弾の設定
	}

	// エネミーの種類
	switch (Etype)
	{
	case ENEMY_TYPE_NORMAL:
	case ENEMY_TYPE_CURRENT:
	case ENEMY_TYPE_SNAKE:
	case ENEMY_TYPE_ROTATION:

		// テクスチャ振り分け
		switch (Ctype)
		{
			// 白いとき
		case COLOR_TYPE_WHITE:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[0]);

			 // 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_WHITE);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[1]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_BLACK);
			break;

		}
		break;

		// 真っすぐ弾を打ってくる敵
	case ENEMY_TYPE_THROUGH:

		// 初期化処理
		pEnemy->InitThrough(pos);

		// 色の種類
		switch (Ctype)
		{
			// 白いとき
		case COLOR_TYPE_WHITE:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[2]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_WHITE);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[3]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_BLACK);
			break;
		}

		break;

	default:
		break;
	}

	return pEnemy;
}

//=============================================================================
// 自機追従弾を打つ敵
//=============================================================================
CEnemy * CEnemy::Current(
	D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 size, 
	TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, 
	int nTime)
{
	// 初期化処理
		CEnemy *pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		// 初期化処理
		pEnemy->Init(pos, size, type);			// 初期化情報
		pEnemy->SetSize(size);					// サイズの設定
		pEnemy->SetEnemyType(Ctype, Etype);		// エネミーの種類設定
		pEnemy->SetMove();						// 移動量設定
		pEnemy->SetLife();						// ライフ初期化
		pEnemy->SetCounter();					// カウンター弾設定
		pEnemy->SetFirstBulletTime(nTime);		// 最初の発射の設定
	}

	// エネミーの種類
	switch (Etype)
	{
	case ENEMY_TYPE_NORMAL:
	case ENEMY_TYPE_CURRENT:
	case ENEMY_TYPE_SNAKE:

		// 真っすぐ弾を打敵の処理
	case ENEMY_TYPE_THROUGH:

		// テクスチャ振り分け
		switch (Ctype)
		{
			// 白い敵
		case COLOR_TYPE_WHITE:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[2]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_WHITE);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[3]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_BLACK);
			break;
		}

		break;
	}

	return pEnemy;
}

//=============================================================================
// 回る敵
//=============================================================================
CEnemy * CEnemy::Rotation(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, 
	TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, 
	float fRadius, float fTheta)
{
	// 初期化処理
	CEnemy *pEnemy = new CEnemy;
	if (pEnemy != NULL)
	{
		// 初期化処理
		pEnemy->Init(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);	// 初期化情報
		pEnemy->SetSize(size);									// サイズの設定
		pEnemy->SetEnemyType(Ctype, Etype);						// 敵の種類設定
		pEnemy->SetMove();										// 移動量設定
		pEnemy->SetLife();										// 体力の設定
		pEnemy->SetCounter();									// カウンター弾の設定
		pEnemy->SetRotation(fRadius, fTheta);					// 回転情報
		pEnemy->m_fScaleNum = ROTATION_SCALE_UP;				// 大きさの値の設定

		// 色の種類
		switch (Ctype)
		{
			// 白い敵の時
		case COLOR_TYPE_WHITE:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[0]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X - 20.0f, LIGHT_SIZE_Y - 20.0f, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_WHITE);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[1]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X - 20.0f, LIGHT_SIZE_Y - 20.0f, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_BLACK);
			break;
		}
	}

	return pEnemy;
}

//=============================================================================
// サークルの配置の敵
//=============================================================================
CEnemy * CEnemy::Circle(D3DXVECTOR3 pos, D3DXVECTOR3 size, 
	TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, float fRadius, float fTheta)
{
	// メモリ確保
	CEnemy *pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		// 初期化処理
		pEnemy->Init(pos, size, type);			// 初期化情報
		pEnemy->SetSize(size);					// サイズの設定
		pEnemy->SetEnemyType(Ctype, Etype);		// 敵の種類の情報
		pEnemy->SetMove();						// 移動量の設定
		pEnemy->SetLife();						// ライフ初期化
		pEnemy->SetCounter();					// カウンター弾の設定
		pEnemy->SetRotation(fRadius, fTheta);	// 回転の設定

		if (pos.x <= SCREEN_WIDTH / 2)
		{
			pEnemy->m_center = D3DXVECTOR3(WALL_SIZE_X, CIRCLE_INITIAL_Y, 0.0f);
		}
		else
		{
			pEnemy->m_center = D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X, CIRCLE_INITIAL_Y, 0.0f);
		}

		// 色の設定
		switch (Ctype)
		{
			// 白いとき
		case COLOR_TYPE_WHITE:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[0]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X , LIGHT_SIZE_Y , 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_WHITE);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[1]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X , LIGHT_SIZE_Y , 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_BLACK);
			break;
		}
	}
	return pEnemy;
}

//=============================================================================
// 方向転換の敵
//=============================================================================
CEnemy * CEnemy::Direction(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, int nNumber)
{
	// 初期化処理
	CEnemy *pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		// エネミーの情報設定
		pEnemy->Init(pos, size, type);			// 初期化情報
		pEnemy->SetSize(size);					// サイズ設定
		pEnemy->SetEnemyType(Ctype, Etype);		// エネミーのタイプ情報
		pEnemy->SetMove();						// 移動量設定
		pEnemy->SetLife();						// ライフ初期化
		pEnemy->SetCounter();					// カウンター弾の設定
		pEnemy->m_nDirection = nNumber;			// 番号の設定

		// テクスチャ振り分け
		switch (Ctype)
		{
			// 白いとき
		case COLOR_TYPE_WHITE:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[2]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_WHITE);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:
			pEnemy->BindTexture(m_apTexture[3]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_BLACK);
			break;
		}
	}

	return pEnemy;
}

//=============================================================================
// 追従してくる敵
//=============================================================================
CEnemy * CEnemy::Follow(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype)
{
	// メモリ確保
	CEnemy *pEnemy = new CEnemy;

	if (pEnemy != NULL)
	{
		// 初期化処理
		pEnemy->Init(pos, size, type);			// 初期化処理
		pEnemy->SetSize(size);					// サイズ設定
		pEnemy->SetEnemyType(Ctype, Etype);		// エネミーの種類設定
		pEnemy->m_nSubNum = COLOR_NUM;			// カラー変更の値
		pEnemy->SetLife();						// ライフ初期化
		pEnemy->SetCounter();					// カウンター弾の設定
		pEnemy->TransparentInit();				// 透明にさせる
	}
		// 色の種類
		switch (Ctype)
		{
			// 白いとき
		case COLOR_TYPE_WHITE:

			//　テクスチャの設定
			pEnemy->BindTexture(m_apTexture[0]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_WHITE);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:

			// テクスチャの設定
			pEnemy->BindTexture(m_apTexture[1]);

			// 光の生成
			pEnemy->m_pLight = CLight::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(LIGHT_SIZE_X, LIGHT_SIZE_Y, 0.0f),
				TYPE_LIGHT, CLight::LIGHT_TYPE_BLACK);
			break;
		}

	return pEnemy;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/enemy_q (1).png",
		&m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/enemy_q (3).png",
		&m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/BlueEnemy.png",
		&m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/RedEnemy.png",
		&m_apTexture[3]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CEnemy::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_ENEMY_TEXTURE; nCount++)
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
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// バレット間隔初期化
	m_fBulletFlame = BULLET_INTERVAL + (float)(rand() % BULLET_RANDOM_INTERVAL);

	// 初期化処理
	CScene2D::Init(pos, size, type);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// 2Dポリゴン終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// バレットのフレーム
	m_fBulletFlame--;

	// 2Dポリゴン更新処理
	CScene2D::Update();

	// 座標更新
	m_Pos = GetPosition();

	// 移動の更新
	m_Pos += m_Move;
	
	if (m_pLight != NULL)
	{
		// ライトの移動
		m_pLight->SetPosition(m_Pos);
	}

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// エネミーの状態
	switch (m_State)
	{
		// 通常時
	case ENEMY_STATE_NORMAL:
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右下頂点の色	透明度255
		break;

		// ダメージ時
	case ENEMY_STATE_DAMAGE:
		
		// 状態カウンター
		m_nStateCnt++;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);	// 右下頂点の色	透明度255

		// 一定で通常状態に戻る
		if (m_nStateCnt >= ENEMY_STATE_COUNT)
		{
			m_State = ENEMY_STATE_NORMAL;
			m_nStateCnt = 0;
		}
		break;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// エネミーの種類
	switch (m_Etype)
	{
		// 通常時
	case ENEMY_TYPE_NORMAL:

		// 斜め移動処理
		ObliqueUpdate();
		break;

		// 自機取得弾を打つ敵
	case ENEMY_TYPE_CURRENT:

		// 自機取得の敵
		CurrentUpdate();

		// 弾の発射
		if (m_fBulletFlame <= 0.0f)
		{
			// 連続弾発射
			CurrentBullet(CURRENT_NUM_BULLET);
		}
		break;

		// ジグザグの敵
	case ENEMY_TYPE_SNAKE:
		// ジグザグ移動
		SnakeUpdate();
		break;

		// 真っすぐ打ってくる敵
	case ENEMY_TYPE_THROUGH:
		// 更新処理
		ThroughUpdate();
		break;

		// 回る敵
	case ENEMY_TYPE_ROTATION:
		// 更新処理
		RotationUpdate();
		break;

		// サークル配置の敵
	case ENEMY_TYPE_CIRCLE:
		CircleUpdate();
		break;

		// 方向転換する敵
	case ENEMY_TYPE_DIRECTION:
		DirectionUpdate();
		break;

		// 追従
	case ENEMY_TYPE_FOLLOW:
		FollowUpdate();
		break;
	}

	// 座標を渡す
	SetPosition(m_Pos);

	// プレイヤーとの当たり判定
	Collision();

	// 敵が画面外に出たら
	if (m_Pos.x < 0.0f ||
		m_Pos.x > SCREEN_WIDTH - WALL_SIZE_X ||
		m_Pos.y < 0.0f ||
		m_Pos.y > SCREEN_HEIGHT)
	{
		// 画面外加算
		m_nOutsideScreenCnt++;
	}

	// 画面外に100フレームいたら
	if (m_nOutsideScreenCnt >= ENEMY_OUTSIDE_FLAME)
	{
		m_nLife = 0;
	}

	// ライフが0だったら
	if (m_nLife <= 0)
	{
		// ライト削除
		m_pLight->Uninit();

		// エネミー削除
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// 2Dポリゴン描画処理
	CScene2D::Draw();
}

//=============================================================================
// エネミーヒット時
//=============================================================================
void CEnemy::HitDamage(int nCount)
{
	// 敵が画面外に出たら
	if (m_Pos.x < 0.0f ||
		m_Pos.x > SCREEN_WIDTH - WALL_SIZE_X ||
		m_Pos.y < 0.0f ||
		m_Pos.y > SCREEN_HEIGHT)
	{
	}
	else
	{
		// 体力減算
		m_nLife -= nCount;

		// ライフが0になったら
		if (m_nLife <= 0)
		{
			// サウンド設定
			CSound *pSound = NULL;
			pSound = CManager::GetSound();
			pSound->Play(CSound::SOUND_LABEL_EXPLOSION);

			// 色の種類
			switch (m_Ctype)
			{
				// 白いとき
			case COLOR_TYPE_WHITE:

				for (int nCount = 0; nCount < SPARK_NUM; nCount++)
				{
					// 火花の処理
					CSpark::Create(m_Pos,
						D3DXVECTOR3(SPARK_SIZE_X, SPARK_SIZE_Y, 0.0f),
						TYPE_EFFECT, SPARK_ANGLE, SPARK_BASE_ANGLE, SPARK_DISTANCE, SPARK_LIFE);
				}
				break;

				// 黒いとき
			case COLOR_TYPE_BLACK:

				for (int nCount = 0; nCount < SPARK_NUM; nCount++)
				{
					// 火花の処理
					CSpark::Create(m_Pos,
						D3DXVECTOR3(SPARK_SIZE_X, SPARK_SIZE_Y, 0.0f),
						TYPE_EFFECT, SPARK_ANGLE, SPARK_BASE_ANGLE, SPARK_DISTANCE, SPARK_LIFE);
				}
				break;

			default:
				break;
			}

			// スコアを加算
			CScore *pScore = CGame::GetScore();
			pScore->SetScore(ENEMY_SCORE);

			// カウンター弾の処理
			for (int nCount = 0; nCount < MAX_POLYGON; nCount++)
			{
				CScene *pScene = NULL;

				if (pScene == NULL)
				{
					// メモリ確保
					pScene = CScene::GetScene(nCount);

					// メモリのキャスト
					CScene2D *pScene2D = (CScene2D*)pScene;

					if (pScene2D != NULL)
					{
						// ターゲットの情報確保
						CScene::TYPE type = pScene2D->GetType();

						// 弾だった時
						if (type == TYPE_BULLET)
						{
							// 弾の色取得
							CBullet *pBullet = (CBullet*)pScene2D;
							CBullet::BULLET_COLOR Ctype = pBullet->GetCtype();

							// 色の種類
							switch (Ctype)
							{
								// 黒いとき
							case CBullet::BULLET_COLOR_BLACK:

								// 自分が黒だったら
								if (m_Ctype == COLOR_TYPE_BLACK)
								{
									// カウンター弾生成
									CBullet::CounterAttack(
										m_Pos, D3DXVECTOR3(COUNTER_BULLET_SIZE_X, COUNTER_BULLET_SIZE_Y, 0.0f),
										TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY,
										CBullet::BULLET_COLOR_BLACK, CBullet::BULLET_CATEGORY_COUNTER,
										1000, m_nCounterBulletNum);
								}
								break;

								// 白いとき
							case CBullet::BULLET_COLOR_WHITE:

								// 自分が城だったら
								if (m_Ctype == COLOR_TYPE_WHITE)
								{
									// カウンター弾生成
									CBullet::CounterAttack(
										m_Pos, D3DXVECTOR3(COUNTER_BULLET_SIZE_X, COUNTER_BULLET_SIZE_Y, 0.0f),
										TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY,
										CBullet::BULLET_COLOR_WHITE, CBullet::BULLET_CATEGORY_COUNTER,
										1000, m_nCounterBulletNum);
								}
								break;
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			// ダメージ状態にする
			m_State = ENEMY_STATE_DAMAGE;
		}
	}
}

//=============================================================================
// 座標設定
//=============================================================================
void CEnemy::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// エネミーサイズ設定
//=============================================================================
void CEnemy::SetSize(D3DXVECTOR3 size)
{
	m_Size = size;
}

//=============================================================================
// 初弾の発生フレーム設定
//=============================================================================
void CEnemy::SetFirstBulletTime(int nTime)
{
	m_nFirstBulletFrame = nTime;
}

//=============================================================================
// 角度の設定
//=============================================================================
void CEnemy::SetRotation(float fRadius, float fTheta)
{
	m_fTheta = fTheta;
	m_fRadius = fRadius;
	m_fRadiusNum = fRadius;
}

//=============================================================================
// エネミーのタイプ設定
//=============================================================================
void CEnemy::SetEnemyType(COLOR_TYPE Ctype, ENEMY_TYPE Etype)
{
	m_Ctype = Ctype;
	m_Etype = Etype;
}

//=============================================================================
// エネミーのライフ設定
//=============================================================================
void CEnemy::SetLife(void)
{
	switch (m_Etype)
	{
	case ENEMY_TYPE_NORMAL:
		m_nLife = ENEMY_NORMAL_LIFE;
		break;

	case ENEMY_TYPE_SNAKE:
		m_nLife = SNAKE_LIFE;
		break;

		// 自機取得弾を打つ敵
	case ENEMY_TYPE_CURRENT:
		m_nLife = CURRENT_ENEMY_LIFE;
		break;

	case ENEMY_TYPE_THROUGH:
		m_nLife = THROUGH_ENEMY_LIFE;
		break;

	case ENEMY_TYPE_ROTATION:
		m_nLife = ROTATION_LIFE;
		break;

	case ENEMY_TYPE_CIRCLE:
		m_nLife = CIRCLE_LIFE;
		break;

	case ENEMY_TYPE_DIRECTION:
		m_nLife = DIRECTION_LIFE;
		break;

	case ENEMY_TYPE_FOLLOW:
		m_nLife = FOLLOW_LIFE;
		break;
	default:
		break;
	}
}

//=============================================================================
// 斜め移動処理
//=============================================================================
void CEnemy::SetMove(void)
{
	// 移動量初期設定
	switch (m_Etype)
	{
		// 通常の敵
	case ENEMY_TYPE_NORMAL:
		m_Move = D3DXVECTOR3(0.0f, OBLIQUE_MOVE_Y, 0.0f);
		break;

		// 自機取得弾を打つ敵
	case ENEMY_TYPE_CURRENT:
		m_Move = D3DXVECTOR3(0.0f, CURRENT_MOVE_Y, 0.0f);
		break;

		// ジグザクの敵
	case ENEMY_TYPE_SNAKE:
		m_Move = D3DXVECTOR3(0.0f, SNAKE_MOVE_Y, 0.0f);
			break;

			// 弾を打つ敵
	case ENEMY_TYPE_THROUGH:
		m_Move = D3DXVECTOR3(-0.001f, THROUGH_MOVE_Y, 0.0f);
		break;

		// 回ってくる敵
	case ENEMY_TYPE_ROTATION:
		m_fOmega = 2.0f;
		break;

		// サークル配置の敵
	case ENEMY_TYPE_CIRCLE:
		m_Move = D3DXVECTOR3(0.0f, CIRCLE_MOVE_Y, 0.0f);
		m_fOmega = 0.2f;
		break;

		// 方向転換する敵
	case ENEMY_TYPE_DIRECTION:
		m_Move = D3DXVECTOR3(0.0f, DIRECTION_MOVE_Y, 0.0f);
		break;
	default:
		break;
	}
}

//=============================================================================
// カウンター弾の数の設定
//=============================================================================
void CEnemy::SetCounter(void)
{
	// 移動量初期設定
	switch (m_Etype)
	{
		// 通常の敵
	case ENEMY_TYPE_NORMAL:
	case ENEMY_TYPE_SNAKE:
		m_nCounterBulletNum = NORMAL_COUNTER_BULLET_NUM;
		break;

		// 自機取得弾を打つ敵
	case ENEMY_TYPE_CURRENT:
		m_nCounterBulletNum = CURRENT_COUNTER_BULLET_NUM;
		break;

	case ENEMY_TYPE_THROUGH:
		m_nCounterBulletNum = THROUGH_COUNTER_BULLET_NUM;
		break;

	case ENEMY_TYPE_ROTATION:
		m_nCounterBulletNum = ROTATION_COUNTER_BULLET_NUM;
		break;
	case ENEMY_TYPE_CIRCLE:
		m_nCounterBulletNum = CIRCLE_COUNTER_BULLET_NUM;
		break;

		// 方向転換する敵
	case ENEMY_TYPE_DIRECTION:
		m_nCounterBulletNum = DIRECTION_COUNTER_BULLET_NUM;
		break;

		// 方向転換する敵
	case ENEMY_TYPE_FOLLOW:
		m_nCounterBulletNum = FOLLOW_COUNTER_BULLET_NUM;
		break;
	}
}

//=============================================================================
// 斜め移動敵更新処理
//=============================================================================
void CEnemy::ObliqueUpdate(void)
{
	switch (m_Ctype)
	{
		// 白いとき
	case COLOR_TYPE_WHITE:

		// Y座標が一定以上に行ったら
		if (m_Pos.y > OBLIQUE_POS_Y)
		{
			// 加速させる
			m_Move.x += OBLIQUE_ACCELERATION_X;
			m_Move.y += OBLIQUE_ACCELERATION_Y;
		}
		break;

		// 黒いとき
	case COLOR_TYPE_BLACK:

		// Y座標が一定以上に行ったら
		if (m_Pos.y > OBLIQUE_POS_Y)
		{
			// 加速させる
			m_Move.x -= OBLIQUE_ACCELERATION_X;
			m_Move.y += OBLIQUE_ACCELERATION_Y;
		}
		break;
	}

	// 上限以上移動量を増やさない
	if (m_Move.x >= OBLIQUE_MOVE_LIMIT)
	{
		m_Move.x = OBLIQUE_MOVE_LIMIT;
	}
	if (m_Move.y >= OBLIQUE_MOVE_LIMIT)
	{
		m_Move.y = OBLIQUE_MOVE_LIMIT;
	}
}

//=============================================================================
// 自機取得弾を打つ敵の更新
//=============================================================================
void CEnemy::CurrentUpdate(void)
{
	// フレームカウント
	m_nEnemyCounter++;

	// Y座標が一定以上に行ったら
	if (m_nEnemyCounter >= 5)
	{
		// 減速
		m_Move.y -= CURRENT_DECELERATION_SPEED;
	}

	// 一定量になったら
	if (m_Move.y <= 0.0f && m_nEnemyCounter <= CURRENT_RETURN_FLAME)
	{
		m_Move.y = 0.0f;
	}

	if (m_nEnemyCounter >= CURRENT_RETURN_FLAME)
	{
		// 画面外へ戻る処理
		m_Move.y -= CURRENT_ACCELERATION_SPEED;
	}

	if (m_nEnemyCounter >= CURRENT_LAPSE_FLAME)
	{
		// 消す
		m_nLife = 0;
	}
}

//=============================================================================
// ジグザグ移動更新
//=============================================================================
void CEnemy::SnakeUpdate(void)
{
	// Y座標が一定以上に行ったら
	if (m_Pos.y > OBLIQUE_POS_Y)
	{
		m_nEnemyCounter++;

		if (m_bSnakeSwitch == false)
		{
			m_Move = D3DXVECTOR3(SNAKE_MOVE_X, SNAKE_MOVE_Y, 0.0f);
			m_bSnakeSwitch = true;
		}
		if (m_nEnemyCounter % SNAKE_SWITCH_TIME == 0)
		{
			m_Move.x *= -1;
		}
	}
}

//=============================================================================
// 直進の敵の更新
//=============================================================================
void CEnemy::ThroughUpdate(void)
{
	m_fThroughBulletCnt--;

	// 直線弾の発射
	if (m_fThroughBulletCnt <= 0)
	{
		ContinuousBullet(THROUGH_BULLET_NUM);
	}

	if (m_bTwo_Sides == false)
	{
		// 出現したら
		if (m_Pos.x >= SCREEN_WIDTH / 2)
		{
			// 横の移動を増やす
			m_Move.x -= THROUGH_ACCELERATION_X;
			m_Move.y += THROUGH_ACCELERATION_Y;
		}
		else
		{	// 半分を越えたら
			// 移動を減らす
			m_Move.x += THROUGH_DECELERATION_X;
			m_Move.y -= THROUGH_DECELERATION_Y;
		}
		if (m_Move.x >= 0.0f)
		{
			m_nThroughStopCnt++;
			m_Move.x = 0.0f;
		}

		if (m_Move.y <= 0.0f)
		{
			m_Move.y = 0.0f;
		}

		// 静止時間を過ぎたら
		if (m_nThroughStopCnt >= THROUGH_STOP_TIME)
		{
			m_bTwo_Sides = true;
			m_nThroughStopCnt = 0;
		}
	}
	else
	{
		// 出現したら
		if (m_Pos.x <= SCREEN_WIDTH / 2)
		{
			// 横の移動を増やす
			m_Move.x += THROUGH_ACCELERATION_X;
			m_Move.y += THROUGH_DECELERATION_Y;
		}
		else
		{	// 半分を越えたら
			// 移動を減らす
			m_Move.x -= THROUGH_DECELERATION_X;
			m_Move.y -= THROUGH_DECELERATION_Y;
		}

		if (m_Move.x <= 0.0f)
		{
			m_nThroughStopCnt++;
			m_Move.x = 0.0f;
		}
		if (m_Move.y <= 0.0f)
		{
			m_Move.y = 0.0f;
		}

		// 静止時間を過ぎたら
		if (m_nThroughStopCnt >= THROUGH_STOP_TIME)
		{
			m_bTwo_Sides = false;
			m_nThroughStopCnt = 0;
		}
	}

	// 指定された弾分出たら発射フレームを設定
	if (m_nContinueCnt >= THROUGH_BULLET_NUM)
	{
		m_fThroughBulletCnt = THROUGH_STOP_TIME;
		m_nContinueCnt = 0;
	}
}

//=============================================================================
// 回転する敵
//=============================================================================
void CEnemy::RotationUpdate(void)
{
	// 中心を設定
	D3DXVECTOR3 Center = D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f);
	
	// 拡大
	ScaleUp();

	// 半径加算
	m_fRadius -= m_fAddRadius;

	// 角度加算
	m_fTheta += m_fOmega;

	if (m_fRadius <= m_fRadiusNum - 190.0f)
	{
		m_fAddRadius *= -1.2f;
	}

	// 座標
	m_Pos = D3DXVECTOR3(Center.x + m_fRadius * cosf(D3DXToRadian(m_fTheta)), 
		Center.y + m_fRadius * sinf(D3DXToRadian(m_fTheta)), 0.0f);
}

//=============================================================================
// サークル配置の敵の更新
//=============================================================================
void CEnemy::CircleUpdate(void)
{
	m_nEnemyCounter++;

	// 半分より左だったら
	if (m_center.x <= SCREEN_WIDTH / 2)
	{
		// 反転するタイミング
		if (m_nEnemyCounter <= CIRCLE_FLAME)
		{
			if (m_Move.x <= 2.0f)
			{
				m_Move.x += 0.1f;
			}
			else
			{
				m_Move.x = 2.0f;
			}
		}
		else
		{
			if (m_Move.x >= -2.0f)
			{
				m_Move.x -= 0.05f;
			}
			else
			{
				m_Move.x = -2.0f;
			}
		}

		// 中心の移動
		m_center += m_Move;

		// 角度加算
		m_fTheta += m_fOmega;

		// 座標
		m_Pos = D3DXVECTOR3(m_center.x + m_fRadius * cosf(D3DXToRadian(m_fTheta)),
			m_center.y + m_fRadius * sinf(D3DXToRadian(m_fTheta)), 0.0f);
	}
	else
	{
		// 反転するタイミング
		if (m_nEnemyCounter <= CIRCLE_FLAME)
		{
			if (m_Move.x >= -2.0f)
			{
				m_Move.x -= 0.1f;
			}
			else
			{
				m_Move.x = -2.0f;
			}
		}
		else
		{
			if (m_Move.x <= 2.0f)
			{
				m_Move.x += 0.05f;
			}
			else
			{
				m_Move.x = +2.0f;
			}
		}

		// 中心の移動
		m_center += m_Move;

		// 角度加算
		m_fTheta -= m_fOmega;

		// 座標
		m_Pos = D3DXVECTOR3(m_center.x + m_fRadius * cosf(D3DXToRadian(m_fTheta)),
			m_center.y + m_fRadius * sinf(D3DXToRadian(m_fTheta)), 0.0f);
	}
}

//=============================================================================
// 方向転換する敵の更新処理
//=============================================================================
void CEnemy::DirectionUpdate(void)
{
	// プレイヤーの情報を取得
	CPlayer *pPlayer = CGame::GetPlayer();
	
	// 座標取得
	D3DXVECTOR3 Target = pPlayer->GetPosition();

	// プレイヤーと同じ高さになったら
	if (m_Pos.y >= Target.y)
	{
		// 画面半分より左にいたら
		if (m_Pos.x <= SCREEN_WIDTH /2)
		{
			// 横に移動する
			m_Move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
		}
		else
		{
			// 横に移動する
			m_Move = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		}
	}

	if (m_nDirection == 0)
	{
		if (m_Pos.x >= SCREEN_WIDTH / 2)
		{
			// 消す
			m_nLife = 0;
		}
	}
	else if (m_nDirection == 1)
	{
		if (m_Pos.x <= SCREEN_WIDTH / 2)
		{
			// 消す
			m_nLife = 0;
		}
	}
}

//=============================================================================
// 追従してくる敵
//=============================================================================
void CEnemy::FollowUpdate(void)
{
	if (m_bFlashFlag == false)
	{
		// 透明度の変更
		Appearance();
	}

	if (m_bFollowFlag == false)
	{
		m_nEnemyCounter++;

		if (m_nEnemyCounter >= 50)
		{
			m_bFollowFlag = true;

			// プレイヤー情報を取得
			CPlayer *pPlayer = CGame::GetPlayer();
			D3DXVECTOR3 Ppos = pPlayer->GetPosition();

			//自機を取得する
			float fPposx = Ppos.x, fPposy = Ppos.y;		// 自機の座標
			float fEposx = m_Pos.x, fEposy = m_Pos.y;	// 敵の座標
			float fAngle;								// 角度

			//角度を決める
			fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));

			// 透明な敵の移動
			D3DXVECTOR3 FollowMove = D3DXVECTOR3(
				(sinf(fAngle)*-FOLLOW_MOVE_X),
				cosf(fAngle)*-FOLLOW_MOVE_Y, 0.0f);

			// 移動量の設定
			m_Move = FollowMove;
		}
	}

}

//=============================================================================
// 通常弾発射
//=============================================================================
void CEnemy::NormalBullet(void)
{
	// 色の種類
	switch (m_Ctype)
	{
		// 白いとき
	case COLOR_TYPE_WHITE:
		CBullet::Create(D3DXVECTOR3(m_Pos.x + 0.0f, m_Pos.y + 40.0f, 0.0f),
			D3DXVECTOR3(0.0f, 5.0f, 0.0f),
			D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
			TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_WHITE,
			1000);
		m_fBulletFlame = BULLET_INTERVAL + (float)(rand() % BULLET_RANDOM_INTERVAL);
		break;

		// 黒いとき
	case COLOR_TYPE_BLACK:
		CBullet::Create(D3DXVECTOR3(m_Pos.x + 0.0f, m_Pos.y + 40.0f, 0.0f),
			D3DXVECTOR3(0.0f, 5.0f, 0.0f),
			D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
			TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_BLACK,
			1000);

		// 弾を打つフレーム
		m_fBulletFlame = BULLET_INTERVAL + (float)(rand() % BULLET_RANDOM_INTERVAL);
		break;
	}
}

//=============================================================================
// 自機取得発射
//=============================================================================
void CEnemy::CurrentBullet(int nBulletNum)
{
	// 発射フレーム加算
	m_nCountinueInter++;

	// 設定した初弾フレームに達したら
	if (m_nCountinueInter >= m_nFirstBulletFrame)
	{
		// カウンターの速さ
		if (m_nCountinueInter % CONTINUOUS_INTERVAL == 0)
		{
			// 色の種類7
			switch (m_Ctype)
			{
				// 白いとき
				case COLOR_TYPE_WHITE:
				{
					// 現在の位置を取得
					CPlayer *pPlayer = CGame::GetPlayer();
					D3DXVECTOR3 Target = pPlayer->GetPosition();

					// 自機取得弾を発射
					CBullet::CurrentAttack(D3DXVECTOR3(
						m_Pos.x + (float)(rand() % CURRENT_BULLET_AMPLITUDE_X - rand() % CURRENT_BULLET_AMPLITUDE_X),
						m_Pos.y + 40.0f + (float)(rand() % CURRENT_BULLET_AMPLITUDE_Y - rand() % CURRENT_BULLET_AMPLITUDE_Y), 0.0f),
						D3DXVECTOR3(Target.x + (float)(rand() % TARGET_POS_AMPLITUDE - rand() % TARGET_POS_AMPLITUDE), Target.y, 0.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
						TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_WHITE,
						1000);

					// 弾のカウント
					m_nBulletNum++;
					break;
				}

				// 黒い弾の時
				case COLOR_TYPE_BLACK:
				{
					// 自機の座標取得
					CPlayer *pPlayer = CGame::GetPlayer();
					D3DXVECTOR3 Target = pPlayer->GetPosition();

					// 自機取得弾を発射
					CBullet::CurrentAttack(D3DXVECTOR3(
						m_Pos.x + (float)(rand() % CURRENT_BULLET_AMPLITUDE_X - rand() % CURRENT_BULLET_AMPLITUDE_X),
						m_Pos.y + 40.0f + (float)(rand() % CURRENT_BULLET_AMPLITUDE_Y - rand() % CURRENT_BULLET_AMPLITUDE_Y), 0.0f),
						D3DXVECTOR3(Target.x + (float)(rand() % TARGET_POS_AMPLITUDE - rand() % TARGET_POS_AMPLITUDE), Target.y, 0.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
						TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_BLACK,
						1000);

					// 弾のカウント
					m_nBulletNum++;
					break;
				}
			}
		}

		// 一定量弾を出したら
		if (m_nBulletNum >= nBulletNum)
		{
			// 弾の発生フレームを戻す
			m_fBulletFlame = 50000.0f;
		}
	}
}

//=============================================================================
// 連続弾発射
//=============================================================================
void CEnemy::ContinuousBullet(int nBulletNum)
{
	// 発射フレーム加算
	m_nCountinueInter++;

	// 連続弾の発射間隔
	if (m_nCountinueInter % COUNTINUE_BULLET_INTERVAL == 0)
	{
		// 色の種類
		switch (m_Ctype)
		{
			// 白いとき
		case COLOR_TYPE_WHITE:
			CBullet::Create(D3DXVECTOR3(m_Pos.x + 0.0f, m_Pos.y + 40.0f, 0.0f),
				D3DXVECTOR3((float)(rand() % 2 - rand() % 2), THROUGH_BULLET_SPEED_Y, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_WHITE,
				1000);
			break;

			// 黒いとき
		case COLOR_TYPE_BLACK:
			CBullet::Create(D3DXVECTOR3(m_Pos.x + 0.0f, m_Pos.y + 40.0f, 0.0f),
				D3DXVECTOR3((float)(rand() % 2 - rand() % 2), THROUGH_BULLET_SPEED_Y, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				TYPE_BULLET, CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_BLACK,
				1000);
			break;
		}

		// 連続弾の弾の数
		m_nContinueCnt++;
	}

	// 指定された弾分出たら発射しなくする
	if (m_nContinueCnt >= nBulletNum)
	{
		m_fBulletFlame = BULLET_INTERVAL + (float)(rand() % BULLET_RANDOM_INTERVAL);
	}
}

//====================================================================
// 拡大
//====================================================================
void CEnemy::ScaleUp(void)
{
	if (m_fScale <= 1.0f)
	{
		// 縮小量を加算
		m_fScale += m_fScaleNum;

		// サイズの変更
		D3DXVECTOR3 size = D3DXVECTOR3(m_Size.x * m_fScale, m_Size.y * m_fScale, 0.0f);

		// Scene2Dにサイズを渡す
		SetSize2D(size);
	}
	else
	{
		m_fScale = 1.0f;
		m_bRotationScale = true;
	}
}

//=============================================================================
// 透明から出現
//=============================================================================
void CEnemy::Appearance(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーの減算
	m_nSubNumber -= m_nSubNum;

	// カラー変更の最大値
	if (m_nSubNumber <= 0)
	{
		// 制限
		m_nSubNumber = 0;

		m_bFlashFlag = true;
	}

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255 - m_nSubNumber);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CEnemy::Collision(void)
{
	// プレイヤーの情報を受け取る
	CPlayer *pPlayer = CGame::GetPlayer();

	// 座標を受け取る
	D3DXVECTOR3 Target = pPlayer->GetPosition();

	if (Target.x - DECISION_PLAYER_SIZE_X / 2 < m_Pos.x + m_Size.x / 2 &&
		Target.x + DECISION_PLAYER_SIZE_X / 2 > m_Pos.x - m_Size.x / 2 &&
		Target.y - DECISION_PLAYER_SIZE_Y / 2 < m_Pos.y + m_Size.y / 2 &&
		Target.y + DECISION_PLAYER_SIZE_Y / 2 > m_Pos.y - m_Size.y / 2)
	{
		// 当たり判定処理
			if (m_Etype != ENEMY_TYPE_ROTATION)
			{
				// プレイヤーにダメージを与える
				pPlayer->HitDamage(10);
			}
			else
			{
				if (m_bRotationScale == true)
				{
					// プレイヤーにダメージを与える
					pPlayer->HitDamage(10);
				}
			}

			return true;
	}

	return false;
}

//=============================================================================
// 透明の初期化
//=============================================================================
void CEnemy::TransparentInit(void)
{
	if (m_Etype == ENEMY_TYPE_FOLLOW)
	{
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

		// 頂点情報を設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);	// 右下頂点の色	透明度255

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

	}
}

//=============================================================================
// 真っすぐな敵の初期化
//=============================================================================
void CEnemy::InitThrough(D3DXVECTOR3 pos)
{
	if (pos.x <= SCREEN_WIDTH / 2)
	{
		m_bTwo_Sides = true;
	}
}

//=============================================================================
//  サイズ情報
//=============================================================================
D3DXVECTOR3 CEnemy::GetSize(void)
{
	return m_Size;
}