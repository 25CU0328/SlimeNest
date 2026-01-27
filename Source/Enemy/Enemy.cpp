#include "Enemy.h" 

// コリジョングループの列挙型を使うために 
// エネミー状態もここにあります
#include "GameDef.h" 

// CollisionManagerを使うのでinclude 
#include "Fwk/Framework.h" 

// 初期化 
void Enemy::Init()
{
    //状態をアクティブに 
    mIsActive = true;
    //被ダメージ音を読み込む 
    mSound.Load("Sound/damageE.wav");
    //音源の初期化 
    mSoundSource.Init(mSound, 1);

    //衝突形状（コライダー）の設定（共通の項目のみ） 
    {
        //自分は敵グループのコライダー 
        mCollider.SetGroup((int)CollisionGroup::Enemy);
        //Colliderのオーナーに自分を設定 
        mCollider.AddHitGroup((int)CollisionGroup::Player);
        //Colliderのオーナーに自分を設定 
        mCollider.SetOwner(this);
        //活性状態にしておく 
        mCollider.SetActive(true);
        //コリジョンマネージャにColliderを登録 
        CollisionManager_I->Register(&mCollider);
    }

    // 視線コライダーの設定
    {
        //自分は視線グループのコライダー 
        mViewCollider.SetGroup((int)CollisionGroup::View);
        //Colliderのオーナーに自分を設定 
        mViewCollider.AddHitGroup((int)CollisionGroup::Player);
        //Colliderのオーナーに自分を設定 
        mViewCollider.SetOwner(this);
        //活性状態にしておく 
        mViewCollider.SetActive(true);

        // 衝突時のコールバック関数を設定する
        mViewCollider.SetCallbackFunction(CreateCollisionEventCallback(Enemy::_onPlayerFound));

        //コリジョンマネージャにColliderを登録 
        CollisionManager_I->Register(&mViewCollider);
    }

    // パトロール状態に設定する
    mState = EnemyState::Patrol;
}

// 後片付け 
void Enemy::Term()
{
    //コリジョンマネージャから登録解除 
    CollisionManager_I->Unregister(&mCollider);

    //スプライトの解放 
    mSprite.Term();
    //テクスチャの破棄 
    mTexture.Unload();

    //サウンドの破棄 
    mSound.Unload();
    //音源の解放 
    mSoundSource.Term();
}

// 更新 
void Enemy::Update()
{
    //スプライトを更新 
    mSprite.Update();
    //スプライトの位置を更新 
    mSprite.SetPosition(mPosition);
    //衝突形状の位置を更新 
    mCollider.SetPosition(mPosition);

    //ダメージ時に加算する色（赤）の強さが0でなければ 
    if (mDamagedColor > 0.0f) {
        //0.25秒くらいで0に戻るように減算 
        mDamagedColor -= Time_I->GetDeltaTime() * 4.0f;
        //マイナスの値にならないように調整 
        if (mDamagedColor < 0.0f) {
            mDamagedColor = 0.0f;
        }
        //加算する赤色の強さを指定 
        mSprite.SetAdditionalColor(mDamagedColor, 0.0f, 0.0f);
    }

    switch (mState)
    {
    case EnemyState::Patrol:
        UpdatePatrol();
        break;

    case EnemyState::Alert:
        UpdateAlert();
        break;

    case EnemyState::Attack:
        UpdateAttack();
        break;

    case EnemyState::Follow:
        UpdateFollow();
        break;
    }
}

// 描画 
void Enemy::Render()
{
    //アクティブでなければ描画処理は行わない 
    if (!IsActive()) {
        return;
    }

    mSprite.Draw();
}

// アクティブか？
bool Enemy::IsActive() {
    return mIsActive;
}

// アクティブ状態の設定 
void Enemy::SetActive(bool isActive) {
    mIsActive = isActive;
    //衝突形状も合わせておく 
    mCollider.SetActive(isActive);
}

// 現在位置を取得 
Vector2f Enemy::GetPosition() {
    return mPosition;
}

//現在位置を設定 
void Enemy::SetPosition(Vector2f position) {
    mPosition = position;
    mSprite.SetPosition(mPosition);
    //衝突形状の位置も更新 
    mCollider.SetPosition(mPosition);
}

// ダメージを受ける 
void Enemy::OnDamaged(int damage) {
    // ダメージ音の再生 
    mSoundSource.Play();
}

// 生成された時に呼び出す 
// 派生クラスで上書きして使う 
void Enemy::OnCreated() {
    // なし 
}

// やられたときに呼びだされる 
// 派生クラスで上書きして使う 
void Enemy::OnDefeated() {
    // なし 
}

// パトロール状態の更新処理
void Enemy::UpdatePatrol()
{
    // なし 
}

// 攻撃状態の更新処理
void Enemy::UpdateAttack()
{
    // なし 
}

// ファロー状態の更新処理
void Enemy::UpdateFollow()
{
    // なし 
}

// 警戒状態の更新処理
void Enemy::UpdateAlert()
{
    // なし 
}

void Enemy::_onPlayerFound(const CollisionEvent& collisionEvent)
{
    if (collisionEvent.ColliderB->GetHitGroup() != (int)CollisionGroup::Player)
        return;

    if (collisionEvent.EventType == CollisionEventType::Enter)
    {
        if (mState == EnemyState::Patrol)
        {
            mState = EnemyState::Alert;
        }

        else if (mState == EnemyState::Follow)
        {
            mState = EnemyState::Attack;
        }
    }

    else if (collisionEvent.EventType == CollisionEventType::Exit)
    {
        if (mState == EnemyState::Alert)
        {
            mState = EnemyState::Patrol;
        }

        else if (mState == EnemyState::Attack)
        {
            mState = EnemyState::Follow;
        }
    }
}