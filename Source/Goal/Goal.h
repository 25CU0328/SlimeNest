#pragma once 

//SpriteやTexture,Animationを使うためのヘッダファイル 
#include "Fwk/Graphics.h" 

//Vector2fクラスを使うためのヘッダファイル 
#include "Lib/Math/Vector.h" 
using namespace Lib::Math;

//衝突形状クラスを使うのでinclude 
#include "Fwk/Collision/Collider.h" 
//衝突関連クラスの名前空間を使う 
using namespace Fwk::Collision;

//ゴール地点クラス 
class Goal
{
public:
	//初期化 
	void Init();
	//終了 
	void Term();
	//更新 
	void Update();
	//描画 
	void Render();

	//位置設定 
	void SetPosition(Vector2f position);

private:
	Texture mTexture;
	Sprite mSprite;
	Collider mCollider;
};