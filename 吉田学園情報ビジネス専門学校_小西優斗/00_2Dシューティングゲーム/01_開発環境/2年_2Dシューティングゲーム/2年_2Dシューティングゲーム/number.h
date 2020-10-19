#ifndef _NUMBER_H_
#define _NUMBER_H_
//=============================================================================
//
// ナンバーヘッダー [number.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_NUMBER_TEXTURE (2)		// テクスチャの最大数

//=============================================================================
// レンダラークラス
//=============================================================================
class CNumber : public CScene
{
public:
	//=============================================================================
	// 構造体定義
	//=============================================================================
	typedef struct
	{
		D3DXVECTOR3 pos;	// 頂点座標
		float rhw;			// 座標
		D3DCOLOR col;		// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
	}VERTEX_2D;

	typedef enum
	{
		NUMBER_TYPE_NONE = 0,	// 初期値
		NUMBER_TYPE_SCORE,		// スコア
		NUMBER_TYPE_RANKING,	// ランキング
		NUMBER_TYPE_TIME,		// タイム
		NUMBER_TYPE_MAX
	}NUMBER_TYPE;

	CNumber();				// コンストラクタ
	~CNumber();				// デストラクタ

	// メンバ関数
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);					// 初期化処理
	void Uninit(void);															// 終了処理
	void Update(void);															// 更新処理
	void Draw(void);															// 描画処理
	static HRESULT Load(void);													// テクスチャロード
	static void UnLoad(void);													// テクスチャアンロード
	static CNumber * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, 
		TYPE type, NUMBER_TYPE Ntype);											// クリエイト
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);								// テクスチャの設定
	void SetNumber(int nScore);													// ナンバーの設定
	void NumberInit(D3DXVECTOR3 pos, D3DXVECTOR3 size);							// ナンバー初期化設定
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);									// バッファ情報
	void SetNType(NUMBER_TYPE Ntype);											// ナンバーのタイプ設定

private:
	D3DXVECTOR3 m_Pos;											// 座標
	D3DXVECTOR3 m_Size;											// サイズ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// バッファ
	D3DXVECTOR3 m_Move;											// 移動
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_NUMBER_TEXTURE];	// テクスチャのポインタ
	static LPDIRECT3DDEVICE9 m_pDevice;							// デバイスのポインタ
	NUMBER_TYPE m_Ntype;										// ナンバーのタイプ
};

#endif