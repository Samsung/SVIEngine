#pragma once

#include "../../SVICores.h"
#include <string>

namespace SVI {
	struct SVIParticle;

	class SVIParticleUpdater {
	public:
		SVIParticleUpdater();
		~SVIParticleUpdater();

		virtual void update(SVIParticle * pParticle , SVIFloat fDeltaTime){}		
	}; 
	
} // end namespace SVI

