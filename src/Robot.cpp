/*
 * Code For "Sword In The Stone" Robot
 */

#include "WPILib.h"
#include "../util/830utilities.h"
#include "Clamp.h"
#include "Camera.h"


class Robot: public IterativeRobot{
private:

	//PWM pins
	static const int FRONT_LEFT_PWM = 1;
	static const int FRONT_RIGHT_PWM = 2;
	static const int BACK_LEFT_PWM = 3;
	static const int BACK_RIGHT_PWM = 4;

	static const int SWORD_CLAMP_PWM = 5;
	static const uint8 SWORD_CLAMP_CHANNEL = 13;
	static const int CLAMP_SWITCH_DIO = 1;
	static const int SWORD_SWITCH_DIO = 0;
	static const int FLY_WHEEL_PWM = 6;

	//static const int GYRO_ANALOG = 0;
	static const int ENCODER_DIO_A = 2;
	static const int ENCODER_DIO_B = 3;


	RobotDrive *drive;

	Clamp * clamp;

	GamepadF310 *pilot;

	PowerDistributionPanel * pdp;

	BuiltInAccelerometer * acceler;
	//Gyro  * gyro;

	VictorSP * flywheel;

	Encoder * encoder;

	CAMERAFEEDS *cameraFeeds;

	enum drive_mode_t { TANK_DRIVE, ARCADE_DRIVE };
	drive_mode_t drive_mode;
	// tank drive
	float left_speed, right_speed;
	// arcade drive
	float move_speed, rot_speed;
	static const int TICKS_TO_ACCEL = 10;
	SendableChooser *drive_mode_chooser;

	static constexpr float MOVE_SPEED_LIMIT = 1.0;

	const int kCam0Button = 1;
	const int kCam1Button = 2;
	const bool kError = false;
	const bool kOk = true;

	void RobotInit()
	{
		SetDriveMode(ARCADE_DRIVE);

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

		//gyro = new Gyro(GYRO_ANALOG);

		flywheel = new VictorSP(FLY_WHEEL_PWM);

		drive_mode_chooser = new SendableChooser;
		drive_mode_chooser->AddObject("tank", new drive_mode_t(TANK_DRIVE));
		drive_mode_chooser->AddObject("arcade", new drive_mode_t(ARCADE_DRIVE));


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
		auto new_mode_p = (drive_mode_t*)drive_mode_chooser->GetSelected();
		auto new_mode = new_mode_p ? *new_mode_p : ARCADE_DRIVE;
		if (new_mode != drive_mode)
			SetDriveMode(new_mode);
		if (drive_mode == TANK_DRIVE) {
			left_speed = accel(left_speed, pilot->LeftY(), TICKS_TO_ACCEL);
			right_speed = accel(right_speed, pilot->RightY(), TICKS_TO_ACCEL);
			drive->TankDrive(left_speed, right_speed);
		}
		else {
			rot_speed = accel(rot_speed, pilot->RightX(), TICKS_TO_ACCEL);
			SmartDashboard::PutNumber("rotation speed", rot_speed);
			rot_speed = pilot->RightX();
			move_speed = accel(move_speed, pilot->LeftY(), TICKS_TO_ACCEL);
			drive->ArcadeDrive(move_speed * MOVE_SPEED_LIMIT, -rot_speed * MOVE_SPEED_LIMIT, false);
		}
		SmartDashboard::PutBoolean("clamp open", clamp->isOpen());
		SmartDashboard::PutBoolean("sword in", clamp->isSwordIn());

		//SmartDashboard::PutNumber("gyroscope", gyro->GetAngle());

//		for (uint8 i = 0; i <= 15; ++i)
//			SmartDashboard::PutNumber(std::string("current #") + std::to_string(i), pdp->GetCurrent(i));
		SmartDashboard::PutNumber("Current", pdp->GetTotalCurrent());

		if (pilot->ButtonState(F310Buttons::A)) {
			clamp->open();
		}
		else if (pilot->ButtonState(F310Buttons::B)){
			clamp->close();
		}

		clamp->update();

		SmartDashboard::PutNumber("accelerometer Z", acceler->GetZ());

		SmartDashboard::PutNumber("Encoder", encoder->Get());

		flywheel->Set(pilot->RightTrigger());

		if (pilot->LeftTrigger() != 0)
			flywheel->Set(-pilot->LeftTrigger());


		SmartDashboard::PutNumber("Left Trigger:", pilot->LeftTrigger());

		if (pilot->ButtonState(F310Buttons::X)) {
			cameraFeeds-> changeCam(cameraFeeds->kBtCamFront);
		}
		if (pilot->ButtonState(F310Buttons::Y)){
			cameraFeeds-> changeCam(cameraFeeds->kBtCamBack);
		}

		cameraFeeds->run();

	}

	void TestPeriodic()
	{

	}

	void DisabledInit() {
		SmartDashboard::PutData("drive mode", drive_mode_chooser);
		SmartDashboard::PutString("test", "test");
		cameraFeeds -> end();
	}
};

START_ROBOT_CLASS(Robot);
