#include "EnemyMng.h" 

//SAFE_DELETEを使う為に#include 
#include "Lib/LibDef.h" 

//生成する敵のクラスの宣言をinclude 
#include "Slime.h" 
#include "Onibi.h" 
#include "Ghost.h"
#include "Skull.h"

//初期化 
void EnemyMng::Init() {
    //アクティブな敵のポインタの配列を空にしておく 
    mActiveEnemies.clear();
}

//後片付け 
void EnemyMng::Term() {
    //アクティブな敵のポインタの配列を空にしておく 
    mActiveEnemies.clear();
    //すべての敵プールを破棄 
    DestroyPoolAll();
}

//更新
void EnemyMng::Update() {

    //アクティブな敵を全て更新する 
    for (int i = 0; i < mActiveEnemies.size(); ++i) {
        //更新 
        mActiveEnemies[i]->Update();
    }

    //更新の結果非アクティブとなった要素を取り除く 
    for (int i = 0; i < mActiveEnemies.size(); /*ここは書かない*/) {

        if (mActiveEnemies[i]->IsActive()) {
            //mActiveEnemies[i]がアクティブのままであれば 
            //次の要素を処理するのでiをインクリメント 
            ++i;
        }
        else {
            // mActiveEnemies[i]が非アクティブとなっていたらその要素を取り除く 
            mActiveEnemies.erase(mActiveEnemies.begin() + i);

            //※ここでは i をインクリメントしない. 
        }
    }
}

//描画 
void EnemyMng::Render() {
    //アクティブな敵を全て描画 
    for (int i = 0; i < mActiveEnemies.size(); ++i) {
        //描画 
        mActiveEnemies[i]->Render();
    }
}

//オブジェクトプールの生成 
void EnemyMng::GeneratePool(string enemyName, int poolSize) {
    //スライムのプール作成 
    if (enemyName == "Slime") {
        //生成する敵クラスにSlimeを指定 
        mEnemyPools[enemyName].Init<Slime>(poolSize);
    }
    //鬼火のプール作成 
    if (enemyName == "Onibi") {
        //生成する敵クラスにSlimeを指定 
        mEnemyPools[enemyName].Init<Onibi>(poolSize);
    }
    //幽霊のプール作成 
    if (enemyName == "Ghost") {
        //生成する敵クラスにGhostを指定 
        mEnemyPools[enemyName].Init<Ghost>(poolSize);
    }
    //がいこつのプール作成 
    if (enemyName == "Skull") {
        //生成する敵クラスにGhostを指定 
        mEnemyPools[enemyName].Init<Skull>(poolSize);
    }
}

// 全ての敵のオブジェクトプールを解放
void EnemyMng::DestroyPoolAll()
{
    // スライムのプールを破棄
    DestroyPool("Slime");
    //鬼火のプールを破棄 
    DestroyPool("Onibi");
    DestroyPool("Ghost");
    DestroyPool("Skull");
}


//指定した敵のオブジェクトプールの破棄 
void EnemyMng::DestroyPool(string enemyName) {

    //敵のプール破棄 
    if (mEnemyPools.count(enemyName) > 0) {
        mEnemyPools[enemyName].Term();
        mEnemyPools.erase(enemyName);
    }
}

//敵の生成 
void EnemyMng::CreateEnemy(string enemyName, Vector2f position) {

    //指定の敵プールから非アクティブのオブジェクトを取得する 
    Enemy* pEnemy = nullptr;

    //敵プールから空きオブジェクトを取得 
    if (mEnemyPools.count(enemyName) > 0) {
        pEnemy = mEnemyPools[enemyName].Alloc();
    }

    //非アクティブの敵オブジェクトがなければ生成失敗 
    if (pEnemy == nullptr) {
        return;
    }

    //位置を設定 
    pEnemy->SetPosition(position);

    //生成時の関数を呼び出す 
    pEnemy->OnCreated();

    //アクティブな敵配列に追加しておく 
    mActiveEnemies.push_back(pEnemy);
}

//残りの敵の数 
int EnemyMng::GetEnemyCount() {
    return (int)mActiveEnemies.size();
}

// CSVデータから敵を生成する
void EnemyMng::CreateEnemies(CSVData* pCsvData)
{
    //nullptrが渡されてきたら何もしないで関数を抜ける
    if (pCsvData == nullptr)
    {
        return;
    }

    // 敵の情報は、名前、X座標、Y座標の3つで1組。
    // CSVファイルの全要素を3で割って敵の配置情報の数とする
    int enemyNum = pCsvData->GetElementCount() / 3;

    // 配置する敵の数だけ繰り返し
    for (int i = 0; i < enemyNum; ++i)
    {
        // 敵1体の配置データの先頭の要素番号
        int index = i * 3;

        // 敵の名前を取得
        string enemyName;
        pCsvData->GetString(index, &enemyName);

        // 配置先の座標を取得
        Vector2f position;
        position.x = pCsvData->GetFloat(index + 1);
        position.y = pCsvData->GetFloat(index + 2);

        // 敵を生成
        CreateEnemy(enemyName, position);

    }
}
