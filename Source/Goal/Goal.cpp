#include "Goal.h" 

//CollisionManagerを使うために必要なのでinclude 
#include "Fwk/Framework.h" 

//CollisionGroupを使うために必要 
#include "GameDef.h" 

//初期化 
void Goal::Init()
{
	//テクスチャの読み込み 
	mTexture.Load("Images/2dAction/Goal.png");

	//Spriteの初期化と各種設定 
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(64.0f, 128.0f);

	//衝突形状の設定 
	{
		//自分はゴールグループ 
		mCollider.SetGroup((int)CollisionGroup::Goal);
		//タグを設定 
		mCollider.SetTag("Goal");
		//オーナーに自分を設定 
		mCollider.SetOwner(this);
		//衝突形状を指定（矩形） 
		mCollider.SetRect(-32.0f, -64.0f, 64.0f, 128.0f);
		//活性状態にしておく 
		mCollider.SetActive(true);
		//コリジョンマネージャにコライダーを登録 
		CollisionManager_I->Register(&mCollider);
	}
}

//終了 
void Goal::Term()
{
	//コリジョンマネージャから登録解除 
	CollisionManager_I->Unregister(&mCollider);
	mSprite.Term();
	mTexture.Unload();
}

//更新 
void Goal::Update() {
	//スプライトの更新 
	mSprite.Update();
}

//描画 
void Goal::Render() {
	mSprite.Draw();
}

//位置設定 
void Goal::SetPosition(Vector2f position) {
	//スプライトの位置を更新 
	mSprite.SetPosition(position);
	//衝突形状の位置も更新 
	mCollider.SetPosition(position);
}