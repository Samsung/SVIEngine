#include "SVIKeyFrameParticles.h"
#include "SVIParticleEffectManager.h"
#include "./Updater/SVIParticleUpdater.h"
#include "../Animation/SVIParticleKeyFrameAnimation.h"

namespace SVI {

SVIKeyFrameParticles::SVIKeyFrameParticles(SVIGLSurface* surface)
	: SVIParticles(surface),
	mKeyTime(0.0f) {
	mSVIGLSurface  = surface;
	mGeneratorPosition.x = mGeneratorPosition.y = mGeneratorPosition.z = 0.0f;
}


void SVIKeyFrameParticles::setKeyTime(SVIFloat keyTime) {
	mKeyTime = keyTime;
}


SVIParticles* SVIKeyFrameParticles::clone() {
	SVIKeyFrameParticles* cloneParticles = new SVIKeyFrameParticles(mSVIGLSurface);
	cloneParticles->copy(this);
	
	return cloneParticles;
}


void SVIKeyFrameParticles::copy(const SVIKeyFrameParticles* rhs) {
	SVIParticles::copy(rhs);
	mKeyTime = rhs->mKeyTime;
}


void SVIKeyFrameParticles::setGeneratorPosition(SVIVector3 pos) {
	mGeneratorPosition = pos;
}


void SVIKeyFrameParticles::validate() {
	mParticles = new SVIParticle[mParticleCount];

	SVIInt randomValue = 0;
	SVIInt changeValue = 0;

	for(SVIUInt i = 0; i < mParticleCount; ++i) {
		mParticles[i]._active = SVITRUE;

		// set duration
		if(mRandomDuration > 0) {
			randomValue = rand() % mRandomDuration;
			mParticles[i]._life = (mMaxDuration - mRandomDuration) + randomValue;
		}
		else {
			mParticles[i]._life = mMaxDuration;
		}

		if( (SVIInt)mMaxDuration - (SVIInt)mRandomDuration + randomValue < 0 ) {
			mParticles[i]._life = 0;
		}

		// set vertex position
		SVIFloat x, y, z;
		if(mRandomPosition.x > 0.0f) {
			changeValue = SVIInt(mRandomPosition.x * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			x = mDefaultPosition.x + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			x = mDefaultPosition.x;
		}
		if(mRandomPosition.y > 0.0f) {
			changeValue = SVIInt(mRandomPosition.y * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			y = mDefaultPosition.y + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			y = mDefaultPosition.y;
		}
		if(mRandomPosition.z > 0.0f) {
			changeValue = SVIInt(mRandomPosition.z * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			z = mDefaultPosition.z + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			z = mDefaultPosition.z;
		}
		
		mParticles[i]._originPosition.x = x + mGeneratorPosition.x;
		mParticles[i]._originPosition.y = y + mGeneratorPosition.y;
		mParticles[i]._originPosition.z = z + mGeneratorPosition.z;
		
		mParticles[i]._position = mParticles[i]._originPosition + mParentPosition;

		// set color
		if(mRandomColor.mR > 0.0f) {
			changeValue = SVIInt(mRandomColor.mR * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mR = (mMaxColor.mR - mRandomColor.mR) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mR = mMaxColor.mR;
		}
		if(mParticles[i]._originParticleColor.mR > 1.0f) {
			mParticles[i]._originParticleColor.mR = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mR < 0.0f) {
			mParticles[i]._originParticleColor.mR = 0.0f;
		}

		if(mRandomColor.mG > 0.0f) {
			changeValue = SVIInt(mRandomColor.mG * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mG = (mMaxColor.mG - mRandomColor.mG) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mG = mMaxColor.mG;
		}
		if(mParticles[i]._originParticleColor.mG > 1.0f) {
			mParticles[i]._originParticleColor.mG = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mG < 0.0f) {
			mParticles[i]._originParticleColor.mG = 0.0f;
		}

		if(mRandomColor.mB > 0.0f) {
			changeValue = SVIInt(mRandomColor.mB * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mB = (mMaxColor.mB - mRandomColor.mB) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mB = mMaxColor.mB;
		}
		if(mParticles[i]._originParticleColor.mB > 1.0f) {
			mParticles[i]._originParticleColor.mB = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mB < 0.0f) {
			mParticles[i]._originParticleColor.mB = 0.0f;
		}

		if(mRandomColor.mA > 0.0f) {
			changeValue = SVIInt(mRandomColor.mA * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originParticleColor.mA = (mMaxColor.mA - mRandomColor.mA) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originParticleColor.mA = mMaxColor.mA;
		}
		if(mParticles[i]._originParticleColor.mA > 1.0f) {
			mParticles[i]._originParticleColor.mA = 1.0f;
		}
		else if(mParticles[i]._originParticleColor.mA < 0.0f) {
			mParticles[i]._originParticleColor.mA = 0.0f;
		}

		mParticles[i]._particleColor.mR = mParticles[i]._originParticleColor.mR * mParentColor.mR;
		mParticles[i]._particleColor.mG = mParticles[i]._originParticleColor.mG * mParentColor.mG;
		mParticles[i]._particleColor.mB = mParticles[i]._originParticleColor.mB * mParentColor.mB;
		mParticles[i]._particleColor.mA = mParticles[i]._originParticleColor.mA * mParentColor.mA;

		// set gravity
		if(mRandomGravity.x > 0.0f) {
			changeValue = SVIInt(mRandomGravity.x * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._gravity.x = mDefaultGravity.x + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._gravity.x = mDefaultGravity.x;
		}
		
		if(mRandomGravity.y > 0.0f) {
			changeValue = SVIInt(mRandomGravity.y * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._gravity.y = mDefaultGravity.y + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._gravity.y = mDefaultGravity.y;
		}

		if(mRandomGravity.z > 0.0f) {
			changeValue = SVIInt(mRandomGravity.z * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._gravity.z = mDefaultGravity.z + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._gravity.z = mDefaultGravity.z;
		}

		// set force
		if(mRandomForce.x > 0.0f) {
			changeValue = SVIInt(mRandomForce.x * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._force.x = mDefaultForce.x + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._force.x = mDefaultForce.x;
		}

		if(mRandomForce.y > 0.0f) {
			changeValue = SVIInt(mRandomForce.y * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._force.y = mDefaultForce.y + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._force.y = mDefaultForce.y;
		}

		if(mRandomForce.z > 0.0f) {
			changeValue = SVIInt(mRandomForce.z * 1000);
			randomValue = rand() % changeValue;
			randomValue -= (rand() % changeValue);
			mParticles[i]._force.z = mDefaultForce.z + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._force.z = mDefaultForce.z;
		}

		// set mass
		if(mRandomMass > 0.0f) {
			changeValue = SVIInt(mRandomMass * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._mass = (mMaxMass - mRandomMass) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._mass = mMaxMass;
		}

		// set size
		if(mRandomParticleSize.x > 0.0f) {
			changeValue = SVIInt(mRandomParticleSize.x * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originSize.x = (mMaxParticleSize.x - mRandomParticleSize.x) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originSize.x = mMaxParticleSize.x;
		}

		if(mParticles[i]._originSize.x < 0.0f) {
			mParticles[i]._originSize.x = 0.0f;
		}
		if(mRandomParticleSize.y > 0.0f) {
			changeValue = SVIInt(mRandomParticleSize.y * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originSize.y = (mMaxParticleSize.y - mRandomParticleSize.y) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originSize.y = mMaxParticleSize.y;
		}

		if(mParticles[i]._originSize.y < 0.0f) {
			mParticles[i]._originSize.y = 0.0f;
		}
		if(mRandomParticleSize.z > 0.0f) {
			changeValue = SVIInt(mRandomParticleSize.z * 1000);
			randomValue = rand() % changeValue;
			mParticles[i]._originSize.z = (mMaxParticleSize.z - mRandomParticleSize.z) + (SVIFloat)randomValue / 1000.0f;
		}
		else {
			mParticles[i]._originSize.z = mMaxParticleSize.z;
		}

		if(mParticles[i]._originSize.z < 0.0f) {
			mParticles[i]._originSize.z = 0.0f;
		}

		mParticles[i]._size = mParticles[i]._originSize * mParentScale;
	}

	if(mScaleKeyFrameAnimation != NULL && mMaxDuration < mScaleKeyFrameAnimation->mDuration) {
		mScaleKeyFrameAnimation->mDuration = mMaxDuration;
	}
	if(mColorKeyFrameAnimation != NULL && mMaxDuration < mColorKeyFrameAnimation->mDuration) {
		mColorKeyFrameAnimation->mDuration = mMaxDuration;
	}
	if(mPositionKeyFrameAnimation != NULL && mMaxDuration < mPositionKeyFrameAnimation->mDuration) {
		mPositionKeyFrameAnimation->mDuration = mMaxDuration;
	}
	
	mValidated = SVITRUE;
}


void SVIKeyFrameParticles::update(SVIUInt drawTime, SVIUInt effectStartTime, SVIUInt effectDuration) {
	
	if(mStatus == IDLE) {
		SVIFloat keyTime = (SVIFloat)(drawTime - effectStartTime) / (SVIFloat)effectDuration;
		if(mKeyTime > keyTime) {
			return;
		}
		else {
			SVIFloat gapKeyTime = keyTime - mKeyTime;
			SVIUInt gapDuration = (SVIFloat)effectDuration * gapKeyTime;
			mMaxDuration -= gapDuration;
			SVIUInt availableTime = (SVIFloat)effectDuration * (1.0f - keyTime);
			if(mMaxDuration > availableTime) {
				mMaxDuration = availableTime;
			}
		}
		if(mMaxDuration > 0) {
			validate();
			mStatus = ANIMATING;
			mStartTime = drawTime;
		}
		else {
			mStatus = END;
			return;
		}
	}
	else if(mStatus == END) {
		return;
	}
	
	if(mUseTexture == SVITRUE) {
		// TODO: bind texture
		// textureFileName = mTextureFileName
	}

	if(mStatus == ANIMATING) {
		SVIUInt activeTime = drawTime - mStartTime;
		SVIFloat fDeltaTime = activeTime / 1000.0f;

		if(mScaleKeyFrameAnimation != NULL) {
			mScaleKeyFrameAnimation->update(drawTime);
		}
		if(mColorKeyFrameAnimation != NULL) {
			mColorKeyFrameAnimation->update(drawTime);
		}
		if(mPositionKeyFrameAnimation != NULL) {
			mPositionKeyFrameAnimation->update(drawTime);
		}

		if (m_pParticleUpdater != NULL){
			for(SVIUInt i = 0; i < mParticleCount; ++i) {
				m_pParticleUpdater->update(&mParticles[i], fDeltaTime);
			}
		}
	}

	if(mStartTime + mMaxDuration < drawTime) {
		mStatus = END;
		// TODO: add kill code !!!
	}
}


void SVIKeyFrameParticles::updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration) {
	if(mStatus == IDLE) {
		if(mKeyTime > interpolateTime) {
			return;
		}
		else {
			SVIFloat gapKeyTime = interpolateTime - mKeyTime;
			SVIUInt gapDuration = (SVIFloat)animationDuration * gapKeyTime;
			mMaxDuration -= gapDuration;
			SVIUInt availableTime = (SVIFloat)animationDuration * (1.0f - interpolateTime);
			if(mMaxDuration > availableTime) {
				mMaxDuration = availableTime;
			}
		}
		if(mMaxDuration > 0) {
			validate();
			mStatus = ANIMATING;
			mStartTime = drawTime;
		}
		else {
			mStatus = END;
			return;
		}
	}
	else if(mStatus == END) {
		return;
	}
	
	if(mUseTexture == SVITRUE) {
		// TODO: bind texture
		// textureFileName = mTextureFileName
	}

	if(mStatus == ANIMATING) {
		SVIUInt activeTime = drawTime - mStartTime;
		SVIFloat fDeltaTime = activeTime / 1000.0f;

		if(mScaleKeyFrameAnimation != NULL) {
			mScaleKeyFrameAnimation->update(drawTime);
		}
		if(mColorKeyFrameAnimation != NULL) {
			mColorKeyFrameAnimation->update(drawTime);
		}
		if(mPositionKeyFrameAnimation != NULL) {
			mPositionKeyFrameAnimation->update(drawTime);
		}

		if (m_pParticleUpdater != NULL){
			for(SVIUInt i = 0; i < mParticleCount; ++i) {
				m_pParticleUpdater->update(&mParticles[i], fDeltaTime);
			}
		}
	}

	if(mStartTime + mMaxDuration < drawTime) {
		mStatus = END;
		// TODO: add kill code !!!
	}
}


} // end namespace SVI