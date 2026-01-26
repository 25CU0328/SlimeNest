#pragma once

// PlayerStateを継承する為にInclude
#include "PlayerState.h"

// ダメージを受けた時の状態
class StateDamaged : public PlayerState
{
	// ダメージ状態に入った時の処理
	void OnStateEnter();
	// ダメージ状態の更新処理
	void OnStateUpdate();
	
private:
	// 状態の残り時間を表す変数
	float mDuration;
};