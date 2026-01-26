#pragma once

// PlayerState‚ğg‚¤ˆ×‚Éinclude
#include "StateDef.h"
#include "StateAttack.h"
#include "StateDamaged.h"
#include "StateDead.h"
#include "StateMoving.h"
#include "StateCharging.h"
#include "StateWater.h"

#include <Fwk/Framework.h>

class StateMng
{
public:
	// ‰Šú‰»
	void Init();
	// XVˆ—
	void Update();
	// ó‘Ô‚ğØ‚è‘Ö‚¦
	void SwitchState(StateType newStateType);

	// ¡‚Ìó‘Ô‚Ìí—Ş‚ğ•Ô‚·
	StateType GetCurrentStateType();

	// “G‚É‚Ô‚Â‚©‚é‚Ìˆ—
	//void OnHit();

private:

	PlayerState* mpCurrentState;
};
