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

//core Engine imports
import java.util.ArrayList;

import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.visualview.animation.SVAnimation;
import com.github.visualview.animation.SVKeyFrameAnimation;
import com.github.visualview.animation.SVTransitionAnimation;



/**
 * This class defines a Visual View package slide object. A slide is a container 
 * object that holds the geometry, timing and visual properties for the surface, 
 * but it only provides the content for the display. It is not responsible for but 
 * the actual display.
 * 
 */
public class SVSlide  {
    
    /** 
     * Blend Multiply. It Multiplies current pixel color with the background color pixel.
     */
    public static final int BLEND_MULTIPLY = SVISlide.BlendType.BLEND_MULTIPLY;
	
    /** 
     * Blend Add. It add the current pixel color with the back ground pixel color. 
     */
    public static final int BLEND_ADD  = SVISlide.BlendType.BLEND_ADD;
	
    /** 
     * Blend One. It uses Original Image's color.
     */
    public static final int BLEND_ONE =  SVISlide.BlendType.BLEND_ONE;
    
    /** No Light. */
    public static final int LIGHT_NONE= SVISlide.LightType.NO_LIGHT;
    
    /**Spot Light. */
    public static final int LIGHT_SPOT= SVISlide.LightType.SPOT_LIGHT;
	
    /**Linear Light. */
    public static final int LIGHT_LINEAR= SVISlide.LightType.LINEAR_LIGHT;
    
    /**
     * Z Order BOTTOM.The Slide with this order is rendered first.
     */
    public static final int Z_ORDER_BOTTOM = SVISlide.SlideZOrderType.BOTTOM;
    
    /** Z Order NORMAL.  */
    public static final int Z_ORDER_NORMAL = SVISlide.SlideZOrderType.NORMAL;
    
    /**
     * Z Order TOP.  The Slide with this order is rendered Last. So it appears on top of all other slides. 
     */
    public static final int Z_ORDER_TOP= SVISlide.SlideZOrderType.TOPMOST;
    
    
    /** Matrix Image Scale . */
    public static final int IMAGE_SCALE_MATRIX = SVISlide.ImageScaleType.MATRIX;
    
    /**
     * Fit XY Image Scale. It fills the Slide with the Image.
     */
    public static final int IMAGE_SCALE_FIT_XY = SVISlide.ImageScaleType.FIT_XY;
    
    /** 
     * Fit Start Image Scale. It aligns with the left and top edge of the slide.It maintains 
     * the image expects ration but ensures that the image fits entirely inside the slide on at least one axis(X or Y)
     * 
     */
    public static final int IMAGE_SCALE_FIT_START = SVISlide.ImageScaleType.FIT_START;
    
    /** 
     * Fit Center Image Scale. It aligns with the center of the slide ,maintaining the aspect ratio of the image
     *  It ensures that the image fits entirely inside the slide on at least one axis(X or Y)
     */
    public static final int IMAGE_SCALE_FIT_CENTER = SVISlide.ImageScaleType.FIT_CENTER;
    
    /** 
     * FIT END Image Scale. It aligns to the bottom and right of the slide maintaining the original aspect ration of the image.
     *  it ensures that the image fits entirely inside the slide on at least one axis(X or Y).
     */
    public static final int IMAGE_SCALE_FIT_END = SVISlide.ImageScaleType.FIT_END;
    
    /** 
     * Center Image Scale. It aligns with the center of the slide without any scaling 
     */
    public static final int IMAGE_SCALE_CENTER = SVISlide.ImageScaleType.CENTER;
    
    /** CENTER CROP Image Scale. It Scales the image so that both width and height of the image will be
     * equal or larger then the slide's dimension 
     */
    public static final int IMAGE_SCALE_CENTER_CROP = SVISlide.ImageScaleType.CENTER_CROP;
    
    /** CENTER INSIDE Image Scale. It Scales the image so that both width and height of the image will be
     * equal or less then the slide's dimension
     */
    public static final int IMAGE_SCALE_CENTER_INSIDE = SVISlide.ImageScaleType.CENTER_INSIDE;
    
    
    /** PERSPECTIVE Projection Type . */
    public static final int PROJECTION_PERSPECTIVE = 0;
	
    /** ORTHOGONAL Projection Type . */
    public static final int PROJECTION_ORTHOGONAL = 1;
    
	SVISlide mSlide;
	
	private ArrayList<SVSlide> mListSlide;
	private SVImage mImage = null;
	private SVSlide mParent = null;
	private float mBorderWidth = 0;
	
	/**
     * SVSlide constructor with attributes initialized to default values.<p>
     * The Default Slide region(0.0f,0.0f,1.0f,1.0f) and the default color value is (0.0f,0.0f,0.0f,1.0f).
     *
     */
	public SVSlide() {
    	
    	 mSlide = new SVISlide(0, 0, 0.0f, 0.0f, 1.0f, 1.0f, new float[]{0.0f, 0.0f, 0.0f, 1.0f});//same as root slide
    	 mListSlide = new ArrayList<SVSlide>();
    }
	

    /**
     * Creates an instance of SVSlide with the specified values.
     * <p> the default SVColor (0.0f,0.0f,0.0f,1.0f)
     *
     * @param parentSlide 	The parent slide for this slide.
     * @param x      	x value of this slide's region.
     * @param y      	y value of this slide's region.
     * @param width  	width of the slide's region.
     * @param height 	height of the slide's region.
     * @param color  	color of the slide.
     * @throws IllegalArgumentException if width/height of the Slide is passed negative an exception is thrown. You must make sure the width/height is non negative.
     * 			However Slide can be instantiated with null color and null parentSlide. If the Color is null it takes the default color value for the initialization of  slide.
     * 			Later it can be changed using api's provided for setting the color.
     */
    public SVSlide(SVSlide parentSlide, float x, float y, float width, float height, SVColor color) {

   	 	
    	checkLowValue(width);
    	checkLowValue(height);
    	mListSlide = new ArrayList<SVSlide>();
   	 	float[] argColor = {0.0f,0.0f,0.0f,1.0f};
    	
    	if(color != null){
	    	argColor[3] = color.getAlpha();
	    	argColor[0] = color.getRed();
	    	argColor[1] = color.getGreen();
	    	argColor[2] = color.getBlue();
    	}
    	if(parentSlide != null)
    	{
    		mSlide = new SVISlide(0, 0, x, y, width, height, argColor);
    		parentSlide.mSlide.addSlide(mSlide);
    		parentSlide.mListSlide.add(this);
    		mParent = parentSlide;
    		
    	}
    	else
    	{
    		mSlide = new SVISlide(0, 0, x, y, width, height, argColor);
    	}
    	
    }
    
    
    /**
     * Set the region for this slide.
     * <P> region takes the top left coordinates (x,y) along with the width and height of the Slide(w,h).
     * <P> The values are relative to the parent.
     *
     * @param region SVRect instance defining the region.
     * @throws IllegalArgumentException if the region({@link SVRect})  argument is passed as null value.
     * 
     */
    public void setRegion(SVRect region ) {
    	
    	checkNullValue(region);
    	if(mSlide != null)
    	{
    		if(region != null){
    			mSlide.setRegion(region.getX(), region.getY(), region.getWidth(), region.getHeight() );
    		}
    	}
    	
    }
    
	/**
     * Set region of the slide, 
     * The change causes implicit animation of the Slide.
     * <P> region takes the top left coordinates (x,y) along with the width and height of the Slide(w,h)
     * <P> The values are relative to the parent.
     *
     * @param region SVRect to set for this slide.
     * @param duration millisecond duration of the implicit animation .<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If the region({@link SVRect})  argument is passed as null.
     * <li>2. If duration which which will cause the implicit animation is less then 0
     */
    public void setRegion(SVRect region , int duration) {
    	
    	checkLowValue(duration);
    	checkNullValue(region);
    	if(mSlide != null)
    	{
    		if(region != null)
    			mSlide.setRegion(region.getX(), region.getY(), region.getWidth(), region.getHeight() , duration);
    		
    	}
    	
    }
    
    /**
     * Get current region(bounds) for this slide in Window Coordinates.
     *
     * @return SVRect object with the current area of the slide.
     */
  
	public SVRect getRegion() {
		
    	SVIRect region = null;
    	SVRect retRegion = null;
    	
    	if(mSlide != null){
    		region  = mSlide.getRegion();
    		if(region != null){
    			retRegion = new SVRect();
    			retRegion.setRect(region.mOrigin.mX,
		    								region.mOrigin.mY,
		    								region.mSize.mWidth,
		    								region.mSize.mHeight);
    		}
	    }
    	return retRegion;
    }
    
    /**
     * Get current region for this slide in screen coordinates.
     *
     * @return Current SVRect object with the current region of the slide in screen coordinates.
     */
    public SVRect getGlobalRegion() {
    	
    	SVIRect region = null;
    	SVRect retGlobalRegion = null;
    	
    	if(mSlide != null){
    		region  = mSlide.getGlobalRegion();
    		if(region != null){
    			retGlobalRegion = new SVRect();
    			retGlobalRegion.setRect(region.mOrigin.mX,
						region.mOrigin.mY,
						region.mSize.mWidth,
						region.mSize.mHeight);
    		}
	    }
    	return retGlobalRegion;
    }
    
    /**
     * Set position of the Center of the Slide
     *
     * @param point Position in windows coordinates for the center of the slide.
     * @throws IllegalArgumentException if the input point parameter  is passed as null.
     * 
     */
    public void setPosition(SVPoint point) {
    	
    	checkNullValue(point);
    	if(mSlide != null)
    	{
    		if(point != null)
    			mSlide.setPosition(point.getX(), point.getY());
    		
    	}
    	
    }
    
    /**
     * Set position of the center for this slide, 
     * This will cause implicit animation.
     *
     * @param point Center of the Slide. 
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If the point is passed as null argument.
     * <li> 2. If duration which which will cause the implicit animation is less then 0
     */
    public void setPosition(SVPoint point,int duration) {
    	checkLowValue(duration);
    	checkNullValue(point);
    	if(mSlide != null)
    	{
    		if(point != null)
    			mSlide.setPosition(point.getX(), point.getY(), duration);
    		
    	}
    	
    }

    /**
     * Get current position of the center of the Slide.
     *
     * @return SVPoint Current position for this slide.
     */
    public SVPoint getPosition() {
    	
    	SVIPoint point = null;
    	SVPoint retPosition = null;
    	
    	if(mSlide != null){
    		point = mSlide.getPosition();
    		if(point !=null){
    			retPosition = new SVPoint();
    			retPosition.setPosition(point.mX,point.mY);
    		}
    	}
    	
    	return retPosition;
    }

    /**
     * Set pivot point of the slide.
     * <p> The default pivot point is (0.5f,0.5f).
     *
     * @param point SVPoint about which the slide pivots.
     * @throws IllegalArgumentException if the Pivot point is passed as null argument.
     * 
     */
    public void setPivotPoint(SVPoint point ) {
    	checkNullValue(point);
    	if(mSlide != null){
    		if(point != null)
    			mSlide.setPivotPoint(point.getX(), point.getY());
    		
    	}
    	
    }
    
    /**
     * Set pivot point of the slide, 
     *  This will cause implicit animation.
     *  <p> The default pivot point is (0.5f,0.5f).
     *
     * @param point SVPoint about which the slide pivots
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. if the pivot point is passed as null argument.
     * <li> 2. If duration which which will cause the implicit animation is less then 0
     */
    public void setPivotPoint(SVPoint point , int duration) {
    	
    	checkNullValue(point);
    	checkLowValue(duration);
    	if(mSlide != null){
    		if(point != null)
    			mSlide.setPivotPoint(point.getX(), point.getY(), duration);
    		
    	}
    	
    }
    
    /**
     * Get current pivot point for this slide.
     *
     * @return Current pivot point for this slide.
     */
    public SVPoint getPivotPoint() {
    	
    	SVIPoint point = null;
    	SVPoint retPivotPoint = null;
    	
    	if(mSlide != null){
    		point = mSlide.getPivotPoint();
    		if(point !=null){
    			retPivotPoint = new SVPoint();
    			retPivotPoint.setPosition(point.mX,point.mY);
    		}
    	}
    	
    	return retPivotPoint;
    }

    /**
     * Set the Specified rotation angles in Degrees for this slide about the three axis.
     * <p> The Rotation of the Slide happens from the original position of the slide created.
     * 
     *
     * @param axisAngle  The Euler angles to apply the rotation.
     *  <p>axisAngle[0] = x-axis angle</p>
     *  <p>axisAngle[1] = y-axis angle</p>
     *  <p>axisAngle[2] = z-axis angle</p>
     * @throws IllegalArgumentException if the axisAngle parameter is passed as null.You need to instantiate the SVVector which is passed as argument.
     * 
     */
    public void setRotation(SVVector3 axisAngle) {
    	checkNullValue(axisAngle);
    	
    	if(mSlide != null){
    		if(axisAngle != null){
    			mSlide.setRotation(axisAngle.getX(), axisAngle.getY(), axisAngle.getZ(),0);
    		}
    	}
    }
    
    /**
     * Set the specified rotation angles in Degrees for this slide about the three axis, 
     *  This will cause implicit animation.
     *  <p> The Rotation of the Slide happens from the original position of the slide created.
     *
     * @param axisAngle  The Euler angles to apply the rotation.
     *  <p>axisAngle[0] = x-axis angle</p>
     *  <p>axisAngle[1] = y-axis angle</p>
     *  <p>axisAngle[2] = z-axis angle</p>
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. if the axisAngle({@link SVVector3}) is not instantiated and is passed as null argument.
     * <li> 2. if duration which which will cause the implicit animation is less then 0
     * 
     */
    public void setRotation(SVVector3 axisAngle, int duration) {
    	checkNullValue(axisAngle);
    	checkLowValue(duration);
    	
    	if(mSlide != null){
    		if(axisAngle != null){
    			mSlide.setRotation(axisAngle.getX(), axisAngle.getY(), axisAngle.getZ(),0, duration);
    		}
    	}
    }
    
    /**
     * Get current rotation value for this slide.
     *
     * @return SVVector3 object with the current slide rotation values.
     */
    public SVVector3 getRotation() {
    	
       SVVector3 rotation = null;
       if(mSlide != null){
    	   SVIVector3 copyRotation = mSlide.getRotation();
		   if(copyRotation != null){
			   rotation = new SVVector3();
    	   	   rotation.setVector(copyRotation.mX,
				       copyRotation.mY,
				       copyRotation.mZ);
		       
		   }
       }
       return rotation;
    }

    
    /**
     * Set the specified x,y,z  scale factors for this slide.
     *
     * @param scale The scale to set for this slide.
     *  <p>scale[0] = x scale value [Scales along the x axis]
     *  <p>scale[1] = y scale value [Scales along the Y axis]
     *  <p>scale[2] = z scale value</p>
     * @throws IllegalArgumentException if the scale({@link SVVector3}) is not instantiated and is passed as null argument.
     *
     */
    public void setScale(SVVector3 scale) {
    	
    	checkNullValue(scale);
    	if(mSlide != null){
    		if(scale != null)
    			mSlide.setScale(scale.getX(), scale.getY(), scale.getZ());
    		
    	}
    }
    
    
    /**
     * Set the specified x,y,z  scale factors for this slide, 
     * This will cause implicit animation.
     *
     * @param scale The scale to set for this slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     *  <p>scale[0] = x scale value [Scales along the x axis]
     *  <p>scale[1] = y scale value [Scales along the Y axis]
     *  <p>scale[2] = z scale value</p>
     * @throws IllegalArgumentException <li>1. if the scale({@link SVVector3}) is not instantiated and is passed as null argument.
     * <li> 2. if duration which which will cause the implicit animation is less then 0
     */
    public void setScale(SVVector3 scale, int duration) {
    	
    	checkNullValue(scale);
    	checkLowValue(duration);
    	if(mSlide != null){
    		if(scale != null)
    			mSlide.setScale(scale.getX(), scale.getY(), scale.getZ(),duration);
    	}
    }
    
    /**
     * Enable or disable block hierarchical scaling.
     * <p> When hold scale of the the particle slide is set true, That Slide and its children doesn't undergo the scaling. whereas rest of the hierarchy undergoes the scaling.
     *
     * @param enabled Set this to true to enable block hierarchical scaling. Set this to false to disable block hierarchical scaling.
     */
    public void setHoldScaleEnabled(boolean enabled) {
    	
    	if(mSlide != null)
    	{
    		mSlide.setHoldScale(enabled);
    	}
    }
    
    /**
     * Check if hold scale is enabled.
     *
     * @return  <li>true // if hold scale is enabled.
	 *		 	<li>false // if hold scale is not enabled.
     */
    public boolean isHoldScaleEnabled() {
    	if(mSlide != null)
    	{
    		return mSlide.getHoldScale();
    	}
    	return false;
    }
    
    /**
     * Get current scale for this slide.
     *
     * @return SVVector3 object with current scale values.
     */
    public SVVector3 getScale() {
    	
    	SVIVector3 scale = null;
    	SVVector3 retScale = null;
    	
    	if(mSlide != null){
    		scale = mSlide.getScale();
    		if(scale != null){
    			retScale = new SVVector3();
    			retScale.setVector(scale.mX,
					    			scale.mY,
					    			scale.mZ);
    		}
    	}
    	
    	return retScale;
    }

    /**
     * Set the specified background color for the slide using normalized color values.
     *
     * @param color Background color for the slide.
     * @throws IllegalArgumentException if the color({@link SVColor}) is not instantiated and is passed as null argument.
     */
    public void setBackgroundColor(SVColor color) {
    	
    	checkNullValue(color);
    	float[] argColor = {0.0f,0.0f,0.0f,1.0f};
    	
    	if(color != null){
			argColor[0] = color.getRed();
			argColor[1] = color.getGreen();
			argColor[2] = color.getBlue();
			argColor[3] = color.getAlpha();
    	}
    	if(mSlide != null)
    	{
    		mSlide.setBackgroundColor(argColor);
    	}
    }
    
    /**
     * Set the specified background color for the slide using normalized color values.
     *  This causes implicit background color animation.
     *
     * @param color Background color for this slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. if the color({@link SVColor}) is not instantiated and is passed as null argument.
     * <li> 2. If duration which which will cause the implicit animation is less then 0
     * @see SVColor
     */
    public void setBackgroundColor(SVColor color, int duration) {
    	
    	checkNullValue(color);
    	checkLowValue(duration);
    	float[] argColor = {0.0f,0.0f,0.0f,1.0f};
    	
    	if(color != null){
			argColor[0] = color.getRed();
			argColor[1] = color.getGreen();
			argColor[2] = color.getBlue();
			argColor[3] = color.getAlpha();
    	}
    	if(mSlide != null)
    	{
    		mSlide.setBackgroundColor(argColor, duration);
    	}
    }
    
    
    /**
     * Get current background color of the slide.
     *
     * @return SVColor Current background color of the slide.
     */
    public SVColor getBackgroundColor() {
    	
    	SVIColor bgColor = null;
    	SVColor retBgColor = null;
    	
    	if(mSlide != null){
    		bgColor = mSlide.getBackbgoundColor();
    		if(bgColor != null){
    			retBgColor = new SVColor();
    			retBgColor.setColor(bgColor.mR,
				    			bgColor.mG,
				    			bgColor.mB,
				    			bgColor.mA);
    		}
    	}
    	
    	return retBgColor;
    }
    
    
    /**
     * Set the specified opacity for the slide.
     *
     * @param opacity Float value for the opacity (0.0f - 1.0f).
     * @throws IllegalArgumentException as mentioned the opacity should lie between 0~1. Any value out of this range throws an {@link IllegalArgumentException}
     * 
     */
    public void setOpacity(float opacity) {
    	checkLowValue(opacity);
    	checkHighValue(opacity);
    	if(mSlide != null)
    	{
    		mSlide.setOpacity(opacity);
        }
    }
    
    
    /**
     * Set the specified opacity for the slide.
     * This will cause implicit animation.
     * 
     *
     * @param opacity Float value for the opacity (0.0f - 1.0f).
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. As mentioned the opacity can lie between 0~1. Any value out of this range throws an {@link IllegalArgumentException}
     * <li>2. if duration which which will cause the implicit animation is less then 0
     */
    public void setOpacity(float opacity , int duration) {
    	checkLowValue(opacity);    	
    	checkHighValue(opacity);
    	checkLowValue(duration);
    	if(mSlide != null)
    	{
    		mSlide.setOpacity(opacity, duration);
    	}
    }
    
    /**
     * Enable or disable hierarchical opacity. . child slide will have it's own value.
     * 
     * <p> In a tree when opacity is set for a parent all the child inherits the same opacity value. Using this API you can hold the opacity of that particular Slide.
     *
     * @param enabled  Set to true to enable native slide opacity.<p>Set to false to disable block hierarchical opacity.
     */
    public void setHoldOpacityEnabled(boolean enabled) {
    	
    	if(mSlide != null)
    	{
    		mSlide.setHoldOpacity(enabled);
    	}
    }
    
    /**
     * Check if hold opacity is enabled.
     *
     * @return <li>true �? if hold opacity is enabled.
     * 	<li>false �? if hold opacity is not enabled
     */
    public boolean isHoldOpacityEnabled() {
    	if(mSlide != null)
    	{
    		return mSlide.getHoldOpacity();
    	}
        return false;
    }
    
    /**
     * Get the current opacity of the slide.
     *
     * @return float value for the current opacity of the slide (0.0f - 1.0f).
     */
    public float getOpacity() {
    	
    	float retOpacity = 0;
    	
    	if(mSlide != null)
    	{
    		retOpacity = mSlide.getOpacity();
    	}
    	
    	return retOpacity;
    }
    
    
    /**
     * Set the specified corner radius for the slide in screen coordinate units.
     * 
     * @param cornerRadius Float value for the corner radius of the slide.
     * @throws IllegalArgumentException  The radius of the slide corner must be a non negative value.
     * 
     */
    public void setCornerRadius(float cornerRadius) {
    	checkLowValue(cornerRadius);
    	
    	if(mSlide != null)
    	{
    		mSlide.setCornerRadius(cornerRadius );
    	}
    }
    
    
    /**
     * Set the specified corner radius for the slide in screen coordinate units.
     * This will cause implicit animation.
     * 
     * @param cornerRadius Float value for the corner radius of the slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. if radius of the slide corner is a negative value.
     * <li>2. if duration which which will cause the implicit animation is less then 0.
     * 
     */
    public void setCornerRadius(float cornerRadius, int duration) {
    	checkLowValue(cornerRadius);
    	checkLowValue(duration);
    	
    	if(mSlide != null)
    	{
    		mSlide.setCornerRadius(cornerRadius , duration);
    	}
    }
    
    /**
     * Get the current corner radius of the slide.
     *
     * @return float value with corner radius of the slide.
     */
    public float getCornerRadius() {
    	
    	float retCornerRadius = 0;
    	
    	if(mSlide != null)
    	{
    		retCornerRadius = mSlide.getCornerRadius();
    	}
    	
    	return retCornerRadius;
    }
    
    /**
     * Set the specified color for the light hitting the slide using normalized values.
     *
     * @param lightColor Color for the light hitting the slide.
     * @throws IllegalArgumentException if the lightColor({@link SVColor}) is not instantiated and is passed as null argument.
     *
     * @see SVColor
     */
    public void setLightColor(SVColor lightColor) {
    	
    	checkNullValue(lightColor);
    	float[] argColor = {1.0f,1.0f,1.0f,1.0f};
    	
    	if(lightColor != null){
	    	argColor[3] = lightColor.getAlpha();
	    	argColor[0] = lightColor.getRed();
	    	argColor[1] = lightColor.getGreen();
	    	argColor[2] = lightColor.getBlue();
    	}
    	if(mSlide != null)
    	{
    		mSlide.setLightColor(argColor);
    	}
    }
    
    /**
     * Set the specified color for the light hitting the slide using normalized values.
     * This will cause implicit animation.
     *
     * @param lightColor Color for the light hitting the slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. if the lightColor({@link SVColor}) is not instantiated and is passed as null argument.
     * <li>2. if duration which which will cause the implicit animation is less then 0.
     */
    public void setLightColor(SVColor lightColor, int duration) {
    	
    	checkNullValue(lightColor);
    	checkLowValue(duration);
    	float[] argColor = {1.0f,1.0f,1.0f,1.0f};
    	
    	if(lightColor != null){
	    	argColor[3] = lightColor.getAlpha();
	    	argColor[0] = lightColor.getRed();
	    	argColor[1] = lightColor.getGreen();
	    	argColor[2] = lightColor.getBlue();
    	}
    	if(mSlide != null)
    	{
    		mSlide.setLightColor(argColor,duration);
    	}
    }
   
    /**
     * Set the specified shadow radius for the slide using normalized values.
     * <p>The Shadow can only be casted if the Opacity of the Slide is 1.0f
     *
     * @param shadowRadius Float value for the shadow radius of the slide.
     * @throws IllegalArgumentException if shadow radius is a negative value.
     * 
     */
    public void setShadowRadius(float shadowRadius) {
    	
    	checkLowValue(shadowRadius);
    	if(mSlide != null)
    	{
    		mSlide.setShadowRadius(shadowRadius);
    	}
    }
    
    /**
     * Set the specified shadow radius for the slide using normalized values.
     * This will cause implicit animation.
     * <p>The Shadow can only be casted if the Opacity of the Slide is 1.0f
     * @param shadowRadius Float value for the shadow radius of the slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If the shadowRadius is passed as a negative value.
     * <li>2. If duration which which will cause the implicit animation is less then 0.
     * 
     */
    public void setShadowRadius(float shadowRadius, int duration) {
    	checkLowValue(shadowRadius);
    	checkLowValue(duration);
    	if(mSlide != null){
    		mSlide.setShadowRadius(shadowRadius, duration);
    		
    	}
    }
    
    /**
     * Set the specified shadow color for the slide.
     *
     * @param shadowColor Color for shadow in slide.
     * @throws IllegalArgumentException If the shadowColor({@link SVColor}) is not instantiated and passed as a null argument.
     *
     */
    public void setShadowColor(SVColor shadowColor) {
    	
    	checkNullValue(shadowColor);
    	float[] argColor = {0.0f,0.0f,0.0f,0.6f};
    	
    	if(shadowColor != null){
	    	argColor[3] = shadowColor.getAlpha();
	    	argColor[0] = shadowColor.getRed();
	    	argColor[1] = shadowColor.getGreen();
	    	argColor[2] = shadowColor.getBlue();
    	}
    	if(mSlide != null)
    		mSlide.setShadowColor(argColor);
    }
    
    /**
     * Set the specified shadow color for the slide.
     * This will cause implicit animation.
     *
     * @param shadowColor Color for shadow in slide
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If the shadowColor({@link SVColor}) is not instantiated and passed as a null argument.
     * <li>2. If duration which which will cause the implicit animation is less then 0.
     *
     */
    public void setShadowColor(SVColor shadowColor , int duration) {
    	
    	checkNullValue(shadowColor);
    	checkLowValue(duration);
    	float[] argColor = {0.0f,0.0f,0.0f,0.6f};
    	
    	if(shadowColor != null){
	    	argColor[3] = shadowColor.getAlpha();
	    	argColor[0] = shadowColor.getRed();
	    	argColor[1] = shadowColor.getGreen();
	    	argColor[2] = shadowColor.getBlue();
    	}
    	if(mSlide != null)
    		mSlide.setShadowColor(argColor,duration);
    }
    
    /**
     * Set the specified shadow offset for the slide using normalized values.
     *
     * @param xOffset the slide x-axis start value
     * @param yOffset the slide y-axis start value
     *
     */
    public void setShadowOffset(float xOffset, float yOffset) {
    	
    	if(mSlide != null)
    		mSlide.setShadowOffset(xOffset,yOffset);
    }
    
    /**
     * Set the specified shadow offset for the slide using normalized values.
     * This will cause implicit animation.
     *
     * @param xOffset The slide x-axis start value
     * @param yOffset The slide y-axis start value
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException If duration which which will cause the implicit animation is less then 0.
     *
     */
    public void setShadowOffset(float xOffset, float yOffset, int duration) {
    	checkLowValue(duration);
    	if(mSlide != null)
    		mSlide.setShadowOffset(xOffset,yOffset,  duration);
    }
    
    /**
     * Set the specified shadow opacity for the slide using normalized values.
     *
     * @param shadowOpacity Float value for opacity for shadow on slide.
     * @throws IllegalArgumentException if opacity value is not passed within the range of 0.0f ~ 1.0f
     *
     */
    public void setShadowOpacity(float shadowOpacity) {
    	
    	checkLowValue(shadowOpacity);
    	checkHighValue(shadowOpacity);
    	if(mSlide != null)
    		mSlide.setShadowOpacity(shadowOpacity );
    }
    
    /**
     * Set the specified shadow opacity for the slide using normalized values.
     * This will cause implicit animation.	
     *
     * @param shadowOpacity Float value for opacity for shadow on slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If shadowOpacity value is not passed within the range of 0.0f ~ 1.0f
     * <li>2. If duration which which will cause the implicit animation is less then 0.
     *
     */
    public void setShadowOpacity(float shadowOpacity, int duration) {
    	checkLowValue(shadowOpacity);
    	checkHighValue(shadowOpacity);
    	checkLowValue(duration);
    	if(mSlide != null)
    		mSlide.setShadowOpacity(shadowOpacity , duration);
    }
    
    /**
     * Set the specified type for the light hitting the slide. The value must lie between 0~2.<p>
     * {@link SVSlide#LIGHT_NONE} = 0;<p>
     * {@link SVSlide#LIGHT_SPOT} = 1;<p>
     * {@link SVSlide#LIGHT_LINEAR} = 2;<p>
     * The Default value is LIGHT_NONE
     *
     * @param type The light type
     * @throws IllegalArgumentException As Explained the light type can be 0, 1 or 2 with default value of 0. Exception is thrown with any other value.
     */
    public void setLightType(int type) {
    	
    	checkNullValue(type);
    	if(type <0 || type >2){
    		throw new IllegalArgumentException("Set the correct Light type ..The value must be 0 ~ 2"); 
    	}
    	if(mSlide != null){
    		mSlide.setLightType(type);
    	}
    }
    
    /**
     * Get the type of the light hitting the slide.
     *
     * @return int The current light type for the slide.
     */
    public int getLightType() {
    	
    	int NO_LIGHT =0;
    	if(mSlide != null){
    		NO_LIGHT = mSlide.getLightType();
    	}
    	
    	return NO_LIGHT;
    }
    
    /**
     * Set the specified opacity for the light hitting the slide using normalized values.
     *
     * @param lightOpacity Float value for the opacity of the light hitting  the slide.
     * @throws IllegalArgumentException If lightOpacity value is not passed within the range of 0.0f ~ 1.0f
     */
    public void setLightOpacity(float lightOpacity) {
    	
    	checkLowValue(lightOpacity);
    	checkHighValue(lightOpacity);
    	if(mSlide != null)
    		mSlide.setLightOpacity(lightOpacity );
    }
    
    /**
     * Set the specified opacity for the light hitting the slide using normalized values.
     * This will cause implicit animation.
     *
     * @param lightOpacity Float value for the opacity of the light hitting  the slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If lightOpacity value is not passed within the range of 0.0f ~ 1.0f
     * <li>2. If duration which which will cause the implicit animation is less then 0.
     *
     */
    public void setLightOpacity(float lightOpacity, int duration) {
    	checkLowValue(duration);
    	checkLowValue(lightOpacity);
    	checkHighValue(lightOpacity);
    	if(mSlide != null)
    		mSlide.setLightOpacity(lightOpacity, duration );
    }
    
    /**
     * Get the opacity for the light hitting the slide.
     *
     * @return Float value with the opacity of the light hitting the slide.
     */
    public float getLightOpacity() {
    	
    	if(mSlide != null)
    		return mSlide.getLightOpacity();
    	
    	return 1.0f;
    }
    
    
    /**
     * Set the specified angle for the light hitting the slide.
     *
     * @param lightAngle Float value for the angle the light hits the slide.
     * 
     */
    public void setLightAngle(float lightAngle) {
    	
    	if(mSlide != null)
    		mSlide.setLightAngle(lightAngle);
    }
    
    /**
     *Set the specified angle for the light hitting the slide.
     * This will cause light angle implicit animation.
     *
     * @param lightAngle Float value for the angle the light hits the slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException If duration which which will cause the implicit animation is less then 0.
     * 
     */
    public void setLightAngle(float lightAngle, int duration) {
    	checkLowValue(duration);
    	if(mSlide != null)
    		mSlide.setLightAngle(lightAngle, duration);
    }
    
    /**
     * Get the angle of the light hitting the slide.
     *
     * @return Float value with the angle the light hits the slide.
     */
    public float getLightAngle() {
    	if(mSlide != null)
    		return mSlide.getLightAngle();
    	
    	return 0;
    }
    
    /**
     * Set the specified light offset for the slide.
     *
     * @param xOffset The x Offset Position
     * @param yOffset The y Offset Position
     * 
     */
    public void setLightOffset(float xOffset, float yOffset) {
    	
    	if(mSlide != null)
    		mSlide.setLightOffset(xOffset,yOffset);
    }
    
    /**
     * Set the specified light offset for the slide.
     * This will cause light offset implicit animation.
     *
     * @param xOffset The x light offset.
     * @param yOffset The y light offset.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException If duration which which will cause the implicit animation is less then 0.
     * 
     */
    public void setLightOffset(float xOffset, float yOffset,int duration) {
    	checkLowValue(duration);
    	if(mSlide != null)
    		mSlide.setLightOffset(xOffset,yOffset,duration);
    }
    
    /**
     * Get the light offset of the slide.
     *
     * @return SVPoint object with current light offset.
     */
    public SVPoint getLightOffset() {
    	SVIPoint point = null;
    	SVPoint retLightOffset = null;
    	
    	if(mSlide != null){
    		point = mSlide.getLightOffset();
    		if(point !=null){
    			retLightOffset = new SVPoint();
    			retLightOffset.setPosition(point.mX,point.mY);
    		}
    	}
    	
    	return retLightOffset;
    }
    
    /**
     * Set the specified light radius for the slide for the specified duration. Light type must be Spot type.
     *
     * @param lightRadius Float value for the light radius of the slide.
     * @throws IllegalArgumentException If the lightRadius is passed as a negative value. 
     * 
     */
    public void setLightRadius(float lightRadius) {
    	checkLowValue(lightRadius);
    	if(mSlide != null)
    		mSlide.setLightRadius(lightRadius);
    }
    
    /**
     * Set the specified light radius for the slide for the specified duration. Light type must be Spot type.
     *  This will cause light radius implicit animation.
     *  
     * @param lightRadius Float value for the light radius of the slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If the lightRadius is passed as a negative value. 
     * <li>2. If duration which which will cause the implicit animation is less then 0.
     * 
     */
    public void setLightRadius(float lightRadius,int duration) {
    	checkLowValue(duration);
    	checkLowValue(lightRadius);
    	if(mSlide != null)
    		mSlide.setLightRadius(lightRadius,duration);
    }
    
    /**
     * Get the light radius of the slide.
     *
     * @return Float value with current light radius.
     */
    public float getLightRadius() {
    	if(mSlide != null)
    		return mSlide.getLightRadius();
    	
    	return 0.5f;
    }
    
    /**
     * Get the color of the light hitting the slide.
     *
     * @return SVColor object with the current light used in the slide.
     */
    public SVColor getLightColor() {
    	
    	SVIColor lightColor = null;
    	SVColor retlightColor = null;
    	
    	if(mSlide != null){
    		lightColor = mSlide.getLightColor();
    		if(lightColor != null){
    			retlightColor = new SVColor();
    			retlightColor.setColor(lightColor.mR,
					    			lightColor.mG,
					    			lightColor.mB,
					    			lightColor.mA);
    		}
    	}
    	
    	return retlightColor;
    }
    
    /**
     * Set the specified intensity for the light hitting the slide.
     *
     * @param lightPower �? Float value for the light intensity.
     * @throws IllegalArgumentException if negative power
     * 
     */
    public void setLightPower(float lightPower) {
    	checkLowValue(lightPower);
    	if(mSlide != null)
    		mSlide.setLightPower(lightPower);
    }
    
    /**
     * Set the specified intensity for the light hitting the slide.
     *  This will cause implicit animation.
     *
     * @param lightPower �? Float value for the light intensity.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException The intensity of the light cannot be a negative value. You must make sure it is greater then 0.
     */
    public void setLightPower(float lightPower, int duration) {
    	checkLowValue(lightPower);
    	checkLowValue(duration);
    	if(mSlide != null)
    		mSlide.setLightPower(lightPower, duration);
    }
    
    /**
     * Get intensity of the light hitting the slide.
     *
     * @return Float value with the current light intensity.
     */
    public float getLightPower() {
    	
    	float lightPower = 0;
    	
    	if(mSlide != null)
    		lightPower = mSlide.getLightPower();
    	
    	return lightPower;
    }
    
    
    
    
    /**
     * Set the specified texture region for the slide.
     * <p> if You want to set any rectangular part of the image Texture to set to the slide , this API can be used. 
     * 
     *
     * @param texRegion The texture region to set.
     * @throws IllegalArgumentException If the texRegion({@link SVRect})  argument is passed as a null value.
     * 
     */
    public void setTextureRegion(SVRect texRegion) {
    	
    	checkNullValue(texRegion);
    	if(mSlide != null){
    		if(texRegion != null){
    			mSlide.setTextureRegion(texRegion.getX(), texRegion.getY(), texRegion.getWidth(), texRegion.getHeight());
    		}
    		
    	}
    		
    }
    
    
    /**
     * Set the specified texture region for the slide for the specified duration
     *  This will cause implicit animation.
     * <p> if You want to set any rectangular part of the image Texture to set to the slide with animation from its original image to the new image , this API can be used. 
     * @param texRegion The texture region to set.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If the texRegion({@link SVRect})  argument is passed as a null value. 
     * <li>2. If duration which which will cause the implicit animation is less then 0.
     */
    public void setTextureRegion(SVRect texRegion, int duration) {
    	checkNullValue(texRegion);
    	checkLowValue(duration);
    	if(mSlide != null){
    		if(texRegion != null)
    			mSlide.setTextureRegion(texRegion.getX(), texRegion.getY(), texRegion.getWidth(), texRegion.getHeight(), duration);
    		
    	}
    }
    
    
    /**
     * Set the specified scale to fit region for the slide.
     * <p> You can use this API if you want to set the particular region of the slide where image can fit itself.  The (X,Y) of the scaleToFitRegion is with respect to the top left corner of the Slide.
     *
     * @param scaleToFitRegion texture region to set for this slide.
     * @throws IllegalArgumentException If the scaleToFitRegion({@link SVRect})  argument is passed as a null value.
     * 
     */
    public void setScaleToFitRegion(SVRect scaleToFitRegion) {
    	
    	checkNullValue(scaleToFitRegion);
    	if(mSlide != null ){
    		if(scaleToFitRegion!= null )
    			mSlide.setScaleToFitRegion(scaleToFitRegion.getX(), scaleToFitRegion.getY(), scaleToFitRegion.getWidth(), scaleToFitRegion.getHeight());
    		
    	}
    }
    
    
    /**
     * Set the specified scale to fit region for the slide.
     * This will cause implicit animation.
     *<p> You can use this API if you want to set the particular region of the slide where image can fit itself.  The (X,Y) of the scaleToFitRegion is with respect to the top left corner of the Slide.The Image will scale from 
     *its original position to the region where you want it to fit the image.
     * @param scaleToFitRegion the scale to fit region
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. If the scaleToFitRegion({@link SVRect})  argument is passed as a null value. 
     * <li>2. If duration which which will cause the implicit animation is less then 0.
     */
    public void setScaleToFitRegion(SVRect scaleToFitRegion, int duration) {
    	checkNullValue(scaleToFitRegion);
    	checkLowValue(duration);
    	if(mSlide != null ){
    		if(scaleToFitRegion!= null )
    			mSlide.setScaleToFitRegion(scaleToFitRegion.getX(), scaleToFitRegion.getY(), scaleToFitRegion.getWidth(), scaleToFitRegion.getHeight(),duration);
    	}
    }
    
    /**
     * Get the current texture region for the slide.
     *
     * @return SVRect The current texture region.
     */
    public SVRect getTextureRegion() {
    	SVIRect texRegion = null;
    	SVRect retTexRegion = null;
    	
    	if(mSlide != null){
    		texRegion  = mSlide.getTextureRegion();
    		if(texRegion != null){
    			retTexRegion = new SVRect();
    			retTexRegion.setRect(texRegion.mOrigin.mX,
							    			texRegion.mOrigin.mY,
							    			texRegion.mSize.mWidth,
							    			texRegion.mSize.mHeight);
    		}
	    }
    	return retTexRegion;
    }
    
    
    /**
     * Enable or disable the clipping of child slides attached to this slide.
     * <p>If you set the clipping as true, then all the Slide in the tree below the current Slide are clipped in the current slide's region.
     * 
     * @param enabled Set to true to enable clipping, set to false to disable clipping.
     */
    public void setClipSubSlidesEnabled(boolean enabled) {
    	
    	if(mSlide != null){
    		mSlide.setClipSubSlides(enabled);
    	}
    }
    
    /**
     * Check if child slide clipping is enabled.
     *
     * @return boolean <li>true - if child clipping is enabled
     * <li>false - if child clipping is not enabled
     */
    public boolean isClipSubslidesEnabled(){
    	
    	if(mSlide != null)
    		return mSlide.getClipSubSlides();
    	
    	return false;
    }
    
    /**
     * set projection type for the slide.
     * <p>For more information on the projection type ..Please refer the wikipedia page(http://en.wikipedia.org/wiki/3D_projection)
     * 
     * @param projectionType set 0 if {@link SVSlide#PROJECTION_PERSPECTIVE}, 1 if {@link SVSlide#PROJECTION_ORTHOGONAL}
     * <p>The Default Projection type is Orthogonal</p>
     * @throws IllegalArgumentException if projection type is not 0 or 1
     */
    public void setProjectionType(int projectionType) {

    	if(projectionType <0 || projectionType >1){
    		throw new IllegalArgumentException("Incorrect input..!! ProjectionType can have (0 == perpective) or (1 == orthogonal) as its input");
    	}
    	if(mSlide != null){
    		if(projectionType == PROJECTION_PERSPECTIVE){
    			mSlide.setOrthogonal(false);
    		}else{
    			mSlide.setOrthogonal(true);
    		}
    	}
    }
    
    
    /**
     * Get The projection Type.<p>
     * If orthogonal projection is enabled for this slide then will be return true.
     *
     * @return <li>0 is PROJECTION_PERSPECTIVE. <li>1 is PROJECTION_ORTHOGONAL
     */
    public int getProjectionType(){
    	if(mSlide != null)
    	{
    		if(mSlide.isOrthogonal()){
    			return PROJECTION_ORTHOGONAL;
    		}else{
    			return PROJECTION_PERSPECTIVE;
    		}
    	}
		return 1;
    }
    
    /**
     * Set the specified blending type for the slide..The Value must be between 0 ~ 2.<p>
     * <p>{@linkplain SVSlide#BLEND_MULTIPLY} = 0 ==> It Multiplies current pixel color with the background color pixel
     * <p>{@linkplain SVSlide#BLEND_ADD} = 1 ==> It add the current pixel color with the back ground pixel color.
     * <p>{@linkplain SVSlide#BLEND_ONE} = 2 ==> it uses Original Image's color.
     * 
     * <p>The Default value is BLEND_MULTIPLY. Blend one and Add have almost similar effect if bitmap doesn't have the unique alpha map attached with it.
     * 
     * @param blendType blending type to set for this slide.
     * @throws IllegalArgumentException blendType can be take the value as 0,1 or 2 ,any other value throws an {@link IllegalArgumentException} 
     * 
     */
    public void setBlendType(int blendType) {
    	if(blendType <0 || blendType >2){
    		throw new IllegalArgumentException("Value must be between 0 ~ 2");
    	}
    	
    	if(mSlide != null){
    		mSlide.setBlendType(blendType);
    	}
    }
    
    /**
     * Enable or disable anti aliasing for the slide.
     * <p>when the anti aliasing is enabled, the edges of points, lines, and polygons appear smooth rather than angular and jagged. 
     *
     * @param enabled the value of AntiAliasing(true = use anti-aliasing)
     *
     */
    public void setAntiAliasingEnabled(boolean enabled) {
    	
    	
    	if(mSlide != null){
    		mSlide.setAntiAliasing(enabled);
    	}
    }
    
    /**
     * Check if anti aliasing is enabled for this slide.
     *
     *@return boolean <li>true �? if anti aliasing is enabled.
     * <li>false �? if anti aliasing is not enabled.
     */
    public boolean isAntiAliasingEnabled() {
    	
    	boolean isEnableAntiAlias = false;
    	if(mSlide != null){
    		isEnableAntiAlias =  mSlide.getAntiAliasing();
    	}
    	
    	return isEnableAntiAlias;
    }
    
    /**
     *  Set the specified Z order type.
     *  Set the priority of the slide is rendered. The value must lie between 0~2<p>
     *  <p> Zorder type is used only when the projection type is orthogonal.
     *  <p>{@link SVSlide#Z_ORDER_BOTTOM}= 0
     *  <p>{@link SVSlide#Z_ORDER_NORMAL} = 1
     *  <p>{@link SVSlide#Z_ORDER_TOP} = 2
     *  
     *  The Default value of the normal order but when you add the Slide as the child Slide the order of the child Slides are top to avoid the Z fighting of child.
     *  
     * @param type The Z order type.
     * 
     * <p>The top most slide is rendered last then normal slide and then the bottom side. So you see the top most slide on  top of other slide</p>
     * @throws IllegalArgumentException <li>1. ZOrderType can be take the value as 0,1 or 2 ,any other value throws an {@link IllegalArgumentException} 
     * <li>2. Z order type can only be used when the orthogonal projection is used for a particular Slide
     *
     */
    public void setZOrderType(int type) {
    	if(getProjectionType() != PROJECTION_ORTHOGONAL){
    		throw new IllegalArgumentException("ZOrderType must based on Orthogonal Projection.");
    	}
    	if(type <0 || type >2){
    		throw new IllegalArgumentException("Value must be between 0 ~ 2");
    	}
    	if(mSlide != null){
    		mSlide.setZOrderType(type);
    	}
    }
    /**
     *  Get the Z order type of the slide .
     *
     * @return int  The ZOrderType
     * 
     * <p>The top most slide is rendered last then normal slide and then the bottom side. So you see the top slide on the top.  </p>
     * <p> All the slides are initialized as the normal Slide .</p>
     * @throws IllegalArgumentException if you try to get the z order of the Slide with perspective projection
     * 
     */
    public int getZOrderType() {
    	if(getProjectionType() != PROJECTION_ORTHOGONAL){
    		throw new IllegalArgumentException("ZOrderType must based on Orthogonal Projection.");
    	}
    	if(mSlide != null){
    		return mSlide.getZOrderType();
    	}
    	
    	return Z_ORDER_NORMAL;
    }
    
    /**
     *  Set the specified Z Position value of the Slide.
     *  <p>Z Position only works for Perspective Projection type. 
     *  <p>Z position is maintained even when the you change the projection to the slide to the orthogonal.
     *     Z position is relative to the parent given that all the child also should be in the perspective projection
     *
     * @param zPosition the Z Position value of the slide
     * @throws IllegalArgumentException if the the projection choosen for the slide is Orthogonal.
     *
     */
    public void setZPosition(float zPosition){
    	if(getProjectionType() == PROJECTION_ORTHOGONAL){
			throw new IllegalArgumentException("Z Position must based on Perspective Projection.");
		}
    	if(mSlide != null){
    		 mSlide.setZOrderType(1);
    		 mSlide.setZPosition(zPosition);
    	}
    	
    }
    
    /**
     *  Set the specified Z Position value of the Slide.
     *  This will cause the implicit Z Position Animation.
     *  <p>Z Position only works for Perspective Projection type. 
     *  <p>Z position is maintained even when the you change the projection to the slide to the orthogonal.
     *     Z position is relative to the parent given that all the child also should be in the perspective projection
     *
     * @param zPosition the Z Position value of the slide
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1.If duration which which will cause the implicit animation is less then 0.
     * <li>2. Z position is specifically for the slides with the perspective projection. API can't be used by the slides with Orthogonal projection
 	 *
     */
    public void setZPosition(float zPosition, int duration){
    	checkLowValue(duration);
    	if(getProjectionType() == PROJECTION_ORTHOGONAL){
			throw new IllegalArgumentException("Z Position must based on Perspective Projection.");
		}
    	if(mSlide != null){
   		 	 mSlide.setZOrderType(1);
    		 mSlide.setZPosition(zPosition, duration);
    	}
    	
    }
    
    /**
     *  Get the Z Position value of the Slide.
     *  <p> the default Z value is 0.0f</p>
     *
     * @return float the Z Position value of the slide
 	 * @throws IllegalArgumentException if you try to access the Z position of a slide with orthogonal projection
     */
    public float getZPosition(){
    	
    	if(getProjectionType() == PROJECTION_ORTHOGONAL){
			throw new IllegalArgumentException("Z Position must based on Perspective Projection.");
		}
    	if(mSlide != null){
    		 return mSlide.getZPosition();
    	}
    	
    	return 0;
    	
    }
    
    /**
     * Set the specified border color for the slide.
     *
     * @param borderColor Border color for the slide.
     * @throws IllegalArgumentException if the borderColor({@link SVColor}) is not instantiated and is passed as a null argument to the API.
     *
     */
    public void setBorderColor(SVColor borderColor) {
    	
    	checkNullValue(borderColor);
    	float[] argColor = {0.5f, 0.5f, 0.5f, 1.0f};
    	
    	if(borderColor != null){
	    	argColor[3] = borderColor.getAlpha();
	    	argColor[0] = borderColor.getRed();
	    	argColor[1] = borderColor.getGreen();
	    	argColor[2] = borderColor.getBlue();
    	}
    	if(mSlide != null)
    		mSlide.setBorderColor(argColor);
    }
    
    /**
     * Set the specified border color for the slide.
     * This will cause implicit animation.
     *
     * @param borderColor Border color for the slide.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * @throws IllegalArgumentException <li>1. if the borderColor({@link SVColor}) is not instantiated and is passed as a null argument to the API.
     * <li>2. if the duration of the implicit animation is a negative value.
     *
     */
    public void setBorderColor(SVColor borderColor, int duration) {
    	
    	checkLowValue(duration);
    	checkNullValue(borderColor);
    	float[] argColor = {0.5f, 0.5f, 0.5f, 1.0f};
    	
    	if(borderColor != null){
	    	argColor[3] = borderColor.getAlpha();
	    	argColor[0] = borderColor.getRed();
	    	argColor[1] = borderColor.getGreen();
	    	argColor[2] = borderColor.getBlue();
    	}
    	if(mSlide != null)
    		mSlide.setBorderColor(argColor, duration);
    }
    
    /**
     * Get border-Color of slide.
     *
     * @return SVColor The current BorderColor for this slide.
     */
    public SVColor getBorderColor() {
        
    	SVIColor borderColor = null;
    	SVColor retBorderColor = null;
    	
    	if(mSlide != null){
    		borderColor = mSlide.getBorderColor();
    		if(borderColor != null){
    			retBorderColor = new SVColor();
    			retBorderColor.setColor(borderColor.mR,
				    					borderColor.mG,
				    					borderColor.mB,
				    					borderColor.mA);
    		}
    	}
    	
    	return retBorderColor;
        	
    }
    
    /**
     *  Set the specified border width for the slide.
     *
     * @param borderWidth Float value with the border width.
     * @throws IllegalArgumentException BorderWidth cannot be passes with a negative float value.
     * 
     */
    public void setBorderWidth(float borderWidth){
    	checkLowValue(borderWidth);
    	mBorderWidth = borderWidth;
    	if(borderWidth <= 0.01) {//hack to solve bug of setting 0 border after setting some other value.
    		borderWidth = 0.0101f;
    	}
    	
    	if(mSlide != null){
    		  mSlide.setBorderWidth(borderWidth);
    	}
    	
    	
    }
    
    /**
     *  Set the specified border width for the slide.
     *  This will cause implicit border-width animation.
     *  
     * @param borderWidth  Float value with the border width.
     * @param duration The duration of the implicit animation in milliseconds<p>Set to 0 if you do not want implicit animation
     * 
     * @throws IllegalArgumentException <li>1. if the borderWidth is passed as a negative value
     * <li>2. if the implicit animation duration is a negative integer.
     * 
     */
    public void setBorderWidth(float borderWidth, int duration){
    	checkLowValue(duration);
    	checkLowValue(borderWidth);
    	mBorderWidth = borderWidth;
    	
    	if(mSlide != null){
    		  mSlide.setBorderWidth(borderWidth, duration);
    	}
    	
    }
    
    /**
     *  Get the border width of the Slide.
     *  
     * @return Float value with the border width.
 	 *
     */
    public float getBorderWidth(){
    	
    	return mBorderWidth;
    	
    }
    
    
    /**
     * Get the current blending type for the slide.
     *
     * @return The current blending type.
     */
    public int getBlendType() {
    	
    	int blendType = 0;
    	if(mSlide != null){
    		blendType = mSlide.getBlendType();
    	}
    	return blendType;
    	
    }
    
    /**
     * Add the specified subslide to the slide.
     * 
     *  <p> if child to be added already has the parent then child get itself detached from this parent and considers the new Slide as its parent.
     *  
     * @param childSlide the slide to be added.
     * @throws IllegalArgumentException <li>1. In the slide tree a child cannot add itself or the ancestor as a its child. However a child can change its parent.
     * <li>2. The childSlide({@link SVSlide}) must not be a null argument. 
     */
    public void addSubSlide(SVSlide childSlide){
    	
    	checkNullValue(childSlide);
    	
    	
    	if(mSlide != null && childSlide != null){
    		if(this.equals(childSlide) || hasSlideAsAncestor(childSlide))
        		throw new IllegalArgumentException("Can't add self or parent as child");
    		if(mListSlide.contains(childSlide)) //childSlide is already added to parent ...nothing to Do So return
    			return;
    		
    		SVISlide childParent = childSlide.mSlide.getParent();
    		
    		if(childParent != null){ //if child already had parent .. remove the previous 
    			childSlide.mParent.mListSlide.remove(childSlide);
    			childParent.removeSlide(childSlide.mSlide);
    		}
    
    		mSlide.addSlide(childSlide.mSlide);
    		childSlide.mParent = this;
    		mListSlide.add(childSlide);
    		
    	}
    	
    }
    
    private boolean hasSlideAsAncestor(SVSlide slide){
    	
    	SVISlide rootSlide = null;
    	
    	rootSlide = mSlide.findUltimateRoot();
    	if(rootSlide == null)
    		return false;
    	SVISlide currentParent = mSlide.getParent();
    	
    	if(currentParent != null){	
    	
	    	while(!(currentParent == null )){
	    		if(currentParent.equals(slide.mSlide))
	    			return true;
	    		currentParent = currentParent.getParent();
			}
	    }
    	return false;
    }

    /**
     * Remove the specified sub slide.
     * Nothing will happen if the user passes the null child.
     * 
     * @param childSlide the sub-slide to be removed.
     */
    public void removeSlide(SVSlide childSlide){
    	
    	
    	if(mSlide != null && childSlide != null){
    		mSlide.removeSlide(childSlide.mSlide);
    		childSlide.mParent = null;
    		mListSlide.remove(childSlide);
    	}
    }
    
    /**
     * Get sub-slide count attched to the Slide.
     *
     * @return Number of  subslides of the slide.
     */
    public int getSubSlideCount() {
    	if(mListSlide == null){
    		return 0;
    	}
    	return mListSlide.size();    	
    }
    
    /**
     * Get the child slide attached to the slide specified by the index.
     *
     * @param index The index of the subslide.
     * @return SVSlide The subslide
     */
    public SVSlide getSubSlide(int index) {
    	
    	checkLowValue(index);
    	if(mListSlide == null){
    		return null;
    	}
    	return mListSlide.get(index);
    	
    }
    
    
    /**
     * Remove all the attached Slide to a particular Slide.
     * 
     */
    public void removeAllAttachedSlides() {
    	
    	int subSlideCount = mListSlide.size();
    	
    	if(mSlide != null && subSlideCount != 0){
    		for(int i = 0; i < subSlideCount; i++){
    			mSlide.removeSlide(mListSlide.get(i).mSlide);
    		}
    		if(mListSlide.size() != 0){
    			mListSlide.removeAll(mListSlide);
    		}
    	}
    }
    
    
    /**
     * Set the specified image for the slide.
     * 
     * @param image Image to be added to the slide.
     */
    public void setImage(SVImage image) {
    	
    	SVColor originBgColor = this.getBackgroundColor();
    	
    	mImage = image;
    	SVIImage argImage = new SVIImage();
    	
    	if(mImage != null){
	    	argImage.setBitmap(mImage.getBitmap());
	    	if(mSlide != null && argImage != null)
	    		mSlide.setImage(argImage);
	    	}
    	else{
	    	if(mSlide != null)
	        		mSlide.setImage((SVIImage)null);
    	}
    	
    	if(originBgColor != null){
    		setBackgroundColor(originBgColor);
    	}
    		
    }
    
    
    /**
     * Enable or disable slide hiding. 
     * <p> when a slide visibility is true then all the children's attached to the slide also hides
     *
     * @param enabled Set to true to enable slide hiding, set to false to disable slide hiding.
     */
    public void setHiddenEnabled(boolean enabled) {
    	
    	if(mSlide != null){
    		mSlide.setHidden(enabled);
    	}
    }
    
    /**
     * Check if slide hiding is enabled.
     *
     * @return true- if hiding is enabled ,otherwise false.
     */
    public boolean isHiddenEnabled() {
    	
    	if(mSlide != null)
    		return mSlide.getHidden();
    	
    	return false;
    }
  
    /**
     *Get the current image on the slide.
     *
     * @return SVImage object with the current image on the slide.
     */
    public SVImage getImage() {
    	
    	if(mSlide != null){
    		return mImage;
    	}
    	return null;
    }
    
    /**
     * Start the specified explicit animation on the slide.
     *
     * @param animation SVAnimation object with your animation.
     * @throws IllegalArgumentException you must not put a Null animation object as argument.
     */
    public void startAnimation(SVAnimation animation) {
    	
    	checkNullValue(animation);
    	if(mSlide != null ){
	    	if(animation instanceof SVKeyFrameAnimation){
	    		if(!((SVKeyFrameAnimation) animation).isKeyFrameTimeValid()){
					return;
				}
		    }
	    	if(animation instanceof SVTransitionAnimation){
	    		if(mParent == null){
	    			throw new IllegalArgumentException("Invalid transition animation uses. Slide is not the part of the Slide tree.");
	    		}
	    	}
	    	
			mSlide.startAnimation(animation.getAnimationFromEngine());
    	}
    }
    
    
    /**
     * Stop the animation currently running on the slide.
     */
    public void stopAnimation() {
    	
    	if(mSlide != null){
    		 mSlide.stopAnimation();
    	}	
    	
    }
     
    
    /**
     * Get the handle for the slide handle.
     * <p> you cannot set the value as it is internally assigned while creating the Slide.</p>
     * 
     * @return The value of Slide Handle.
     * 
     */
    public int getSlideHandle() {
		
    	if(mSlide != null){
    		return mSlide.getNativeSlideHandle();
    	}
    	
    	return -1;
	}

    /**
     * Set the specified image scale type for the slide. The value of image Scale type is (0 ~ 7). <p>
     * <p>{@linkplain SVSlide#IMAGE_SCALE_MATRIX} = 0 
     * <p>{@linkplain SVSlide#IMAGE_SCALE_FIT_XY} = 1;
     * <p>{@linkplain SVSlide#IMAGE_SCALE_FIT_START} = 2;
     * <p>{@linkplain SVSlide#IMAGE_SCALE_FIT_CENTER} = 3;
     * <p>{@linkplain SVSlide#IMAGE_SCALE_FIT_END} = 4;
     * <p>{@linkplain SVSlide#IMAGE_SCALE_CENTER} = 5;
     * <p>{@linkplain SVSlide#IMAGE_SCALE_CENTER_CROP} = 6;
     * <p>{@linkplain SVSlide#IMAGE_SCALE_CENTER_INSIDE} = 7;
     * <p> The default value is take as {@linkplain SVSlide#IMAGE_SCALE_FIT_XY}
     * 
     * @param imageSclaeType The image scale type to set
     * @throws IllegalArgumentException The ImageScale type must lie in the range 0~7, value out of the range throws {@link IllegalArgumentException}
     */
	public void setImageScaleType(int imageSclaeType) {
		if(imageSclaeType <0 || imageSclaeType>7){
			throw new IllegalArgumentException("ImageScaleType Must be between 0 ~ 7");
		}
		
		if(mSlide != null){
			mSlide.setImageScaleType(imageSclaeType);
		}
	}
	
	/**
	 * Get the image scale type for a slide.
	 *
	 * @return The Image Scale Type
	 */
	public int getImageScaleType() {
		if(mSlide != null){
			return mSlide.getImageScaleType();
		}
		
		return IMAGE_SCALE_CENTER;
	}
	
	/**
	 * Check if animation is running on the slide.
	 * 
	 * @return true - if animation is running on the slide
	 */
	public boolean isAnimating(){
		
		if(mSlide != null){
			return mSlide.isAnimating();
		}
		
		return false;
	}
	
	private void checkLowValue(float value) throws IllegalArgumentException{
		if(value < 0.0f){
			throw new IllegalArgumentException("Value must be bigger than 0.0f.");
		}
	}
	
	private void checkHighValue(float value) throws IllegalArgumentException{
		if(value > 1.0f){
			throw new IllegalArgumentException("Value must be smaller than 1.0f.");
		}
	}
	
	private void checkNullValue(Object object) throws IllegalArgumentException{
		
		if(object == null)
			throw new IllegalArgumentException("Value must not be null");
	}
	 
	
}

