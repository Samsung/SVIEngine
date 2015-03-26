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

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.glsurface.SVIGLSurface;

public abstract class SVIPropertyAnimation extends SVIAnimation {

	protected int mAnimationType;
	
	/** PropertyAnimationType */
	public interface PropertyAnimationType {
		public static final int NONE = 0;
		public static final int REGION = 1;
		public static final int BOUND = 2;
		public static final int POSITION = 3;
		public static final int PIVOT_POINT = 4;
		public static final int ZPOSITION = 5;
		public static final int ROTATION = 6;
		public static final int SCALE = 7;
		public static final int BG_COLOR = 8;
		public static final int OPACITY = 9;
		public static final int CORNER_RADIUS = 10;
		public static final int BORDER_WIDTH = 11;
		public static final int BORDER_COLOR = 12;
		public static final int SHADOW_RADIUS = 13;
		public static final int SHADOW_COLOR = 14;
		public static final int SHADOW_OPACITY = 15;
		public static final int SHADOW_OFFSET = 16;
				
		//2011-08-29 masterkeaton27@gmail.com
		public static final int TEXTURE_REGION = 17;
		public static final int BACKFACE_TEXTURE_REGION = 18;
		
		//2011-08-26 masterkeaton27@gmail.com
		public static final int LIGHT_RADIUS = 19; 
		public static final int LIGHT_COLOR = 20;
		public static final int LIGHT_OPACITY = 21;
		public static final int LIGHT_ANGLE = 22;
		public static final int LIGHT_OFFSET = 23;
		public static final int LIGHT_POWER = 24;
		
				
		//2011-10-11 masterkeaton27@gmail.com
		public static final int SCALETOFIT_REGION = 25;
		public static final int ROTATION_FROM = 26;
		
		//2012-06-21 masterkeaton27@gmail.com
		public static final int SWIPE_RATIO = 27;
		
		public static final int DEFORM_RATIO = 28;
		
		//2012-01-02 phyuzion@gmail.com
		public static final int RTE_RATIO = 29;
		
		// 2011/11/30 rareboy0112@facebook.com
		public static final int MODEL_POSITION = 30;
		public static final int MODEL_ROTATION = 31;
		public static final int MODEL_SCALE = 32;
		
		//2012-11-21 masterkeaton27@gmail.com
		public static final int LIGHT_DIRECTION = 33;
		
		//2012-11-27 masterkeaton27@gmail.com
		public static final int VIEW_DIRECTION = 34;

		//2012/12/10 rareboy0112@facebook.com
		public static final int MORPHING_TIME = 35;
		
		public static final int PROPERTY_ANI_COUNT = 36;
	}
	
	SVIPropertyAnimation(){
		this(null);
	}
	
	SVIPropertyAnimation(SVIGLSurface saglSurface){
		super(saglSurface);
	}
}
