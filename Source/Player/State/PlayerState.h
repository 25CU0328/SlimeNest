#pragma once
#include "StateDef.h"
#include "Fwk/Collision/Collider.h"

using namespace std;

class PlayerState
{
public:
	// Stateの初期化
	virtual void Init();

	// 今のStateに切り替えた時に呼び出す関数
	virtual void OnStateEnter();

	// 今のStateを更新する時に呼び出す関数
	virtual void OnStateUpdate();

	// 別のStateに切り替える前に呼び出す関数 (片付け)
	virtual void OnStateExit();

	// 敵とぶつかる時の処理
	//virtual void OnHitEnemy();

	StateType GetCurrentState();
	StateType GetNextState();
protected:
	// 状態に切り替えた時の時間
	DWORD mEnterStateTime;

	StateType mCurrentState;

	// 次の状態
	StateType mNextState;

	// プレイヤーの関数がよく使うので、ここに保存する
	//Player* mPlayer;
};
