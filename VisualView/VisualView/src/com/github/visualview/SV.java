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
package com.github.visualview;

import com.github.sviengine.animation.SVIAnimation;




/**
 *  This class provides access to the Visual View Package.
 *  <p>
 *  The Class Provides the Version Code, the Version name and the initialization methods for the 
 *  Visual View package. It also provides different interpolator types.
 *  <p> You can use the interpolators to control the animation speed and acceleration 
 *  
 */

public class SV {

	
	/** Linear interpolator where the animation speed does not change over time. */
    public static final int INTERPOLATOR_LINEAR = SVIAnimation.InterpolatorType.LINEAR;
    /** Accelerate interpolator where the animation speed increases after each frame. */
    public static final int INTERPOLATOR_ACCELERATE = SVIAnimation.InterpolatorType.ACCELERATE;
    /** Decelerate interpolator where the animation speed decreases after each frame. */
    public static final int INTERPOLATOR_DECELERATE = SVIAnimation.InterpolatorType.DECELERATE;
    /** AccelerateDecelerate interpolator where the animation speed increases slowly to start with and decreases towards the end. */
    public static final int INTERPOLATOR_ACCELERATE_DECELERATE = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
    /** Anticipate interpolator where the animation starts backwards and flings forward. */
    public static final int INTERPOLATOR_ANTICIPATE = SVIAnimation.InterpolatorType.ANTICIPATE;
    /** AnticipateOvershoot interpolator where the animation starts backwards, flings forward, and overshoots the final value before settling back at the end. */
    public static final int INTERPOLATOR_ANTICIPATE_OVERSHOOT = SVIAnimation.InterpolatorType.ANTICIPATE_OVERSHOOT;
    /** Bounce interpolator where the animation bounces after hitting the end. */
    public static final int INTERPOLATOR_BOUNCE = SVIAnimation.InterpolatorType.BOUNCE;
    /** Overshoot interpolator where the animation flings forward, overshoots the end value and then settles back at the end. */
    public static final int INTERPOLATOR_OVERSHOOT = SVIAnimation.InterpolatorType.OVERSHOOT;
    /** Cycle interpolator where the animation gets repeated for a specified number of cycles. */
    public static final int INTERPOLATOR_CYCLE = SVIAnimation.InterpolatorType.CYCLE;
    /** Back ease-in interpolator where the rate of change for the animation is fast at the start and slows down towards the end. */
    public static final int INTERPOLATOR_BACK_EASE_IN = SVIAnimation.InterpolatorType.BACKEASEIN;
    /** The rate of change is slow at start, then accelerates in the middle, then slows again towards the end. */
    public static final int INTERPOLATOR_BACK_EASE_IN_OUT = SVIAnimation.InterpolatorType.BACKEASEINOUT;
    /** Back ease-out interpolator where the rate of change for the animation is slow at the start and accelerates towards the end. */
    public static final int INTERPOLATOR_BACK_EASE_OUT = SVIAnimation.InterpolatorType.BACKEASEOUT;
    /** Bounce ease-in interpolator where the bounce effect is ease -in. */
    public static final int INTERPOLATOR_BOUNCE_EASE_IN = SVIAnimation.InterpolatorType.BOUNCEEASEIN;
    /**Bounce ease-in-out interpolator where the bounce effect is ease -in-out. */
    public static final int INTERPOLATOR_BOUNCE_EASE_IN_OUT = SVIAnimation.InterpolatorType.BOUNCEEASEIN;
    /** Bounce ease-out interpolator where the bounce effect is ease -out. */
    public static final int INTERPOLATOR_BOUNCE_EASE_OUT = SVIAnimation.InterpolatorType.BOUNCEEASEOUT;
    /** Circular ease-in interpolator where the acceleration and deceleration curves match a circular function. */
    public static final int INTERPOLATOR_CIRCULAR_EASE_IN = SVIAnimation.InterpolatorType.CIRCULAREASEIN;
    /** Circular ease-in-out interpolator where the acceleration and deceleration curves match a circular function. */
    public static final int INTERPOLATOR_CIRCULAR_EASE_IN_OUT = SVIAnimation.InterpolatorType.CIRCULAREASEINOUT;
    /** Circular ease-out interpolator where the acceleration and deceleration curves match a circular function. */
    public static final int INTERPOLATOR_CIRCULAR_EASE_OUT = SVIAnimation.InterpolatorType.CIRCULAREASEOUT;
    /** Cubic ease-in interpolator where the acceleration and deceleration curves match a cubic function. */
    public static final int INTERPOLATOR_CUBIC_EASE_IN = SVIAnimation.InterpolatorType.CUBICEASEIN;
    /** Cubic ease-in-out interpolator where the acceleration and deceleration curves match a cubic function. */
    public static final int INTERPOLATOR_CUBIC_EASE_IN_OUT = SVIAnimation.InterpolatorType.CUBICEASEINOUT;
    /** Cubic ease-out interpolator where the acceleration and deceleration curves match a cubic function. */
    public static final int INTERPOLATOR_CUBIC_EASE_OUT = SVIAnimation.InterpolatorType.CUBICEASEOUT;
    /** Elastic ease-in interpolator where the animation simulates a spring function. */
    public static final int INTERPOLATOR_ELASTIC_EASE_IN = SVIAnimation.InterpolatorType.ELASTICEASEIN;
    /** Elastic ease-in-out interpolator where the animation simulates a spring function. */
    public static final int INTERPOLATOR_ELASTIC_EASE_IN_OUT = SVIAnimation.InterpolatorType.ELASTICEASEINOUT;
    /** Elastic ease-out interpolator where the animation simulates a spring function. */
    public static final int INTERPOLATOR_ELASTIC_EASE_OUT = SVIAnimation.InterpolatorType.ELASTICEASEOUT;
    /** Exponential ease-in interpolator where the acceleration and deceleration curves match an exponential function. */
    public static final int INTERPOLATOR_EXPONENTIAL_EASE_IN = SVIAnimation.InterpolatorType.EXPONENTIALEASEIN;
    /**  Exponential ease-in-out interpolator where the acceleration and deceleration curves match an exponential function. */
    public static final int INTERPOLATOR_EXPONENTIAL_EASE_IN_OUT = SVIAnimation.InterpolatorType.EXPONENTIALEASEINOUT;
    /**  Exponential ease-out interpolator where the acceleration and deceleration curves match an exponential function. */
    public static final int INTERPOLATOR_EXPONENTIAL_EASE_OUT = SVIAnimation.InterpolatorType.EXPONENTIALEASEOUT;
    /**  Quad ease-in interpolator where the acceleration and deceleration curves match a quadratic function. */
    public static final int INTERPOLATOR_QUAD_EASE_IN = SVIAnimation.InterpolatorType.QUADEASEIN;
    /** Quad ease-in-out interpolator where the acceleration and deceleration curves match a quadratic function. */
    public static final int INTERPOLATOR_QUAD_EASE_IN_OUT = SVIAnimation.InterpolatorType.QUADEASEINOUT;
    /**  Quad ease-out interpolator where the acceleration and deceleration curves match a quadratic function. */
    public static final int INTERPOLATOR_QUAD_EASE_OUT = SVIAnimation.InterpolatorType.QUADEASEOUT;
    /**  Quart ease-in interpolator where the acceleration and deceleration curves match a quartic power (x, 4) function. */
    public static final int INTERPOLATOR_QUART_EASE_IN =SVIAnimation.InterpolatorType.QUARTEASEIN;
    /**  Quart ease-in-out interpolator where the acceleration and deceleration curves match a quartic power (x, 4) function. */
    public static final int INTERPOLATOR_QUART_EASE_IN_OUT = SVIAnimation.InterpolatorType.QUARTEASEINOUT;
    /** Quart ease-out interpolator where the acceleration and deceleration curves match a quartic power (x, 4) function. */
    public static final int INTERPOLATOR_QUART_EASE_OUT = SVIAnimation.InterpolatorType.QUARTEASEOUT;
    /** Quint ease-in interpolator where the acceleration and deceleration curves match a quint power (x, 5) function. */
    public static final int INTERPOLATOR_QUINT_EASE_IN = SVIAnimation.InterpolatorType.QUARTEASEIN;
    /**  Quint ease-in-out interpolator where the acceleration and deceleration curves match a quint power (x, 5) function. */
    public static final int INTERPOLATOR_QUINT_EASE_IN_OUT = SVIAnimation.InterpolatorType.QUARTEASEINOUT;
    /**  Quint ease-out interpolator where the acceleration and deceleration curves match a quint power (x, 5) function. */
    public static final int INTERPOLATOR_QUINT_EASE_OUT = SVIAnimation.InterpolatorType.QUARTEASEOUT;
    /**  Sine ease-in interpolator where the acceleration and deceleration curves match a sinusoidal function. */
    public static final int INTERPOLATOR_SINE_EASE_IN = SVIAnimation.InterpolatorType.SINEEASEIN;
    /** Sine ease-in-out interpolator where the the acceleration and deceleration curves match a sinusoidal function. */
    public static final int INTERPOLATOR_SINE_EASE_IN_OUT = SVIAnimation.InterpolatorType.SINEEASEINOUT;
    /**  Sine ease-out interpolator where the acceleration and deceleration curves match a sinusoidal function. */
    public static final int INTERPOLATOR_SINE_EASE_OUT = SVIAnimation.InterpolatorType.SINEEASEOUT;
	/**Strong ease-in interpolator where the acceleration and deceleration curves are more pronounced with a higher order function. */
    public static final int INTERPOLATOR_STRONG_EASE_IN = SVIAnimation.InterpolatorType.STRONGEASEIN;
	/** Strong ease-in-out interpolator where the acceleration and deceleration curves are more pronounced with a higher order function. */
    public static final int INTERPOLATOR_STRONG_EASE_IN_OUT = SVIAnimation.InterpolatorType.STRONGEASEINOUT;
	/** Strong ease-out interpolator where the acceleration and deceleration curves are more pronounced with a higher order function. */
    public static final int INTERPOLATOR_STRONG_EASE_OUT = SVIAnimation.InterpolatorType.STRONGEASEOUT;
	
    

}
