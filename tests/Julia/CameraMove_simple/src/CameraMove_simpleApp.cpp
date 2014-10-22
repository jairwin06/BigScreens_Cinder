#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/gl/Batch.h"
#include "cinder/gl/Shader.h"

#include "cinder/Camera.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

enum LightingModel{
    
    FLAT,
    PHONG,
    CELL
    
};

class CameraMove_test_092714App : public AppNative {
public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyUp( KeyEvent event ) override;
	void update() override;
	void draw() override;
    void prepareSettings( Settings *settings );
    
    //shapes & particles
    gl::BatchRef mPlane, mSphere, mLightBulb;
    bool mCentralGravity;
    //    ParticleController mParticleController;
    
    //lighthing
    LightingModel mCurrentModel;
    vec4 mLightPos;
    gl::GlslProgRef mFlatShading;
    gl::GlslProgRef mPhongShading;
    gl::GlslProgRef mCellShading;
    vec3 mCellColors[3];
    
    //camera
    CameraPersp mCamera;
    quat mSceneRotation;
    float mCameraDistance;
    vec3 mEye, mCenter, mUp;
    
    //params
    params::InterfaceGlRef mParams;
    
};

void CameraMove_test_092714App::prepareSettings( Settings *settings ){
    settings->setWindowSize( 3840, 1080 );
    settings->setFullScreen();
    //settings->setFrameRate( 60.0f );
}

void CameraMove_test_092714App::setup()
{
    
    //setup shaders
    mFlatShading = gl::GlslProg::create(loadAsset("Flat.vert"), loadAsset("Flat.frag"));
    mPhongShading = gl::GlslProg::create(loadAsset("Phong.vert"), loadAsset("Phong.frag"));
    mCellShading = gl::GlslProg::create(loadAsset("Cell.vert"), loadAsset("Cell.frag"));
    
    //create primitives
    mPlane = gl::Batch::create(geom::Plane().enable(geom::Attrib::NORMAL), mFlatShading);
    mSphere = gl::Batch::create(geom::Sphere().subdivisions(50).enable(geom::Attrib::NORMAL), mFlatShading);
    
    //create lighting
    mLightBulb = gl::Batch::create(geom::Sphere().radius(.1), gl::getStockShader(gl::ShaderDef().color()));
    mLightPos = vec4( 5,5,5, 1);
    mCellColors[0] = vec3( .25,.0,1. );
    mCellColors[1] = vec3( .5,.25,1. );
    mCellColors[2] = vec3( 1.,.75,1. );
    
    
    //setup camera
    mCameraDistance = 5.0f;
    mEye			= vec3( 0.0f, 0.0f, mCameraDistance );                 //position of the camera
	mCenter			= vec3( 0 );                                           //the point in space in which the camera is pointing
	mUp				= vec3( 0, 1, 0 );                                     //a normalized vector which tell the Camera which direction it should use as the up orientation (generally corresponds with the y-axis(most intuitive) but can use z axis)
    mCamera.setPerspective(60.0f, getWindowAspectRatio(), .1f, 10000.0f);
    //the above setup sets up a camera that is looking at the origin along the z-axis from a distance of 500 with an up orientation corresponding to the y-axis
    
    //setup params
    mParams = params::InterfaceGl::create( "Camera Movement", ivec2( 255, 200 ) );
	mParams->addParam( "Scene Rotation", &mSceneRotation, "opened=1" );
    //	mParams->addSeparator( "Separator", );
	mParams->addParam( "Eye Distance", &mCameraDistance, "min=1.0 max=50.0 step=1.0 keyIncr=s keyDecr=w" );
    //	mParams->addParam( "Center Gravity", &mCentralGravity, "keyIncr=g" );
    
    gl::enableDepthWrite();
    gl::enableDepthRead();
}


void CameraMove_test_092714App::keyUp( KeyEvent event )
{
    
    if( event.getChar() == '1' ){
        mCurrentModel = FLAT;
        
        mPlane->setGlslProg(mFlatShading);
        mSphere->setGlslProg(mFlatShading);
        
    }else if( event.getChar() == '2' ){
        mCurrentModel = PHONG;
        
        mPlane->setGlslProg(mPhongShading);
        mSphere->setGlslProg(mPhongShading);
        
    }else if( event.getChar() == '3' ){
        mCurrentModel = CELL;
        
        mPlane->setGlslProg(mCellShading);
        mSphere->setGlslProg(mCellShading);
    }
    
}

void CameraMove_test_092714App::mouseDown( MouseEvent event )
{
}

void CameraMove_test_092714App::update()
{
    //update lighting
    //    mLightPos = vec4( vec3(cos(getElapsedSeconds())*3, sin(getElapsedSeconds()*2)*3, sin(getElapsedSeconds())*3)*.5f, 1.);
    //        mLightPos = vec4( vec3(cos(getElapsedSeconds()), sin(getElapsedSeconds()*2), sin(getElapsedSeconds()*3))*.5f, 1.);
    
    //update camera
    float mappedSine = mCameraDistance*(lmap((float)sin(getElapsedSeconds()), (float)-1, (float)1, (float)0, (float)1.5)+0.28);
    mEye= vec3( 0.0f, 0.0f, mappedSine);
    mCamera.lookAt(mEye, mCenter, mUp);
    
    gl::setMatrices(mCamera);
//    gl::rotate(mSceneRotation);
}

void CameraMove_test_092714App::draw()
{
    
    gl::clear( Color( 0, 0, 0 ) );
    
    //draw params window
    //    mParams->draw();
    
    mPlane->getGlslProg()->uniform("lightPosition",  vec3( gl::getModelView() * mLightPos ) );
    
    if(mCurrentModel == CELL )
        mSphere->getGlslProg()->uniform("cells", mCellColors, 3 );
    
    /*{
     gl::ScopedMatrices push;
     gl::translate(vec3(2,0,0));
     gl::rotate((float)getElapsedSeconds()*.08 , vec3(1,1,1) );
     mPlane->draw();
     }*/
    
    
    {
        gl::ScopedMatrices push;
        gl::translate(vec3(0,0,0));
        gl::rotate( (float)getElapsedSeconds()*.07, vec3(-1,-1,-1) );
        mSphere->draw();
    }
    
    /* {
     gl::ScopedMatrices push;
     gl::translate(mLightPos.x, mLightPos.y, mLightPos.z);
     mLightBulb->draw();
     }*/
}

CINDER_APP_NATIVE( CameraMove_test_092714App, RendererGl )
