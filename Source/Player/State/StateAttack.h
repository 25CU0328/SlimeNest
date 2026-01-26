#pragma once
#include "PlayerState.h"

class StateAttack : public PlayerState
{
	void OnStateEnter();
	void OnStateUpdate();
	void OnStateExit();
};
