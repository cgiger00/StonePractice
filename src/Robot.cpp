/*
 * Code For "Sword In The Stone" Robot
 */

#include "WPILib.h"
#include "../util/830utilities.h"
#include "Clamp.h"

class Robot: public IterativeRobot{
private:

	//PWM pins
	static const int FRONT_LEFT_PWM = 1;
	static const int FRONT_RIGHT_PWM = 2;
	static const int BACK_LEFT_PWM = 3;
	static const int BACK_RIGHT_PWM = 4;

	static const int SWORD_CLAMP_PWM = 5;
	static const int CLAMP_SWITCH_DIO = 1;
	static const int SWORD_SWITCH_DIO = 0;

	RobotDrive *drive;

	Clamp * clamp;

	GamepadF310 *pilot;

	float left_speed, right_speed;
	static const int TICKS_TO_ACCEL = 50;

	void RobotInit()
	{
		left_speed = 0;
		right_speed = 0;
		drive = new RobotDrive(
				new Victor(FRONT_LEFT_PWM),
				new Victor(BACK_LEFT_PWM),
				new Victor(FRONT_RIGHT_PWM),
				new Victor(BACK_RIGHT_PWM)
		);
		clamp = new Clamp(
				new Victor(SWORD_CLAMP_PWM),
				new DigitalInput(CLAMP_SWITCH_DIO),
				new DigitalInput(SWORD_SWITCH_DIO)
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
		left_speed = accel(left_speed, pilot->LeftY(), TICKS_TO_ACCEL);
		right_speed = accel(right_speed, pilot->RightY(), TICKS_TO_ACCEL);
		drive->TankDrive(left_speed, right_speed);
		SmartDashboard::PutBoolean("clamp open", clamp->isOpen());
		SmartDashboard::PutBoolean("sword in", clamp->isSwordIn());

		if (pilot->ButtonState(F310Buttons::A)) {
			clamp->open();
		}
		else if (pilot->ButtonState(F310Buttons::B)){
			clamp->close();
		}

		clamp->update();
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot);
