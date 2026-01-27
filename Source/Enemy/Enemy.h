#pragma once 

// (敵)基底クラス 

// SpriteやTexture,Animationを使うためのヘッダファイル 
#include "Fwk/Graphics.h" 

// Vector2fクラスを使うためのヘッダファイル 
#include "Lib/Math/Vector.h" 
using namespace Lib::Math;

// 衝突形状クラスを使うのでinclude 
#include "Fwk/Collision/Collider.h" 
// 衝突関連クラスの名前空間を使う 
using namespace Fwk::Collision;

// 音源クラスを#include
#include "Fwk/Audio/SoundSource.h" 
// Waveファイルを扱うクラスを#include 
#include "Fwk/Audio/WaveSound.h" 

// エネミー状態を使うためにinclude
#include "GameDef.h"

class Enemy
{
public:
	// 初期化 
	virtual void Init();
	// 後片付け 
	virtual void Term();
	// 更新 
	virtual void Update();
	// 描画 
	virtual void Render();

	// アクティブか？ 
	bool IsActive();
	// アクティブ状態の設定 
	void SetActive(bool isActive);

	// 現在位置を取得 
	Vector2f GetPosition();
	// 現在位置を設定 
	void SetPosition(Vector2f position);

	// ダメージを受ける 
	virtual void OnDamaged(int damage);

	// 生成された時に呼び出す 
	// 派生クラスで上書きして使う 
	virtual void OnCreated();
	
	// やられたときに呼びされる 
	// 派生クラスで上書きして使う 
	virtual void OnDefeated();

	// パトロール状態の更新処理
	virtual void UpdatePatrol();

	// 攻撃状態の更新処理
	virtual void UpdateAttack();

	// ファロー状態の更新処理
	virtual void UpdateFollow();

	// 警戒状態の更新処理
	virtual void UpdateAlert();
protected:
	// プレイヤーが視線に入った時と離れた時の処理
	void _onPlayerFound(const CollisionEvent& collisionEvent);

#pragma region Variables
	// アクティブ状態か否か 
	bool mIsActive;
	// 描画用テクスチャ 
	Texture mTexture;
	// 描画用スプライト 
	Sprite mSprite;
	// 現在の位置 
	Vector2f mPosition;
	// 自分の体の衝突形状 
	Collider mCollider;
	
	// 視線を表す衝突形状
	Collider mViewCollider;
	// 敵の状態を表す変数
	EnemyState mState;
	// エネミーの向きを表す変数
	Direction mDirection;

	// 移動速度の最大値
	float mMaxVelocity;
	// 移動速度
	float mVelocity;

	// ファロー状態の継続時間
	float mFollowTime;

	// 効果音 
	WaveSound mSound;
	// 音源 
	SoundSource mSoundSource;
#pragma endregion
};