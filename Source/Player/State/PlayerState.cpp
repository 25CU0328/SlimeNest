#include "PlayerState.h"

// Input_Iを使うためのヘッダファイル
#include "Fwk/Framework.h"
#include "GameObjectMng/GameObjectMng.h"

// Stateの初期化
void PlayerState::Init()
{
	mCurrentState = StateType::None;
	mNextState = StateType::None;
	mEnterStateTime = DWORD(0);
}

// 今のStateに切り替えた時に呼び出す関数
void PlayerState::OnStateEnter()
{
	mEnterStateTime = Time_I->GetTime();
}

// 今のStateを更新する時に呼び出す関数
void PlayerState::OnStateUpdate()
{

}

// 別のStateに切り替える前に呼び出す関数 (片付け)
void PlayerState::OnStateExit()
{
	mEnterStateTime = 0;
}

StateType PlayerState::GetCurrentState()
{
	return mCurrentState;
}

StateType PlayerState::GetNextState()
{
	return mNextState;
}

