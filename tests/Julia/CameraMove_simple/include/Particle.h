//
//  Particle.h
//  CameraMove_test_092714
//
//  Created by Julia Irwin on 9/28/14.
//
//
#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <vector>

class Particle {
public:
	Particle();
	Particle( ci::vec3, ci::vec3 );
	void pullToCenter( const ci::vec3 &center );
	void update();
	void draw();
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	ci::vec3	mAcc;
	
	float		mDecay;
	float		mRadius;
};