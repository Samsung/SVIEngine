

#include "SVIParticleUpdater.h"
#include "SVIPhysicParticleUpdater.h"

#include "../SVIParticles.h"
#include "../SVIParticleEffect.h"

namespace SVI {	
	SVIPhysicParticleUpdater::SVIPhysicParticleUpdater(){

	}

	SVIPhysicParticleUpdater::~SVIPhysicParticleUpdater(){

	}

	void SVIPhysicParticleUpdater::update(SVIParticle * pParticle , SVIFloat fDeltaTime){
		SVIFloat fMass = pParticle->_mass;
		SVIVector3 vForce = pParticle->_force;
		SVIVector3 vAccel;
		if(fMass < 0.00001f) {
			vAccel = vForce;
		}
		else {
			vAccel = vForce / fMass;
		}
		SVIVector3 vGravity = pParticle->_gravity;

		SVIVector3 vDeltaVelocity = vAccel * fDeltaTime + vGravity * fDeltaTime;

		pParticle->_velocity += vDeltaVelocity;

		pParticle->_position += pParticle->_velocity * fDeltaTime;

		// 2012/07/30 rareboy0112@facebook.com
		// TODO: change logic
		//pParticle->_particleColor.mA -= 0.1f;
	}
} // end namespace SVI