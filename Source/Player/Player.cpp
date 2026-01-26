#include "Player/Player.h"

// Input_Iを使うためのヘッダファイル
#include "Fwk/Framework.h"

// GetMapを使うためGameObjectMng.hをinclude
#include "GameObjectMng/GameObjectMng.h"

#include "GameDef.h"

// WINDOW_HEIGHTを使うのでinclude
#include "AppDef.h"
#include "State/StateMng.h"


#pragma region Basic Functions

// プレイヤーの初期化
// This Comment Is Add by Scholl PC　
void Player::Init() {

	// 位置の初期化
	mPosition = { 96.0f, -96.0f };

	// テクスチャの読み込み
	mTexture.Load("Images/2dAction/Slime.png");

	// スプライトの初期化
	{
		mSprite.Init();
		mSprite.SetTexture(mTexture);
		// スプライトのサイズ設定 
		mSprite.SetSize(64.0f, 64.0f);
		// テクスチャの描画範囲を指定 
		mSprite.SetTexCoord(0.0f, 0.0f, 0.5f, 0.5f);
		mSprite.SetPosition(mPosition);

		//※追加 やられ状態から復帰してきたときに再表示する必要がある 
		mSprite.SetVisible(true);
		//※追加 やられ状態から復帰してきたときに回転をリセットする必要がある 
		mSprite.SetRotationZ(0);
	}
	// コライダー設定
	{
		// 自分は敵グループのコライダー
		mCollider.SetGroup((int)CollisionGroup::Player);

		// 衝突対象を追加
		mCollider.AddHitGroup((int)CollisionGroup::Enemy);
		mCollider.AddHitGroup((int)CollisionGroup::Goal);

		// コライダーのタグを設定
		mCollider.SetTag("Player");
		// オーナーに自分を設定
		mCollider.SetOwner(this);
		mCollider.SetActive(true);
		// 形状を指定する
		mCollider.SetCircle(0.0f, 0.0f, 20.0f);
		// 衝突時のコールバック関数を設定する
		mCollider.SetCallbackFunction(CreateCollisionEventCallback(Player::_onHit));
		CollisionManager_I->Register(&mCollider);
	}

	_initAnimation();

	// 汎用タイマリセット
	mTimer = 0.0f;
	// 汎用ステップカウンタリセット
	mStep = 0;

	mInvicibilityFrames = 0;

	(StateType::Moving);

	//初期の向きを右向きにしておく 
	mDirection = Direction::Right;

	// 速度の初期化
	mVelocity = { 0.0f, 0.0f };

	// HPを初期化しておく
	mHpMax = 5;
	mHp = mHpMax;

	//Waveデータ読み込み 
	mSounds[(int)Sound::Jump].Load("Sound/jumpC.wav");
	mSounds[(int)Sound::Damaged].Load("Sound/damageB.wav");
	//音源 
	mSoundSources[(int)Sound::Jump].Init(mSounds[(int)Sound::Jump]);
	mSoundSources[(int)Sound::Damaged].Init(mSounds[(int)Sound::Damaged]);

	mStateMng.Init();
}

// プレイヤーの更新
void Player::Update() {
	mStateMng.Update();
	// スプライトの座標を設定する
	mSprite.SetPosition(mPosition);

	// プレイヤーのコライダーの位置を更新する
	mCollider.SetPosition(mPosition);

	// スプライトの更新
	mSprite.Update();


}

// プレイヤーの描画
void Player::Render() {
	mSprite.Draw();

}

// プレイヤーの後片付け
void Player::Term() {

	mSprite.Term();
	mTexture.Unload();

	CollisionManager_I->Unregister(&mCollider);

	//サウンドの解放 
	for (int i = 0; i < (int)Sound::Count; ++i) {
		mSounds[i].Unload();
		mSoundSources[i].Term();
	}
}

#pragma endregion

#pragma region Private Functions

// アニメーションの初期化
void Player::_initAnimation()
{
	/*float uvW = 1.0f / 8.0f;
	float uvH = 1.0f / 1.0f;

	UVRect uvs[] = {
		{uvW * 0.0f, 0.0f, uvW, uvH}, //前向き　パターン1
		{uvW * 1.0f, 0.0f, uvW, uvH}, //前向き　パターン1
		{uvW * 2.0f, 0.0f, uvW, uvH}, //前向き　パターン1
		{uvW * 3.0f, 0.0f, uvW, uvH}, //前向き　パターン1
		{uvW * 4.0f, 0.0f, uvW, uvH}, //前向き　パターン1
		{uvW * 5.0f, 0.0f, uvW, uvH}, //前向き　パターン1
		{uvW * 6.0f, 0.0f, uvW, uvH}, //前向き　パターン1
		{uvW * 7.0f, 0.0f, uvW, uvH}, //前向き　パターン1
	};

	UVRect idle_front[] = { uvs[0] };	// 前
	UVRect idle_right[] = { uvs[2] };	// 右
	UVRect idle_back[] = { uvs[4] };	// 後
	UVRect idle_left[] = { uvs[6] };	// 左

	// プレイヤー移動時のアニメ
	// 前
	UVRect walk_front[] = {
		uvs[0],
		uvs[1],
		{}
	};

	// 右
	UVRect walk_right[] = {
		uvs[2],
		uvs[3],
		{}
	};

	// 後
	UVRect walk_back[] = {
		uvs[4],
		uvs[5],
		{}
	};

	// 左
	UVRect walk_left[] = {
		uvs[6],
		uvs[7],
		{}
	};

	// アニメーションデータ
	Animation animation[8];

	// アイドル時アニメーションデータの作成
	// 前向き
	CreateAnimationUV(animation[0], "idle_front", 1, 0.0f, false, idle_front);
	// 右向き
	CreateAnimationUV(animation[1], "idle_right", 1, 0.0f, false, idle_right);
	// 後向き
	CreateAnimationUV(animation[2], "idle_back", 1, 0.0f, false, idle_back);
	// 左向き
	CreateAnimationUV(animation[3], "idle_left", 1, 0.0f, false, idle_left);

	// 歩行時アニメーションデータの作成
	// 前向き
	CreateAnimationUV(animation[4], "walk_front", 3, 0.45f, true, walk_front);
	// 右向き
	CreateAnimationUV(animation[5], "walk_right", 3, 0.45f, true, walk_right);
	// 後向き
	CreateAnimationUV(animation[6], "walk_back", 3, 0.45f, true, walk_back);
	// 左向き
	CreateAnimationUV(animation[7], "walk_left", 3, 0.45f, true, walk_left);

	for (int i = 0; i < 8; ++i) {
		mSprite.AddAnimation(animation[i]);
	}

	//やられ時のアニメを追加
	{
		//0度→360度の回転アニメーション
		float rotDeg[] = {
			0.0f,
			360.0f,
		};
		Animation animDying;
		//回転アニメを生成。キーフレーム数は２。0.5秒で一回転、ループする,
		CreateAnimationRotation(animDying, "dying", 2, 0.5, true, rotDeg);
		//↑で生成したアニメにUVアニメを追加。正面向きにします。
		AddAnimationUV(animDying, 1, 0.5, idle_front);
		//Spriteにやられた時のアニメを追加
		mSprite.AddAnimation(animDying);
	}

	// 初期アニメーションを設定する
	mSprite.PlayAnimation("idle_right");*/
}

// 衝突する時のコールバック関数
void Player::_onHit(const CollisionEvent& collisionEvent)
{
	if (collisionEvent.ColliderB->GetHitGroup() == (int)CollisionGroup::Enemy && mInvicibilityFrames == 0)
	{

		// ダメージ状態にする
		mStatus = Status::Damaged;

		// タイマーをリセット
		mTimer = 0;
		// 汎用ステップをリセット
		mStep = 0;

		mCollider.SetActive(false);

		//速度をリセット 
		mVelocity = { 0.0f,0.0f };

		//自分の体力を減らす
		--mHp;

		//被ダメージ音を再生 
		mSoundSources[(int)Sound::Damaged].Play();

		// 体力が負数にならないようにしておく
		if (mHp == 0)
		{
			//以降敵と衝突しないように衝突をOFF 
			mCollider.SetActive(false);
			//やられた時のアニメ 
			mSprite.PlayAnimation("dying");
			//上向きのベロシティを設定する 
			mVelocity.y = 20.0f;
			//やられ中状態にしておく 
			mStatus = Status::Dying;
			return;
		}

		string debugStr = "HP = " + to_string(mHp);
		DebugLog(debugStr);
	}

	//ゴールに衝突していた時の処理 
	if (collisionEvent.ColliderB->GetTag() == "Goal") {
		//以降敵と衝突しないように衝突をOFF 
		mCollider.SetActive(false);
		//プレイヤーをゴール到達状態にしておく 
		mStatus = Status::Goal;
		//前向きにしておく 
		mSprite.PlayAnimation("idle_front");
	}
}

//自分をやられ状態にする 
void Player::_die()
{
	//体力を０にする 
	mHp = 0;
	//以降敵と衝突しないように衝突をOFF 
	mCollider.SetActive(false);
	//やられた時のアニメ 
	mSprite.PlayAnimation("dying");
	//上向きのベロシティを設定する 
	mVelocity.y = 20.0f;
	//やられ中状態にしておく 
	mStatus = Status::Dying;
}

#pragma endregion


//現在位置を取得する 
Vector2f Player::GetPosition()
{
	return mPosition;
}

// 新しい位置に移動させる
void Player::SetPosition(Vector2f newPosition)
{
	mPosition = newPosition;
}

// 現在体力を取得する
int Player::GetHp()
{
	return mHp;
}

// 現在体力を取得する
float Player::GetNormalizedHp()
{
	return (float)mHp / (float)mHpMax;
}

//自分がやられ状態でなければtrueを返す 
bool Player::IsAlive() {
	return !IsDead();
}

//自分がやられ状態であればtrueを返す 
bool Player::IsDead() {
	return (mStatus == Status::Dying || mStatus == Status::Dead);
}

//ゴールに到達したか否かを取得する 
bool Player::IsReachedGoal() {
	return (mStatus == Status::Goal);
}

Sprite* Player::GetSprite()
{
	return &mSprite;
}

// 移動速度を取得する
Vector2f Player::GetVelocity()
{
	return mVelocity;
}
// 移動速度を引数の値通りに設定する
void Player::SetVelocity(Vector2f newVelocity)
{
	mVelocity = newVelocity;
}
