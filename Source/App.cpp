#include "App.h"

#include "GameObjectMng/GameObjectMng.h"		// Player関連の定義を取り込む

#include "Scene/SceneMng.h" 
#include "GamePlayData/GamePlayData.h"
#include "Mst/StageDataMst.h" 

SceneMng sceneMng;


//ゲームの初期化処理を行う関数
void App::Init() {

	//画面の左上がゲーム空間の原点(0,0)となるようにカメラを移動させる 
	{
		RenderManager_I->SetCameraPosition(WINDOW_WIDTH / 2.0f, -WINDOW_HEIGHT / 2.0f);
	}
	//UIレイヤー用のカメラの追加・設定 
	{
		RenderManager_I->AddLayer("UI");
		RenderManager_I->AddCamera("UI");
		RenderManager_I->AddCameraRenderLayer("UI", "UI");
		//UI用のカメラの設定を更新 
		RenderManager_I->SetCameraPosition("UI", WINDOW_WIDTH / 2.0f, -WINDOW_HEIGHT / 2.0f);
	}
	// ゲームプレイデータを初期化する 
	GetGamePlayData().Init();
	// ステージ情報マスタを初期化する 
	GetStageDataMst().Init();

	//シーン管理クラスを初期化。最初のシーンはタイトルシーン 
	sceneMng.Init(SceneType::Title);
}

//ゲームの更新処理を行う関数
void App::Update() {
	//シーン更新 
	sceneMng.Update();
}

//ゲームの描画処理を行う関数
void App::Render() {
	//シーン描画 
	sceneMng.Render();
}

// ゲームの後片付け処理を行う関数
void App::Term() {
	//シーン解放 
	sceneMng.Term();
}
