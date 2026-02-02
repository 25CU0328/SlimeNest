

#include "StateCharging.h"
#include "GameObjectMng/GameObjectMng.h"

#include "Fwk/Framework.h"
void StateCharging::OnStateEnter()
{
	// 貯めた力を初期化する
	mChargeForce = 0;

	// 力を出す方向
	mReleaseDirection = Vector2f(0.0f, 0.0f);
}

void StateCharging::OnStateUpdate()
{
	if (!(Input_I->IsButtonPressed(0, GAMEPAD_BUTTON::B) && !Input_I->IsKeyPressed('Z')))
	{
		if (mChargeForce >= mMinChargeForce)
		{
			Player* pPlayer = GetPlayer();
			Vector2f originVelocity = pPlayer->GetVelocity();
			pPlayer->SetVelocity(mReleaseDirection* mChargeForce);
		}
		mNextState = StateType::Moving;
		return;
	}
	// 力を溜める
	mChargeForce += Time_I->GetDeltaTime() * mChargeCoefficent;
	mChargeForce = mChargeForce > mMaxChargeForce ? mMaxChargeForce : mChargeForce;
	DebugLog(to_string(mChargeForce));
	Direction wallDirection = Direction::None;
	if (_isGround())
	{
		_updateGround();
	}
	else if(_isClimbingWall(&wallDirection))
	{
		_updateWall(wallDirection);
	}
	else if (_isClimbingCeil())
	{
		_updateCeiling();
	}

	

	
}

// 今は地上にいるかを返す関数
bool StateCharging::_isGround()
{
	Vector2f vPlayerPos = GetPlayer()->GetPosition();

	//衝突判定用の矩形の幅と高さ 
	float fCollisionWidth = 40.0f;
	float fCollisionHeight = 1.0f;

	bool bIsWallDown = GetMap()->IsInsideWall(vPlayerPos + Vector2f(0.0f, -20.0f), fCollisionWidth, fCollisionHeight);

	return bIsWallDown;
}

// 今は壁に貼り付いているかを返す関数
bool StateCharging::_isClimbingCeil()
{
	Vector2f vPlayerPos = GetPlayer()->GetPosition();

	//衝突判定用の矩形の幅と高さ 
	float fCollisionWidth = 40.0f;
	float fCollisionHeight = 1.0f;

	bool bIsWallDown = GetMap()->IsInsideWall(vPlayerPos + Vector2f(0.0f, 20.0f), fCollisionWidth, fCollisionHeight);

	return bIsWallDown;
}

// 今は天井に貼り付いているかを返す関数
bool StateCharging::_isClimbingWall(Direction* wallDirection)
{
	// プレイヤーの位置
	Vector2f vPlayerPos = GetPlayer()->GetPosition();

	//衝突判定用の矩形の幅と高さ 
	float fCollisionWidth = 40.0f;
	float fCollisionHeight = 1.0f;

	// 壁の右隣にいるか
	bool bIsWallLeftSide = GetMap()->IsInsideWall(vPlayerPos + Vector2f(-20.0f, 0.0f), fCollisionWidth, fCollisionWidth);
	// 壁の右隣にいるか
	bool bIsWallRightSide = GetMap()->IsInsideWall(vPlayerPos + Vector2f(20.0f, 0.0f), fCollisionWidth, fCollisionWidth);

	// 壁との相対位置を代入する
	if (bIsWallLeftSide)
	{
		*wallDirection = Direction::Left;
	}
	else if (bIsWallRightSide)
	{
		*wallDirection = Direction::Right;
	}
	


	return bIsWallLeftSide || bIsWallRightSide;
}


// 地上にいる時の処理
void StateCharging::_updateGround()
{
	//ゲームパッドの入力を取得 
	Vector2f vInput = { 0.0f, 0.0f };
	vInput = Input_I->GetStickInput(0, GAMEPAD_STICK::LEFT);

	// ゲームパットの入力がない場合
	// キーボードの入力を取得
	if (vInput.GetLength() == 0.0f)
	{
		if (Input_I->IsKeyPressed(VK_RIGHT))
		{
			vInput.x += 1.0f;
		}
		if (Input_I->IsKeyPressed(VK_LEFT))
		{
			vInput.x -= 1.0f;
		}
		if (Input_I->IsKeyPressed(VK_UP))
		{
			vInput.y += 1.0f;
		}

		if (vInput.x != 0 && vInput.y != 0)
		{
			vInput.x /= 1.414f;
			vInput.y /= 1.414f;
		}
	}

	mReleaseDirection = vInput;
}

// 壁に貼り付いている時の処理
void StateCharging::_updateWall(Direction wallDirection)
{
	//ゲームパッドの入力を取得 
	Vector2f vInput = { 0.0f, 0.0f };
	vInput = Input_I->GetStickInput(0, GAMEPAD_STICK::LEFT);

	// ゲームパットの入力がない場合
	// キーボードの入力を取得
	// 壁は左にある場合(右側にジャンプ)
	if (wallDirection == Direction::Left)
	{
		if (vInput.GetLength() == 0.0f)
		{
			if (Input_I->IsKeyPressed(VK_RIGHT))
			{
				vInput.x += 1.0f;
			}
			if (Input_I->IsKeyPressed(VK_UP))
			{
				vInput.y += 1.0f;
			}
			if (Input_I->IsKeyPressed(VK_DOWN))
			{
				vInput.y -= 1.0f;
			}
			if (vInput.x != 0 && vInput.y != 0)
			{
				vInput.x /= 1.414f;
				vInput.y /= 1.414f;
			}
		}
	}
	// 壁は右にある場合(左側にジャンプ)
	else if (wallDirection == Direction::Right)
	{
		if (vInput.GetLength() == 0.0f)
		{
			if (Input_I->IsKeyPressed(VK_LEFT))
			{
				vInput.x -= 1.0f;
			}
			if (Input_I->IsKeyPressed(VK_UP))
			{
				vInput.y += 1.0f;
			}
			if (Input_I->IsKeyPressed(VK_DOWN))
			{
				vInput.y -= 1.0f;
			}
			if (vInput.x != 0 && vInput.y != 0)
			{
				vInput.x /= 1.414f;
				vInput.y /= 1.414f;
			}
		}
	}
	
	mReleaseDirection = vInput;
}

// 天井に貼り付いている時の処理
void StateCharging::_updateCeiling()
{
	//ゲームパッドの入力を取得 
	Vector2f vInput = { 0.0f, 0.0f };
	vInput = Input_I->GetStickInput(0, GAMEPAD_STICK::LEFT);

	// ゲームパットの入力がない場合
	// キーボードの入力を取得
	if (vInput.GetLength() == 0.0f)
	{
		if (Input_I->IsKeyPressed(VK_RIGHT))
		{
			vInput.x += 1.0f;
		}
		if (Input_I->IsKeyPressed(VK_LEFT))
		{
			vInput.x -= 1.0f;
		}
		if (Input_I->IsKeyPressed(VK_DOWN))
		{
			vInput.y -= 1.0f;
		}
		if (vInput.x != 0 && vInput.y != 0)
		{
			vInput.x /= 1.414f;
			vInput.y /= 1.414f;
		}
	}

	mReleaseDirection = vInput;
}
