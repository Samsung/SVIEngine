#include "nativeBasicAnimation.h"
#include "../SVICores.h"
#include "../Animation/SVIBasicAnimation.h"

using namespace SVI;

JNIEXPORT jint JNICALL Java_com_github_sviengine_animation_SVIBasicAnimation_nativeCreateSVIBasicAnimation
    (JNIEnv * env, jclass jobject, jint type, jint surfaceHandle) {
    //LOGI("SVIBasicAnimationJNILib_create : type=%d", type);
    SVIGLSurface* saGLSurface =(SVIGLSurface*)surfaceHandle;

    jint animationAdress = 0;
    SVIBasicAnimation* basicAnimation = NULL;

    basicAnimation = SVIBasicAnimation::create((SVIPropertyAnimation::PropertyAnimationType)type, saGLSurface);

    if (basicAnimation == 0) {
        LOGE("Can't create basicAnimation!");
        return -1;
    }
    else {
        animationAdress = (jint)basicAnimation;
        return animationAdress;
    }
}

JNIEXPORT void JNICALL Java_com_github_sviengine_animation_SVIBasicAnimation_nativeSetValueSVIBasicAnimation
    (JNIEnv *env, jclass jobject, jint aniHandle, jint type, jfloatArray from, jfloatArray to) {

    if (aniHandle == 0) {
        LOGE("SVIBasicAnimationJNILib_setValue : aniHandle is null, error!!!");
        return;
    }

    //LOGI("SVIBasicAnimationJNILib_setValue : type=%d", type);

    SVIBasicAnimation* animation = (SVIBasicAnimation*)aniHandle;
    SVIFloat* fromData = env->GetFloatArrayElements(from, NULL);
    SVIFloat* toData = env->GetFloatArrayElements(to, NULL);

    switch (type) {
		case SVIPropertyAnimation::TEXTURE_SCALEFIT_REGION:
			animation->mFrom = SVIRect(fromData[0], fromData[1], fromData[2], fromData[3]);
			animation->mTo = SVIRect(toData[0], toData[1], toData[2], toData[3]);
			break;
        case SVIPropertyAnimation::TEXTURE_REGION:
            animation->mFrom = SVIRect(fromData[0], fromData[1], fromData[2], fromData[3]);
            animation->mTo = SVIRect(toData[0], toData[1], toData[2], toData[3]);
            break;

		case SVIPropertyAnimation::BACKFACE_TEXTURE_REGION:
			animation->mFrom = SVIRect(fromData[0], fromData[1], fromData[2], fromData[3]);
			animation->mTo = SVIRect(toData[0], toData[1], toData[2], toData[3]);
			break;

        case SVIPropertyAnimation::REGION:
            animation->mFrom = SVIRect(fromData[0], fromData[1], fromData[2], fromData[3]);
            animation->mTo = SVIRect(toData[0], toData[1], toData[2], toData[3]);
            break;
        case SVIPropertyAnimation::BOUND:
            animation->mFrom = SVIRect(fromData[0], fromData[1], fromData[2], fromData[3]);
            animation->mTo = SVIRect(toData[0], toData[1], toData[2], toData[3]);
            break;
        case SVIPropertyAnimation::POSITION:
            animation->mFrom = SVIPoint(fromData[0], fromData[1]);
            animation->mTo = SVIPoint(toData[0], toData[1]);
            break;
        case SVIPropertyAnimation::PIVOT_POINT:
            animation->mFrom = SVIPoint(fromData[0], fromData[1]);
            animation->mTo = SVIPoint(toData[0], toData[1]);
            break;
        case SVIPropertyAnimation::ZPOSITION:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::ROTATION:			
            {
            SVIFloat x, y, z;

            x = SVI_DEGTORAD(fromData[0]);
            y = SVI_DEGTORAD(fromData[1]);
            z = SVI_DEGTORAD(fromData[2]);
            SVIQuaternion q;
            q.setIdentity();
            q.setEuler(SVIVector3(x, y, z));
            animation->mFrom = SVIVector4(q.x, q.y, q.z, q.w);

            x = SVI_DEGTORAD(toData[0]);
            y = SVI_DEGTORAD(toData[1]);
            z = SVI_DEGTORAD(toData[2]);
            q.setIdentity();
            q.setEuler(SVIVector3(x, y, z));
            animation->mTo = SVIVector4(q.x, q.y, q.z, q.w);
            }
            break;
        case SVIPropertyAnimation::SCALE:
            animation->mFrom = SVIVector3(fromData[0], fromData[1], fromData[2]);
            animation->mTo = SVIVector3(toData[0], toData[1], toData[2]);
            break;
        case SVIPropertyAnimation::BG_COLOR:
            animation->mFrom = SVIColor(fromData[0], fromData[1], fromData[2], fromData[3]);
            animation->mTo = SVIColor(toData[0], toData[1], toData[2], toData[3]);
            break;
        case SVIPropertyAnimation::OPACITY:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::CORNER_RADIUS:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::BORDER_WIDTH:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::BORDER_COLOR:
            animation->mFrom = SVIColor(fromData[0], fromData[1], fromData[2], fromData[3]);
            animation->mTo = SVIColor(toData[0], toData[1], toData[2], toData[3]);
            break;
        case SVIPropertyAnimation::SHADOW_RADIUS:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::SHADOW_COLOR:
            animation->mFrom = SVIColor(fromData[0], fromData[1], fromData[2], fromData[3]);
            animation->mTo = SVIColor(toData[0], toData[1], toData[2], toData[3]);
            break;
        case SVIPropertyAnimation::SHADOW_OPACITY:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::SHADOW_OFFSET:
            animation->mFrom = SVIPoint(fromData[0], fromData[1]);
            animation->mTo = SVIPoint(toData[0], toData[1]);
            break;
        case SVIPropertyAnimation::LIGHT_OFFSET:
            animation->mFrom = SVIPoint(fromData[0], fromData[1]);
            animation->mTo = SVIPoint(toData[0], toData[1]);
            break;
        case SVIPropertyAnimation::LIGHT_OPACITY:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::LIGHT_ANGLE:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::LIGHT_RADIUS:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
        case SVIPropertyAnimation::LIGHT_COLOR:
            animation->mFrom = SVIColor(fromData[0], fromData[1], fromData[2], fromData[3]);
            animation->mTo = SVIColor(toData[0], toData[1], toData[2], toData[3]);
            break;
		case SVIPropertyAnimation::LIGHT_POWER:
			animation->mFrom = fromData[0];
			animation->mTo = toData[0];
			break;

		case SVIPropertyAnimation::SWIPE_RATIO:
			animation->mFrom = SVIVector3(fromData[0], fromData[1], fromData[2]);
			animation->mTo = SVIVector3(toData[0], toData[1], toData[2]);
			break;
			
		//2012-06-21 masterkeaton27@gmail.com
		//new shader effect for mobile office
		case SVIPropertyAnimation::DEFORM_RATIO:
			animation->mFrom = SVIVector3(fromData[0], fromData[1], fromData[2]);
			animation->mTo = SVIVector3(toData[0], toData[1], toData[2]);
			break;

		case SVIPropertyAnimation::RTE_RATIO:
			animation->mFrom = SVIVector3(fromData[0], fromData[1], fromData[2]);
			animation->mTo = SVIVector3(toData[0], toData[1], toData[2]);
			break;

		case SVIPropertyAnimation::MODEL_POSITION:
			animation->mFrom = SVIVector3(fromData[0], fromData[1], fromData[2]);
			animation->mTo = SVIVector3(toData[0], toData[1], toData[2]);
			break;

		case SVIPropertyAnimation::MODEL_ROTATION:			
            {
            SVIFloat x, y, z;

            x = SVI_DEGTORAD(fromData[0]);
            y = SVI_DEGTORAD(fromData[1]);
            z = SVI_DEGTORAD(fromData[2]);
            SVIQuaternion q;
            q.setIdentity();
            q.setEuler(SVIVector3(x, y, z));
            animation->mFrom = SVIVector4(q.x, q.y, q.z, q.w);

            x = SVI_DEGTORAD(toData[0]);
            y = SVI_DEGTORAD(toData[1]);
            z = SVI_DEGTORAD(toData[2]);
            q.setIdentity();
            q.setEuler(SVIVector3(x, y, z));
            animation->mTo = SVIVector4(q.x, q.y, q.z, q.w);
            }
            break;

		case SVIPropertyAnimation::MODEL_SCALE:
			animation->mFrom = SVIVector3(fromData[0], fromData[1], fromData[2]);
			animation->mTo = SVIVector3(toData[0], toData[1], toData[2]);
			break;

        case SVIPropertyAnimation::MORPHING_TIME:
            animation->mFrom = fromData[0];
            animation->mTo = toData[0];
            break;
    	}

    env->ReleaseFloatArrayElements(from, fromData, 0);
    env->ReleaseFloatArrayElements(to, toData, 0);
}
