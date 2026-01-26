#pragma once

//----------------------------------------------
// タイルマップクラスの定義
//----------------------------------------------

// SpriteとTextureを使う為のヘッダファイル
#include<Fwk/Graphics.h>

// CSVaデータを読み込むためのライブラリ
#include <Fwk/File/CSVFile.h>

class Tilemap
{
public:
	// 初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Render();

	// 後片付け
	void Term();

	// タイルサイズを取得する
	float GetTileSize();

	Vector2f GetMapSize();

	// 指定された座標にあるタイルが壁であるかを戻す
	bool IsWall(Vector2f position);

	// 指定した矩形の内側に壁が入り込んでいるかを返す
	bool IsInsideWall(Vector2f position, float width, float height);

	// マップをCSVだたから生成する
	void CreateMap(int colNum, int rowNum, CSVData* pMapData);
private:
	// 指定された列・行が壁であるかを返す
	bool _isWall(int col, int row);
	// データを
	void _clearMapData();

	//画面一枚を描画する為に必要なタイル(スプライト)の列数・行数
	enum {
		RENDER_COL_NUM = 16 + 1,
		RENDER_ROW_NUM = 12 + 1,
	};

	// マップの列数・行数
	int mColNum;
	int mRowNum;

	// タイルパターンの画像を読み込む為のテクスチャー
	Texture mTexture;
	// 列数 * 行数分のスプライト
	Sprite mSprite[RENDER_ROW_NUM][RENDER_COL_NUM];

	// タイルの配置情報
	// クラスの定義でメンバ変数の初期値を書くこともできる
	// 課題においてはこの通りの配置情報でなくてもよい
	int** mpMapData;
};