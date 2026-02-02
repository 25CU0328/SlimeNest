#include "Tilemap.h"

//RenderMangerを使うのでinclude
#include "Fwk/Framework.h"
#include "Fwk/File/KeyValueFile.h"

//ウィンドウサイズの定数を使うのでinclude
#include "AppDef.h"
#include "GameObjectMng/GameObjectMng.h"

// 定数 タイル一枚分のスプライトサイズ
const float TileSize = 64.0f;

// 初期化
void Tilemap::Init()
{
	// テクスチャーファイルの読み込み
	mTexture.Load("Images/2dAction/tiles.png");

	mColNum = 0;
	mRowNum = 0;

	mpMapData = nullptr;
	//スプライトの初期化と各種設定
	for (int i = 0; i < RENDER_ROW_NUM; ++i) {
		for (int n = 0; n < RENDER_COL_NUM; ++n) {
			//初期化
			mSprite[i][n].Init();
			//テクスチャ設定
			mSprite[i][n].SetTexture(mTexture);
			//サイズを設定
			mSprite[i][n].SetSize(TileSize, TileSize);
			//Pivotの設定。左上をスプライトの中心点とする
			mSprite[i][n].SetPivot(Pivot::TopLeft);
		}
	}
}

// 更新
void Tilemap::Update()
{
	// 現在のカメラ位置
	Vector2f cameraPosition = RenderManager_I->GetCamera().GetPosition();

	//カメラ位置をもとに画面の左上に位置するタイルの列・行を求め
	float Left = cameraPosition.x - WINDOW_WIDTH / 2.0f;
	float Top = cameraPosition.y +WINDOW_HEIGHT / 2.0f;

	int LeftCol = (int)(Left / TileSize);
	int TopRow = (int)(-Top / TileSize);

	//タイル描画用のスプライトに、位置とUV座標を設定する
	for (int i = 0; i < RENDER_ROW_NUM; ++i)
	{
		for (int j = 0; j < RENDER_COL_NUM; ++j)
		{
			//mSprite[i][n]が表示するタイルの列・行
			int col = LeftCol + j;
			int row = TopRow + i;

			//マップデータの領域外を参照しないようにチェック
			if (col >= mColNum || row >= mRowNum) {
				//無駄な描画命令を出さないように非表示にしておく
				mSprite[i][j].SetVisible(false);
				continue;
			}

			// 位置を設定
			mSprite[i][j].SetPosition(
				(float)col * TileSize,
				(float)-row * TileSize
			);

			//マップデータからタイルIDを取得
			int id = mpMapData[row][col];

			//スプライトのUV座標設定
			float w = 16.0f / 256.0f;
			float h = 16.0f / 256.0f;
			float u = (float)(id % 16) * w;
			float v = (float)(id / 16) * h;
			mSprite[i][j].SetTexCoord(u, v, w, h);

			mSprite[i][j].SetVisible(true);
		}
	}
}


//描画
void Tilemap::Render() {
	//スプライトの描画
	for (int i = 0; i < RENDER_ROW_NUM; ++i) {
		for (int n = 0; n < RENDER_COL_NUM; ++n) {
			mSprite[i][n].Draw();
		}
	}
}


// 後片付け
void Tilemap::Term() {
	//スプライトの解放
	for (int i = 0; i < RENDER_ROW_NUM; ++i) {
		for (int n = 0; n < RENDER_COL_NUM; ++n) {
			mSprite[i][n].Term();
		}
	}

	// テクスチャーを破棄する
	mTexture.Unload();
}

// タイルサイズを取得する
float Tilemap::GetTileSize() {
	return TileSize;
}

// 指定された座標にあるタイルが壁であるかを戻す
bool Tilemap::IsWall(Vector2f position) {
	// 与えられた座標(x, y)をタイルサイズで割り端数を切り捨てて
	// マップデータの列数・行数とする. y座標→行数は符号に注意。
	int col = (int)(position.x / TileSize);
	int row = (int)(position.y / -TileSize);

	return _isWall(col, row);
}



// 指定した矩形の内側に壁が入り込んでいるかを返す
bool Tilemap::IsInsideWall(Vector2f position, float width, float height)
{
	// 矩形範囲内のすべてのタイルについて、壁であるか、矩形範囲に入り込んでいるかを判定する
	// まずは矩形の四隅の座標を作る(中心点±幅の半分、中心点+-高さの半分)
	float left = position.x - width / 2.0f;
	float top = position.y + height / 2.0f;
	float right = position.x + width / 2.0f;
	float bottom = position.y - height / 2.0f;

	// 矩形の四隅の座標をそれぞれマップデータの行・列に変換
	// これをもとに左上→右下までのタイルを一つずつ衝突判定していく
	int col_left = (int)(left / TileSize);
	int row_top = -(int)(top / TileSize);
	int col_right = (int)(right / TileSize);
	int row_bottom = -(int)(bottom / TileSize);

	for (int row = row_top; row <= row_bottom; ++row) {
		for (int col = col_left; col <= col_right; ++col)
		{
			// 壁でなければ次のタイルの判定を処理する
			if (!_isWall(col, row)) {
				continue;
			}

			// 列と行から、壁と判定したタイルの中心座標を作る
			// 列 * タイルサイズ、行 * タイルサイズでタイルの左上の座標となり、
			// そこにタイルサイズの半分 + X方向、-Y方向に足すことでタイルの中心座標(x,y)となる
			float tile_x = (float)col * TileSize + (TileSize * 0.5f);
			float tile_y = (float)row * -TileSize - (TileSize * 0.5f);

			// 壁の中心点の座標と引数で指定された矩形の中心点の座標との差の大きさを得る
			float dx = fabsf(tile_x - position.x);
			float dy = fabsf(tile_y - position.y);

			// 二つの矩形が入り込んでいるかをチェックする
			if (dx < (width + TileSize) * 0.5f &&
				dy < (height + TileSize) * 0.5f) {
				return true;
			}
		}
	}

	// チェックに一度も引っかからればければ、
	// 矩形の中に壁は入り込んでいない
	return false;
}

// マップをCSVだたから生成する
void Tilemap::CreateMap(int colNum, int rowNum, CSVData* pMapData)
{
	if (pMapData == nullptr)
	{
		return;
	}

	mColNum = colNum;
	mRowNum = rowNum;

	mpMapData = new int* [mRowNum];

	for (int i = 0; i < mRowNum; ++i)
	{
		mpMapData[i] = new int[mColNum];
	}

	for (int i = 0; i < mRowNum; ++i)
	{
		for (int j = 0; j < mColNum; ++j)
		{
			// j列i行目のタイルIDをCSVDATAから取得
			// (i, j) = (i * 列数 + j)番目のデータ
			int id = pMapData->GetInt(j + (i * mColNum));
			//マップデータのn列i行目のタイルIDを上書き
			mpMapData[i][j] = id;
		}
	}
}

// 指定された列・行が壁であるかを返す
bool Tilemap::_isWall(int col, int row)
{
	// 不正値チェック
	if (col < 0 || row < 0 || col >= mColNum || row >= mRowNum)
		return false;

	// 指定の列・行のタイルIDを取得する
	int id = mpMapData[row][col];

	// タイルID 1番を壁として判定する
	if (id == 1) {
		return true;
	}

	return false;
}

Vector2f Tilemap::GetMapSize()
{
	Vector2f result(
		mColNum * TileSize,
		mRowNum * TileSize
	);

	return result;
}


// 指定した位置のタイルの中心位置を返す関数
Vector2f Tilemap::GetTileCenterPosition(Vector2f vTargetPosition)
{
	// 指定した位置のタイルマップ座標
	Vector2i vTargetCoordinate = Vector2i((int)(vTargetPosition.x / TileSize), (int)(vTargetPosition.y / TileSize));

	// タイルマップの座標から中心位置を計算する
	return Vector2f(vTargetCoordinate.x * TileSize, vTargetCoordinate.y * TileSize);
}