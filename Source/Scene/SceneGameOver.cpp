#include "SceneGameOver.h" 

#include "Fwk/Framework.h" 

//初期化 
void SceneGameOver::Init() {
	//”次のシーン”の初期値を”なし”にしておく 
	mNextScene = SceneType::None;
}

//終了 
void SceneGameOver::Term() {
	//なし 
}

//更新 
void SceneGameOver::Update() {
	//Aボタンを押したら”次のシーン”にTitleを設定する 
	if (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A)) {
		mNextScene = SceneType::Title;
	}
}

//描画 
void SceneGameOver::Render() {
	//文字列を描画する 
	PrintText("ゲームオーバー・・・", 350.0f, 250.0f);
}
