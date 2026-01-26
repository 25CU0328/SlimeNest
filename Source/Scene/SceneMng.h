#pragma once 

#include "GameDef.h" 

//各シーンクラスのヘッダファイルを#include 
#include "SceneTitle.h" 
#include "SceneInGame.h"
#include "SceneGameOver.h" 
#include "SceneGameClear.h" 

//シーン管理クラス 
class SceneMng
{
public:
	//初期化(最初に動かすシーンを指定できるようにしておく) 
	void Init(SceneType startScene);
	//終了 
	void Term();
	//更新 
	void Update();
	//描画 
	void Render();

private:
	//新しいシーンを開始する 
	void _beginScene(SceneType newScene);
	//現在のシーンを終了する 
	void _endCurrentScene();

private:
	//現在のシーン 
	SceneType mCurrentScene;
	//次のシーン 
	SceneType mNextScene;
	//現在動いているシーンクラスの変数のポインタ 
	Scene* mpScene;
};