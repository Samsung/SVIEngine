#include "../SVICores.h"
#include "SVIViewport.h"
#include "SVIRenderer.h"

namespace SVI{
	

SVIViewport::SVIViewport(SVIGLSurface* surface) { // Multi-Instance Support
	mProjection.setIdentity(); 
	mModelView.setIdentity();
	mModelViewProjection.setIdentity();

	mViedWidth = 600.0f;
	mViedHeight = 800.0f;

	mNear = -1000.0f;
	mFar = 5000.0f;

	mFieldOfView = SVI_DEGTORAD(60.0f);
	mSVIGLSurface  = surface;
}

SVIViewport::~SVIViewport(){
	mSVIGLSurface  = NULL;
}

void SVIViewport::initialize(){
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_SVIMPLE_ALPHA_TO_COVERAGE);
	//glEnable(GL_SVIMPLE_COVERAGE);
	//glSampleCoverage(0.25f,GL_FALSE);
	

	//2011-07-04 masterkeaton27@gmail.com
	//disable dithering
	glDisable(GL_DITHER);

	//glEnable(GL_DEPTH_TEST);
	
	//2011-06-27 masterkeaton27@gmail.com
	//ES 2.0 does not support these options.  
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 

	initializeOrtho(800.0f,600.0f,-1000.0f,1000.0f);

}

void lookAtMatrix( SVIMatrix * m, SVIVector3 eye, 
	SVIVector3 at, SVIVector3 up){

		SVIVector3 XAxis, YAxis, ZAxis;

		ZAxis = at - eye;
		ZAxis.normalize();
		XAxis = up.Cross(ZAxis);
		XAxis.normalize();
		YAxis = ZAxis.Cross(XAxis);

		m->_11 = XAxis.x;
		m->_12 = XAxis.y;
		m->_13 = XAxis.z;
		m->_14 = -XAxis.dot(eye);

		m->_21 = YAxis.x;
		m->_22 = YAxis.y;
		m->_23 = YAxis.z;
		m->_24 = -YAxis.dot(eye);

		m->_31 = ZAxis.x;
		m->_32 = ZAxis.y;
		m->_33 = ZAxis.z;
		m->_34 = -ZAxis.dot(eye);

		m->_41 = eye.x;
		m->_42 = eye.y;
		m->_43 = eye.z;
		m->_44 = 1.0f;
}

void SVIViewport::setOrtho(SVIFloat width, SVIFloat height, SVIFloat near, SVIFloat far, SVIBool bReverse){
	mViedWidth = width;
	mViedHeight = height;
	mNear = near;
	mFar = far;

	
	mModelView.setIdentity();
	if (bReverse){
		mProjection = SVIMatrix::ortho(0.0f,0.0f,mViedWidth,mViedHeight,mNear,mFar);
	}else mProjection = SVIMatrix::ortho(0.0f,mViedHeight,mViedWidth,0.0f,mNear,mFar);
	

	//2011-06-15 masterkeaton27@gmail.com
	//set perspective factor
	mProjection._34 = 1.0f / 900.0f;
	mModelViewProjection = mModelView * mProjection;
}

void SVIViewport::setProjectionRatio(SVIFloat  value){
	mProjection._34 = value;
}

void SVIViewport::initializeOrtho(SVIFloat width, SVIFloat height, SVIFloat near, SVIFloat far) {
	mViedWidth = width;
	mViedHeight = height;
	mNear = near;
	mFar = far;

	
	mModelView.setIdentity();
	mProjection = SVIMatrix::ortho(0.0f,mViedHeight,mViedWidth,0.0f,mNear,mFar);

	//2011-06-15 masterkeaton27@gmail.com
	//set perspective factor
	mProjection._34 = 1.0f / 900.0f;
	/*
	mProjection = SVIMatrix::perspective(SVI_PI/2.0f,width/height,mNear,mFar);
	lookAtMatrix(&mModelView,SVIVector3(0.0f,0.0f,400.0f),SVIVector3(0.0f,0.0f,0.0f),SVIVector3(0.0f,1.0f,0.0));
	*/

	mModelViewProjection = mModelView * mProjection;

	glViewport(0, 0, mViedWidth, mViedHeight);

}

void SVIViewport::restore(){
	
}

void SVIViewport::update(){

}

void SVIViewport::resize(SVIFloat width, SVIFloat height){
	mViedWidth = width;
	mViedHeight = height;	
	initializeOrtho(mViedWidth,mViedHeight,mNear,mFar);
}

void SVIViewport::finalize(){

}


}
