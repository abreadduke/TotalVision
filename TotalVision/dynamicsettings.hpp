#pragma once

class DynamicSettings {
public:
	static bool IsTimerActive();
	static void SetTimerActiveMode(bool activeMode);
private:
	static bool timerActive;
};