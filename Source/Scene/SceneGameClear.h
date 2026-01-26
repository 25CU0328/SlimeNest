#pragma once 

// シーン種別の列挙型を使うので#include 
#include "GameDef.h" 

// Sceneクラスを継承するので#include 
#include "Scene.h"


// ゲームクリアシーンクラス 
class SceneGameClear : public Scene
{
public:
	// 初期化 
	void Init();
	// 終了 
	void Term();
	// 更新 
	void Update();
	// 描画 
	void Render();
};