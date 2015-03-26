#include "SVIParticleUpdater.h"
#include "SVILinearParticleUpdater.h"

#include "../SVIParticles.h"
#include "../SVIParticleEffect.h"

namespace SVI {	
	SVILinearParticleUpdater::SVILinearParticleUpdater(){

	}

	SVILinearParticleUpdater::~SVILinearParticleUpdater(){

	}

	void SVILinearParticleUpdater::update(SVIParticle * pParticle , SVIFloat fDeltaTime){
		SVIFloat fMass = pParticle->_mass;
		SVIVector3 vForce = pParticle->_force;
		SVIVector3 vAccel = vForce / fMass;
		SVIVector3 vDeltaVelocity = vAccel * fDeltaTime;
		pParticle->_velocity += vDeltaVelocity;
		pParticle->_position += pParticle->_velocity * fDeltaTime;
	}		

} // end namespace SVI