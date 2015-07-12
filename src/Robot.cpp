/*
 * Code For "Sword In The Stone" Robot
 */

#include "WPILib.h"
#include "../util/830utilities.h"

class Robot: public IterativeRobot
{
private:

	//PWM pins
	static const int FRONT_LEFT_PWM = 1;
	static const int FRONT_RIGHT_PWM = 2;
	static const int BACK_LEFT_PWM = 3;
	static const int BACK_RIGHT_PWM = 4;

	RobotDrive *drive;

	GamepadF310 *pilot;

	void RobotInit()
	{
		drive = new RobotDrive(
				new Victor(FRONT_LEFT_PWM),
				new Victor(BACK_LEFT_PWM),
				new Victor(FRONT_RIGHT_PWM),
				new Victor(BACK_RIGHT_PWM)
		);

		pilot = new GamepadF310(0);

	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		drive->TankDrive(0.0,0.0);
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot);
