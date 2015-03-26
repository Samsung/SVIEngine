#pragma once

#include "../../SVICores.h"
#include "SVIParticleUpdater.h"
#include <string>

namespace SVI {
	struct SVIParticle;

	class SVIPhysicParticleUpdater : public SVIParticleUpdater{
	public:
		SVIPhysicParticleUpdater();
		~SVIPhysicParticleUpdater();

		virtual void update(SVIParticle * pParticle , SVIFloat fDeltaTime);
	}; 
	
} // end namespace SVI

