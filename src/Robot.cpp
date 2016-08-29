/*
 * Code For "Sword In The Stone" Robot
 */

#include "WPILib.h"
#include "Lib830.h"
#include "Clamp.h"
#include "Camera.h"

using namespace Lib830;

class Robot: public IterativeRobot{
private:

	enum PINS {
		FRONT_LEFT_PWM = 1,
		FRONT_RIGHT_PWM = 2,
		BACK_LEFT_PWM = 3,
		BACK_RIGHT_PWM = 4,

		SWORD_CLAMP_PWM = 5,
		SWORD_CLAMP_CHANNEL = 13,
		CLAMP_SWITCH_DIO = 1,
		SWORD_SWITCH_DIO = 0,
		FLY_WHEEL_PWM = 6,

		GYRO_ANALOG = 0,
		ENCODER_DIO_A = 2,
		ENCODER_DIO_B = 3,
	};

	RobotDrive * drive;

	Clamp * clamp;

	GamepadF310 * pilot;

	PowerDistributionPanel * pdp;

	BuiltInAccelerometer * acceler;
	Lib830::AnalogGyro * gyro;

	VictorSP * flywheel;

	Encoder * encoder;

	CAMERAFEEDS * cameraFeeds;

	static const int TICKS_TO_ACCEL = 15;

	void arcadeDrive(float forward, float turn, bool squared = false){
		drive->ArcadeDrive(-forward, turn, squared);
	}

	void RobotInit()
	{
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

		pdp = new PowerDistributionPanel;

		pilot = new GamepadF310(0);

		gyro = new Lib830::AnalogGyro(GYRO_ANALOG);

		flywheel = new VictorSP(FLY_WHEEL_PWM);

		acceler = new BuiltInAccelerometer;

		encoder = new Encoder(ENCODER_DIO_A, ENCODER_DIO_B);

		cameraFeeds = new CAMERAFEEDS;

		cameraFeeds->init();
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void SetDriveMode(drive_mode_t new_mode)
	{
		drive_mode = new_mode;
		left_speed = 0;
		right_speed = 0;
		rot_speed = 0;
		move_speed = 0;
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		rot_speed = accel(rot_speed, pilot->RightX(), TICKS_TO_ACCEL);

		rot_speed = pilot->RightX();

		move_speed = accel(move_speed, pilot->LeftY(), TICKS_TO_ACCEL);

		drive->ArcadeDrive(move_speed * MOVE_SPEED_LIMIT, -rot_speed * MOVE_SPEED_LIMIT, false);

		SmartDashboard::PutData("gyro", gyro);

		if (pilot->ButtonState(GamepadF310::BUTTON_A)) {
			clamp->open();
		}
		else if (pilot->ButtonState(GamepadF310::BUTTON_B)){
			clamp->close();
		}

		clamp->update();

		SmartDashboard::PutNumber("accelerometer Z", acceler->GetZ());

		SmartDashboard::PutNumber("Encoder", encoder->Get());

		flywheel->Set(pilot->RightTrigger());

		if (pilot->LeftTrigger() != 0)
			flywheel->Set(-pilot->LeftTrigger());


		SmartDashboard::PutNumber("Left Trigger:", pilot->LeftTrigger());

		if (pilot->ButtonState(GamepadF310::BUTTON_X)) {
			cameraFeeds-> changeCam(cameraFeeds->kBtCamFront);
		}
		if (pilot->ButtonState(GamepadF310::BUTTON_Y)){
			cameraFeeds-> changeCam(cameraFeeds->kBtCamBack);
		}

		cameraFeeds->run();

	}

	void TestPeriodic()
	{

	}

	void DisabledInit() {
		cameraFeeds -> end();
	}
};

START_ROBOT_CLASS(Robot)
