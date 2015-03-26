/**
 * SGIEffectAnimCallback.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.main;

import android.view.MotionEvent;

import com.github.sviengine.slide.SVISlide;

public abstract class SVIEffectNodeCallback {
	public SVIEffectNodeCallback(){}
	
	public abstract boolean callbackPannelInitialize(SVIEffectNode node);
	public abstract boolean callbackInitialize(SVIEffectNode node);
	public abstract int 	callbackExtraTask(SVIEffectNode node, MotionEvent event, int index);	
	public abstract boolean callbackClickAnimation(SVIEffectNode node, MotionEvent event, int index);
	public abstract boolean callbackLongPressAnimation(SVIEffectNode node, MotionEvent event);
	public abstract boolean callbackHoveringAnimation(SVIEffectNode node, MotionEvent event);
	
	public abstract boolean callbackSensor(SVIEffectNode node, float [] value, int type);
	public abstract boolean callbackSensor(SVISlide slide, float [] value, int type);
}
