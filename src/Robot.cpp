/*practice code for 2016-2017 season
 *
 *
 */

#include "WPILib.h"
#include "Lib830.h"
#include "Camera.h"

using namespace Lib830;

#include "RobotDrive.h"

using namespace Lib830;

class Robot: public IterativeRobot
{

public:
	enum Mode{ TANK_DRIVE, ARCADE_DRIVE };	

private: 

	static const int LEFT_PWM_ONE = 0;
	static const int LEFT_PWM_TWO = 4;
	static const int RIGHT_PWM_ONE = 1;
	static const int RIGHT_PWM_TWO = 3;
	
	static const int GYRO = 5;
	static const int ENCODER_A = 6;
	static const int ENCODER_B = 7;


	GamepadF310 * pilot;
	GamepadF310 * copilot;

	RobotDrive * drive;

	VictorSP * frontLeft;
	VictorSP * backLeft;
	VictorSP * frontRight;
	VictorSP * backRight;

	SendableChooser * modeChooser;
	Mode driveMode;

	PowerDistributionPanel * pdp;
	
	BuiltInAccelerometer * acceler;
	Lib830::AnalogGyro * gyro;

	Encoder * encoder;

	CAMERAFEEDS * cameraFeeds;

	static const int TICKS_TO_FULL_SPEED = 15;
	
	void RobotInit() {

		drive = new RobotDrive(
			new VictorSP(LEFT_PWM_ONE),
			new VictorSP(LEFT_PWM_TWO),
			new VictorSP(RIGHT_PWM_ONE),
			new VictorSP(RIGHT_PWM_TWO)
		);

		pilot = new GamepadF310(0);
		copilot = new GamepadF310(1);

		pdp = new PowerDistributionPanel();

		modeChooser = new SendableChooser();
		modeChooser-> AddDefault("Arcade Drive", new Mode(ARCADE_DRIVE));
		modeChooser-> AddObject("Tank Drive", new Mode(TANK_DRIVE));

		SmartDashboard::PutData("Moder Chooser", modeChooser);		
		
		gyro = new Lib830::AnalogGyro(GYRO);

		acceler = new BuiltInAccelerometer;

		encoder = new Encoder(ENCODER_A, ENCODER_B);

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
		float previous_forward = 0;
	}

	void TeleopPeriodic()
	{
		switch(driveMode) {
			case TANK_DRIVE:
				float leftforward = accel(leftforward, pilot->LeftY(), TICKS_TO_FULL_SPEED);
				float rightforward = accel(leftforward, pilot->RightY(), TICKS_TO_FULL_SPEED);
				drive->TankDrive(leftforward,rightforward,true);
				break

			case ARCADE_DRIVE:

			default: 
				float targetForward = pilot ->LeftY();
				float turn = pilot->RightX()/1.4;

				float forward = accel(previous_forward, targetForward, TICKS_TO_FULL_SPEED);
				drive->ArcadeDrive(forward, turn, true);
				break

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
	
	void DisabledPeriodic {
		driveMode = modeChooser->GetSelected() ? *(Mode*)modeChooser->GetSelected() : ARCADE_DRIVE;
		//not quite sure about this, I think it's just a default 
	}
};

START_ROBOT_CLASS(Robot)
