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