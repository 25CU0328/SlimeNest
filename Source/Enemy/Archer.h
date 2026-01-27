#pragma once

// エネミークラスを継承するためにinclude
#include "Enemy.h"

// Directionを使うためにinclude
#include "GameDef.h"

// アーチャー(エネミー)のクラス
class Archer : Enemy
{
	// アーチャーの初期化
	void Init();
	// アーチャーの更新処理
	void Update();

	// アーチャーの攻撃状態の更新処理
	void UpdateAttack();

	// アーチャーのファロー状態の更新処理
	void UpdateFollow();
	void UpdatePatrol();
private:
	bool _isNextToWall();

	// 登れる高さ(タイル数)
	float mClimbableHeight;
};
