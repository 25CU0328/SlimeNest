#include "Onibi.h" 

//コリジョングループの列挙型を使うために 
#include "GameDef.h" 
//CollisionManagerを使うのでinclude 
#include "Fwk/Framework.h" 

//初期化 
void Onibi::Init()
{
    //敵共通の初期化処理 
    Enemy::Init();

    //テクスチャの読み込み 
    mTexture.Load("Images/2dAction/onibi.png");

    //スプライトの初期化 
    mSprite.Init();
    //テクスチャの設定 
    mSprite.SetTexture(mTexture);
    //スプライトのサイズ設定 
    mSprite.SetSize(60.0f, 84.0f);
    //テクスチャの描画範囲を指定 
    mSprite.SetTexCoord(0.0f, 0.0f, 0.5f, 0.5f);
    //Pivotを調整。真ん中やや下に。 
    mSprite.SetPivot(0.5f, 0.25f);

    //スプライトにアニメーションを設定 
    _initAnimation();

    //衝突形状（コライダー）の設定 
    {
        //タグを設定
        mCollider.SetTag("Onibi");
        //形状を指定(中心位置はmPositionと同じ、形状は円で） 
        mCollider.SetCircle(0.0f, 0.0f, 20.0f);
    }
}

// 更新
void Onibi::Update()
{
    //アクティブでなければ更新処理は行わない 
    if (!IsActive()) {
        return;
    }

    // プレイヤーの位置 
    Vector2f playerPos = GetPlayer()->GetPosition();
    // 現在位置からプレイヤー位置までの向きベクトル 
    Vector2f vDirection = (playerPos - mPosition).GetNormalized();
    // このフレームでの移動量 
    float speed = 60.0f * Time_I->GetDeltaTime();
    // 向きベクトルを移動量でスカラー倍して現在位置に足しこむ 
    mPosition += vDirection * speed;

    // 敵共通の処理 
    // これは最新の位置をSprite等に反映させるものなので、 
    // 派生クラスの更新処理の最後に呼ぶ 
    Enemy::Update();
}

// アニメーションの初期化
void Onibi::_initAnimation()
{
    // UVの幅と高さ 
    float uvW = 20.0f / 128.0f;
    float uvH = 28.0f / 64.0f;

    // スプライトセル構造体リスト(12パターン分) 
    UVRect uvs[] = {
        {uvW * 0.0f,uvH * 0.0f,uvW,uvH},
        {uvW * 1.0f,uvH * 0.0f,uvW,uvH},
        {uvW * 2.0f,uvH * 0.0f,uvW,uvH},
        {uvW * 3.0f,uvH * 0.0f,uvW,uvH},
        {uvW * 4.0f,uvH * 0.0f,uvW,uvH},
        {uvW * 5.0f,uvH * 0.0f,uvW,uvH},
        {uvW * 0.0f,uvH * 1.0f,uvW,uvH},
        {uvW * 1.0f,uvH * 1.0f,uvW,uvH},
        {uvW * 2.0f,uvH * 1.0f,uvW,uvH},
        {uvW * 3.0f,uvH * 1.0f,uvW,uvH},
        {uvW * 4.0f,uvH * 1.0f,uvW,uvH},
        {uvW * 5.0f,uvH * 1.0f,uvW,uvH},
    };

    // アニメ 
    UVRect idle[] = {
        uvs[0], uvs[1], uvs[2], uvs[3], uvs[4], uvs[5],
        uvs[6], uvs[7], uvs[8], uvs[9], uvs[10], uvs[11],
    };

    // アニメーションデータ 
    Animation anim;

    // アニメーションデータ作成 
    CreateAnimationUV(anim, "idle", 12, 1.0f, true, uvs);

    // アニメーションをスプライトに登録 
    mSprite.AddAnimation(anim);

    // 初期のアニメ設定 
    mSprite.PlayAnimation("idle");
}

// 生成された時に呼び出される 
void Onibi::OnCreated() {
}
