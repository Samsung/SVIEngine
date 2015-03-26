#pragma once

#include "../../SVICores.h"
#include <string>

#include "SVIParticleUpdater.h"

namespace SVI {
	struct SVIParticle;

	class SVILinearParticleUpdater : public SVIParticleUpdater {
	public:
		SVILinearParticleUpdater();
		~SVILinearParticleUpdater();

		virtual void update(SVIParticle * pParticle , SVIFloat fDeltaTime);
	}; 
	
} // end namespace SVI

