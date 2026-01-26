#include "StateMng.h"

// ó‘ÔClass

#include "StateAttack.h"
#include "StateDamaged.h"
#include "StateDead.h"
#include "StateMoving.h"
#include "StateCharging.h"
#include "StateWater.h"


// ‰Šú‰»
void StateMng::Init()
{
	mpCurrentState = new PlayerState();
	SwitchState(StateType::Moving);
}

void StateMng::Update()
{
	if (mpCurrentState == nullptr)
		return;

	mpCurrentState->OnStateUpdate();

	// Ÿ‚Ìó‘Ô‚ªNone‚¶‚á‚È‚¢‚È‚çØ‚è‘Ö‚¦‚é
	if (mpCurrentState->GetNextState() != StateType::None)
	{
		SwitchState(mpCurrentState->GetNextState());
	}
}

void StateMng::SwitchState(StateType newStateType)
{

	// ¡‚Æ“¯‚¶ó‘Ô‚È‚ç–³‹‚·‚é
	if (newStateType == mpCurrentState->GetCurrentState())
		return;

	// Œ»İ‚Ìó‘Ô‚©‚ç•Ï‚¦‚é‘O‚Ìˆ—
	mpCurrentState->OnStateExit();

	// —Ìˆæ‚Ì‰ğ•ú
	delete mpCurrentState;
	mpCurrentState = nullptr;

	// ó‘Ô‚Ìí—Ş‚É‚æ‚Á‚ÄV‚µ‚¢’l‚ğmState‚É‘ã“ü‚·‚é
	switch (newStateType)
	{
	case StateType::Moving:
		mpCurrentState = new StateMoving();
		break;

	case StateType::Attack:
		mpCurrentState = new StateAttack();
		break;

	case StateType::Damaged:
		mpCurrentState = new StateDamaged();
		break;

	case StateType::Dead:
		mpCurrentState = new StateDead();
		break;

	case StateType::Charge:
		mpCurrentState = new StateCharging();
		break;

	case StateType::Water:
		mpCurrentState = new StateWater();
		break;
	}

	// ó‘Ô‚Ì‰Šú‰»
	mpCurrentState->Init();

	// ó‘Ô‚ÉØ‚è‘Ö‚¦‚½‚Ìˆ—
	mpCurrentState->OnStateEnter();
}



StateType StateMng::GetCurrentStateType()
{
	return mpCurrentState->GetCurrentState();
}