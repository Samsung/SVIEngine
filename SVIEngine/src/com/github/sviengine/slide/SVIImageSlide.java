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
package com.github.sviengine.slide;

import com.github.sviengine.glsurface.SVIGLSurface;

import android.graphics.Bitmap;

/**
 * @author G-SUN
 *
 */
public class SVIImageSlide extends SVISlide{
	
	public SVIImageSlide() {
		this((SVIGLSurface)null);
	}

	public SVIImageSlide(Bitmap bitmap) {
		this(null, bitmap);
	}
	
	/**
	 * constructor.
	 * create default image slide.
	 */
	SVIImageSlide(SVIGLSurface surface) {
		super(surface, SVISlide.SlideType.IMAGE_SLIDE);
	}
	
	/**
	 * constructor.
	 * create default image slide.
	 * @param bitmap
	 */
	SVIImageSlide(SVIGLSurface surface, Bitmap bitmap) {
		super(surface, SVISlide.SlideType.IMAGE_SLIDE, bitmap);
	}
}
