#pragma once

// PlayerStateを継承する為にInclude
#include "PlayerState.h"

// Vector2fを使うためのヘッダファイル
#include "Lib/Math/Vector.h"
using namespace Lib::Math;
#include "GameDef.h"
// 移動する時の状態
class StateMoving : public PlayerState
{
	// 移動の種類
	enum class MoveType
	{
		Air, 
		Ground,
		Wall,
		Ceiling,
	};
public:
	void Init();
	void OnStateUpdate();

private:
	
	void _translate(Vector2f vMove);
	Vector2f _getVectorByDirection(Direction direction);
	void _updateInAir(Vector2f vInput);
	// 地上にいる時の処理
	void _updateGround(Vector2f vInput);

	// 壁に貼り付けている時の処理
	void _updateWall(Vector2f vInput);

	// 天井に貼り付けている時の処理
	void _updateCeiling(Vector2f vInput);

	bool _isSwitchTypeSpeed(float fSpeed);

	// 今は地面にいるかを返す関数
	bool _isCeil(Vector2f vVelocity = Vector2f(0.0f, 20.0f));
	// 今は壁の隣にいるかを返す関数
	bool _isNextToWall(Vector2f vVelocity = Vector2f(0.0f, 0.0f));
	// 今は天井の下にいるかを返す関数
	bool _isGround(Vector2f vVelocity = Vector2f(0.0f, -20.0f));

	MoveType mMoveType;

#pragma region Constant
	// 別の壁に貼り付けるかどうかを判断する基準
	const float mMinStickVelocity = 2.0f;
	const float mMaxStickVelocity = 20.0f;

	// 空中移動に関数定数
	const float mAirSpeedCoefficient = 25.0f;
	const float mAirMaxSpeed = 600.0f;
	
	// 天井移動に関数定数
	const float mCeilingSpeedCoefficient = 20.0f;
	const float mCeilingMaxSpeed = 600.0f;

	// 壁移動に関数定数
	const float mWallSpeedCoefficient = 20.0f;
	const float mWallMaxSpeed = 600.0f;

	// 地上移動に関数定数
	const float mGroundSpeedCoefficient = 25.0f;
	const float mGroundMaxSpeed = 600.0f;

#pragma endregion
};
