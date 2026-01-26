#pragma once 

// (敵)鬼火クラス 

// SpriteやTexture,Animationを使うためのヘッダファイル 
#include "Fwk/Graphics.h" 

// Vector2fクラスを使うためのヘッダファイル 
#include "Lib/Math/Vector.h" 
using namespace Lib::Math;

// プレイヤーの位置を参照するのでinclude 
#include "GameObjectMng/GameObjectMng.h"

// 衝突形状クラスを使うのでinclude 
#include "Fwk/Collision/Collider.h" 
// 衝突関連クラスの名前空間を使う 
using namespace Fwk::Collision;

// 親クラス 
#include "Enemy.h"

class Onibi : public Enemy
{
public:
	// 初期化 
	void Init();
	// 更新 
	void Update();

	// 生成された時に呼び出される
	void OnCreated();

private:
	// アニメーションの初期化 
	void _initAnimation();
};