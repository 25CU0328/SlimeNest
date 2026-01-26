
#include "Sword.h"

// DebugLogを使う為に必要なのでinclude
#include "Fwk/Framework.h"

//敵のメンバ関数を呼び出すので#include 
#include "Enemy/Enemy.h"

// PlayerStateを使用するためのライブラリ
#include "GameDef.h"

// 衝突範囲テーブル1（向き別）振り下ろし後のパターンの衝突範囲
const Rect hitRects[4] = {
	{{0.0f, -58.0f}, 32.0f, 64.0f}, // front
	{{68.0f, .0f}, 64.0f, 32.0f},	// right
	{{0.0f, 58.0f}, 32.0f, 64.0f},	// back
	{{-68.0f, .0f}, 64.0f, 32.0f},	// left
};

// 衝突範囲テーブル2（向き別）振り下ろし中（斜めの時）の衝突範囲
const Rect hitRects2[4] = {
 {{ 0.0f,0.0f},0.0f,0.0f}, // front(用意ナシ） 
 {{ 70.0f,42.0f},54.0f,32.0f}, //right 
 {{ 0.0f, 0.0f},0.0f,0.0f}, // back(用意ナシ） 
 {{-70.0f,42.0f},54.0f,52.0f}, //left 
};

// 初期化
void Sword::Init() {
	// テクスチャの読み込み
	mTexture.Load("Images/2dAction/sword.png");

	// スプライトの初期化
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(128.0f, 64.0f);
	mSprite.SetVisible(false);

	// Waveデータの読み込み
	mSound.Load("Sound/swingC.wav");
	// 音源初期化
	mSoundSource.Init(mSound);

	_initAnimation();

	// コライダーの設定
	{
		// 自分は敵グループのコライダー
		mCollider.SetGroup((int)CollisionGroup::Weapon);
		// 衝突対象にプレイヤーグループのコライダーを追加
		mCollider.AddHitGroup((int)CollisionGroup::Enemy);
		// コライダーのタグを設定
		mCollider.SetTag("Sword");
		// オーナーに自分を設定
		mCollider.SetOwner(this);
		mCollider.SetActive(false);
		// 形状を指定する
		mCollider.SetRect(0.0f, 0.0f, 40.0f, 40.0f);
		// 衝突時のコールバック関数を設定する
		mCollider.SetCallbackFunction(CreateCollisionEventCallback(Sword::OnHit));

		CollisionManager_I->Register(&mCollider);
	}
}

// 更新
void Sword::Update() {
	mSprite.Update();
}

// 描画
void Sword::Render() {
	mSprite.Draw();
}

// 後片付け
void Sword::Term() {
	CollisionManager_I->Unregister(&mCollider);

	mSprite.Term();
	mTexture.Unload();

	// 効果音の破棄
	mSound.Unload();
	// 音源の解放
	mSoundSource.Term();
}

// 位置設定
void Sword::SetPosition(Vector2f position) {
	// スプライトの位置更新
	mSprite.SetPosition(position);

	mCollider.SetPosition(position);
}

// 指定方向に剣を振る
void Sword::Swing(Direction direction) {
	const string animationName_swing[] = { "swing_front", "swing_right", "swing_back", "swing_left" };
	mSprite.SetVisible(true);
	mSprite.PlayAnimation(animationName_swing[(int)direction]);

	mDirection = direction;
	// スイング音の再生
	mSoundSource.Play();
}

void Sword::_initAnimation() {
	// UV座標の幅と高さ
	float uvW = 1.0f / 4.0f;
	float uvH = 1.0f / 2.0f;

	// 8パターンのUV座標とPivot
	SpriteCell cells[8] = {
		{{uvW * 0.0f, 0.0f, uvW, uvH}, {0.5f, -0.5f}}, //0 上
		{{uvW * 1.0f, 0.0f, uvW, uvH}, {0.0f, 0.0f}}, //1 右上
		{{uvW * 2.0f, 0.0f, uvW, uvH}, {0.0f, 0.5f}}, //2 右
		{{uvW * 3.0f, 0.0f, uvW, uvH}, {0.0f, 1.0f}}, //3 右下

		{{uvW * 0.0f, 0.5f, uvW, uvH}, {0.5f, 1.5f}}, //4 下
		{{uvW * 1.0f, 0.5f, uvW, uvH}, {1.0f, 1.5f}}, //5 左下
		{{uvW * 2.0f, 0.5f, uvW, uvH}, {1.0f, 0.5f}}, //6 左
		{{uvW * 3.0f, 0.5f, uvW, uvH}, {1.0f, 0.0f}}, //7 左上
	};

	// UV・Pivot設定リスト 右に剣を振る
	SpriteCell swing_right[]{
		cells[0],
		cells[1],
		cells[2],
		{}
	};

	// UV・Pivot設定リスト 前に剣を振る
	SpriteCell swing_front[]{
		cells[2],
		cells[3],
		cells[4],
		{}
	};

	// UV・Pivot設定リスト 左に剣を振る
	SpriteCell swing_left[]{
		cells[0],	//上 
		cells[7],	//左上 
		cells[6],
		{}
	};

	// UV・Pivot設定リスト 後ろに剣を振る
	SpriteCell swing_back[]{
		cells[6],
		cells[7],
		cells[0],
		{}
	};

	// キーフレーム時刻
	float keyTimes[] = {
		0.0f,
		0.05f,
		0.10f,
		0.25f,
	};

	// アニメーションデータを作成
	Animation animation[4];
	CreateAnimationSpriteCell(animation[0], "swing_front", 4, keyTimes, false, swing_front);
	CreateAnimationSpriteCell(animation[1], "swing_right", 4, keyTimes, false, swing_right);
	CreateAnimationSpriteCell(animation[2], "swing_back", 4, keyTimes, false, swing_back);
	CreateAnimationSpriteCell(animation[3], "swing_left", 4, keyTimes, false, swing_left);

	// 剣のスプライトにアニメーションを追加する
	for (int i = 0; i < 4; ++i) {
		// 各アニメーションイベントのコールバック関数を登録する
		animation[i].SetEventCallback(CreateAnimationEventCallback(Sword::OnAnimationEvent));

		mSprite.AddAnimation(animation[i]);
	}

	// 描画の順番を上げておく
	mSprite.SetPriority(10);
}

void Sword::OnAnimationEvent(const AnimationEvent& animationEvent) {
	switch (animationEvent.EventType)
	{
	case AnimationEventType::Begin:

		break;

	case  AnimationEventType::KeyframeProgressed:
	{
		if (animationEvent.KeyFrameIndex == 1) {
			// 衝突を活性状態にする
			mCollider.SetActive(true);
			// パターンに合わせた衝突範囲を設定する
			mCollider.SetRect(hitRects2[(int)mDirection]);
		}
		if (animationEvent.KeyFrameIndex == 2) {
			// パターンに合わせた衝突範囲を設定する
			mCollider.SetRect(hitRects[(int)mDirection]);
		}
	}
	break;

	case AnimationEventType::Stopped:
		break;

	case AnimationEventType::Completed:
		mCollider.SetActive(false);
		break;
	}
}

void Sword::OnHit(const CollisionEvent& collisionEvent) {
	// 衝突した瞬間のイベントのみ処理をさせる 
	if (collisionEvent.EventType == CollisionEventType::Enter) {
		// 相手が敵である場合 
		if (collisionEvent.ColliderB->GetGroup() == (int)CollisionGroup::Enemy) {
			Enemy* pEnemy = (Enemy*)(collisionEvent.ColliderB->GetOwner());
			pEnemy->OnDamaged(1);
		}
	}
}

bool Sword::IsSwinged() {

	// アニメーションしていたらtrueを返す
	return mSprite.IsAnimationPlaying();
}