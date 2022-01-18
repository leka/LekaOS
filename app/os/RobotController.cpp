
#include "RobotController.h"

#include "LogKit.h"

using namespace leka;
using namespace system::robot;

void RobotController::wakeupSystem()
{
	log_info("Wakeup System.");
}

void RobotController::fallAsleepSystem()
{
	log_info("Fall asleep System.");
}

void RobotController::onEntryWaitingForCommands()
{
	log_info("On Entry Waiting For Commands.");
}
