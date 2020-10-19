//=============================================================================
//
// ポリゴンの設定 [scene.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene.h"
#include "renderer.h"
#include "scene2d.h"

//=============================================================================
// static初期化
//=============================================================================
CScene *CScene::m_apScene[MAX_OBJECT] = {};
int CScene::m_nNumAll = 0;
int CScene::m_nNumEnemy = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene()
{
	// メンバ変数初期化
	m_nID = 0;
	m_type = TYPE_NONE;

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		// それぞれNULLのとき
		if (m_apScene[nCntScene] == NULL)
		{
			// Scene2Dの情報
			m_apScene[nCntScene] = this;
			m_nID = nCntScene;
			// 一体加算
			m_nNumAll++;
			break;
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		// それぞれNULLのとき
		if (m_apScene[nCntScene] != NULL)
		{
			// 更新処理
			m_apScene[nCntScene]->Update();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	TYPE type;

	// 優先順位が上のTYPEから描画
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			// 使われていた時
			if (m_apScene[nCntScene] != NULL)
			{
				// その番号のタイプを代入
				type = m_apScene[nCntScene]->GetType();

				// タイプが一致したら
				if (type == nCntType)
				{
					// 描画処理
					m_apScene[nCntScene]->Draw();
				}
			}
		}
	}
}

//=============================================================================
// 全てのリリース
//=============================================================================
void CScene::ReleaseAll(void)
{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			// それぞれNULLのとき
			if (m_apScene[nCntScene] != NULL)
			{
				TYPE type = m_apScene[nCntScene]->GetType();

				if (type != TYPE_FADE)
				{
					// 終了処理
					m_apScene[nCntScene]->Uninit();
				}
			}
		}

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			// それぞれNULLのとき
			if (m_apScene[nCntScene] != NULL)
			{
				TYPE type = m_apScene[nCntScene]->GetType();

				if (type != TYPE_FADE)
				{
					// メモリ開放
		//			delete[] * m_apScene;
					m_apScene[nCntScene] = NULL;
				}
			}
		}

	// エネミーの総数をリセット
	m_nNumEnemy = 0;
}

//=============================================================================
// シーン情報を返す
//=============================================================================
CScene * CScene::GetScene(int nCount)
{
	return m_apScene[nCount];
}

//=============================================================================
// 単数リリース
//=============================================================================
void CScene::Release()
{
	if (m_apScene[m_nID] != NULL)
	{
		TYPE type;
		int nID = m_nID;

		// その番号のタイプを代入
		type = m_apScene[m_nID]->GetType();

		// 現在の番号のメモリを開放
		delete m_apScene[nID];
		m_apScene[nID] = NULL;

		// 総数を減算
		m_nNumAll--;

		// エネミーだったら減算
		if (type == TYPE_ENEMY)
		{
			m_nNumEnemy--;
		}
	}
}

//=============================================================================
// エネミーのカウント
//=============================================================================
void CScene::CountEnemy()
{
	m_nNumEnemy++;
}

//=============================================================================
// タイプ設定
//=============================================================================
void CScene::SetType(TYPE type)
{
	m_type = type;
}

//=============================================================================
// タイプ情報
//=============================================================================
CScene::TYPE CScene::GetType(void)
{
	return m_type;
}

//=============================================================================
// エネミー数の情報
//=============================================================================
int CScene::GetEnemyNum(void)
{
	return m_nNumEnemy;
}