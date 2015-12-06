/*
 * Clamp.h
 *
 *  Created on: Nov 22, 2015
 *      Author: ratpack
 */

#ifndef SRC_CLAMP_H_
#define SRC_CLAMP_H_

#include "WPILib.h"

class Clamp {
public:
	Clamp(Victor * clamp, DigitalInput * fullOpen, DigitalInput * swordIn);
	void close();

	void update();
	void open();
	void stop();

	bool isOpen();
	bool isSwordIn();
private:
	Timer * timer;
	Victor * clamp;
	DigitalInput * fullOpen;
	DigitalInput * swordIn;

	const double OPEN_CLOSE_SPEED = .4;
	double clampSpeed = 0;
};


#endif /* SRC_CLAMP_H_ */
