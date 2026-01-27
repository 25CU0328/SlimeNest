#pragma once 

//(敵)がいこつクラス 

//親クラス 
#include "Enemy.h" 
//列挙型Directionを使うので#include 
#include "GameDef.h" 

class Skull : public Enemy
{
	enum Status {
		Moving, //移動中 
		Idle, //待機中 
	};

public:
	//初期化 
	void Init();
	//更新 
	void Update();

private:
	//移動状態の更新処理 
	void _updateMoving();
	//待機状態の更新処理 
	void _updateIdle();
	//加速度の更新 
	void _updateVelocity();
	//アニメーションの初期化 
	void _initAnimation();
	//やられたときに呼び出される 
	void OnDefeated();

private:
	//状態 
	Status mStatus;
	//経過時間 
	float mTimer;
	//加速度 
	Vector2f mVelocity;
	//現在の向き 
	Direction mDirection;
};