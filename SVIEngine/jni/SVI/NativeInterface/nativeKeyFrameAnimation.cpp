#include "nativeKeyFrameAnimation.h"
#include "../SVICores.h"
#include "../Animation/SVIKeyFrameAnimation.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_animation_SVIKeyFrameAnimation_nativeCreateSVIKeyFrameAnimation
	(JNIEnv * env, jclass jobject, jint type, jint surfaceHandle) {
	SVIGLSurface* saGLSurface =(SVIGLSurface*)surfaceHandle;
		
	jint animationAdress = 0;
	SVIKeyFrameAnimation* keyAni = 0;
	
	keyAni = SVIKeyFrameAnimation::create((SVIPropertyAnimation::PropertyAnimationType)type, saGLSurface);

	if (keyAni == 0) {
		LOGE("Can't create keyAnimation!");
		return -1;
	}
	else {
		animationAdress = (jint)keyAni;
		return animationAdress;
	}
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIKeyFrameAnimation_nativeAddKeyProperty
 	 (JNIEnv *env, jclass jobject, jint aniHandle, jint type, jfloat keyTime, jfloatArray property) {	

	if (aniHandle == 0) {
		LOGE("SVIKeyFrameAnimationJNILib_setKeyFrameProperty : aniHandle is null, error!!!");
		return;
	}

	SVIKeyFrameAnimation* keyAni = (SVIKeyFrameAnimation*)aniHandle;
	SVIFloat* data = env->GetFloatArrayElements(property, NULL);
	SVIFloat time = (SVIFloat)keyTime;

	switch (type) {
	case SVIPropertyAnimation::REGION:
		keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIRect(data[0], data[1], data[2], data[3])));
		break;
		case SVIPropertyAnimation::BOUND:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIRect(data[0], data[1], data[2], data[3])));
			break;
		case SVIPropertyAnimation::POSITION:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIPoint(data[0], data[1])));
			break;
		case SVIPropertyAnimation::PIVOT_POINT:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIPoint(data[0], data[1])));
			break;
		case SVIPropertyAnimation::ZPOSITION:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::ROTATION:			
			{
				SVIFloat x, y, z;

				x = SVI_DEGTORAD(data[0]);
				y = SVI_DEGTORAD(data[1]);
				z = SVI_DEGTORAD(data[2]);
				SVIQuaternion q;
				q.setIdentity();
				q.setEuler(SVIVector3(x, y, z));
				
				keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector4(q.x, q.y, q.z, q.w)));
			}
			break;
		case SVIPropertyAnimation::SCALE:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector3(data[0], data[1], data[2])));
			break;
		case SVIPropertyAnimation::BG_COLOR:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIColor(data[0], data[1], data[2], data[3])));
			break;
		case SVIPropertyAnimation::OPACITY:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::CORNER_RADIUS:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::BORDER_WIDTH:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::BORDER_COLOR:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIColor(data[0], data[1], data[2], data[3])));
			break;
		case SVIPropertyAnimation::SHADOW_RADIUS:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::SHADOW_COLOR:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIColor(data[0], data[1], data[2], data[3])));
			break;
		case SVIPropertyAnimation::SHADOW_OPACITY:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::SHADOW_OFFSET:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIPoint(data[0], data[1])));
			break;
		case SVIPropertyAnimation::LIGHT_OPACITY:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::LIGHT_ANGLE:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::LIGHT_POWER:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::LIGHT_RADIUS:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
			break;
		case SVIPropertyAnimation::LIGHT_COLOR:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIColor(data[0], data[1], data[2], data[3])));
			break;
		case SVIPropertyAnimation::LIGHT_OFFSET:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIPoint(data[0], data[1])));
			break;
		case SVIPropertyAnimation::TEXTURE_REGION:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIRect(data[0], data[1], data[2], data[3])));
			break;

		case SVIPropertyAnimation::BACKFACE_TEXTURE_REGION:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIRect(data[0], data[1], data[2], data[3])));
			break;
		case SVIPropertyAnimation::TEXTURE_SCALEFIT_REGION:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIRect(data[0], data[1], data[2], data[3])));
			break;

		//2012-06-21 masterkeaton27@gmail.com
		//new shader effect for mobile office
		case SVIPropertyAnimation::SWIPE_RATIO:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector3(data[0], data[1], data[2])));
			break;

		case SVIPropertyAnimation::DEFORM_RATIO:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector3(data[0], data[1], data[2])));
			break;

		//2012-01-19 masterkeaton27@gmail.com
		//missing rte ratio key insertion added.
		case SVIPropertyAnimation::RTE_RATIO:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector3(data[0], data[1], data[2])));
			break;

		case SVIPropertyAnimation::MODEL_POSITION:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector3(data[0], data[1], data[2])));
			break;
            
        //2012-11-21 masterkeaton27@gmail.com
        case SVIPropertyAnimation::LIGHT_DIRECTION:
            keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector4(data[0], data[1], data[2], data[3])));
            break;

		//2012-11-27 masterkeaton27@gmail.com
		case SVIPropertyAnimation::VIEW_DIRECTION:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector4(data[0], data[1], data[2], data[3])));
			break;

		case SVIPropertyAnimation::MODEL_ROTATION:			
			{
				SVIFloat x, y, z;

				x = SVI_DEGTORAD(data[0]);
				y = SVI_DEGTORAD(data[1]);
				z = SVI_DEGTORAD(data[2]);
				SVIQuaternion q;
				q.setIdentity();
				q.setEuler(SVIVector3(x, y, z));
				
				keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector4(q.x, q.y, q.z, q.w)));
			}
			break;

		case SVIPropertyAnimation::MODEL_SCALE:
			keyAni->addKeyProperty(SVIKeyFrameProperty(time, SVIVector3(data[0], data[1], data[2])));
			break;

        case SVIPropertyAnimation::MORPHING_TIME:
            keyAni->addKeyProperty(SVIKeyFrameProperty(time, data[0]));
            break;
		}
	
	env->ReleaseFloatArrayElements(property, data, 0);
}
