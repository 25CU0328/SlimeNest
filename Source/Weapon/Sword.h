#pragma once

//SpriteやTexture, Animationを使うためのヘッダファイル
#include "Fwk/Graphics.h"

// Vector2fを使うためのヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

// 向きの定義を使う為のでinclude
#include "GameDef.h"
// Colliderを使うのでinclude
#include "Fwk/Collision/Collider.h"
using namespace Fwk::Collision;

//音源クラスを#includew 
#include "Fwk/Audio/SoundSource.h" 
//Waveファイルを扱うクラスを#include 
#include "Fwk/Audio/WaveSound.h" 


// 武器：剣クラス
class Sword
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

	// 位置設定
	void SetPosition(Vector2f position);

	// 指定方向に剣を振る
	// 振り終わった後、コールバック関数を呼び出す
	void Swing(Direction direction);

	// 剣が振られているか
	bool IsSwinged();

private:
	// アニメーションの初期化
	void _initAnimation();

	// アニメーションイベントのコールバック関数
	void OnAnimationEvent(const AnimationEvent& animationEvent);

	// 衝突イベントのコールバック関数
	void OnHit(const CollisionEvent& collisionEvent);

private:
	Texture mTexture;
	Sprite mSprite;

	Direction mDirection;
	Collider mCollider;

	//効果音 
	WaveSound mSound;
	//音源 
	SoundSource mSoundSource;
};