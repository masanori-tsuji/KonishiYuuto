#ifndef _ENEMY_H_
#define _ENEMY_H_
//=============================================================================
//
// エネミークラスヘッダー [enemy.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene2d.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ENEMY				(256)			// 敵の最大数
#define ENEMY_SIZE_X			(50)			// エネミーのサイズ
#define ENEMY_SIZE_Y			(50)			// エネミーのサイズ
#define ENEMY_NORMAL_SIZE_X		(60)			// 敵の横幅
#define ENEMY_NORMAL_SIZE_Y		(60)			// 敵の縦幅
#define ENEMY_CURRENT_SIZE_X	(70)			// 自機取得弾を打つ敵の大きさ
#define ENEMY_CURRENT_SIZE_Y	(70)			// 自機取得弾を打つ敵の大きさ
#define ENEMY_SNAKE_SIZE_X		(40)			// ジグザグ敵
#define ENEMY_SNAKE_SIZE_Y		(40)			// ジグザグ敵
#define ENEMY_THROUGH_SIZE_X	(70)			// 真っすぐ弾を打つ敵のサイズ
#define ENEMY_THROUGH_SIZE_Y	(70)			// 真っすぐ弾を打つ敵のサイズ
#define ENEMY_DIRECTION_SIZE_X	(50)			// 方向転換する敵のサイズ
#define ENEMY_DIRECTION_SIZE_Y	(50)			// 方向転換する敵のサイズ
#define ENEMY_FOLLOW_SIZE_X		(50)			// 真っすぐ弾を打つ敵のサイズ
#define ENEMY_FOLLOW_SIZE_Y		(50)			// 真っすぐ弾を打つ敵のサイズ

#define MAX_ENEMY_TEXTURE		(4)				// エネミーのテクスチャ

#define INSIDE_ROTATION_RADIUS	(400)			// 回る敵の外側の半径
#define OUTSIDE_ROTATION_RADIUS	(325)			// 回る敵の内側の半径

#define CIRCLE_ENEMY_NUM		(5)				// サークル配置の敵の数
#define CIRCLE_ANGLE			(-45.0f)		// サークル配置の敵の角度
#define CIRCLE_MAX_ANGLE		(150)			// 敵の出現のサークルの角度
#define CIRCLE_DISTANCE			(210.0f)		// サークルの距離
#define CIRCLE_INITIAL_Y		(-125.0f)		// 初期の座標

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;
class CLight;

//=============================================================================
// エネミークラス
//=============================================================================
class CEnemy : public CScene2D
{
public:
	// 敵の色
	typedef enum
	{
		COLOR_TYPE_NONE = 0,	// 初期値
		COLOR_TYPE_WHITE,		// 白い敵
		COLOR_TYPE_BLACK,		// 赤い敵
		COLOR_TYPE_MAX
	}COLOR_TYPE;

	// 敵の種類
	typedef enum
	{
		ENEMY_TYPE_NONE = 0,		// 初期値
		ENEMY_TYPE_NORMAL,			// 通常敵
		ENEMY_TYPE_CURRENT,			// 自機取得弾を打つ敵
		ENEMY_TYPE_SNAKE,			// ジグザグ移動
		ENEMY_TYPE_THROUGH,			// 真っすぐ弾を打つ敵
		ENEMY_TYPE_ROTATION,		// 回ってくる敵
		ENEMY_TYPE_CIRCLE,			// サークル配置の敵
		ENEMY_TYPE_DIRECTION,		// 方向転換する敵
		ENEMY_TYPE_FOLLOW,			// 追従する敵
		ENEMY_TYPE_MAX
	}ENEMY_TYPE;

	// 敵の状態
	typedef enum
	{
		ENEMY_STATE_NORMAL = 0,		// 通常状態
		ENEMY_STATE_DAMAGE,			// ダメージを受けている状態
		ENEMY_STATE_MAX				// 最大
	}ENEMY_STATE;

	CEnemy();			// コンストラクタ
	~CEnemy();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード

	D3DXVECTOR3 GetPos(void) { return m_Pos; }						// 情報を返す
	void SetPos(D3DXVECTOR3 pos);									// 座標設定
	void SetSize(D3DXVECTOR3 size);									// サイズ設定
	void SetEnemyType(COLOR_TYPE Ctype, ENEMY_TYPE Etype);			// 情報セット
	void SetLife(void);												// ライフの設定
	void SetMove(void);												// 移動設定
	void SetCounter(void);											// カウンターバレットの設定
	void SetFirstBulletTime(int nTime);								// 初弾の発生フレーム設定
	void SetRotation(float fRadius, float fTheta);					// 角度の設定
	bool Collision(void);											// 当たり判定
	void InitThrough(D3DXVECTOR3 pos);								// 初期位置取得
	D3DXVECTOR3 GetSize(void);										// サイズの情報
	void TransparentInit(void);										// 透明の初期化

	void ObliqueUpdate(void);										// 斜め移動の更新
	void CurrentUpdate(void);										// 自機取得弾を打つ敵の動き
	void SnakeUpdate(void);											// 横移動更新
	void ThroughUpdate(void);										// 真っすぐ弾を打つ敵の更新
	void RotationUpdate(void);										// ローテーションの敵の更新
	void CircleUpdate(void);										// サークル配置の敵の更新
	void DirectionUpdate(void);										// 方向転換の敵
	void FollowUpdate(void);										// 追従してくる敵

	void HitDamage(int nCount);										// エネミーのヒット時

	void NormalBullet(void);										// 通常弾
	void CurrentBullet(int nBulletNum);								// 自機取得弾
	void ContinuousBullet(int nBulletNum);							// 連続弾
	void ScaleUp(void);												// 拡大
	void Appearance(void);											// 透明から出現

	static CEnemy * Create(
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, 
		TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype);									// エネミー生成

	static CEnemy * Current(
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size,
		TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, int nTime);						// 追従の敵生成

	static CEnemy * Rotation(
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size,
		TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, float fRadius, float fTheta);	// 回るエネミー生成

	static CEnemy * Circle(
		D3DXVECTOR3 pos, D3DXVECTOR3 size,
		TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, float fRadius, float fTheta);	// サークルの敵

	static CEnemy * Direction(
		D3DXVECTOR3 pos, D3DXVECTOR3 size,
		TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype, int nNumber);					// 方向転換の敵

	static CEnemy*Follow(D3DXVECTOR3 pos, D3DXVECTOR3 size,								// 追従してくる敵
		TYPE type, COLOR_TYPE Ctype, ENEMY_TYPE Etype);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_ENEMY_TEXTURE];		// テクスチャ情報のポインタ
	D3DXVECTOR3 m_Pos;						// 位置
	D3DXVECTOR3 m_Move;						// 移動
	D3DXVECTOR3 m_Size;						// サイズ
	int m_nLife;							// 体力
	CLight *m_pLight;						// ライトのポインタ
	D3DXVECTOR3 m_center;					// センター
	COLOR_TYPE m_Ctype;						// カラーの種類
	ENEMY_TYPE m_Etype;						// エネミーの種類
	ENEMY_STATE m_State;					// エネミーの状態
	float m_fBulletFlame;					// バレット間隔のカウンター
	int m_nEnemyCounter;					// エネミー制御のカウンター
	int m_nStateCnt;						// エネミーの状態カウンター
	int m_nCountinueInter;					// 連続弾の間隔
	int m_nContinueCnt;						// 連続弾のカウント
	int m_nBulletNum;						// 弾の数
	int m_nCounterBulletNum;				// カウンター弾の設定
	int m_nFirstBulletFrame;				// 初弾のフレーム
	int m_nThroughStopCnt;					// 止まっているフレーム
	int m_nOutsideScreenCnt;				// スクリーン外のカウント
	int m_nDirection;						// 方向転換の敵のナンバー
	int m_nSubNumber;						// テクスチャの透明度変更変数
	int m_nSubNum;							// 透明度の数値
	float m_fScale;							// 縮小用変数
	float m_fScaleNum;						// 縮小量
	float m_fThroughBulletCnt;				// 真っすぐ飛ばす敵のバレットカウント
	float m_fRadius;						// 半径
	float m_fAddRadius;						// 半径加算量
	float m_fTheta;							// 角度
	float m_fOmega;							// 角度加算量
	float m_fRadiusNum;						// 半径の値
	bool m_bSnakeSwitch;					// ジグザグ移動のフラグ
	bool m_bTwo_Sides;						// 真っすぐ弾を打つ敵のフラグ
	bool m_bRotationScale;					// ローテーションの拡大フラグ
	bool m_bFlashFlag;						// フラッシュのフラグ
	bool m_bFollowFlag;						// 追従の敵のフラグ
};

#endif