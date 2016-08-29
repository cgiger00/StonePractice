/*practice code for 2016-2017 season
 *
 *
 */

#include "WPILib.h"
#include "Lib830.h"
#include "Camera.h"

using namespace Lib830;

class Robot: public IterativeRobot{
private:

	enum PINS {
		FRONT_LEFT_PWM = 1,
		FRONT_RIGHT_PWM = 2,
		BACK_LEFT_PWM = 3,
		BACK_RIGHT_PWM = 4,

		GYRO_ANALOG = 0,
		ENCODER_DIO_A = 2,
		ENCODER_DIO_B = 3,
	};

	RobotDrive * drive;

	GamepadF310 * pilot;

	PowerDistributionPanel * pdp;

	BuiltInAccelerometer * acceler;
	Lib830::AnalogGyro * gyro;

	Encoder * encoder;

	CAMERAFEEDS * cameraFeeds;

	static const int TICKS_TO_ACCEL = 15;

	void arcadeDrive(float forward, float turn, bool squared = false){
		drive->ArcadeDrive(-forward, turn, squared);
	}

	void tankDrive(float forward, float turn, bool squared = false){
		drive->TankDrive(-forward, turn, squared);
	}

	void RobotInit()
	{
		drive = new RobotDrive(
				new Victor(FRONT_LEFT_PWM),
				new Victor(BACK_LEFT_PWM),
				new Victor(FRONT_RIGHT_PWM),
				new Victor(BACK_RIGHT_PWM)
		);

		pdp = new PowerDistributionPanel;

		pilot = new GamepadF310(0);

		gyro = new Lib830::AnalogGyro(GYRO_ANALOG);

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

	void TeleopInit()
	{
		int drive_mode = 1;
	}

	void TeleopPeriodic()
	{
		float targetForward = pilot->LeftY();
		float turn = pilot->RightX()/1.4;

		float forward = accel(previousForward, targetForward, TICKS_TO_FULL_SPEED);

		previousForward = forward;

		if (pilot->RightTrigger()){
			drive_mode = 0;
		} else if (pilot->LeftTrigger()){
			drive_mode = 1;
		}

		if (drive_mode = 0){
			arcadeDrive(forward, turn, true);
		} else {
			tankDrive(forward, turn, true);
		}


		SmartDashboard::PutData("gyro", gyro);

		SmartDashboard::PutNumber("accelerometer Z", acceler->GetZ());

		SmartDashboard::PutNumber("Encoder", encoder->Get());

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
