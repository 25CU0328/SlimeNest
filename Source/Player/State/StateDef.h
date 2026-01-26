#pragma once

//プレイヤーの状態 
enum class StateType {
	None, // 定義のない状態
	Moving, //通常移動中 
	Attack, //攻撃中 
	Damaged,//被ダメージ中 
	Water, // 隠れている
	Charge, // 壁・天井に貼り付けている状態
	Dead, //やられた
};