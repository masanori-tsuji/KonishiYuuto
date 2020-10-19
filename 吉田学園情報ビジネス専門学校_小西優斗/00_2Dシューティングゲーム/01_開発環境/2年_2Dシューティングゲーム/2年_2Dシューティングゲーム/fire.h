#ifndef _FIRE_H_
#define _FIRE_H_
//=============================================================================
//
// 花火の最初のクラスヘッダー [fire.h]
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
#define MAX_FIRE (256)					// 弾の最大数
#define FIRE_SIZE_X (25)				// 弾の横幅
#define FIRE_SIZE_Y (25)				// 弾の縦幅
#define MAX_FIRE_TEXTURE (1)			// テクスチャの最大数

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//=============================================================================
// バレットクラス
//=============================================================================
class CFire : public CScene2D
{
public:
	typedef enum
	{
		FIRE_COLOR_NONE = 0,	// 初期値
		FIRE_COLOR_WHITE,		// 白い弾
		FIRE_COLOR_BLACK,		// 黒い弾
		FIRE_COLOR_MAX
	}FIRE_COLOR;

	CFire();			// コンストラクタ
	~CFire();			// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);			// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理

	static CFire * Create(
		D3DXVECTOR3 pos, D3DXVECTOR3 size,
		TYPE type, FIRE_COLOR Ctype,
		int nLife);														// ポリゴン生成

	void SetFire(int nLife, D3DXVECTOR3 size);							// 弾の情報を設定
	void SetType(FIRE_COLOR Ctype);										// タイプ設定
	static HRESULT Load(void);											// テクスチャロード
	static void UnLoad(void);											// テクスチャアンロード
	FIRE_COLOR GetCtype(void);											// 色の情報
	void InitMove(void);												// 移動量
private:
	D3DXVECTOR3 m_Pos;												// 位置
	D3DXVECTOR3 m_move;												// 移動
	D3DXVECTOR3 m_Size;												// サイズ
	int m_nLife;													// 制限距離
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FIRE_TEXTURE];		// テクスチャ情報のポインタ
	FIRE_COLOR m_Ctype;												// 火花カラー
};

#endif