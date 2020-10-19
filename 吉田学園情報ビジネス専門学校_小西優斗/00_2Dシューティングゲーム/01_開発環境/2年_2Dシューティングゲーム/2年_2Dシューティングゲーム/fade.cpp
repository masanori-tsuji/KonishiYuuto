//=====================================================
//
// フェードクラス [fade.cpp]
// Author:小西優斗
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "scene.h"

//=====================================================
// static初期化処理
//=====================================================

//=====================================================
// コンストラクタ
//=====================================================
CFade::CFade()
{
	m_ColorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_fade = FADE_NONE;
	m_modeNext = CManager::MODE_TITLE;
	m_pPolygon = NULL;
}

//=====================================================
// デストラクタ
//=====================================================
CFade::~CFade()
{

}

//=====================================================
// create
//=====================================================
CFade * CFade::Create(void)
{
	// 初期化処理
	CFade *pFade = new CFade;

	//メモリが確保できていたら
	if (pFade != NULL)
	{
		//初期化処理呼び出し
		pFade->Init();
	}

	return pFade;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CFade::Init(void)
{
	m_fade = FADE_OUT;

	if (m_pPolygon == NULL)
	{
		//ポリゴンクラスを生成
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), CScene::TYPE_FADE);
		m_pPolygon->SetType(CScene::TYPE_FADE);
	}

	//色を設定
	m_pPolygon->SetColor(m_ColorFade);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFade::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		//ポリゴンクラスの終了処理呼び出し
		m_pPolygon->Uninit();

		//メモリのクリア
		m_pPolygon = NULL;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CFade::Update(void)
{
	if (m_pPolygon != NULL)
	{
		if (m_fade != FADE_NONE)
		{
			// フェード処理中
			if (m_fade == FADE_IN)
			{
				// フェードイン処理
				m_ColorFade.a -= FADE_RATE;		// a値を減算して後ろの画面を浮き上がらせる
				if (m_ColorFade.a <= 0.0f)
				{
					// フェード処理終了
					m_ColorFade.a = 0.0f;
					m_fade = FADE_NONE;
				}
			}
			else if (m_fade == FADE_OUT)
			{
				// フェードアウト処理
				m_ColorFade.a += FADE_RATE;		// a値を加算して後ろの画面を消していく
				if (m_ColorFade.a >= 1.0f)
				{
					// フェードイン処理に切り替え
					m_ColorFade.a = 1.0f;
					m_fade = FADE_IN;

					// モードを設定
					CManager::SetMode(m_modeNext);
				}
			}
			//透明度の設定
			m_pPolygon->SetColor(m_ColorFade);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CFade::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		//ポリゴンクラスの描画処理呼び出し
		m_pPolygon->Draw();
	}
}

//=====================================================
// フェードの状態設定処理
//=====================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_ColorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=====================================================
// フェード状態の情報
//=====================================================
CFade::FADE CFade::Get(void)
{
	return m_fade;
}
