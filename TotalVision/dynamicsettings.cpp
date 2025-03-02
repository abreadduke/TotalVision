#include "dynamicsettings.hpp"

bool DynamicSettings::timerActive = false;
bool DynamicSettings::IsTimerActive() {
	return timerActive;
}

void DynamicSettings::SetTimerActiveMode(bool activeMode)
{
	DynamicSettings::timerActive = activeMode;
}
