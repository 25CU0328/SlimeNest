#include "SceneGameClear.h" 

#include "Fwk/Framework.h" 

// 初期化 
void SceneGameClear::Init() {
	Scene::Init();
	// ” 次のシーン”の初期値を”なし”にしておく 
	mNextScene = SceneType::None;
}

// 終了 
void SceneGameClear::Term() {
	// なし 
}

// 更新 
void SceneGameClear::Update() {
	// Aボタンを押したら”次のシーン”にTitleを設定する 
	if (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A)) {
		mNextScene = SceneType::Title;
	}
}

// 描画 
void SceneGameClear::Render() {
	// 文字列を描画する 
	PrintText("ゲームクリアー！", 380.0f, 250.0f);
}
