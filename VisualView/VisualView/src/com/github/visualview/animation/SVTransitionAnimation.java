/*
 * Copyright (C) 2013 SVV.
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
package com.github.visualview.animation;

import com.github.sviengine.animation.SVITransitionAnimation;

/**
 * This Class is responsible to make all the transition animation.
 */
public class SVTransitionAnimation extends SVAnimation{

	/** Blind TransitionAnimation */
	public static final int TRANSITION_BLIND	=	SVITransitionAnimation.TransitionType.	BLIND; // 0
	/** Break TransitionAnimation */
	public static final int TRANSITION_BREAK	=	SVITransitionAnimation.TransitionType.	BREAK;  //1;
	/** Curtain TransitionAnimation*/
	public static final int TRANSITION_CURTAIN	=	SVITransitionAnimation.TransitionType.CURTAIN;  //2;
	/** Tumble TransitionAnimation */
	public static final int TRANSITION_TUMBLE	=	SVITransitionAnimation.TransitionType.	TUMBLE;  //8;
	/** Wizzle TransitionAnimation */
	public static final int TRANSITION_WIZZLE	=	SVITransitionAnimation.TransitionType.	WIZZLE;  //10;
	/** Rotate TransitionAnimation */
	public static final int TRANSITION_ROTATE	=	SVITransitionAnimation.TransitionType.	ROTATE;  //11;
	/** Rotate Cube TransitionAnimation */
	public static final int TRANSITION_ROTATE_CUBE	=	SVITransitionAnimation.TransitionType.	ROTATE_CUBE;  //12;
	/** Side Window TransitionAnimation */
	public static final int TRANSITION_SIDE_WINDOW	=	SVITransitionAnimation.TransitionType.	SIDEWINDOW;  //13;
	/** Slide TransitionAnimation */
	public static final int TRANSITION_SLIDE	=	SVITransitionAnimation.TransitionType.	SLIDE;  //14;
	/**Wave TransitionAnimation */
	public static final int TRANSITION_WAVE	=	SVITransitionAnimation.TransitionType.	WAVE;  //15;
	/** Zoom In TransitionAnimation*/
	public static final int TRANSITION_ZOOM_IN	=	SVITransitionAnimation.TransitionType.	ZOOM_IN;  //16;
	/** Exploid TransitionAnimation */
	public static final int TRANSITION_EXPLOID	=	SVITransitionAnimation.TransitionType.	EXPLOID;  //17;
	/** Flip TransitionAnimation */
	public static final int TRANSITION_FLIP	=	SVITransitionAnimation.TransitionType.	FLIP;  //18;
	/** Scale TransitionAnimation */
	public static final int TRANSITION_SCALE	=	SVITransitionAnimation.TransitionType.	SCALE;  //19;
	/** Smooth TransitionAnimation */
	public static final int TRANSITION_SMOOTH	=	SVITransitionAnimation.TransitionType.	SMOOTH;  //20;
	/** Box TransitionAnimation */
	public static final int TRANSITION_BOX	=	SVITransitionAnimation.TransitionType.	BOX;  //22;
	/** Cover TransitionAnimation */
	public static final int TRANSITION_COVER	=	SVITransitionAnimation.TransitionType.	COVER;  //24;
	/** Flash TransitionAnimation */
	public static final int TRANSITION_FLASH	=	SVITransitionAnimation.TransitionType.	FLASH;  //26;
	/** Flip TransitionAnimation */
	public static final int TRANSITION_FLIP_2	=	SVITransitionAnimation.TransitionType.	FLIP_2;  //27;
	/** Fly through TransitionAnimation */
	public static final int TRANSITION_FLY_THROUGH	=	SVITransitionAnimation.TransitionType.	FLY_THROUGH;  //28;
	/** Gallery TransitionAnimation */
	public static final int TRANSITION_GALLERY	=	SVITransitionAnimation.TransitionType.	GALLERY;  //29;
	/** HoneyComb TransitionAnimation */
	public static final int TRANSITION_HONEYCOMB	=	SVITransitionAnimation.TransitionType.	HONEYCOMB;  //30;
	/** Reveal TransitionAnimation */
	public static final int TRANSITION_REVEAL	=	SVITransitionAnimation.TransitionType.	REVEAL;  //31;
	/** Shape TransitionAnimation */
	public static final int TRANSITION_SHAPE	=	SVITransitionAnimation.TransitionType.	SHAPE;  //32;
	/** Shred TransitionAnimation */
	public static final int TRANSITION_SHRED	=	SVITransitionAnimation.TransitionType.	SHRED;  //33;
	/** Switch TransitionAnimation*/
	public static final int TRANSITION_SWITCH	=	SVITransitionAnimation.TransitionType.	SWITCH;  //35;
	/** Uncover TransitionAnimation */
	public static final int TRANSITION_UNCOVER	=	SVITransitionAnimation.TransitionType.	UNCOVER;  //36;
	/** Wipe TransitionAnimation */
	public static final int TRANSITION_WIPE	=	SVITransitionAnimation.TransitionType.	WIPE;  //37;
	/** Center blind TransitionAnimation */
	public static final int TRANSITION_CENTER_BLIND = SVITransitionAnimation.TransitionType.CENTER_BLIND; //38;
	/** Fade through Color TransitionAnimation */
	public static final int TRANSITION_FADE_THROUGH_COLOR = SVITransitionAnimation.TransitionType.FADE_THROUGH_COLOR;   //39;
	/** Fall TransitionAnimation */
	public static final int TRANSITION_FALL	=	SVITransitionAnimation.TransitionType.	FALL;  //40;
	/** Move In TransitionAnimation */
	public static final int TRANSITION_MOVE_IN	=	SVITransitionAnimation.TransitionType.	MOVE_IN;  //41;
	/** Revolving door TransitionAnimation */
	public static final int TRANSITION_REVOLVING_DOOR	=	SVITransitionAnimation.TransitionType.	REVOLVING_DOOR;  //42;
	/** Swap TransitionAnimation */
	public static final int TRANSITION_SWAP	=	SVITransitionAnimation.TransitionType.	SWAP;  //43;
	/** Swoosh TransitionAnimation */
	public static final int TRANSITION_SWOOSH	=	SVITransitionAnimation.TransitionType.	SWOOSH;  //44;
	/** Twirl TransitionAnimation */
	public static final int TRANSITION_TWIRL	=	SVITransitionAnimation.TransitionType.	TWIRL;  //45;
	/** Brick Cube TransitionAnimation */
	public static final int TRANSITION_BRICK_CUBE	=	SVITransitionAnimation.TransitionType.	BRICK_CUBE;  //46;
	/** 2 Piece Cube TransitionAnimation  */
	public static final int TRANSITION_CUBE_2PIECES =  SVITransitionAnimation.TransitionType.CUBE_2PIECES; //47
	/** Folding Screen TransitionAnimation */
	public static final int TRANSITION_FOLDING_SCREEN = SVITransitionAnimation.TransitionType.FOLDING_SCREEN; //49
	/** Gallery 2 TransitionAnimation */
	public static final int TRANSITION_GALLERY_2	=	SVITransitionAnimation.TransitionType.	GALLERY_2;  //50;
	
	/**
	 * DirectionType.
	 */
		
	/** Transition towards the left */
	public static final int DIRECTION_LEFT = SVITransitionAnimation.DirectionType.LEFT;
	
	/** Transition towards the Right */
	public static final int DIRECTION_RIGHT = SVITransitionAnimation.DirectionType.RIGHT;
	
	/** Transition towards up. */
	public static final int DIRECTION_UP = SVITransitionAnimation.DirectionType.UP;
	
	/** Transition towards the downward direction */
	public static final int DIRECTION_DOWN = SVITransitionAnimation.DirectionType.DOWN;
	
	SVITransitionAnimation mTransitionAnimation;
	
	
	/**
	 * Creates an instance of SVTransitionAnimation with the specified transition and direction types.
	 *
	 * @param transitionType The type of transition used, see the constants for more information.
	 * @param directionType The direction the transition goes in, see the constants for more information.
	 * @throws IllegalArgumentException if invalid transition type or direction type
	 * @see SVTransitionAnimation
	 */
	public SVTransitionAnimation(int transitionType, int directionType){
		
		if(!validateTransitionType(transitionType))
			throw new IllegalArgumentException("Transition Type is not Valid..!!");
		if(!validateDirectionType(directionType))
			throw new IllegalArgumentException("Direction type is not Valid..!!");
		mAnimation = new SVITransitionAnimation(transitionType);
		mTransitionAnimation = (SVITransitionAnimation)mAnimation;
		mTransitionAnimation.setDirectionType(directionType);
	}
	
	/**
	 * Set the direction the transition animation goes in.
	 *
	 * @param directionType The direction the transition goes in, see the constants for more information.
	 * @throws IllegalArgumentException if the direction type is invalid, see the constants for more information.
	 */
	
	public void setDirectionType(int directionType){
		if(!validateDirectionType(directionType))
			throw new IllegalArgumentException("Direction type is not Valid..!!");
		if(mTransitionAnimation != null){
			 mTransitionAnimation.setDirectionType(directionType);
		}
	}
	
	/**
	 * Set the type of transition the animation uses.
	 *
	 * @param transitionType The transition animation type, see the constants for more information.
	 * @throws IllegalArgumentException if the transition type is invalid, see the constants for more information.
	 */
	
	public void setTransitionType(int transitionType){
		
		if(!validateTransitionType(transitionType))
			throw new IllegalArgumentException("Transition Type is not Valid..!!");
		if(mTransitionAnimation != null){
			mTransitionAnimation.setTransitionType(transitionType);
		}
	}
	
	private boolean validateDirectionType(int directionType){
		if(directionType == DIRECTION_LEFT
				||directionType == DIRECTION_RIGHT
				||directionType == DIRECTION_DOWN
				||directionType == DIRECTION_UP){
			return true;
			
		}else
			return false;
	}
	
	private boolean validateTransitionType(int type){
		
		if(type == TRANSITION_BLIND
				|| type == 		TRANSITION_BREAK
				|| type == 		TRANSITION_CURTAIN
				|| type == 		TRANSITION_TUMBLE
				|| type == 		TRANSITION_WIZZLE
				|| type == 		TRANSITION_ROTATE
				|| type == 		TRANSITION_ROTATE_CUBE
				|| type == 		TRANSITION_SIDE_WINDOW
				|| type == 		TRANSITION_SLIDE
				|| type == 		TRANSITION_WAVE
				|| type == 		TRANSITION_ZOOM_IN
				|| type == 		TRANSITION_EXPLOID
				|| type == 		TRANSITION_FLIP
				|| type == 		TRANSITION_SCALE
				|| type == 		TRANSITION_BOX
				|| type == 		TRANSITION_SMOOTH
				|| type == 		TRANSITION_COVER
				|| type == 		TRANSITION_FLASH
				|| type == 		TRANSITION_FLIP_2
				|| type == 		TRANSITION_FLY_THROUGH
				|| type == 		TRANSITION_GALLERY
				|| type == 		TRANSITION_HONEYCOMB
				|| type == 		TRANSITION_REVEAL
				|| type == 		TRANSITION_SHAPE
				|| type == 		TRANSITION_SHRED
				|| type == 		TRANSITION_SWITCH
				|| type == 		TRANSITION_UNCOVER
				|| type == 		TRANSITION_WIPE
				|| type == 		TRANSITION_CENTER_BLIND
				|| type == 		TRANSITION_FADE_THROUGH_COLOR
				|| type == 		TRANSITION_FALL
				|| type == 		TRANSITION_MOVE_IN
				|| type == 		TRANSITION_REVOLVING_DOOR
				|| type == 		TRANSITION_SWAP
				|| type == 		TRANSITION_SWOOSH
				|| type == 		TRANSITION_TWIRL
				|| type == 		TRANSITION_BRICK_CUBE
				|| type == 		TRANSITION_CUBE_2PIECES
				|| type == 		TRANSITION_FOLDING_SCREEN
				|| type == 		TRANSITION_GALLERY_2){
			
			return true;
		}else
			return false;
		
	
		
	}
}
