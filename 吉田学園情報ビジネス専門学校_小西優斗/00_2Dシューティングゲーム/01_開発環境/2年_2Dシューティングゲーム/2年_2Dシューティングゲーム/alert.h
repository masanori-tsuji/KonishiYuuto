#ifndef _ALERT_H_
#define _ALERT_H_
//=====================================================
//
// 警報ポリゴンクラスヘッダー [alert.h]
// Author : Konishi Yuuto
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "polygon.h"

//=====================================================
// マクロ定義
//=====================================================
#define ALERT_SIZE_X	(580.0f)		// アラートのサイズ
#define ALERT_SIZE_Y	(300.0f)		// アラートのサイズ

//=====================================================
// 前方宣言
//=====================================================
class CScene;
class CWarning;

//=====================================================
// クラスヘッダー
//=====================================================
class CAlert : public CPolygon
{
public:
	CAlert();										// コンストラクタ
	~CAlert();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type);					// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理

	static CAlert *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type);		// ポリゴン生成

	void FlashPolygon(void);													// ポリゴンの点滅
	void CreateAlertLogo(void);													// アラートロゴの生成
	void InitColor(void);														// 色の初期化
	bool GetAlertFlag(void);													// アラートのフラグ情報

private:
	int m_nSubNumber;			// カラー減算
	int m_nSubNum;				// カラーの減算量
	int m_nAlertFlame;			// 点滅のフレーム
	bool m_bAlertFlag;			// 点滅終了のフラグ
	CWarning *m_pWarning;		// アラートロゴのポインタ
};

#endif