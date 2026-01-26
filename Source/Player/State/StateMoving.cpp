
#include "StateMoving.h"
// Input_Iを使うためのヘッダファイル
#include "Fwk/Framework.h"
#include "GameObjectMng/GameObjectMng.h"

void StateMoving::Init()
{
	PlayerState::Init();
	mMoveType = MoveType::Air;
	GetPlayer()->GetSprite()->SetRotationZ(0.0f);
}

void StateMoving::OnStateUpdate()
{
	PlayerState::OnStateUpdate();
	switch (mMoveType)
	{
	case MoveType::Air:
		DebugLog("In Air");
		break;
	case MoveType::Ceiling:
		DebugLog("Ceiling");
		break;
	case MoveType::Ground:
		DebugLog("Ground");
		break;
	case MoveType::Wall:
		DebugLog("Wall");
		break;
	}
	
	//ゲームパッドの入力を取得 
	Vector2f vInput = { 0.0f, 0.0f };
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
		if (Input_I->IsKeyPressed(VK_DOWN))
		{
			vInput.y -= 1.0f;
		}
	}
	
	//ゲームパッドのBボタンが押されたら＋Y方向に加速する（ジャンプ） 
	if ((Input_I->IsButtonDown(0, GAMEPAD_BUTTON::B) || Input_I->IsKeyDown('Z'))) {
		if (mMoveType != MoveType::Air)
		{
			mNextState = StateType::Charge;
		}
	}
	if (mMoveType == MoveType::Ground)
	{
		_updateGround(vInput);
		return;
	}
	else if (mMoveType == MoveType::Wall)
	{
		_updateWall(vInput);
		return;
	}
	else if (mMoveType == MoveType::Ceiling)
	{
		_updateCeiling(vInput);
		return;
	}
	else
	{
		_updateInAir(vInput);
		return;
	}
}

//速度を更新する 
void StateMoving::_translate(Vector2f vMove)
{
	if (vMove.GetLength() == 0.0f)
		return;
	// タイルマップからタイルサイズを取得
	const float TileSize = GetMap()->GetTileSize();
	// このフレームの移動量
	// プレイヤーんｐタイルとの衝突幅・高さ
	float CollisionSize = 40.0f;
	// コリジョンサイズの半分をよく使うので変数にしておく
	float CollisionSizeHalf = CollisionSize / 2.0f;
	Player* pPlayer = GetPlayer();
	// X軸方向の移動処理
	if (vMove.x != 0.0f) {
		// 現在位置に移動ベクトルの x 成分を足して移動先の位置とする
		Vector2f vTargetPos = pPlayer->GetPosition() + Vector2f(vMove.x, 0.0f);

		// 壁との衝突チェック
		if (!GetMap()->IsInsideWall(vTargetPos, CollisionSize, CollisionSize)) {
			pPlayer->SetPosition(vTargetPos);
		}
		else
		{
			// 右向きに移動しようとしていた場合
			if (vMove.x > 0.0f)
			{
				// 衝突したタイルの列数を算出
				int col = (int)((vTargetPos.x + CollisionSizeHalf) / TileSize);
				// 衝突したタイルの左端の x 座標を作る
				float wall_left = (float)col * TileSize;
				// 衝突したタイルの左端と接するようにプレイヤーの位置を調整する
				Vector2f playerPos = pPlayer->GetPosition();
				playerPos.x = wall_left - CollisionSizeHalf;

				pPlayer->SetPosition(playerPos);
			}
			// 左向きに移動しようとしていた場合
			if (vMove.x < 0.0f)
			{
				// 衝突したタイルの列数を算出
				int col = (int)((vTargetPos.x - CollisionSizeHalf) / TileSize);
				// 衝突したタイルの左端の x 座標を作る
				float wall_right = (float)col * TileSize + TileSize;
				// 衝突したタイルの左端と接するようにプレイヤーの位置を調整する
				// 衝突したタイルの左端と接するようにプレイヤーの位置を調整する
				Vector2f playerPos = pPlayer->GetPosition();
				playerPos.x = wall_right + CollisionSizeHalf;

				pPlayer->SetPosition(playerPos);
			}
		}
	}
	// Ｙ軸方向の処理
	if (vMove.y != 0.0f) {
		// 現在位置に移動ベクトルのy成分を足して移動先の位置とする
		Vector2f vTargetPos = pPlayer->GetPosition() + Vector2f(0.0f, vMove.y);

		// 壁との衝突チェック
		if (!GetMap()->IsInsideWall(vTargetPos, CollisionSize, CollisionSize)) {
			pPlayer->SetPosition(vTargetPos);
		}
		else {
			// 上向きに移動しようとしていた場合
			if (vMove.y > 0.0f) {
				// 衝突した行数を算出
				int row = (int)((vTargetPos.y + CollisionSizeHalf) / TileSize);
				// 衝突したタイルの下端と接するようにプレイヤーの位置を調整する
				float wall_bottom = (float)row * TileSize - TileSize;

				Vector2f playerPos = pPlayer->GetPosition();
				playerPos.y = wall_bottom - CollisionSizeHalf;

				pPlayer->SetPosition(playerPos);
			}
			// 下向きに移動しようとしていた場合
			if (vMove.y < 0.0f) {
				// 衝突した行数を算出
				int row = (int)((vTargetPos.y - CollisionSizeHalf) / TileSize);
				// 衝突したタイルの上端と接するようにプレイヤーの位置を調整する
				float wall_top = (float)row * TileSize;

				Vector2f playerPos = pPlayer->GetPosition();
				playerPos.y = wall_top + CollisionSizeHalf;

				pPlayer->SetPosition(playerPos);
			}
		}
	}
}

void StateMoving::_updateInAir(Vector2f vInput)
{
	//このフレームでの加速量 
	float acceleration = mAirSpeedCoefficient * Time_I->GetDeltaTime();
	//このフレームでの最大移動量 
	float maxSpeed = mAirMaxSpeed * Time_I->GetDeltaTime();

	//重力の倍率 
	float gravityScale = 5.0f;
	//このフレームでの重力加速量。 
	float gravity = -9.8f * Time_I->GetDeltaTime() *gravityScale;
	//このフレームでの最大重力。 
	float maxGravity = -1200.0f * Time_I->GetDeltaTime();

	Player* pPlayer = GetPlayer();
	Vector2f vVelocity = pPlayer->GetVelocity();

	//摩擦による速度の減衰 
	if(_isGround())
	{
		vVelocity.x *= 0.9f;
		//ある程度速度が小さくなったらゼロに丸めてしまってOK 
		if (fabsf(vVelocity.x) < 0.001f) {
			vVelocity.x = 0.0f;
		}
	}

	//X軸方向の入力があれば加速させる 
	if (vInput.x != 0.0f) {
		//速度ベクトルに、入力方向×加速量を足しこむ 
		vVelocity.x += vInput.x * acceleration;
		//速さが上限を超えないようにする 
		if (fabs(vVelocity.x) > maxSpeed) {
			vVelocity.x = maxSpeed * (vVelocity.x) / (fabsf(vVelocity.x));
		}
	}

	// 天井にぶつかる場合、上方向の移動量を0にする
	if (_isCeil() && vVelocity.y > 0.0f)
	{
		vVelocity.y = 0.0f;
	}

	Sprite* vSprite = pPlayer->GetSprite();
	if (_isGround(vVelocity.GetNormalized()) && _isSwitchTypeSpeed(vVelocity.y) && vVelocity.y < 0.0f)
	{
		mMoveType = MoveType::Ground;
		vSprite->SetRotationZ(0.0f);
	}
	else if (_isNextToWall(Vector2f(vVelocity.GetNormalized().x, 0.0f)) && _isSwitchTypeSpeed(vVelocity.x))
	{
		mMoveType = MoveType::Wall;
		if (vVelocity.x > 0)
		{
			vSprite->SetRotationZ(90.0f);
		}
		else
		{
			vSprite->SetRotationZ(-90.0f);
		}
	}


	//重力の計算 
	vVelocity.y += gravity;
	if (vVelocity.y < maxGravity) {
		vVelocity.y = maxGravity;
	}

	pPlayer->SetVelocity(vVelocity);

	_translate(vVelocity);
}

// 地上にいる時の処理
void StateMoving::_updateGround(Vector2f vInput)
{
	//このフレームでの加速量 
	float acceleration = mGroundSpeedCoefficient * Time_I->GetDeltaTime();
	//このフレームでの最大移動量 
	float maxSpeed = mGroundMaxSpeed * Time_I->GetDeltaTime();

	

	Player* pPlayer = GetPlayer();
	Vector2f vVelocity = pPlayer->GetVelocity();

	// y方向の速度は処理しないため、0にする
	vVelocity.y = 0;

	//摩擦による速度の減衰 
	{
		vVelocity.x *= 0.95f;
		//ある程度速度が小さくなったらゼロに丸めてしまってOK 
		if (fabsf(vVelocity.x) < 0.001f) {
			vVelocity.x = 0.0f;
		}
	}

	//X軸方向の入力があれば加速させる 
	if (vInput.x != 0.0f) {
		//速度ベクトルに、入力方向×加速量を足しこむ 
		vVelocity.x += vInput.x * acceleration;
		//速さが上限を超えないようにする 
		if (fabs(vVelocity.x) > maxSpeed) {
			vVelocity.x = maxSpeed * (vVelocity.x) / (fabsf(vVelocity.x));
		}
	}

	pPlayer->SetVelocity(vVelocity);
	if (!_isGround())
	{
		mMoveType = MoveType::Wall;
		Sprite* vSprite = pPlayer->GetSprite();
		if (vVelocity.x > 0)
		{
			vSprite->SetRotationZ(-90.0f);
			vVelocity = Vector2f(vVelocity.y, -vVelocity.x);
		}
		else
		{
			vSprite->SetRotationZ(90.0f);
			vVelocity = Vector2f(vVelocity.y, vVelocity.x);
		}
		
		
	}
	else if (_isNextToWall(vVelocity.GetNormalized()) && _isSwitchTypeSpeed(vVelocity.x))
	{
		mMoveType = MoveType::Wall;
		Sprite* vSprite = pPlayer->GetSprite();
		if (vVelocity.x > 0)
		{
			vSprite->SetRotationZ(90.0f);
		}
		else
		{
			vSprite->SetRotationZ(-90.0f);
		}
	}

	_translate(vVelocity);
}

// 壁に貼り付けている時の処理
void StateMoving::_updateWall(Vector2f vInput)
{
	
	//このフレームでの加速量 
	float acceleration = mWallSpeedCoefficient * Time_I->GetDeltaTime();
	//このフレームでの最大移動量 
	float maxSpeed = mWallMaxSpeed * Time_I->GetDeltaTime();

	Player* pPlayer = GetPlayer();
	Vector2f vVelocity = pPlayer->GetVelocity();
	/*if (_isNextToWall(Vector2f(2.0f, 0.0f)))
	{
		pPlayer->GetSprite()->SetRotationZ(90.0f);
	}
	else
	{
		pPlayer->GetSprite()->SetRotationZ(-90.0f);
	}*/
	// x方向の速度は処理しないため、0にする
	vVelocity.x = 0;

	//摩擦による速度の減衰 
	{
		vVelocity.y *= 0.9f;
		//ある程度速度が小さくなったらゼロに丸めてしまってOK 
		if (fabsf(vVelocity.y) < 0.001f) {
			vVelocity.y = 0.0f;
		}
	}

	//X軸方向の入力があれば加速させる 
	if (vInput.y != 0.0f) {
		//速度ベクトルに、入力方向×加速量を足しこむ 
		vVelocity.y += vInput.y * acceleration;
		//速さが上限を超えないようにする 
		if (fabs(vVelocity.y) > maxSpeed) {
			vVelocity.y = maxSpeed * (vVelocity.y) / (fabsf(vVelocity.y));
		}
	}

	pPlayer->SetVelocity(vVelocity);
		
	float CollisionSize = 40.0f;
	Sprite* vSprite = pPlayer->GetSprite();
	// 壁の隣にいない場合
	if (!_isNextToWall())
	{
		if (vVelocity.y > 0)
		{
			mMoveType = MoveType::Ground;
			if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(10.0f, -10.0f), CollisionSize, CollisionSize)) {
				vVelocity = Vector2f(vVelocity.y, vVelocity.x);
			}
			else if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(-10.0f, -10.0f), CollisionSize, CollisionSize)) 
			{
				vVelocity = Vector2f(-vVelocity.y, vVelocity.x);
			}
			vSprite->SetRotationZ(0.0f);
		}
		else
		{
			mMoveType = MoveType::Ceiling;
			if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(10.0f, 10.0f), CollisionSize, CollisionSize)) {
				vVelocity = Vector2f(-vVelocity.y, vVelocity.x);
			}
			else if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(-10.0f, 10.0f), CollisionSize, CollisionSize))
			{
				vVelocity = Vector2f(vVelocity.y, vVelocity.x);
			}
			vSprite->SetRotationZ(180.0f);
		}
	}
	else 
	{
		if (vVelocity.y > 0 && _isSwitchTypeSpeed(vVelocity.y))
		{
			if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(0.0f, vVelocity.GetNormalized().y * CollisionSize / 4.0f), CollisionSize, CollisionSize)) {
				mMoveType = MoveType::Ceiling;
				vSprite->SetRotationZ(180.0f);
			}
		}
		else if (vVelocity.y < 0 && _isSwitchTypeSpeed(vVelocity.y))
		{
			if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(0.0f, vVelocity.GetNormalized().y * CollisionSize / 4.0f), CollisionSize, CollisionSize)) {
				mMoveType = MoveType::Ground;
				vSprite->SetRotationZ(0.0f);
			}
		}
	}

	_translate(vVelocity);
	

}

// 天井に貼り付けている時の処理
void StateMoving::_updateCeiling(Vector2f vInput)
{
	//このフレームでの加速量 
	float acceleration = mCeilingSpeedCoefficient * Time_I->GetDeltaTime();
	//このフレームでの最大移動量 
	float maxSpeed = mCeilingMaxSpeed * Time_I->GetDeltaTime();

	Player* pPlayer = GetPlayer();
	Vector2f vVelocity = pPlayer->GetVelocity();
	
	// y方向の速度は処理しないため、0にする
	vVelocity.y = 0;

	//摩擦による速度の減衰 
	{
		vVelocity.x *= 0.9f;
		//ある程度速度が小さくなったらゼロに丸めてしまってOK 
		if (fabsf(vVelocity.x) < 0.001f) {
			vVelocity.x = 0.0f;
		}
	}

	//X軸方向の入力があれば加速させる 
	if (vInput.x != 0.0f) {
		//速度ベクトルに、入力方向×加速量を足しこむ 
		vVelocity.x += vInput.x * acceleration;
		//速さが上限を超えないようにする 
		if (fabs(vVelocity.x) > maxSpeed) {
			vVelocity.x = maxSpeed * (vVelocity.x) / (fabsf(vVelocity.x));
		}
	}
	

	pPlayer->SetVelocity(vVelocity);

	float CollisionSize = 40.0f;
	Sprite* vSprite = GetPlayer()->GetSprite();
	if (!_isCeil())
	{
		mMoveType = MoveType::Wall;
		if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(10.0f, 10.0f), CollisionSize, CollisionSize)) {
			vVelocity = Vector2f(vVelocity.y, -vVelocity.x);
			vSprite->SetRotationZ(90.0f);
		}
		else if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(-10.0f, 10.0f), CollisionSize, CollisionSize))
		{
			vVelocity = Vector2f(vVelocity.y, vVelocity.x);
			vSprite->SetRotationZ(-90.0f);
		}
		
	}
	else if (_isSwitchTypeSpeed(vVelocity.x))
	{
		if (GetMap()->IsInsideWall(pPlayer->GetPosition() + Vector2f(vVelocity.GetNormalized().x * CollisionSize / 4.0f, 0.0f), CollisionSize, CollisionSize)) {
			mMoveType = MoveType::Wall;
			if (vVelocity.x > 0)
			{
				vSprite->SetRotationZ(90.0f);
			}
			else
			{
				vSprite->SetRotationZ(-90.0f);
			}
		}
		
	}

	_translate(vVelocity);
}

bool StateMoving::_isSwitchTypeSpeed(float fSpeed)
{
	return abs(fSpeed) > mMinStickVelocity;
}

Vector2f StateMoving::_getVectorByDirection(Direction direction)
{
	float CollisionWidth = 40.0f;

	switch (direction)
	{
		case::Direction::Up:
			return Vector2f(0, CollisionWidth / 2.0f);
			break;

		case::Direction::Right:
			return Vector2f(CollisionWidth / 2.0f, 0);
			break;

		case::Direction::Down:
			return Vector2f(0, -CollisionWidth / 2.0f);
			break;

		case::Direction::Left:
			return Vector2f(-CollisionWidth / 2.0f, 0);
			break;
	}
}

// 今は地面にいるかを返す関数
bool StateMoving::_isGround(Vector2f vVelocity)
{
	//衝突判定用の矩形の中心点を自分の足元にする 
	Vector2f vTargetPos = GetPlayer()->GetPosition() + Vector2f(0, -10.0f);
	//衝突判定用の矩形の幅と高さ 
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;
	//壁に衝突していたら地面に立っている 
	return GetMap()->IsInsideWall(vTargetPos, CollisionWidth, CollisionWidth);
}

// 今は壁の隣にいるかを返す関数
bool StateMoving::_isNextToWall(Vector2f vVelocity)
{
	// プレイヤーの位置
	Vector2f vPlayerPos = GetPlayer()->GetPosition();

	//衝突判定用の矩形の幅と高さ 
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;
	
	if (vVelocity.GetLength() > 0)
	{
		return GetMap()->IsInsideWall(vPlayerPos + vVelocity, CollisionWidth, CollisionWidth);
	}
	else {
		
		// 壁の右隣にいるか
		bool isWallLeftSide = GetMap()->IsInsideWall(vPlayerPos + Vector2f(CollisionWidth/4.0f, 0.0f), CollisionWidth, CollisionWidth);
		// 壁の右隣にいるか
		bool isWallRightSide = GetMap()->IsInsideWall(vPlayerPos + Vector2f(-CollisionWidth / 4.0f, 0.0f), CollisionWidth, CollisionWidth);

		return isWallRightSide || isWallLeftSide;
	}
}

// 今は天井の下にいるかを返す関数
bool StateMoving::_isCeil(Vector2f vVelocity)
{
	//衝突判定用の矩形の中心点を自分の足元にする 
	Vector2f vTargetPos = GetPlayer()->GetPosition() + vVelocity;

	//衝突判定用の矩形の幅と高さ 
	float CollisionWidth = 40.0f;
	float CollisionHeight = 1.0f;

	//壁に衝突していたら地面に立っている 
	return GetMap()->IsInsideWall(vTargetPos, CollisionWidth, CollisionWidth);
}

