//=============================================================================
//
// ボスクラス	[boss.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "boss.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "ui.h"
#include "fade.h"
#include "hpbar.h"
#include "fire.h"
#include "spark.h"
#include "surroundings.h"
#include "time.h"
#include "explosion.h"
#include "score.h"
#include "game.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BOSS_LIFE				(500)					// ボスの体力
#define BOSS_SECOND_LIFE		(600)					// 第二フェイズの体力
#define THIRD_BOSS_LIFE			(800)					// 第三フェーズの体力

#define APPEARANCE_MOVE_Y		(15.0f)					// 登場時のスピード
#define BOSS_MOVE_X				(0.02f)					// ボスの横移動
#define BULLET_SHOT_FLAME		(200)					// 弾発射のフレーム
#define BULLET_SWITCH_CNT		(10)					// 色の変わるカウント
#define BOSS_COLOR_NUM			(10)					// 色の変化
#define BOSS_COLOR_MAX			(150)					// カラー変更の最大
#define BOSS_COLOR_MIN			(0)						// カラー変更の最小
#define BOSS_ARMOR_TIME			(5)						// 無敵の時間
#define BOSS_CHANGE_COLOR		(35)					// カラーを変えるフレーム
#define BOSS_CHANGE_PHASES		(350)					// フェーズを変える時間
#define BOSS_END_FLAME			(100)					// ボスが死んだときの消える時間

#define FIRST_BULLET_NUM		(32)					// 最初の弾の数
#define FIRST_BULLET_INTERVAL	(20)					// 弾を打つ間隔
#define FIRST_CHANGE_ANGLE		(15)					// 角度の変更
#define FIRST_SPARK_INTERVAL	(50)					// ボスの火花が出るフレーム
#define FIRST_SPARK_NUM			(80)					// 火花の数

#define SECOND_BULLET_FLAME		(30)					// 第二フェイズ
#define SECOND_BOSS_MOVE		(1.0f)					// 第二フェーズ移動量
#define SECOND_SPARK_INTERVAL	(30)					// ボスの火花が出るフレーム
#define SECOND_SPARK_NUM		(80)					// 火花の数

#define THIRD_BOSS_MOVE			(1.5f)					// 第三フェーズの移動量
#define THIRD_SHOT_FLAME		(20)					// 弾発射のフレーム
#define THIRD_SWITCH_CNT		(400)					// 色の変わるカウント
#define THIRD_BULLET_NUM		(6)						// 第三フェーズの弾の量
#define THIRD_BULLET_SPPED		(5)						// 第三フェーズの弾の速度
#define THIRD_CHANGE_ANGLE		(15)					// 角度の変更

#define RETIRE_MOVE_Y			(-5.0f)					// 撤退の時の移動量
#define RETIRE_TIME				(300)					// 撤退時のフレーム

#define BOSS_EXPLOSION_FLAME	(10)					// ボスの爆発フレーム
#define BOSS_EXPLOSION_COUNT	(10)					// 爆発の回数

#define BOSS_SCORE				(50000)					// ボスのスコア

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CBoss::m_apTexture[MAX_BOSS_TEXTURE] = {};

//=============================================================================
// ポリゴン生成
//=============================================================================
CBoss * CBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// メモリ確保
	CBoss *pBoss = new CBoss;

	if (pBoss != NULL)
	{
		// 初期化処理
		pBoss->Init(pos, size, type);

		// テクスチャ設定
		pBoss->BindTexture(m_apTexture[0]);

		// 初期移動量
		pBoss->AppearanceMove();

		// ボスステータス設定
		pBoss->SetPos(pos);
		pBoss->SetSize(size);

		// HPバー
		pBoss->pHpbar = CHpbar::Create(
			D3DXVECTOR3(SCREEN_WIDTH / 2, 25.0f, 0.0f),
			D3DXVECTOR3(BAR_SIZE_X, BAR_SIZE_Y, 0.0f), TYPE_UI, BOSS_LIFE);
	}

	return pBoss;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CBoss::CBoss()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_nBulletFlame = 0;
	m_nBulletCnt = 0;
	m_nBossPhases = 0;
	m_bColorFlag = true;
	m_State = ENEMY_STATE_NORMAL;
	m_nStateCnt = 0;
	pHpbar = NULL;
	m_nFlashFlame = 0;
	m_nSubNumber = 0;
	m_nSubNum = BOSS_COLOR_NUM;
	m_nPhaseFlame = 0;
	m_bExplosion = false;
	m_nExplosionCount = 0;
	m_bBossEnd = false;
	m_nEndCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBoss::~CBoss()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBoss::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	// 初期化処理
	CScene2D::Init(pos, size, type);

	// 値の初期化
	m_Pos = pos;					// 自分の座標
	m_nLife = BOSS_LIFE;			// ボスの体力
	m_Fhase = BOSS_PHASE_FIRST;		// ボスのフェーズ

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBoss::Update(void)
{
	// 更新処理
	CScene2D::Update();

	// 座標更新
	m_Pos = GetPosition();

	// 移動の更新
	m_Pos += m_Move;

	// ボスの状態
	switch (m_State)
	{
		// 通常時
	case ENEMY_STATE_NORMAL:
		break;

	// ダメージを受けたとき
	case ENEMY_STATE_DAMAGE:

		// フレームカウント
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

		// カウントが一定になったら
		if (m_nStateCnt >= BOSS_ARMOR_TIME)
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

	// フェーズの更新
	FhaseUpdate();

	// 座標を渡す
	SetPosition(m_Pos);
	SetPos(m_Pos);

	// 撤退の時間が過ぎたら
	if (CTime::GetRetire() == true)
	{
		// 撤退の処理
		Retire();
	}

	// 当たり判定
	Collision();

	// ボスが死んだら
	if (m_bBossEnd == true)
	{
		// フレームカウント
		m_nEndCount++;

		// 一定時間になったら
		if (m_nEndCount >= BOSS_END_FLAME)
		{
			// 終了処理
			Uninit();

			// リザルト画面へ
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);

			return;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBoss::Draw(void)
{
	// ボスが死んでいないとき
	if (m_bBossEnd == false)
	{
		// 描画処理
		CScene2D::Draw();
	}
}

//=============================================================================
// 登場時の移動
//=============================================================================
void CBoss::AppearanceMove(void)
{
	m_Move = D3DXVECTOR3(0.0f, APPEARANCE_MOVE_Y, 0.0f);
}

//=============================================================================
// ボスのダメージ処理
//=============================================================================
void CBoss::HitBossDamage(int nNumber)
{
	// 撤退時じゃないとき
	if (CTime::GetRetire() == false)
	{
		// ボスのチェンジフレームじゃないとき
		if (m_Fhase != BOSS_PHASE_CHANGE_01 && m_Fhase != BOSS_PHASE_CHANGE_02 && m_Fhase != BOSS_PHASE_EXPLOSION)
		{
			// サウンド処理
			CSound *pSound = NULL;
			pSound = CManager::GetSound();
			pSound->Play(CSound::SOUND_LABEL_HIT);

			// 体力の減算
			m_nLife -= nNumber;

			// 体力バーの減算
			pHpbar->AddDamage(nNumber);
		}

		// 体力が0になったら
		if (m_nLife <= 0)
		{
			// フェーズの種類
			switch (m_Fhase)
			{
			// 第一フェイズの時
			case BOSS_PHASE_FIRST:
			{
				m_nPhaseFlame = 0;

				// 幕間1に移行
				m_Fhase = BOSS_PHASE_CHANGE_01;

				// サウンド処理
				CSound *pSound = NULL;
				pSound = CManager::GetSound();
				pSound->Play(CSound::SOUND_LABEL_CHANGE);
			}
				break;

			// 第二フェイズの時
			case BOSS_PHASE_SECOND:
			{
				m_nPhaseFlame = 0;

				// 幕間2に移行
				m_Fhase = BOSS_PHASE_CHANGE_02;

				// サウンド処理
				CSound *pSound = NULL;
				pSound = CManager::GetSound();
				pSound->Play(CSound::SOUND_LABEL_CHANGE);
			}
			break;

			// 第三フェーズ
			case BOSS_PHASE_THIRD:
			{
				//　フェーズのカウントを0にする
					m_nPhaseFlame = 0;

					// スコアを加算
					CScore *pScore = CGame::GetScore();
					pScore->SetScore(BOSS_SCORE);

					// 幕間2に移行
					m_Fhase = BOSS_PHASE_EXPLOSION;
					m_bExplosion = true;
			}
			break;

			default:
				break;

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
// フェイズの更新
//=============================================================================
void CBoss::FhaseUpdate(void)
{
	// フェーズの種類
	switch (m_Fhase)
	{
		// 第一フェイズ
	case BOSS_PHASE_FIRST:
		FirstPhases();
		break;

		// チェンジフェーズ
	case BOSS_PHASE_CHANGE_01:

		// フレームを進める
		m_nPhaseFlame++;

		// 火花が出る間隔
		if (m_nPhaseFlame % FIRST_SPARK_INTERVAL == 0)
		{
			// 火花が出る方向 
			int nBaseAngle = 0 - rand() % 180 - rand() % 180;

			for (int nCount = 0; nCount < FIRST_SPARK_NUM; nCount++)
			{
				// 火花の生成
				CSpark::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y , 0.0f),
					D3DXVECTOR3(SPARK_SIZE_X, SPARK_SIZE_Y, 0.0f),
					TYPE_EXPLOSION, SPARK_BOSS_ANGLE, nBaseAngle, SPARK_CHANGE_DISTANCE, SPARK_CHANGE_LIFE);
			}
		}

		// フェーズ移行
		ChangePhases();

		// ゲージの表示
		pHpbar->SetHp(BOSS_SECOND_LIFE);

		if (m_nPhaseFlame >= BOSS_CHANGE_PHASES)
		{
			// 第二フェーズ以降
			m_Fhase = BOSS_PHASE_SECOND;

			// 第二フェーズの初期化
			InitSecondPhase();

			// サウンド処理
			CSound *pSound = NULL;
			pSound = CManager::GetSound();
			pSound->Stop(CSound::SOUND_LABEL_CHANGE);
		}
		break;

		// 第二フェイズ
	case BOSS_PHASE_SECOND:

		// 第二フェーズの処理
		SecondPhases();
		break;

		// チェンジフェーズ2
	case BOSS_PHASE_CHANGE_02:

		// フレーム数カウント
		m_nPhaseFlame++;

		// 火花が出る間隔
		if (m_nPhaseFlame % SECOND_SPARK_INTERVAL == 0)
		{
			// 火花が出る方向 
			int nBaseAngle = 0 - rand() % 180 - rand() % 180;

			for (int nCount = 0; nCount < SECOND_SPARK_NUM; nCount++)
			{
				// 火花を出す処理
				CSpark::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f),
					D3DXVECTOR3(SPARK_SIZE_X, SPARK_SIZE_Y, 0.0f),
					TYPE_EXPLOSION, SPARK_BOSS_ANGLE, nBaseAngle, SPARK_CHANGE_DISTANCE, SPARK_CHANGE_LIFE);
			}
		}

		// フェーズ移行
		ChangePhases();

		// ゲージの表示
		pHpbar->SetHp(THIRD_BOSS_LIFE);

		// フェーズを変える処理
		if (m_nPhaseFlame >= BOSS_CHANGE_PHASES)
		{
			// 第三フェーズ移行
			m_Fhase = BOSS_PHASE_THIRD;

			// 第三フェーズの初期化
			InitThirdPhase();

			// 音の処理
			CSound *pSound = NULL;
			pSound = CManager::GetSound();
			pSound->Stop(CSound::SOUND_LABEL_CHANGE);

		}
		break;

		// 第三フェーズ
	case BOSS_PHASE_THIRD:

		// 第三フェーズの処理
		ThirdPhase();
		break;

		// 爆発フェーズ
	case BOSS_PHASE_EXPLOSION:

		// 爆発フェーズの処理
		ExplosionPhase();
		break;

	default:
		break;
	}
}

//=============================================================================
// 撤退の処理
//=============================================================================
void CBoss::Retire(void)
{
	// 撤退フレームカウント
	m_nRetireCount++;

	// 移動量設定
	m_Move = D3DXVECTOR3(0.0f, RETIRE_MOVE_Y, 0.0f);

	// カウントが一定を越えたらボスの終了
	if (m_nRetireCount >= RETIRE_TIME)
	{
		m_bBossEnd = true;
	}

}

//=============================================================================
// 第一フェイズ
//=============================================================================
void CBoss::FirstPhases(void)
{
	// 移動更新処理
	FirstUpdateMove();

	// 弾発射処理
	FirstBulletShot();
}

//=============================================================================
// ボスの移動処理
//=============================================================================
void CBoss::FirstUpdateMove(void)
{
	// ボスの止まる場所
	if (m_Move.y > 0.0f)
	{
		m_Move.y -= 0.2f;
	}
	else
	{
		m_Move.y = 0.0f;

		// 横幅の移動の制限
		if (m_Pos.x >= SCREEN_WIDTH - WALL_SIZE_X - 100.0f)
		{
			m_Move.x = -1.0f;
		}
		if (m_Pos.x <= WALL_SIZE_X + 100.0f)
		{
			m_Move.x = 1.0f;
		}
	}
}

//=============================================================================
// 弾の発射処理
//=============================================================================
void CBoss::FirstBulletShot(void)
{
	// バレットのカウント
	m_nBulletFlame++;

	// 弾を打つ間隔
	if (m_nBulletFlame >= BULLET_SHOT_FLAME)
	{
		for (int nCount = 0; nCount < FIRST_BULLET_NUM; nCount++)
		{
			// 色の状態
			if (m_bColorFlag == false)
			{
				// 弾の生成
				CBullet::Create(D3DXVECTOR3(
					m_Pos.x + 100.0f*cosf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM) + m_nBulletCnt * FIRST_CHANGE_ANGLE)),
					m_Pos.y + 100.0f*sinf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM) + m_nBulletCnt * FIRST_CHANGE_ANGLE)),
					0.0f),
					D3DXVECTOR3(cosf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM) + m_nBulletCnt * FIRST_CHANGE_ANGLE)) * 2,
					sinf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM) + m_nBulletCnt * FIRST_CHANGE_ANGLE)) * 2, 0.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), TYPE_BULLET,
					CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_WHITE,
					1000);
			}
			else
			{
				// 弾の生成
				CBullet::Create(D3DXVECTOR3(
					m_Pos.x + 100.0f*cosf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM)+ m_nBulletCnt * FIRST_CHANGE_ANGLE)) ,
					m_Pos.y + 100.0f*sinf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM)+ m_nBulletCnt * FIRST_CHANGE_ANGLE)),
					0.0f),
					D3DXVECTOR3(cosf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM) + m_nBulletCnt * FIRST_CHANGE_ANGLE)) * 2,
						sinf(D3DXToRadian((nCount * 360 / FIRST_BULLET_NUM) + m_nBulletCnt * FIRST_CHANGE_ANGLE)) * 2, 0.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), TYPE_BULLET,
					CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_BLACK,
					1000);
			}
		}

		// 次のフレームのカウント
		m_nBulletFlame = BULLET_SHOT_FLAME - FIRST_BULLET_INTERVAL;

		// バレット回数カウント
		m_nBulletCnt++;

		// 回数で色を変える
		if (m_nBulletCnt % BULLET_SWITCH_CNT == 0)
		{
			// 色を変える処理
			if (!m_bColorFlag)
			{
				m_bColorFlag = true;
			}
			else
			{
				m_bColorFlag = false;
			}
		}
	}
}

//=============================================================================
// チェンジのフェーズ
//=============================================================================
void CBoss::ChangePhases(void)
{
	// 更新処理
	ChangeUpdateMove();

	// 点滅
	ChangeFlash();
}

//=============================================================================
// チェンジフェーズの更新
//=============================================================================
void CBoss::ChangeUpdateMove(void)
{
	// 移動量
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//====================================================================
// ポリゴンの点滅
//====================================================================
void CBoss::ChangeFlash(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーの減算
	m_nSubNumber += m_nSubNum;

	// カラー変更の最大値
	if (m_nSubNumber >= BOSS_COLOR_MAX)
	{
		// 制限
		m_nSubNumber = BOSS_COLOR_MAX;

		// 値を反転
		m_nSubNum *= -1;
	}
	else if (m_nSubNumber <= BOSS_COLOR_MIN)
	{
		// カラーの最小値の制限
		m_nSubNumber = BOSS_COLOR_MIN;

		// フレームカウント
		m_nFlashFlame++;

		// 一定のフレーム以上になったら
		if (m_nFlashFlame >= BOSS_CHANGE_COLOR)
		{
			m_nFlashFlame = 0;

			// 値を反転
			m_nSubNum *= -1;
		}
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
// 第二フェイズ
//=============================================================================
void CBoss::SecondPhases(void)
{
	// 移動の更新処理
	SecondUpdateMove();

	// 弾を打つ処理
	SecondBulletShot();
}

//=============================================================================
// 第二フェイズの初期化
//=============================================================================
void CBoss::InitSecondPhase(void)
{
	// ライフを増やす
	m_nLife = BOSS_SECOND_LIFE;

	// 移動量設定
	m_Move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 第二フェイズの更新
//=============================================================================
void CBoss::SecondUpdateMove(void)
{
	// 横幅の移動を制限
	if (m_Pos.x >= SCREEN_WIDTH - WALL_SIZE_X - 100.0f)
	{
		m_Move.x = -SECOND_BOSS_MOVE;
	}
	if (m_Pos.x <= WALL_SIZE_X + 100.0f)
	{
		m_Move.x = SECOND_BOSS_MOVE;
	}
}

//=============================================================================
// 第二フェイズの更新
//=============================================================================
void CBoss::SecondBulletShot(void)
{
	// バレットのフレーム加算
	m_nBulletFlame++;

	// 弾を打つ制限
	if (m_nBulletFlame >= SECOND_BULLET_FLAME)
	{
		// ランダムで色を決める
		if (rand() % 2 == 0)			// 白
		{
			// 花火発射
			CFire::Create(m_Pos , D3DXVECTOR3(FIRE_SIZE_X, FIRE_SIZE_Y, 0.0f),
				CFire::TYPE_BULLET, CFire::FIRE_COLOR_WHITE, 50);
		}
		else							// 黒
		{
			// 花火発射
			CFire::Create(m_Pos, D3DXVECTOR3(FIRE_SIZE_X, FIRE_SIZE_Y, 0.0f),
				CFire::TYPE_BULLET, CFire::FIRE_COLOR_BLACK, 50);
		}

		// 次のバレット間隔
		m_nBulletFlame = 0;
	}
}

//=============================================================================
// 第三フェーズ
//=============================================================================
void CBoss::ThirdPhase(void)
{
	// 移動の更新処理
	ThirdUpdateMove();

	// 弾を打つ処理
	ThirdBulletShot();
}

//=============================================================================
// 第三フェーズ初期化
//=============================================================================
void CBoss::InitThirdPhase(void)
{
	// 取り巻きの生成
	CSurroundings::Create(D3DXVECTOR3(WALL_SIZE_X + 50.0f, 50.0f, 0.0f),
		D3DXVECTOR3(SURROUNDINGS_SIZE_X, SURROUNDINGS_SIZE_Y, 0.0f), TYPE_SURROUNDINGS);

	// 取り巻きの生成
	CSurroundings::Create(D3DXVECTOR3(SCREEN_WIDTH - WALL_SIZE_X - 50.0f, 50.0f, 0.0f),
		D3DXVECTOR3(SURROUNDINGS_SIZE_X, SURROUNDINGS_SIZE_Y, 0.0f), TYPE_SURROUNDINGS);

	// 値の代入
	m_nLife = THIRD_BOSS_LIFE;								// ライフを増やす
	m_Move = D3DXVECTOR3(-THIRD_BOSS_MOVE, 0.0f, 0.0f);		// 移動量設定
	m_nBulletFlame = 0;										// 弾のフレームカウント
	m_nBulletCnt = 0;										// バレットの色カウント

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	// 頂点情報を設定
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 第三フェーズ移動更新
//=============================================================================
void CBoss::ThirdUpdateMove(void)
{
	// 移動の制限
	if (m_Pos.x >= SCREEN_WIDTH - WALL_SIZE_X - 100.0f)
	{
		m_Move.x = -THIRD_BOSS_MOVE;
	}

	if (m_Pos.x <= WALL_SIZE_X + 100.0f)
	{
		m_Move.x = THIRD_BOSS_MOVE;
	}
}

//=============================================================================
// 第三フェーズの弾打ち
//=============================================================================
void CBoss::ThirdBulletShot(void)
{
	// バレットのカウント
	m_nBulletFlame++;

	// 角度変更用変数
	static int nNum = 0;

	// 弾を打つ間隔
	if (m_nBulletFlame >= THIRD_SHOT_FLAME)
	{
		nNum++;

		for (int nCount = 0; nCount < THIRD_BULLET_NUM; nCount++)
		{
			// 色の情報
			if (m_bColorFlag == true)
			{
				// 角度をずらす
				if (nNum % 2 == 0)
				{
					CBullet::Create(D3DXVECTOR3(m_Pos.x + 100.0f*cosf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)),
						m_Pos.y + 70.0f*sinf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)), 0.0f),
						D3DXVECTOR3(cosf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)) * THIRD_BULLET_SPPED,
							sinf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)) * THIRD_BULLET_SPPED, 0.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), TYPE_BULLET,
						CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_WHITE,
						1000);
				}
				else
				{
					CBullet::Create(D3DXVECTOR3(m_Pos.x + 100.0f*cosf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)),
						m_Pos.y + 70.0f*sinf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)), 0.0f),
						D3DXVECTOR3(cosf(D3DXToRadian(45+(nCount * 120 / THIRD_BULLET_NUM - THIRD_CHANGE_ANGLE) )) * THIRD_BULLET_SPPED,
							sinf(D3DXToRadian(45 + (nCount * 120 / THIRD_BULLET_NUM- THIRD_CHANGE_ANGLE) )) * THIRD_BULLET_SPPED, 0.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), TYPE_BULLET,
						CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_WHITE,
						1000);
				}
			}
			else
			{
				// 角度をずらす
				if (nNum % 2 == 0)
				{
					CBullet::Create(D3DXVECTOR3(m_Pos.x + 100.0f*cosf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)),
						m_Pos.y + 70.0f*sinf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)), 0.0f),
						D3DXVECTOR3(cosf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)) * THIRD_BULLET_SPPED,
							sinf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)) * THIRD_BULLET_SPPED, 0.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), TYPE_BULLET,
						CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_BLACK,
						1000);
				}
				else
				{
					CBullet::Create(D3DXVECTOR3(m_Pos.x + 100.0f*cosf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)),
						m_Pos.y + 70.0f*sinf(D3DXToRadian(45 + (nCount * 90 / THIRD_BULLET_NUM) + THIRD_CHANGE_ANGLE)), 0.0f),
						D3DXVECTOR3(cosf(D3DXToRadian(45 + (nCount * 120 / THIRD_BULLET_NUM - THIRD_CHANGE_ANGLE))) * THIRD_BULLET_SPPED,
							sinf(D3DXToRadian(45 + (nCount * 120 / THIRD_BULLET_NUM - THIRD_CHANGE_ANGLE))) * THIRD_BULLET_SPPED, 0.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), TYPE_BULLET,
						CBullet::BULLET_TYPE_ENEMY, CBullet::BULLET_COLOR_BLACK,
						1000);
				}
			}
		}

		// フレーム初期化
		m_nBulletFlame = 0;

	}
	// バレット回数カウント
	m_nBulletCnt++;

	// 回数で色を変える
	if (m_nBulletCnt >= THIRD_SWITCH_CNT)
	{
		if (m_bColorFlag == false)
		{
			m_bColorFlag = true;
		}
		else
		{
			m_bColorFlag = false;
		}

		m_nBulletCnt = 0;
	}
}

//=============================================================================
// 爆発フェーズ
//=============================================================================
void CBoss::ExplosionPhase(void)
{
	// 移動量設定
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// フレームカウント
	m_nExplosionFlame++;

	// 爆発がでる間隔
	if (m_nExplosionFlame % BOSS_EXPLOSION_FLAME == 0)
	{
		// 情報を設定
		int nDis = rand() % 70 + rand() % 70- rand() % 70- rand() % 70;					// 距離の設定
		int nAnlfe = rand() % 90 - rand() % 90 + rand() % 90 - rand() % 90;				// 角度の設定

		// ランダムで出現を決める
		D3DXVECTOR3 TargetPos = D3DXVECTOR3(m_Pos.x + nDis * cosf(D3DXToRadian(nAnlfe)),
			m_Pos.y + nDis * sinf(D3DXToRadian(nAnlfe)), 0.0f);

		// 爆発の設定
		CExplosion::Create(D3DXVECTOR3(TargetPos.x, TargetPos.y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(BOSS_EXPLOSION_SIZE_X, BOSS_EXPLOSION_SIZE_Y, 0.0f),
			TYPE_EXPLOSION, CExplosion::COLOR_TYPE_BOSS);

		// 音の設定
		CSound *pSound = NULL;
		pSound = CManager::GetSound();
		pSound->Play(CSound::SOUND_LABEL_BOSS_EXPLOSION);

		// 爆発したカウント
		m_nExplosionCount++;
	}

	// 一定数爆発したら
	if (m_nExplosionCount >= BOSS_EXPLOSION_COUNT)
	{
		m_bBossEnd = true;
	}
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CBoss::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "date/TEXTURE/enemy_boss.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CBoss::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BOSS_TEXTURE; nCount++)
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
// ライフの情報
//=============================================================================
int CBoss::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// ボスのカラー情報
//=============================================================================
bool CBoss::GetColor(void)
{
	return m_bColorFlag;
}

//=============================================================================
// ボスの爆発情報
//=============================================================================
bool CBoss::GetExplosion(void)
{
	return m_bExplosion;
}