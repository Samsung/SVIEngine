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

import com.github.sviengine.SVIEngineDesc;
import com.github.sviengine.SVIEngineThreadProtection;
import com.github.sviengine.glsurface.SVIGLSurface;

// TODO: Auto-generated Javadoc
/**
 * SVITransitionAnimation Class
 * Transition Animation Class make Transition Animation.
 */
public class SVITransitionAnimation extends SVIAnimation {

	/**
	 * TransitionAnimationType.
	 */
	public interface TransitionType {
		public static final int BLIND = 0;
		public static final int BREAK = 1;
		public static final int CURTAIN = 2;
		public static final int DOMINOFLIP = 3;
		public static final int TUMBLE = 4;
		public static final int WIZZLE = 5;
		public static final int ROTATE = 6;
		public static final int ROTATE_CUBE = 7;
		public static final int SIDEWINDOW = 8;
		public static final int SLIDE = 9;
		public static final int WAVE = 10;
		public static final int ZOOM_IN = 11;
		public static final int EXPLOID = 12;
		public static final int FLIP = 13;
		public static final int SCALE = 14;
		public static final int SMOOTH = 15;
		public static final int TWIST = 16;
		public static final int BOX = 17;
		public static final int CHECKER_BOARD = 18;
		public static final int COVER = 19;
		public static final int FLASH = 20;
		public static final int FLIP_2 = 21;
		public static final int FLY_THROUGH = 22;
		public static final int GALLERY = 23;
		public static final int HONEYCOMB = 24;
		public static final int REVEAL = 25;
		public static final int SHAPE = 26;
		public static final int SHRED = 27;
		public static final int SPLIT = 28;
		public static final int SWITCH = 29;
		public static final int UNCOVER = 30;
		public static final int WIPE = 31;
		public static final int CENTER_BLIND = 32;
		public static final int FADE_THROUGH_COLOR	=33;
		public static final int FALL = 34;
		public static final int MOVE_IN = 35;
		public static final int REVOLVING_DOOR = 36;
		public static final int SWAP = 37;
		public static final int SWOOSH = 38;
		public static final int TWIRL = 39;
		public static final int BRICK_CUBE = 40;
		public static final int CUBE_2PIECES = 41;
		public static final int CUBE_4PIECES = 42;
		public static final int FOLDING_SCREEN = 43;
		public static final int GALLERY_2 = 44;
	};
	
	
	/**
	 * DirectionType.
	 */
	public interface DirectionType {
		public static final int LEFT = 0;
		public static final int RIGHT = 1;
		public static final int UP = 2;
		public static final int DOWN = 3;
	}
	
	
	/**
	 * create
	 * @see TransitionType
	 * @param type > int type
	 * @return SVITransitionAnimation
	 */
	static public SVITransitionAnimation createAnimation(int type) {
		return createAnimation(null, type);
	}
	
	static SVITransitionAnimation createAnimation(SVIGLSurface saglSurface, int type) {		
		SVITransitionAnimation transAni = new SVITransitionAnimation(saglSurface);
		transAni.mTransitionType = type;
		transAni.initializeSVITransitionAnimation();
		return transAni;
	}
	
	public SVITransitionAnimation() {
		this(null);
	}
	
	public SVITransitionAnimation(int transitionType){
		this(null, transitionType);
	}
	
	SVITransitionAnimation(SVIGLSurface saglSurface) {
		super(saglSurface);
	}
	
	SVITransitionAnimation(SVIGLSurface saglSurface, int transitionType){
		this(saglSurface);
		mTransitionType = transitionType;
		mNativeAnimation = nativeCreateSVITransitionAnimation(mTransitionType,mSVIGLSurface.getNativeHandle());
	}

	/** 
	 * initialize
	 */
	public void initializeSVITransitionAnimation() {
		mNativeAnimation = nativeCreateSVITransitionAnimation(mTransitionType, mSVIGLSurface.getNativeHandle());
	}
	
	
	/** 
	 * set the transaction type
	 */
	public void setTransitionType(int transitionType) {
		SVIEngineThreadProtection.validateMainThread();

		mTransitionType = transitionType;
		mNativeAnimation = nativeCreateSVITransitionAnimation(mTransitionType,mSVIGLSurface.getNativeHandle());
	}

	/**
	 * set DirectionType
	 * 
	 * @see DirectionType 
	 * @param directionType > int direction type
	 * @return > boolean false = wrong direction
	 */
	public boolean setDirectionType(int directionType) {
		SVIEngineThreadProtection.validateMainThread();
		if(directionType < 0 && directionType > 3) {
			return false;
		}
		
		mDirectionType = directionType;
		
		nativeSetDirectionTypeSVITransitionAnimation(mNativeAnimation, directionType);
		
		return true;
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
	
	
	private int mTransitionType;
	protected int mDirectionType;
	
	/*****************************************************************************************************************/
	/**
	  * SVITransitionAnimation native interface.
	  *
	  */
	static {
		System.loadLibrary(SVIEngineDesc.mName);
	}
	
	private static native int nativeCreateSVITransitionAnimation(int type, int nativeGLSurface);
	private static native void nativeSetDirectionTypeSVITransitionAnimation(int aniHandle, int directionType);
}
