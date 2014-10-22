//
//  ParticleController.h
//  CameraMove_test_092714
//
//  Created by Julia Irwin on 9/28/14.
//
//
class ParticleController {
public:
	ParticleController();
	void pullToCenter( const ci::vec3 &center );
	void update();
	void draw();
	void addParticles( int amt );
	
	std::list<Particle>	mParticles;
};