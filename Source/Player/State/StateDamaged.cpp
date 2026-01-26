
#include "StateDamaged.h"
// Input_Iを使うためのヘッダファイル
#include "Fwk/Framework.h"

void StateDamaged::OnStateEnter()
{
	mDuration = 1;
}

void StateDamaged::OnStateUpdate()
{
	mDuration -= Time_I->GetDeltaTime();

	if (mDuration <= 0)
	{
		mNextState = StateType::Moving;
	}
}