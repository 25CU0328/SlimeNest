#pragma once
#include "PlayerState.h"
#include "GameDef.h"
// ------------------
// 力を溜めている状態
// ------------------
class StateCharging :public PlayerState
{
public:
	// 力溜め状態に切り替えた時の処理
	void OnStateEnter();

	// 力溜め状態の更新処理
	void OnStateUpdate();

	
private:
#pragma region Function
	// 今は地上にいるかを返す関数
	bool _isGround();
	// 今は壁に貼り付いているかを返す関数
	bool _isClimbingWall(Direction* wallDirection);

	// 今は天井に貼り付いているかを返す関数
	bool _isClimbingCeil();

	// 地上にいる時の処理
	void _updateGround();

	// 壁に貼り付いている時の処理
	void _updateWall(Direction wallDirection);

	// 天井に貼り付いている時の処理
	void _updateCeiling();
#pragma endregion
	// 貯めた力
	float mChargeForce;
	// 力の出す方向
	Vector2f mReleaseDirection;

	const float mChargeCoefficent = 50.0f;
	// 発車
	const float mMinChargeForce = 2.0f;
	// 力の最大値
	const float mMaxChargeForce = 20.0f;

};
