/*
 * Copyright (C) 2011 SVI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.github.sviengine.animation;

import android.util.Log;

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIParticleAnimation;
import com.github.sviengine.glsurface.SVIGLSurface;
import com.github.sviengine.particle.SVIParticleEffect;

// TODO: Auto-generated Javadoc
/**
 * SVIParticleAnimation Class
 * Particle Animation Class make Particle Animation.
 */
public class SVIParticleAnimation extends SVIAnimation {
	
	public SVIParticleAnimation() {
		this(null);
	}
	
	/**
     * constructor
     *
     * @see SVIParticleAnimation
     * @param info > particle information
     */
    SVIParticleAnimation(SVIGLSurface saGLSurface) {
    	super(saGLSurface);
        initializeSVIParticleAnimation();
    }
	
	
	/** 
	 * initialize
	 */
	public void initializeSVIParticleAnimation() {
		SVIEngineThreadProtection.validateMainThread();
		mNativeAnimation = nativeCreateSVIParticleAnimation(mSVIGLSurface.getNativeHandle());
	}
	
	
	public void setParticleEffect(SVIParticleEffect particleEffect) {
		SVIEngineThreadProtection.validateMainThread();
		if(particleEffect != null) {
    		if(particleEffect.getType() == SVIParticleEffect.ParticleEffectType.PARTICLE_EFFECT_DEFAULT) {
    			nativeSetParticleEffect( mNativeAnimation, particleEffect.getNativeHandle() );
    		}
    		else if(particleEffect.getType() == SVIParticleEffect.ParticleEffectType.PARTICLE_EFFECT_KEYFRAME) {
    			// TODO : change native call
    			nativeSetKeyFrameParticleEffect( mNativeAnimation, particleEffect.getNativeHandle() );
    		}
    		else {
    			Log.e("SVI", "ParticleEffectInfo's type not supported !!!");
    		}
		}
	}
	
	
	/**
	 * finalize
	 * 
	 * @see com.github.sviengine.animation.SVIAnimation#finalize()
	 */
	protected void finalize() throws Throwable {
		super.finalize();
		deleteNativeAnimationHandle();
    }
	
	
	/*****************************************************************************************************************/
	/**
	  * SVIParticleAnimation native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private static native void	nativeSetParticleEffect(int nativeAnimation, int effectHandle);
	
	private static native void 	nativeSetKeyFrameParticleEffect(int nativeAnimation, int effectHandle);
	
	private static native int 	nativeCreateSVIParticleAnimation(int nativeGLSurface);
}
