#pragma once
enum class Direction {
	None,
	Up,
	Right,
	Down,
	Left,
};

enum class Status
{
	Moving,
	Damaged,
	Attack,
	Chanting,
};

//衝突グループを示す定数 
enum class CollisionGroup {
	Player,
	Weapon,
	Enemy,
	View,
	Goal,
};

//シーン種別 
enum class SceneType {
	None = -1,
	Title,
	InGame,
	GameOver,
	GameClear,
	StageSelect,
};

// 敵の状態を表す列挙型
enum class EnemyState
{
	None = -1,
	Patrol, // パトロール中, 特定なルートに移動する
	Alert,	// 警戒している
	Follow,	// プレイヤーをファロー
	Attack,	// プレイヤーを攻撃する

};