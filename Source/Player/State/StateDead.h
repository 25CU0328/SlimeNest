#pragma once

// PlayerState‚ğŒp³‚·‚éˆ×‚ÉInclude
#include "Player/State/PlayerState.h"

// ƒ_ƒ[ƒW‚ğó‚¯‚½‚Ìó‘Ô
class StateDead : public PlayerState
{
	void OnStateEnter();
	void OnStateUpdate();
};