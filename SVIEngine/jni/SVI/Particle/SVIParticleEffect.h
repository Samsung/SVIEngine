#ifndef __SVI_PARTICLE_EFFECT_H_
#define __SVI_PARTICLE_EFFECT_H_

#include "../SVICores.h"

#include "SVIParticles.h"
#include <list>
#include <string>

namespace SVI {

	class SVIParticleKeyFrameAnimation;
	class SVIParticleUpdater;
	class SVIParticles;

	// Creation Date	: 2012/06/04 rareboy0112@facebook.com
	// Description		: particle effect class
	// Last Modified	: 2012/06/27 rareboy0112@facebook.com
	class SVIParticleEffect {

	public:
		typedef std::list<SVIParticles*> ParticlesList;

		enum ParticleEffectStatus {
			IDLE, 
			START, 
			ANIMATING, 
			END,
			STATUS_COUNT
		};

		SVIParticleEffect(SVIGLSurface* saGLSurface);

		virtual ~SVIParticleEffect();

		void						setDuration(SVIUInt duration);
		void						setPosition(SVIVector3 pos);
		void						setScale(SVIVector3 scale);
		void						setColor(SVIColor color);
		void						addParticles(SVIParticles* particles);

		void						setScaleKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset);
		void						setColorKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset);
		void						setPositionKeyFrameAnimationProperty(SVIUInt duration, SVIAnimation::InterpolatorType type, SVIInt reset);

		void						addScaleKeyFrame(SVIFloat keyTime, SVIVector3 scale);
		void						addColorKeyFrame(SVIFloat keyTime, SVIColor color);
		void						addPositionKeyFrame(SVIFloat keyTime, SVIVector3 position);

		const ParticlesList			getParticlesList() { return mParticlesList; }

		ParticleEffectStatus	    getStatus() { return mStatus; }

		void						checkMaxDuration(SVIUInt maxDuration);

		virtual void				update(SVIUInt drawTime);
		virtual void				updateFromAnimation(SVIUInt drawTime, SVIFloat interpolateTime, SVIUInt animationDuration);
		
		//2012-06-22 masterkeaton27@gmail.com
		void						setParentTransform(SVIMatrix * pTransform) {mParentTransform = pTransform;} 
		SVIMatrix *					getParentTransform() { return mParentTransform; }

		virtual SVIParticleEffect*	clone();

		void						stop();

	protected:
		SVIParticleEffect(const SVIParticleEffect& rhs){copy(&rhs);}
		void operator=(const SVIParticleEffect& rhs){copy(&rhs);}
	protected:
		void						copy(const SVIParticleEffect* rhs);

	protected:
		ParticleEffectStatus			mStatus;
		SVIBool							mIsAnimationUpdate;
		SVIUInt							mStartTime;

		SVIUInt							mDuration;
		SVIVector3						mPosition;
		SVIVector3						mScale;
		SVIColor							mColor;
		
		ParticlesList					mParticlesList;

		//2012-06-22 masterkeaton27@gmail.com
		SVIMatrix *						mParentTransform;
		
		//2012-06-13 masterkeaton27@gmail.com
		SVIParticleUpdater *				mParticleUpdater;

		SVIParticleKeyFrameAnimation*	mScaleKeyFrameAnimation;
		SVIParticleKeyFrameAnimation*	mColorKeyFrameAnimation;
		SVIParticleKeyFrameAnimation*	mPositionKeyFrameAnimation;

		SVIGLSurface* mSVIGLSurface;
	};

}; // end namespace SVI

#endif //__SVI_PARTICLE_EFFECT_H_
