#pragma once

//----------------------------------------------
// タイルマップクラスの定義
//----------------------------------------------

#include "Tilemap/Tilemap.h"
#include "Player/Player.h"
#include "Enemy/EnemyMng.h"
#include "Mst/StageData.h"


// 全オブジェクト初期化
void GameObjectInit(StageData* pStageData);


// 全オブジェクト更新
void GameObjectUpdate();


// 全オブジェクト描画
void GameObjectRender();


// 全オブジェクト解放
void GameObjectTerm();

// Tilemapクラスの変数のアドレスを返す
Tilemap* GetMap();

Player* GetPlayer();

// 敵マネージャクラスのポインタを返す 
EnemyMng* GetEnemyMng();