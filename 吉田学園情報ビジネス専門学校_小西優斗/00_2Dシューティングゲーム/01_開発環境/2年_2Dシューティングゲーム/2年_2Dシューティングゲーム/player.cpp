//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "player.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "shield.h"
#include "ui.h"
#include "laser.h"
#include "fade.h"
#include "game.h"
#include "gage.h"
#include "remain.h"
#include "explosion.h"
#include "muzzle_flash.h"
#include "joypad.h"
#include "bomb.h"
#include "enemy.h"
#include "bullet.h"
#include "bomb_ui.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_LIFE				(1)			// プレイヤーの体力
#define PLAYER_BULLET_SIZE_X	(30)		// プレイヤーの弾のサイズ(横)
#define PLAYER_BULLET_SIZE_Y	(120)		// プレイヤーの弾のサイズ(縦)
#define PLAYER_SPEED			(5.0f)	 	// プレイヤーの速度
#define BULLET_INTERVAL			(5)			// 弾の連射間隔
#define PLAYER_STOCK			(3)			// プレイヤーの残機
#define PLAYER_ARMOR_COUNT		(240)		// 無敵時間
#define PLAYER_BOMB_NUM			(1)			// ボムの数

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BulletState = 0;
	m_nLife = 0;
	m_nStock = 0;
	m_nLaserFlame = 0;
	m_nLaserCounter = 0;
	m_bShildInfo = false;
	m_bShildScaleInfo = false;
	m_pShield = NULL;
	m_pGage = NULL;
	m_pLaser = NULL;
	m_pBombUi = NULL;
	m_nResurrectionCnt = 0;
	m_bPlayerDraw = false;
	m_bUseLaser = false;
	m_State = PLAYER_STATE_NONE;
	m_StateCount = 0;
	m_nBombFlame = 0;
	m_nBombNum = 0;
	m_nBombCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/player001.png",
		&m_pTexture);		// テクスチャへのポインタ

	return S_OK;
}

//=============================================================================
// インスタンス生成
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// メモリ確保
	CPlayer *pPlayer = new CPlayer;

	// 初期化処理
	pPlayer->Init(pos, size, type);	

	// ゲージの枠生成
	pPlayer->m_pGage = CGage::Create(D3DXVECTOR3(WALL_SIZE_X - 100.0f, SCREEN_HEIGHT - 202.5f, 0.0f), D3DXVECTOR3(GAGE_SIZE_X + 5.0f, GAGE_SIZE_Y + 5.0f, 0.0f),
		TYPE_SCORE, CGage::GAGE_TYPE_FLAME);

	// ゲージ生成
	pPlayer->m_pGage = CGage::Create(D3DXVECTOR3(WALL_SIZE_X - 100.0f, SCREEN_HEIGHT - 200.0f, 0.0f), D3DXVECTOR3(GAGE_SIZE_X, GAGE_SIZE_Y, 0.0f),
		TYPE_SCORE, CGage::GAGE_TYPE_BLUE);

	return pPlayer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 値の代入
	m_BulletState = BULLET_INTERVAL;	// 弾の間隔初期化
	m_nLife = PLAYER_LIFE;				// 体力
	m_nLaserFlame = LASER_FLAME;		// レーザーのフレーム

	// 情報を受け渡す
	CScene2D::SetPolygon(pos, size);

	// テクスチャ受け渡し
	BindTexture(m_pTexture);

	// ポリゴン初期化
	CScene2D::Init(pos, size, type);

	// 初期シールドの生成
	m_pShield = CShield::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(SHIELD_SIZE_X, SHIELD_SIZE_Y, 0.0f), 
		TYPE_SHIELD, CShield::SHIELD_TYPE_WHITE);

	if (m_pBombUi == NULL)
	{
		m_pBombUi = CBombUi::Create(
			D3DXVECTOR3(195.0f, 630.0f, 0.0f), D3DXVECTOR3(BOMB_UI_SIZE_X, BOMB_UI_SIZE_Y, 0.0f), TYPE_SCORE);
	}

	// シールドを白に
	m_bShildInfo = true;

	// ボムの所持数
	m_nBombCount = PLAYER_BOMB_NUM;

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CPlayer::UnLoad(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	m_Pos = GetPosition();

	// 更新処理
	CScene2D::Update();

	// プレイヤーの状態
	UpdateState();

	if (m_bPlayerDraw == false)
	{
		// プレイヤー制御関数
		PlayerControl();

		m_Pos += m_Move;

		if (m_pShield != NULL)
		{
			// シールドに座標を渡す
			m_bShildScaleInfo = m_pShield->ScaleUp(m_Pos);
		}
	}

	 // ボムの当たり判定
	if (m_bBombUse == true)
	{
		// ボムの当たり判定
		if (BombCollision() == true)
		{
			CScene *pScene = NULL;
			if (pScene == NULL)
			{
				// シーンを取得
				pScene = CScene::GetScene(m_nBombNum);

				// メモリのキャスト
				CScene2D *pScene2D = (CScene2D*)pScene;

				if (pScene2D != NULL)
				{
					switch (m_BombType)
					{
					case TYPE_ENEMY:
					{
						// 敵のサイズ取得
						CEnemy *pEnemy = (CEnemy*)pScene2D;

						// ライフをへらす
						pEnemy->HitDamage(30);

					}
					break;

					case TYPE_BULLET:
					{
						// 敵のサイズ取得
						CBullet *pBullet = (CBullet*)pScene2D;

						// ライフをへらす
						pBullet->DecreaseLife(10000);
					}
					break;

					}

				}
			}
		}
	}

	// 座標を渡す
	SetPosition(m_Pos);

	// 体力が無くなったら
	if (m_nLife <= 0)
	{
		if (m_pShield != NULL)
		{
			// シールド消滅
			m_pShield->Uninit();
			m_pShield = NULL;
		}
	}

	// 復活処理
	if (m_bPlayerDraw == true)
	{
		Resurrection();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	if (m_bPlayerDraw == false)
	{
		// 描画処理
		CScene2D::Draw();
	}
}

//=============================================================================
// プレイヤー制御関数
//=============================================================================
void CPlayer::PlayerControl(void)
{
	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	if (js.lX != 0.0f || js.lY != 0)
	{
		float fAngle = atan2f((float)js.lX, (float)js.lY);

		// ジョイパッド操作
		m_Pos.x += sinf(fAngle)* PLAYER_SPEED;
		m_Pos.y += cosf(fAngle)* PLAYER_SPEED;
	}

	// キーボード更新
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	// Wキーを押したとき
	if (pKeyboard->GetPress(DIK_W))
	{
		m_Pos.y += cosf(D3DX_PI)*PLAYER_SPEED;
	}
	// Sキーを押したとき
	if (pKeyboard->GetPress(DIK_S))
	{
		m_Pos.y += cosf(0)*PLAYER_SPEED;
	}
	// Aキーを押したとき
	if (pKeyboard->GetPress(DIK_A))
	{
		m_Pos.x -= sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}
	// Dキーを押したとき
	if (pKeyboard->GetPress(DIK_D))
	{
		m_Pos.x += sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}

	// シールド生成の処理
	if (pKeyboard->GetTrigger(DIK_RSHIFT) || CManager::GetJoypad()->GetJoystickTrigger(2, 0))
	{
		if (m_bUseLaser == true)
		{
			m_bUseLaser = false;
		}

		if (m_bShildScaleInfo == true)
		{
			// シールド生成
			if (m_bShildInfo == true)
			{	
				//白いとき
				if (m_pShield != NULL)
				{
					// シールド消滅
					m_pShield->Uninit();
					m_pShield = NULL;
				}
				m_bShildInfo = false;
				// 黒いとき
				m_pShield = CShield::Create(m_Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(SHIELD_SIZE_X, SHIELD_SIZE_Y, 0.0f),
					TYPE_SHIELD, CShield::SHIELD_TYPE_BLACK);
			}
			else
			{
				// 黒いとき
				if (m_pShield != NULL)
				{
					// シールド消滅
					m_pShield->Uninit();
					m_pShield = NULL;
				}
				m_bShildInfo = true;
				// 白いとき
				m_pShield = CShield::Create(m_Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(SHIELD_SIZE_X, SHIELD_SIZE_Y, 0.0f),
					TYPE_SHIELD, CShield::SHIELD_TYPE_WHITE);
			}
		}
	}

	// SPACEキーが押されたら
	if (pKeyboard->GetPress(DIK_SPACE) && m_bUseLaser == false || CManager::GetJoypad()->GetJoystickPress(0, 0))
	{
		// プレイヤーの色
		if (m_bShildScaleInfo == true)
		{
			// バレットの間隔
			if (m_BulletState == BULLET_INTERVAL)
			{
				CSound *pSound = NULL;
				pSound = CManager::GetSound();
				pSound->Play(CSound::SOUND_LABEL_SHOT);

				if (m_bShildInfo == true)
				{
					// 弾の生成
					CBullet::Create(D3DXVECTOR3(m_Pos.x + PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y, 0.0f),
						D3DXVECTOR3(0.0f, -15.0f, 0.0f),
						D3DXVECTOR3(PLAYER_BULLET_SIZE_X, PLAYER_BULLET_SIZE_Y, 0.0f),
						TYPE_BULLET, CBullet::BULLET_TYPE_PLAYER, CBullet::BULLET_COLOR_WHITE,
						100);

					// 弾の生成
					CBullet::Create(D3DXVECTOR3(m_Pos.x - PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y, 0.0f),
						D3DXVECTOR3(0.0f, -15.0f, 0.0f),
						D3DXVECTOR3(PLAYER_BULLET_SIZE_X, PLAYER_BULLET_SIZE_Y, 0.0f),
						TYPE_BULLET, CBullet::BULLET_TYPE_PLAYER, CBullet::BULLET_COLOR_WHITE,
						100);

					// マズルフラッシュの生成
					for (int nCount = 0; nCount < MUZZLE_FLASH_NUM; nCount++)
					{
						CMuzzleFlash::Create(D3DXVECTOR3(m_Pos.x + PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y + 10, 0.0f),
							D3DXVECTOR3(MUZZLE_FLASH_SIZE_X, MUZZLE_FLASH_SIZE_Y, 0.0f),
							TYPE_EFFECT, CMuzzleFlash::MUZZLE_FLASH_COLOR_WHITE, MUZZLE_FLASH_LIFE);

						CMuzzleFlash::Create(D3DXVECTOR3(m_Pos.x - PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y + 10, 0.0f),
							D3DXVECTOR3(MUZZLE_FLASH_SIZE_X, MUZZLE_FLASH_SIZE_Y, 0.0f),
							TYPE_EFFECT, CMuzzleFlash::MUZZLE_FLASH_COLOR_WHITE, MUZZLE_FLASH_LIFE);
					}
				}
				else
				{
					// 弾の生成
					CBullet::Create(D3DXVECTOR3(m_Pos.x + PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y , 0.0f),
						D3DXVECTOR3(0.0f, -15.0f, 0.0f),
						D3DXVECTOR3(PLAYER_BULLET_SIZE_X, PLAYER_BULLET_SIZE_Y, 0.0f),
						TYPE_BULLET, CBullet::BULLET_TYPE_PLAYER, CBullet::BULLET_COLOR_BLACK,
						100);
					// 弾の生成
					CBullet::Create(D3DXVECTOR3(m_Pos.x - PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y, 0.0f),
						D3DXVECTOR3(0.0f, -15.0f, 0.0f),
						D3DXVECTOR3(PLAYER_BULLET_SIZE_X, PLAYER_BULLET_SIZE_Y, 0.0f),
						TYPE_BULLET, CBullet::BULLET_TYPE_PLAYER, CBullet::BULLET_COLOR_BLACK,
						100);

					// マズルフラッシュの生成
					for (int nCount = 0; nCount < MUZZLE_FLASH_NUM; nCount++)
					{
						CMuzzleFlash::Create(D3DXVECTOR3(m_Pos.x + PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y + 10, 0.0f),
							D3DXVECTOR3(MUZZLE_FLASH_SIZE_X, MUZZLE_FLASH_SIZE_Y, 0.0f),
							TYPE_EFFECT, CMuzzleFlash::MUZZLE_FLASH_COLOR_BLACK, MUZZLE_FLASH_LIFE);

						CMuzzleFlash::Create(D3DXVECTOR3(m_Pos.x - PLAYER_SHOT_POS_X, m_Pos.y - PLAYER_SHOT_POS_Y + 10, 0.0f),
							D3DXVECTOR3(MUZZLE_FLASH_SIZE_X, MUZZLE_FLASH_SIZE_Y, 0.0f),
							TYPE_EFFECT, CMuzzleFlash::MUZZLE_FLASH_COLOR_BLACK, MUZZLE_FLASH_LIFE);
					}
				}
			}
		}
		// カウンターを減算
		m_BulletState--;

		// 0になったら元の値に戻す
		if (m_BulletState <= 0)
		{
			m_BulletState = BULLET_INTERVAL;
		}
	}

	// Lキーが押されたら
	if (pKeyboard->GetTrigger(DIK_L) && m_pGage->GetLaserNum() >= 10 || CManager::GetJoypad()->GetJoystickTrigger(7, 0))
	{
			if (m_bShildScaleInfo == true)
			{
				if (m_bShildInfo == true)
				{
					// トリガー
					if (m_bUseLaser == true)
					{
						if (m_nLaserCounter >= 100)
						{
							m_bUseLaser = false;
							m_nLaserCounter = 0;
						}
					}
					else
					{
						if (m_nLaserCounter >= 100)
						{
							if (m_pLaser == NULL)
							{
								// 弾の生成
								m_pLaser = CLaser::Create(D3DXVECTOR3(m_Pos.x + 0.0f, m_Pos.y + 5.0f, 0.0f),
									D3DXVECTOR3(0.0f, -15.0f, 0.0f),
									D3DXVECTOR3(LASER_SIZE_X, LASER_SIZE_Y, 0.0f),
									TYPE_LASER, CLaser::LASER_COLOR_WHITE);
							}

							// サウンドの処理
							CSound *pSound = NULL;
							pSound = CManager::GetSound();
							pSound->Play(CSound::SOUND_LABEL_LASER);

							m_bUseLaser = true;
							m_nLaserCounter = 0;
						}
					}
				}
				else if (m_bShildInfo == false)
				{
					// ボタンが押された
					// トリガー
					if (m_bUseLaser == true)
					{
						if (m_nLaserCounter >= 100)
						{
							m_bUseLaser = false;
							m_nLaserCounter = 0;
						}
					}
					else
					{
						if (m_nLaserCounter >= 100)
						{
							if (m_pLaser == NULL)
							{	
								// レーザーの生成
								m_pLaser = CLaser::Create(D3DXVECTOR3(m_Pos.x + 0.0f, m_Pos.y + 15.0f, 0.0f),
									D3DXVECTOR3(0.0f, -15.0f, 0.0f),
									D3DXVECTOR3(LASER_SIZE_X, LASER_SIZE_Y, 0.0f),
									TYPE_LASER, CLaser::LASER_COLOR_BLACK);
							}

							// 音の処理
							CSound *pSound = NULL;
							pSound = CManager::GetSound();
							pSound->Play(CSound::SOUND_LABEL_LASER);

							m_bUseLaser = true;
							m_nLaserCounter = 0;
						}
					}
				}
			}
	}

	// ボムの処理
	if (m_nBombCount > 0 &&pKeyboard->GetTrigger(DIK_B) && m_bUseLaser == false || 
		m_nBombCount > 0 &&CManager::GetJoypad()->GetJoystickTrigger(5, 0))
	{
		m_nBombCount--;

		CSound *pSound = NULL;
		pSound = CManager::GetSound();
		pSound->Play(CSound::SOUND_LABEL_BOMB);

		if (m_pBombUi != NULL)
		{
			// ボムのUIを消す
			m_pBombUi->Uninit();
			m_pBombUi = NULL;
		}
		if (m_bShildInfo == true)
		{
			// ボムの衝撃波生成
			for (int nCount = 0; nCount < BOMB_NUM; nCount++)
			{
				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SHOCK, CBomb::BOMB_COLOR_WHITE,
					nCount* (BOMB_ANGLE / BOMB_NUM), 180, BOMB_DISTANCE, BOMB_LIFE);

				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SHOCK, CBomb::BOMB_COLOR_WHITE,
					nCount* (BOMB_ANGLE / BOMB_NUM), 180, BOMB_DISTANCE, BOMB_LIFE);
			}

			// ボムの火花生成
			for (int nCount = 1; nCount < BOMB_SPARK_NUM + 1; nCount++)
			{
				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SPARK_SIZE_X, BOMB_SPARK_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SPARK, CBomb::BOMB_COLOR_WHITE,
					nCount * (BOMB_SPARK_ANGLE / BOMB_SPARK_NUM), 90, BOMB_SPARK_DISTANCE, BOMB_SPARK_LIFE);

				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SPARK_SIZE_X, BOMB_SPARK_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SPARK, CBomb::BOMB_COLOR_WHITE,
					nCount * (BOMB_SPARK_ANGLE / BOMB_SPARK_NUM), 90, BOMB_SPARK_DISTANCE, BOMB_SPARK_LIFE);
			}
		}
		else
		{
			// ボムの衝撃波生成
			for (int nCount = 0; nCount < BOMB_NUM; nCount++)
			{
				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SHOCK, CBomb::BOMB_COLOR_BLACK,
					nCount* (BOMB_ANGLE / BOMB_NUM), 180, BOMB_DISTANCE, BOMB_LIFE);

				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SHOCK, CBomb::BOMB_COLOR_BLACK,
					nCount* (BOMB_ANGLE / BOMB_NUM), 180, BOMB_DISTANCE, BOMB_LIFE);
			}

			// ボムの火花生成
			for (int nCount = 1; nCount < BOMB_SPARK_NUM + 1; nCount++)
			{
				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SPARK_SIZE_X, BOMB_SPARK_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SPARK, CBomb::BOMB_COLOR_BLACK,
					nCount * (BOMB_SPARK_ANGLE / BOMB_SPARK_NUM), 90, BOMB_SPARK_DISTANCE, BOMB_SPARK_LIFE);

				CBomb::Create(m_Pos,
					D3DXVECTOR3(BOMB_SPARK_SIZE_X, BOMB_SPARK_SIZE_Y, 0.0f),
					TYPE_EFFECT, CBomb::BOMB_TYPE_SPARK, CBomb::BOMB_COLOR_BLACK,
					nCount * (BOMB_SPARK_ANGLE / BOMB_SPARK_NUM), 90, BOMB_SPARK_DISTANCE, BOMB_SPARK_LIFE);

			}

		}
		m_bBombUse = true;
	}
	if (m_bUseLaser == true)
	{
		m_nLaserFlame++;

		if (m_nLaserFlame >= LASER_FLAME)
		{
			// ゲージの減算
			m_pGage->SubtractGame(10);
			m_nLaserFlame = 0;
		}

		// ゲージが一定量以下だったら
		if (m_pGage->GetLaserNum() <= 10)
		{
			m_bUseLaser = false;
		}
		m_nLaserCounter++;
	}
	else
	{
		m_nLaserCounter++;
	}

	// レーザーが使われていたら
	if (m_pLaser != NULL)
	{

		if (m_pLaser->GetSize().x <= 0.0f)
		{
			// レーザーの終了処理
			m_pLaser->Uninit();
			m_pLaser = NULL;

			m_bUseLaser = false;

			// 音の処理
			CSound *pSound = NULL;
			pSound = CManager::GetSound();
			pSound->Stop(CSound::SOUND_LABEL_LASER);

		}
	}

	// 左判定
	if (m_Pos.x - PLAYER_SIZE_X / 2 < WALL_SIZE_X)
	{
		m_Pos.x = WALL_SIZE_X + PLAYER_SIZE_X / 2;
	}
	// 右判定
	if (m_Pos.x + PLAYER_SIZE_X / 2 > SCREEN_WIDTH - WALL_SIZE_X)
	{
		m_Pos.x =  SCREEN_WIDTH - WALL_SIZE_X - PLAYER_SIZE_X / 2;
	}
	// 上判定
	if (m_Pos.y - PLAYER_SIZE_Y / 2 < 0.0f)
	{
		m_Pos.y = PLAYER_SIZE_Y / 2;
	}
	// 下判定
	if (m_Pos.y + PLAYER_SIZE_Y / 2 > SCREEN_HEIGHT)
	{
		m_Pos.y = SCREEN_HEIGHT - PLAYER_SIZE_Y / 2;
	}
}

//=============================================================================
// 復活
//=============================================================================
void CPlayer::Resurrection(void)
{
	// 復活のカウント
	m_nResurrectionCnt++;

	if (m_bPlayerDraw == false)
	{
		// 復活しました
		m_bPlayerDraw = true;
		m_State = PLAYER_STATE_REVIVE;
		m_nBombCount = PLAYER_BOMB_NUM;

		if (m_pBombUi == NULL)
		{
			m_pBombUi = CBombUi::Create(
				D3DXVECTOR3(195.0f, 630.0f, 0.0f), D3DXVECTOR3(BOMB_UI_SIZE_X, BOMB_UI_SIZE_Y, 0.0f), TYPE_SCORE);
		}

		// スコアを加算
		CRemain *pRemain = CGame::GetRemain();
		if (pRemain != NULL)
		{
			// 残機を減らす
			pRemain->SetRemain(1);
		}
	}

	if (m_nResurrectionCnt >= 50)
	{
		// 残機のカウントを上げる
		m_nStock++;

		// 残機が無かったらリザルト
		if (m_nStock >= PLAYER_STOCK&& CManager::GetFade()->Get() == CFade::FADE_NONE)
		{
			// 終了処理
			Uninit();

			// フェード処理
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			return;
		}

		// レーザーが使われていたら
		if (m_pLaser != NULL)
		{
			// レーザーの終了処理
			m_pLaser->Uninit();
			m_pLaser = NULL;

			m_bUseLaser = false;
		}

		// ライフと座標を戻す
		m_nLife = PLAYER_LIFE;
		m_Pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 700.0f, 0.0f);

		// Scene2Dにposを渡す
		SetPosition(m_Pos);

		// シールド生成
		m_pShield = CShield::Create(m_Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(SHIELD_SIZE_X, SHIELD_SIZE_Y, 0.0f),
			TYPE_SHIELD, CShield::SHIELD_TYPE_WHITE);

		// 初期化
		m_nResurrectionCnt = 0;
		m_bPlayerDraw = false;
		m_bShildInfo = true;

		// レーザーの値を減らす
		int nGame = m_pGage->GetLaserNum();
		m_pGage->SubtractGame(nGame);
	}
}

//=============================================================================
// プレイヤー状態更新
//=============================================================================
void CPlayer::UpdateState(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_State)
	{
		// 通常状態
	case PLAYER_STATE_NORMAL:
		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左上頂点の色	透明度255
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右上頂点の色	透明度255
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左下頂点の色	透明度255
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右下頂点の色	透明度255
		break;

		// 蘇生時
	case PLAYER_STATE_REVIVE:
		// 復活
		m_StateCount++;

		if (m_StateCount % 2 == 0)
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 50);	// 左上頂点の色	透明度255
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 50);	// 右上頂点の色	透明度255
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 50);	// 左下頂点の色	透明度255
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 50);	// 右下頂点の色	透明度255
		}
		else
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左上頂点の色	透明度255
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右上頂点の色	透明度255
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左下頂点の色	透明度255
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右下頂点の色	透明度255
		}

		if (m_StateCount >= PLAYER_ARMOR_COUNT)
		{
			// 通常状態に戻す
			m_State = PLAYER_STATE_NORMAL;
			m_StateCount = 0;
		}
		break;
	case PLAYER_STATE_LASER:
		break;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// ダメージを受けたとき
//=============================================================================
void CPlayer::HitDamage(int nCount)
{
	if (m_State != PLAYER_STATE_REVIVE)
	{
		// ライフ減算
		m_nLife -= nCount;

		if (m_nLife <= 0)
		{
			CSound *pSound = NULL;
			pSound = CManager::GetSound();
			pSound->Play(CSound::SOUND_LABEL_BOMB);

			if (m_bShildInfo == true)
			{
				// 爆発生成
				CExplosion::Create(
					m_Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f),
					TYPE_EXPLOSION, CExplosion::COLOR_TYPE_WHITE);
			}
			else
			{
				// 爆発生成
				CExplosion::Create(
					m_Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f),
					TYPE_EXPLOSION, CExplosion::COLOR_TYPE_BLACK);
			}

			// 復活の処理
			Resurrection();
		}
	}
}

//=============================================================================
// ボムの当たり判定
//=============================================================================
bool CPlayer::BombCollision(void)
{
	m_nBombFlame++;

	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		CScene *pScene = NULL;
		if (pScene == NULL)
		{
			// シーンを取得
			pScene = CScene::GetScene(nCount);

			// メモリのキャスト
			CScene2D *pScene2D = (CScene2D*)pScene;

			if (pScene2D != NULL)
			{
				CScene::TYPE type = pScene->GetType();

				// エネミーの時
				if (type == TYPE_ENEMY)
				{
					// ターゲットの情報確保
					D3DXVECTOR3 Target = pScene2D->GetPosition();

					// 敵のサイズ取得
					CEnemy *pEnemy = (CEnemy*)pScene2D;
					D3DXVECTOR3 size = pEnemy->GetSize();

					for (int nCntAngle = 0; nCntAngle < 360; nCntAngle++)
					{
						// 自分と相手の当たり判定
						if (Target.x - size.x / 2 >= m_Pos.x + 350.0f * cosf(D3DXToRadian(nCntAngle)) &&
							Target.x + size.x / 2 <= m_Pos.x - 350.0f * cosf(D3DXToRadian(nCntAngle)) &&
							Target.y + size.y / 2 <= m_Pos.y - 350.0f * cosf(D3DXToRadian(nCntAngle)) &&
							Target.y - size.y / 2 >= m_Pos.y + 350.0f * cosf(D3DXToRadian(nCntAngle)))
						{
							m_BombType = type;

							// エネミーの番号保管
							m_nBombNum = nCount;
							return true;
						}
					}
				}
				else if(type == TYPE_BULLET)
				{
					// ターゲットの情報確保
					D3DXVECTOR3 Target = pScene2D->GetPosition();

					// 敵のサイズ取得
					CBullet *pBullet = (CBullet*)pScene2D;
					D3DXVECTOR3 size = pBullet->GetSize();
					for (int nCntAngle = 0; nCntAngle < 360; nCntAngle++)
					{
						// 自分と相手の当たり判定
						if (Target.x - size.x / 2 >= m_Pos.x + 350.0f * cosf(D3DXToRadian(nCntAngle)) &&
							Target.x + size.x / 2 <= m_Pos.x - 350.0f * cosf(D3DXToRadian(nCntAngle)) &&
							Target.y + size.y / 2 <= m_Pos.y - 350.0f * cosf(D3DXToRadian(nCntAngle)) &&
							Target.y - size.y / 2 >= m_Pos.y + 350.0f * cosf(D3DXToRadian(nCntAngle)))
						{

							pBullet->DecreaseLife(10000);
							m_BombType = type;

							// エネミーの番号保管
							m_nBombNum = nCount;
						}
					}
				}
			}
		}
	}

	if (m_nBombFlame >= 2)
	{
		m_bBombUse = false;
	}

	return false;
}

//=============================================================================
// シールド状態を返す
//=============================================================================
bool CPlayer::GetShildInfo(void)
{
	return m_bShildInfo;
}

//=============================================================================
// レーザーを使っているか
//=============================================================================
bool CPlayer::GetUseLaser(void)
{
	return m_bUseLaser;
}

//=============================================================================
// レーザーゲージの状態
//=============================================================================
CGage *CPlayer::GetGage(void)
{
	return m_pGage;
}

//=============================================================================
// レーザーの情報
//=============================================================================
CLaser * CPlayer::GetLaser(void)
{
	return m_pLaser;
}
