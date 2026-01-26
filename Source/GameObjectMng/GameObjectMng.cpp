#include "GameObjectMng/GameObjectMng.h"


//RenderMangerを使うのでinclude
#include "Fwk/Framework.h"

#include "Player/Player.h"

// ファイルの読み込み
#include "Fwk/File/KeyValueFile.h"
#include "Fwk/File/CSVFile.h"

#include "AppDef.h"

// UIMngを使うのでinclude
#include "UI/UIMng.h"

Tilemap mMap;
Player mPlayer;
EnemyMng mEnemyMng;
UIMng mUIMng;


// 全オブジェクト初期化
void GameObjectInit(StageData* pStageData)
{
	mMap.Init();
	mPlayer.Init();
	mEnemyMng.Init();
	mUIMng.Init();

	// 敵のオブジェクトプールの確保
	mEnemyMng.GeneratePool("Slime", 10);
	mEnemyMng.GeneratePool("Onibi", 10);
	mEnemyMng.GeneratePool("Ghost", 10);
	mEnemyMng.GeneratePool("Skull", 10);
	// 敵の配置情報をファイルから取得する
	if(pStageData){
		CSVFile csvFile;
		csvFile.Load(pStageData->ArrangementFile);
		mEnemyMng.CreateEnemies(csvFile.GetCSVData());
	}

	// マップデータをファイルから取得する
	if(pStageData){
		KeyValueFile mapDataFile;
		mapDataFile.Load(pStageData->MapFile);
		int iMapColNum = mapDataFile.GetInt("Col");
		int iMapRowNum = mapDataFile.GetInt("Row");

		mMap.CreateMap(iMapColNum, iMapRowNum, mapDataFile.GetCSVData("Data"));


		CSVData* pCSV = mapDataFile.GetCSVData("Goal");
		if (pCSV != nullptr)
		{
			float x = pCSV->GetFloat(0);
			float y = pCSV->GetFloat(1);
		}
	}
}

// カメラ位置の更新
void _updateCamera()
{
	const Vector2f MapSize = GetMap()->GetMapSize();

	const float HALF_WINDOW_WIDTH = WINDOW_WIDTH / 2.0f;
	const float HALF_WINDOW_HEIGHT = WINDOW_HEIGHT / 2.0f;

	Vector2f cameraPosition = mPlayer.GetPosition();

	//カメラ位置がカメラの移動範囲内に収まる様に調整
	if (cameraPosition.x < HALF_WINDOW_WIDTH) {
		cameraPosition.x = HALF_WINDOW_WIDTH;
	}
	if (cameraPosition.y > -HALF_WINDOW_HEIGHT) {
		cameraPosition.y = -HALF_WINDOW_HEIGHT;
	}
	if (cameraPosition.x > MapSize.x - HALF_WINDOW_WIDTH) {
		cameraPosition.x = MapSize.x - HALF_WINDOW_WIDTH;
	}
	if (cameraPosition.y < -MapSize.y + HALF_WINDOW_HEIGHT) {
		cameraPosition.y = -MapSize.y + HALF_WINDOW_HEIGHT;
	}
	RenderManager_I->SetCameraPosition(cameraPosition);
}

// 全オブジェクト更新
void GameObjectUpdate()
{
	mMap.Update();

	mPlayer.Update();

	mEnemyMng.Update();

	mUIMng.Update();
	//プレイヤーの体力をUIマネージャに渡す 
	mUIMng.SetHp(mPlayer.GetHp());
	mUIMng.SetHp(mPlayer.GetNormalizedHp());

	//プレイヤーがやられていなければカメラを追従させる 
	if (mPlayer.IsAlive())
	{
		_updateCamera();
	}

}


// 全オブジェクト描画
void GameObjectRender()
{
	mMap.Render();
	mPlayer.Render();
	mEnemyMng.Render();
	mUIMng.Render();

}


// 全オブジェクト解放
void GameObjectTerm()
{
	mMap.Term();
	mPlayer.Term();
	mEnemyMng.Term();
	mUIMng.Term();

}

// Tilemapクラスの変数のアドレスを返す
Tilemap* GetMap()
{
	return &mMap;
}

// プレイヤークラスのポインタをかえす
Player* GetPlayer()
{
	return &mPlayer;
}

// 敵マネージャクラスのポインタを返す 
EnemyMng* GetEnemyMng() {
	return &mEnemyMng;
}
