/**
 * SGIEffectNodeEvent.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.main;

import android.view.MotionEvent;

public interface SVIEffectNodeEvent {
	boolean onClick(int idx, int type, MotionEvent event); 
	boolean onLongPress(int idx , int type, MotionEvent event); 
	boolean onHovering(int idx, int type, MotionEvent event); 
}
