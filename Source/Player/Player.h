#pragma once

//---------------------------------------------------------
// プレイヤークラスの定義
//---------------------------------------------------------

// This Comment Is Add By School PC

//SpriteやTexture, Animationを使うためのヘッダファイル
#include "Fwk/Graphics.h"

// Vector2fを使うためのヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

// 向きの定義を使う為のでinclude
#include "GameDef.h"

// Swordクラスを使うので include
#include "Weapon/Sword.h"

//音源クラスのを#include 
#include "Fwk/Audio/SoundSource.h"
//Waveファイルを扱うクラスを#include 
#include "Fwk/Audio/WaveSound.h" 

#include "State/StateMng.h"

// プレイヤークラス
class Player {
	//プレイヤーの状態 
	enum class Status {
		Moving, //通常移動中 
		Attack, //攻撃中 
		Chanting, // 詠唱中
		Damaged,//被ダメージ中 
		Dying, //やられ中 
		Dead, //やられた 
		Goal, //ゴール地点に到達した 
	};

	//使う効果音の種類 
	enum class Sound {
		Move,
		Jump_Start,
		Jump_End,
		Jump,
		Damaged,

		Count
	};
public:
	// プレイヤーの初期化
	void Init();

	// プレイヤーの更新
	void Update();

	// プレイヤーの描画
	void Render();

	// プレイヤーの後片付け
	void Term();

	//現在位置を取得する 
	Vector2f GetPosition();

	// 新しい位置に移動させる
	void SetPosition(Vector2f newPosition);

	//現在の体力を取得する 
	int GetHp();
	float GetNormalizedHp();

	//自分がやられ状態でなければtrueを返す 
	bool IsAlive();
	//自分がやられ状態であればtrueを返す 
	bool IsDead();

	//ゴール地点に到達したか否かを取得する 
	bool IsReachedGoal();

	// 移動速度を取得する
	Vector2f GetVelocity();
	// 移動速度を引数の値通りに設定する
	void SetVelocity(Vector2f newVelocity);

	// プレイヤーの向きを取得する
	Direction GetDirection() { return mDirection; }
	// プレイヤーの向きを設定する
	void SetDirection(Direction newDirection) { mDirection = newDirection; }

	// スプライトを取得
	Sprite* GetSprite();

	// サウンドをプレイする
	void PlaySfx(Sound sound);

private:

#pragma region Functions
	// アニメーションの初期化
	void _initAnimation();

	// 衝突する時のコールバック関数
	void _onHit(const CollisionEvent& collisionEvent);

	//自分をやられ状態にする 
	void _die();


#pragma endregion

#pragma region Variables
	// 表示用スプライト
	Sprite mSprite;
	
	// テクスチャ
	Texture mTexture;
	// オブジェクトの位置
	Vector2f mPosition;

	// 現在の向き
	Direction mDirection;

	// 衝突処理用
	Collider mCollider;

	// 汎用タイマ
	float mTimer = 0.0f;
	// 汎用ステップカウンタ
	int mStep = 0;
	// プレイヤーの状態
	Status mStatus;
	StateMng mStateMng;

	// 無敵中の残りフレーム
	int mInvicibilityFrames;

	// 速度
	Vector2f mVelocity;

	// 体力
	int mHp;
	// 体力(最大値)
	int mHpMax;

	// 魔力量
	int mMp;
	// 総魔力量
	int mMpMax;

	//効果音 
	WaveSound mSounds[(int)Sound::Count];
	//音源 
	SoundSource mSoundSources[(int)Sound::Count];
#pragma endregion
};

