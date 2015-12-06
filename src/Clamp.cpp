/*
 * Clamp.cpp
 *
 *  Created on: Nov 22, 2015
 *      Author: ratpack
 */

#include "Clamp.h"

Clamp::Clamp(Victor * clamp, DigitalInput * fullOpen, DigitalInput * swordIn)
	:clamp(clamp), fullOpen(fullOpen), swordIn(swordIn){
	timer = new Timer();
}

void Clamp::update(){
	if(( (isOpen() && clampSpeed == OPEN_CLOSE_SPEED)) || timer->Get() > 1.0){
		stop();
		timer->Stop();
		timer->Reset();
	}
	clamp->Set(clampSpeed);
}

void Clamp::open(){
	clampSpeed = OPEN_CLOSE_SPEED;
}

void Clamp::close(){
	if(isOpen()) {
		clampSpeed = -OPEN_CLOSE_SPEED;
		timer ->Reset();
		timer ->Start();
	}
}

void Clamp::stop() {
	clampSpeed = 0;
}

bool Clamp::isOpen(){
	return fullOpen->Get();
}

bool Clamp::isSwordIn(){
	return swordIn->Get();
}
