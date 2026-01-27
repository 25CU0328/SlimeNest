

#include "Archer.h";

// プレイヤーの位置を取得するためにinclude
#include "GameObjectMng/GameObjectMng.h"

// エネミー状態を使うためにinclude
#include "GameDef.h"

// 初期化
void Archer::Init()
{
	mClimbableHeight = 2;
	mFollowTime = 5;
}

void Archer::Update()
{

}

// 
void Archer::UpdateFollow()
{
	Vector2f vPlayerPosition = GetPlayer()->GetPosition();

	Vector2f vDirection = (vPlayerPosition - mPosition).GetNormalized();
	vDirection = Vector2f(vDirection.x, 0).GetNormalized();

	vDirection *= mVelocity;

	if (_isNextToWall())
	{
		vDirection.y += GetMap()->GetTileSize() * mClimbableHeight;
	}


}

void Archer::UpdateAttack()
{

}

void Archer::UpdatePatrol()
{
	//このフレームでの移動量 
	float speed = 64.0f * Time_I->GetDeltaTime();
	// 移動方向を示す単位ベクトル(Right =(1.0f,0.0f) Left=(-1.0f,0.0f))にする 
	Vector2f vDirection = Vector2f((mDirection == Direction::Right) ? 1.0f : -1.0f, 0.0f);
	// 移動させる
	mPosition += vDirection * speed;

	// 向きを反転するか？ 
	bool isFlipX = false;

	// 壁に衝突するかチェック 
	{
		// 衝突検知位置（vDirectionが1.0であれば右側、-1.0であれば左側になる） 
		Vector2f vCheckPos = mPosition + (vDirection * 32.0f);
		// 衝突検知範囲 
		float CollisionWidth = 1.0f;
		float CollisionHeight = 40.0f;
		// 壁に衝突していたら移動方向反転フラグをON 
		if (GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight)) {
			isFlipX = true;
		}
	}
	// 落とし穴があるかチェック 
	{
		// 衝突検知位置（vDirectionが1.0であれば右側、-1.0であれば左側になる） 
		Vector2f vCheckPos = mPosition + (vDirection * 32.0f);
		// 足元をチェックするので検知位置も足元になるようY座標を下げる 
		vCheckPos.y -= 20.0f;
		// 衝突検知範囲 
		float CollisionWidth = 1.0f;
		float CollisionHeight = 1.0f;
		// 足元が壁でなければ移動方向反転フラグをON 
		if (GetMap()->IsInsideWall(vCheckPos, CollisionWidth, CollisionHeight) == false) {
			isFlipX = true;
		}
	}

	// 移動方向の反転フラグがONになっていれば 
	if (isFlipX) {
		// 向きを反転する 
		mDirection = (mDirection == Direction::Right) ? Direction::Left : Direction::Right;
	}
}

bool Archer::_isNextToWall()
{
	float CollisionWidth = mCollider.GetShapeCircle().radius / 2.0f;
	// 壁の右隣にいるか
	bool isWallLeftSide = GetMap()->IsInsideWall(mPosition + Vector2f(CollisionWidth, 0.0f), CollisionWidth, CollisionWidth);
	// 壁の右隣にいるか
	bool isWallRightSide = GetMap()->IsInsideWall(mPosition + Vector2f(CollisionWidth, 0.0f), CollisionWidth, CollisionWidth);

	return isWallRightSide || isWallLeftSide;
}