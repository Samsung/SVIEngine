package com.github.sviengine.performance;


import java.util.Random;

import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIPoint;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlideManager;
import com.github.sviengine.slide.SVISlide.BlendType;

import android.content.Context;
import android.util.Log;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class PropertyTestWindow extends PlatformWindow implements SVIAnimationListener{

	public PropertyTestWindow(Context context) {
		super(context);
		
		if( getMainSlide() == null )
			buildSubSlide();
		
	
	}
	
	
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	
	

	public void onConfigurationChanged(int rotation) {
	}
	
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		
		
		
		super.onResize(width, height);
	}
	
	protected Random tempRandom = new Random();
	
	public void runKeyFrameAnimation(int animationCase){
		
		int count = 0;

		if(isImplicit){
			
		}
		else {
			mSlideManager.setPauseImplicitAnimation(true);
		}
		
		for (int n = 0; n < mIconSlide.length; n++){
			SVISlide slide = mIconSlide[n];	
			
			
			SVIAnimationSet aniSet = new SVIAnimationSet();
			aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
			aniSet.setDuration(exDuration);
			aniSet.setRepeatCount(repeatCount);
			aniSet.setAutoReverse(false);
			aniSet.setListener(this);
			aniSet.setOffset(offset * count);

			
			/*region*/
			if(mCheckedList[REGION]) {
				SVIRect region = slide.getRegion();
				if(isImplicit){
					if(!isREGION){
						slide.setRegion(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*2.0f, region.mSize.mHeight*2.0f, imDuration);
						isREGION = true;
					}else{
						slide.setRegion(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*0.5f, region.mSize.mHeight*0.5f, imDuration);
						isREGION = false;
					}

				}
				else {
					slide.setImageScaleType(0);
					SVIKeyFrameAnimation keyRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.REGION);
					keyRegionAni.setDuration(exDuration);
					keyRegionAni.setRepeatCount(repeatCount);
					keyRegionAni.setAutoReverse(false);
					keyRegionAni.setOffset(offset * count);

					keyRegionAni.addKeyProperty(0.0f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.0f, region.mSize.mHeight*1.0f));
					keyRegionAni.addKeyProperty(0.3f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*2.0f, region.mSize.mHeight*2.0f));
					keyRegionAni.addKeyProperty(0.7f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*0.0f, region.mSize.mHeight*0.0f));
					keyRegionAni.addKeyProperty(1.0f, new SVIRect(region.mOrigin.mX,region.mOrigin.mY,region.mSize.mWidth*1.0f, region.mSize.mHeight*1.0f));
					aniSet.addAnimation(keyRegionAni);
				}

			}

			if(mCheckedList[POSITION]) {
				float x = slide.getPosition().mX;
				float y = slide.getPosition().mY;

				if(isImplicit){
					if(!isPOSITION){
						slide.setPosition(x+100.0f, y+100.0f, imDuration);
						isPOSITION = true;
					}else{
						slide.setPosition(x-100.0f, y-100.0f, imDuration);
						isPOSITION = false;
					}


				}
				else {
					SVIKeyFrameAnimation keyPositionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.POSITION);
					keyPositionAni.setDuration(exDuration);
					keyPositionAni.setRepeatCount(repeatCount);
					keyPositionAni.setAutoReverse(false);
					keyPositionAni.setOffset(offset * count);
					keyPositionAni.addKeyProperty(0.0f, new SVIPoint(x,y));
					keyPositionAni.addKeyProperty(0.3f, new SVIPoint(x+100.0f,y+100.0f));
					keyPositionAni.addKeyProperty(0.5f, new SVIPoint(x,y-50.0f));
					keyPositionAni.addKeyProperty(0.8f, new SVIPoint(x-100,y));
					keyPositionAni.addKeyProperty(1.0f, new SVIPoint(x,y));
					aniSet.addAnimation(keyPositionAni);
				}
			}


			if(mCheckedList[ZPOSITION]) {

				slide.setOrthogonal(false);
				slide.setZPosition(n*5.0f);
				if(n%2==1){
					if(isImplicit){
						if(!isZPOSITION){
							slide.setZPosition(-100.0f,imDuration);
							isZPOSITION = true;
						}else{
							slide.setZPosition(100.0f,imDuration);
							isZPOSITION = false;
						}
					}
					else {
						SVIKeyFrameAnimation keyZPosionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ZPOSITION);
						keyZPosionAni.setDuration(exDuration);
						keyZPosionAni.setRepeatCount(repeatCount);
						keyZPosionAni.setAutoReverse(false);
						keyZPosionAni.setOffset(offset * count);
						keyZPosionAni.addKeyProperty(0.0f, 0.0f);
						keyZPosionAni.addKeyProperty(0.3f, 100.0f);
						keyZPosionAni.addKeyProperty(0.7f, -100.0f);
						keyZPosionAni.addKeyProperty(1.0f, 0.0f);
						aniSet.addAnimation(keyZPosionAni);
					}
				}

			}

			if(mCheckedList[ROTATION]) {

				if(isImplicit){
					if(!isROTATION){
						slide.setRotation(30*(n+1), 30*(n+1), 30*(n+1), 0.0f,imDuration);
						isROTATION = true;
					}else{
						slide.setRotation(0.0f, 0.0f, 0.0f, 0.0f,imDuration);
						isROTATION = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyRotationAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.ROTATION);
					keyRotationAni.setDuration(exDuration);
					keyRotationAni.setRepeatCount(1);
					keyRotationAni.setAutoReverse(false);
					keyRotationAni.setOffset(offset * count);
					keyRotationAni.addKeyProperty(0.0f, new SVIVector3(0.0f,0.0f,0.0f));
					keyRotationAni.addKeyProperty(0.3f, new SVIVector3(0.0f,0.0f,180.0f));
					keyRotationAni.addKeyProperty(0.5f, new SVIVector3(0.0f,180.0f,0.0f));
					keyRotationAni.addKeyProperty(0.8f, new SVIVector3(180.0f,0.0f,0.0f));
					keyRotationAni.addKeyProperty(1.0f, new SVIVector3(0.0f,0.0f,0.0f));
					aniSet.addAnimation(keyRotationAni);
				}
			}

			if(mCheckedList[SCALE]) {

				if(isImplicit){
					if(!isSCALE){
						slide.setScale(1.5f, 1.5f, 0.0f, imDuration);
						isSCALE = true;
					}else{
						slide.setScale(1.0f, 1.0f, 0.0f, imDuration);
						isSCALE = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
					keyScaleAni.setDuration(exDuration);
					keyScaleAni.setRepeatCount(repeatCount);
					keyScaleAni.setAutoReverse(false);
					keyScaleAni.setOffset(offset * count);
					keyScaleAni.addKeyProperty(0.0f, new SVIPoint(1.0f,1.0f));
					keyScaleAni.addKeyProperty(0.3f, new SVIPoint(1.5f,1.5f));
					keyScaleAni.addKeyProperty(0.7f, new SVIPoint(0.0f,0.0f));
					keyScaleAni.addKeyProperty(1.0f, new SVIPoint(1.0f,1.0f));
					aniSet.addAnimation(keyScaleAni);
				}

			}

			if(mCheckedList[BG_COLOR]) {

				if(isImplicit){
					if(!isBG_COLOR){
						float[] tempBGColor = { tempRandom.nextFloat(), tempRandom.nextFloat(), 1.0f, 1.0f };
						slide.setBackgroundColor(tempBGColor, imDuration);
						isBG_COLOR = true;
					}else{
						slide.setBackgroundColor(defaultColor, imDuration);
						isBG_COLOR = false;
					}

				}
				else {
					SVIKeyFrameAnimation keyBgColorAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BG_COLOR);
					keyBgColorAni.setDuration(exDuration);
					keyBgColorAni.setRepeatCount(repeatCount);
					keyBgColorAni.setAutoReverse(false);
					keyBgColorAni.setOffset(offset * count);
					keyBgColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.8f));
					keyBgColorAni.addKeyProperty(0.3f, 
							new SVIColor(tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f,1.0f));
					keyBgColorAni.addKeyProperty(0.5f, 
							new SVIColor(tempRandom.nextFloat(),1.0f,tempRandom.nextFloat(),1.0f));
					keyBgColorAni.addKeyProperty(0.8f, 
							new SVIColor(1.0f,tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f));
					keyBgColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,1.0f));
					aniSet.addAnimation(keyBgColorAni);
				}
			}

			if(mCheckedList[OPACITY]) {

				if(isImplicit){
					if(!isOPACITY){
						slide.setOpacity(0.5f,imDuration);
						isOPACITY = true;
					}else{
						slide.setOpacity(1.0f,imDuration);
						isOPACITY = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
					keyOpacityAni.setDuration(exDuration);
					keyOpacityAni.setRepeatCount(repeatCount);
					keyOpacityAni.setAutoReverse(false);
					keyOpacityAni.setOffset(offset * count);
					keyOpacityAni.addKeyProperty(0.0f, 0.0f);
					keyOpacityAni.addKeyProperty(0.3f, 1.0f);
					keyOpacityAni.addKeyProperty(0.7f, 0.0f);
					keyOpacityAni.addKeyProperty(1.0f, 1.0f);
					aniSet.addAnimation(keyOpacityAni);
				}
			} 				 

			if(mCheckedList[CORNER_RADIUS]) {

				if(isImplicit){
					if(!isCORNER_RADIUS){
						slide.setCornerRadius(100.0f,imDuration);					
						isCORNER_RADIUS = true;
					}else{
						slide.setCornerRadius(0.0f,imDuration);					
						isCORNER_RADIUS = false;
					}
				}	
				else {
					SVIKeyFrameAnimation keyCornerRadiusAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.CORNER_RADIUS);
					keyCornerRadiusAni.setDuration(exDuration);
					keyCornerRadiusAni.setRepeatCount(repeatCount);
					keyCornerRadiusAni.setAutoReverse(false);
					keyCornerRadiusAni.setOffset(offset * count);
					keyCornerRadiusAni.addKeyProperty(0.0f, 0.0f);
					keyCornerRadiusAni.addKeyProperty(0.3f, 100.0f);
					keyCornerRadiusAni.addKeyProperty(0.7f, 40.0f);
					keyCornerRadiusAni.addKeyProperty(1.0f, 0.0f);
					aniSet.addAnimation(keyCornerRadiusAni);
				}

			}

			if(mCheckedList[BORDER_WIDTH]) {

				if(isImplicit){
					if(!isBORDER_WIDTH){
						slide.setBorderWidth(40.0f, imDuration);
						isBORDER_WIDTH = true;
					}else{
						slide.setBorderWidth(0.0f, imDuration);
						isBORDER_WIDTH = false;
					}

				}
				else {
					SVIKeyFrameAnimation keyBorderWidthAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BORDER_WIDTH);
					keyBorderWidthAni.setDuration(exDuration);
					keyBorderWidthAni.setRepeatCount(repeatCount);
					keyBorderWidthAni.setAutoReverse(false);
					keyBorderWidthAni.setOffset(offset * count);
					keyBorderWidthAni.addKeyProperty(0.0f, 15.0f);
					keyBorderWidthAni.addKeyProperty(0.3f, 40.0f);
					keyBorderWidthAni.addKeyProperty(0.7f, 0.0f);
					keyBorderWidthAni.addKeyProperty(1.0f, 15.0f);
					aniSet.addAnimation(keyBorderWidthAni);
				}
			}

			if(mCheckedList[BORDER_COLOR]) {
				slide.setBorderWidth(20.0f);
				if(isImplicit){
					if(!isBORDER_COLOR){
						float[] tempBorderColor = { 1.0f, tempRandom.nextFloat(), tempRandom.nextFloat(), 1.0f };
						slide.setBackgroundColor(tempBorderColor, imDuration);
						isBORDER_COLOR = true;
					}else{
						slide.setBackgroundColor(defaultDarkColor, imDuration);
						isBORDER_COLOR = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyBorderColorAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.BORDER_COLOR);
					keyBorderColorAni.setDuration(exDuration);
					keyBorderColorAni.setRepeatCount(repeatCount);
					keyBorderColorAni.setAutoReverse(false);
					keyBorderColorAni.setOffset(offset * count);
					keyBorderColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.8f));
					keyBorderColorAni.addKeyProperty(0.3f, 
							new SVIColor(tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f,1.0f));
					keyBorderColorAni.addKeyProperty(0.5f, 
							new SVIColor(tempRandom.nextFloat(),1.0f,tempRandom.nextFloat(),1.0f));
					keyBorderColorAni.addKeyProperty(0.8f, 
							new SVIColor(1.0f,tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f));
					keyBorderColorAni.addKeyProperty(1.0f, new SVIColor(0.5f,0.5f,0.5f,1.0f));
					aniSet.addAnimation(keyBorderColorAni);
				}
			}


			if(mCheckedList[SHADOW_RADIUS]) {
				if(isImplicit){
					if(!isSHADOW_RADIUS){
						slide.setShadowRadius(100.0f, imDuration);
						isSHADOW_COLOR = true;
					}else{
						slide.setShadowRadius(0.0f, imDuration);
						isSHADOW_COLOR = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyShadowAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_RADIUS);
					keyShadowAni.setDuration(exDuration);
					keyShadowAni.setRepeatCount(repeatCount);
					keyShadowAni.setAutoReverse(false);
					keyShadowAni.setOffset(offset * count);
					keyShadowAni.addKeyProperty(0.0f, 30.0f);
					keyShadowAni.addKeyProperty(0.3f, 100.0f);
					keyShadowAni.addKeyProperty(0.7f, 0.0f);
					keyShadowAni.addKeyProperty(1.0f, 30.0f);
					aniSet.addAnimation(keyShadowAni);
				}
			}

			if(mCheckedList[SHADOW_COLOR]) {

				if(isImplicit){
					if(!isSHADOW_COLOR){
						float[] tempShadowColor = { tempRandom.nextFloat(), tempRandom.nextFloat(), 1.0f, 1.0f };
						slide.setShadowColor(tempShadowColor, imDuration);
						isSHADOW_COLOR = true;
					}else{
						slide.setShadowColor(defaultDarkColor, imDuration);
						isSHADOW_COLOR = false;
					}

				}
				else {
					SVIKeyFrameAnimation keyShadowColorAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_COLOR);
					keyShadowColorAni.setDuration(exDuration);
					keyShadowColorAni.setRepeatCount(repeatCount);
					keyShadowColorAni.setAutoReverse(false);
					keyShadowColorAni.setOffset(offset * count);
					keyShadowColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.8f));
					keyShadowColorAni.addKeyProperty(0.3f, 
							new SVIColor(tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f,1.0f));
					keyShadowColorAni.addKeyProperty(0.5f, 
							new SVIColor(tempRandom.nextFloat(),1.0f,tempRandom.nextFloat(),1.0f));
					keyShadowColorAni.addKeyProperty(0.8f, 
							new SVIColor(1.0f,tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f));
					keyShadowColorAni.addKeyProperty(1.0f, new SVIColor(0.0f,0.0f,0.0f,1.0f));
					aniSet.addAnimation(keyShadowColorAni);
				}
			}

			if(mCheckedList[SHADOW_OPACITY]) {
				if(isImplicit){
					if(!isSHADOW_OPACITY){
						slide.setShadowOpacity(0.5f, imDuration);
						isSHADOW_OPACITY = true;
					}else{
						slide.setShadowOpacity(1.0f, imDuration);
						isSHADOW_OPACITY = false;
					}

				}
				else {
					SVIKeyFrameAnimation keyShadowOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OPACITY);
					keyShadowOpacityAni.setDuration(exDuration);
					keyShadowOpacityAni.setRepeatCount(repeatCount);
					keyShadowOpacityAni.setAutoReverse(false);
					keyShadowOpacityAni.setOffset(offset * count);
					keyShadowOpacityAni.addKeyProperty(0.0f, 0.0f);
					keyShadowOpacityAni.addKeyProperty(0.3f, 1.0f);
					keyShadowOpacityAni.addKeyProperty(0.7f, 0.0f);
					keyShadowOpacityAni.addKeyProperty(1.0f, 1.0f);
					aniSet.addAnimation(keyShadowOpacityAni);
				}

			}

			if(mCheckedList[SHADOW_OFFSET]) {
				if(isImplicit){
					if(!isSHADOW_OFFSET){
						slide.setShadowOffset(100.0f, 100.0f, imDuration);
						isSHADOW_OFFSET = true;
					}else{
						slide.setShadowOffset(10.0f, 10.0f, imDuration);
						isSHADOW_OFFSET = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyShadowOffsetAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SHADOW_OFFSET);
					keyShadowOffsetAni.setDuration(exDuration);
					keyShadowOffsetAni.setRepeatCount(repeatCount);
					keyShadowOffsetAni.setAutoReverse(false);
					keyShadowOffsetAni.setOffset(offset * count);
					keyShadowOffsetAni.addKeyProperty(0.0f, new SVIPoint(10.0f,10.0f));
					keyShadowOffsetAni.addKeyProperty(0.5f, new SVIPoint(100.0f,100.0f));
					keyShadowOffsetAni.addKeyProperty(1.0f, new SVIPoint(10.0f,10.0f));
					aniSet.addAnimation(keyShadowOffsetAni);
				}

			}


			if(mCheckedList[TEXTURE_REGION]) {
				SVIRect textureRegion = slide.getRegion();
				if(isImplicit){
					slide.setTextureRegion(0.0f,0.0f,textureRegion.mOrigin.mX/2, textureRegion.mOrigin.mY/2, imDuration);
				}
				else {
					SVIKeyFrameAnimation keyTextureRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.TEXTURE_REGION);
					keyTextureRegionAni.setDuration(exDuration);
					keyTextureRegionAni.setRepeatCount(repeatCount);
					keyTextureRegionAni.setAutoReverse(false);
					keyTextureRegionAni.setOffset(offset * count);

					keyTextureRegionAni.addKeyProperty(0.0f, new SVIRect(0.0f,0.0f,textureRegion.mOrigin.mX/2, textureRegion.mOrigin.mY/2));
					keyTextureRegionAni.addKeyProperty(0.3f, new SVIRect(0.0f,0.0f,textureRegion.mOrigin.mX/4, textureRegion.mOrigin.mY/2));
					keyTextureRegionAni.addKeyProperty(0.7f, new SVIRect(0.0f,0.0f,textureRegion.mOrigin.mX/2, textureRegion.mOrigin.mY/4));
					keyTextureRegionAni.addKeyProperty(1.0f, new SVIRect(0.0f,0.0f,textureRegion.mOrigin.mX/2, textureRegion.mOrigin.mY/2));
					aniSet.addAnimation(keyTextureRegionAni);
				}
			}


			if(mCheckedList[LIGHT_RADIUS]) {
				slide.setLightType(n%2+1);
				slide.setLightRadius(0.3f);

				if(isImplicit){
					if(!isLIGHT_RADIUS){
						slide.setLightRadius(1.0f, imDuration);
						isLIGHT_RADIUS = true;
					}else{
						slide.setLightRadius(0.3f, imDuration);
						isLIGHT_RADIUS = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyLightRadiusAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_RADIUS);
					keyLightRadiusAni.setDuration(exDuration);
					keyLightRadiusAni.setRepeatCount(repeatCount);
					keyLightRadiusAni.setAutoReverse(false);
					keyLightRadiusAni.setOffset(offset * count);
					keyLightRadiusAni.addKeyProperty(0.0f, 0.3f);
					keyLightRadiusAni.addKeyProperty(0.5f, 1.0f);
					keyLightRadiusAni.addKeyProperty(0.8f, 0.0f);
					keyLightRadiusAni.addKeyProperty(1.0f, 0.3f);
					aniSet.addAnimation(keyLightRadiusAni);
				}
			}

			if(mCheckedList[LIGHT_COLOR]) {
				slide.setLightType(n%2+1);
				slide.setLightRadius(0.3f);

				if(isImplicit){
					if(!isLIGHT_COLOR){
						float[] tempLightColor = { tempRandom.nextFloat(), tempRandom.nextFloat(), 1.0f, 1.0f };
						slide.setLightColor(tempLightColor, imDuration);
						isLIGHT_COLOR = true;
					}else{
						slide.setLightColor(defaultColor,imDuration);
						isLIGHT_COLOR = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyLightColorAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_COLOR);
					keyLightColorAni.setDuration(exDuration);
					keyLightColorAni.setRepeatCount(repeatCount);
					keyLightColorAni.setAutoReverse(false);
					keyLightColorAni.setOffset(offset * count);
					keyLightColorAni.addKeyProperty(0.0f, new SVIColor(1.0f,1.0f,1.0f,0.8f));
					keyLightColorAni.addKeyProperty(0.3f, 
							new SVIColor(tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f,1.0f));
					keyLightColorAni.addKeyProperty(0.5f, 
							new SVIColor(tempRandom.nextFloat(),1.0f,tempRandom.nextFloat(),1.0f));
					keyLightColorAni.addKeyProperty(0.8f, 
							new SVIColor(1.0f,tempRandom.nextFloat(),tempRandom.nextFloat(),1.0f));
					keyLightColorAni.addKeyProperty(1.0f, new SVIColor(1.0f,1.0f,1.0f,0.8f));
					aniSet.addAnimation(keyLightColorAni);
				}
			}

			if(mCheckedList[LIGHT_OPACITY]) {
				slide.setLightType(n%2+1);
				slide.setLightRadius(0.3f);

				if(isImplicit){
					if(!isLIGHT_OPACITY){
						slide.setLightRadius(1.0f, imDuration);
						isLIGHT_OPACITY = true;
					}else{
						slide.setLightRadius(0.3f, imDuration);
						isLIGHT_OPACITY = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyLightOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_OPACITY);
					keyLightOpacityAni.setDuration(exDuration);
					keyLightOpacityAni.setRepeatCount(repeatCount);
					keyLightOpacityAni.setAutoReverse(false);
					keyLightOpacityAni.setOffset(offset * count);
					keyLightOpacityAni.addKeyProperty(0.0f, 0.0f);
					keyLightOpacityAni.addKeyProperty(0.3f, 1.0f);
					keyLightOpacityAni.addKeyProperty(0.7f, 0.0f);
					keyLightOpacityAni.addKeyProperty(1.0f, 1.0f);
					aniSet.addAnimation(keyLightOpacityAni);
				}
			}

			if(mCheckedList[LIGHT_ANGLE]) {
				slide.setLightType(2);
				slide.setLightRadius(0.3f);

				if(isImplicit){
					if(!isLIGHT_ANGLE){
						slide.setLightAngle(270.0f, imDuration);
						isLIGHT_ANGLE = true;
					}else{
						slide.setLightAngle(-270.0f, imDuration);
						isLIGHT_ANGLE = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyLightAngleAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_ANGLE);
					keyLightAngleAni.setDuration(exDuration);
					keyLightAngleAni.setRepeatCount(repeatCount);
					keyLightAngleAni.setAutoReverse(false);
					keyLightAngleAni.setOffset(offset * count);
					keyLightAngleAni.addKeyProperty(0.0f, 0.0f);
					keyLightAngleAni.addKeyProperty(0.3f, 90.0f);
					keyLightAngleAni.addKeyProperty(0.5f, 270.0f);
					keyLightAngleAni.addKeyProperty(0.8f, -90.0f);
					keyLightAngleAni.addKeyProperty(1.0f, 0.0f);
					aniSet.addAnimation(keyLightAngleAni);
				}

			}

			if(mCheckedList[LIGHT_OFFSET]) {
				slide.setLightType(n%2+1);
				slide.setLightRadius(0.3f);

				if(isImplicit){
					if(!isLIGHT_OFFSET){
						slide.setLightOffset(1.0f, 1.0f, imDuration);
						isLIGHT_OFFSET = true;
					} else {
						slide.setLightOffset(0.5f, 0.5f, imDuration);
						isLIGHT_OFFSET = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyLightOffsetAni = new 
					SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_OFFSET);
					keyLightOffsetAni.setDuration(exDuration);
					keyLightOffsetAni.setRepeatCount(repeatCount);
					keyLightOffsetAni.setAutoReverse(false);
					keyLightOffsetAni.setOffset(offset * count);
					keyLightOffsetAni.addKeyProperty(0.0f, new SVIPoint(0.0f,0.1f));
					keyLightOffsetAni.addKeyProperty(0.3f, new SVIPoint(0.0f,0.0f));
					keyLightOffsetAni.addKeyProperty(0.5f, new SVIPoint(0.5f,0.5f));
					keyLightOffsetAni.addKeyProperty(0.8f, new SVIPoint(1.0f,1.0f));
					keyLightOffsetAni.addKeyProperty(1.0f, new SVIPoint(0.5f,0.1f));
					aniSet.addAnimation(keyLightOffsetAni);
				}

			}

			if(mCheckedList[LIGHT_POWER]) {
				slide.setLightType(n%2+1);
				slide.setLightRadius(0.3f);

				if(isImplicit){
					if(!isLIGHT_POWER){
						slide.setLightPower(3.0f, imDuration);
						isLIGHT_POWER = true;
					}else{
						slide.setLightPower(1.0f, imDuration);
						isLIGHT_POWER = false;
					}
				}
				else {
					SVIKeyFrameAnimation keyLightPowerAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_POWER);
					keyLightPowerAni.setDuration(exDuration);
					keyLightPowerAni.setRepeatCount(repeatCount);
					keyLightPowerAni.setAutoReverse(false);
					keyLightPowerAni.setOffset(offset * count);
					keyLightPowerAni.addKeyProperty(0.0f, 1.0f);
					keyLightPowerAni.addKeyProperty(0.3f, 7.0f);
					keyLightPowerAni.addKeyProperty(0.7f, 0.0f);
					keyLightPowerAni.addKeyProperty(1.0f, 1.0f);
					aniSet.addAnimation(keyLightPowerAni);
				}

			}

			if(mCheckedList[SCALETOFIT_REGION]) {
				slide.setImageScaleType(0);
				float height = slide.getImage().mBitmap.getHeight();
				float width = slide.getImage().mBitmap.getWidth();

				if(isImplicit){
					if(!isSCALETOFIT_REGION){
						slide.setScaleToFitRegion(0.0f,0.0f,width/2,height/2,imDuration);
						isSCALETOFIT_REGION = true;
					}else{

						slide.setScaleToFitRegion(0.0f,0.0f,width*2,height*2,imDuration);
						isSCALETOFIT_REGION = false;
					}

				}
				else {

					SVIKeyFrameAnimation keyTexRegionAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALETOFIT_REGION);
					keyTexRegionAni.setDuration(exDuration);
					keyTexRegionAni.setRepeatCount(repeatCount);
					keyTexRegionAni.setAutoReverse(false);
					keyTexRegionAni.setOffset(offset * count);
					keyTexRegionAni.addKeyProperty(0.0f, new SVIRect(0.0f,0.0f,width, height));
					keyTexRegionAni.addKeyProperty(0.3f, new SVIRect(-width/2,0.0f,width, height));
					keyTexRegionAni.addKeyProperty(0.5f, new SVIRect(0.0f,0.0f,width/2, height/2));
					keyTexRegionAni.addKeyProperty(0.8f, new SVIRect(-width/2,0.0f,width, height));
					keyTexRegionAni.addKeyProperty(1.0f, new SVIRect(0.0f,0.0f,width, height));
					aniSet.addAnimation(keyTexRegionAni);
				}
			}		
				

			if(isImplicit){
				
			}
			else {
				slide.startAnimation(aniSet);
			}
			count++;
		}
		mSlideManager.setPauseImplicitAnimation(false);
		mSlideManager.requestRender();
		
		mCheckedList = new boolean[SCALETOFIT_REGION - REGION + 1];
	}
	
	
	
	
	
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 0.651f, 0.792f, 0.941f, 1.0f };
				
		SVISlide mainSlide = getMainSlide();		
		
		mainSlide.setBackgroundColor(color);
		
		
	
		mIconSlide = new SVISlide[LANDSCAPE_COLUMN_COUNT*LANDSCAPE_ROW_COUNT];
		
		
		int imageCnt = mImages.length-1;
		int rowCount = PORTRAIT_ROW_COUNT;
		int columnCount = PORTRAIT_COLUMN_COUNT;
		int count = 0;

		if( mWidth > mHeight ) {
			columnCount = LANDSCAPE_COLUMN_COUNT;
			rowCount = LANDSCAPE_ROW_COUNT;
		}

		mCurrentRowCount = rowCount;
		mCurrentColumnCount = columnCount;

		float x, y;
		

		int  slideCount = rowCount * columnCount;
		
		boolean testSlideHierarchy = false;
		for(int i=0; i<slideCount; i++) {
		
			int imgIdx = count % (imageCnt);
			x = mWidth/2-ICON_WIDTH;
			y = mHeight/2 - ICON_HEIGHT;
			if( !testSlideHierarchy ) {
				mIconSlide[i] = new SVISlide(0, 0, x + (i*ICON_WIDTH/2), y + (i*ICON_HEIGHT/2), ICON_WIDTH, ICON_HEIGHT, color);
			} else {
				mIconSlide[i] = new SVISlide(0, 0, (3), (3), ICON_WIDTH, ICON_HEIGHT, color);
			}
			
			if( !testSlideHierarchy ) {
				mainSlide.addSlide(mIconSlide[i]);
			} else {
				if( i == 0 )
					mainSlide.addSlide(mIconSlide[i]);
				else 
					mIconSlide[i-1].addSlide(mIconSlide[i]);
			}
		
			mIconSlide[i].setImage(mImages[imgIdx]);
			
			
			mIconSlide[i].setBlendType(BlendType.BLEND_MULTIPLY);

			
			
			
		
			mIconSlide[i].setShadowRadius(20.0f);
			mIconSlide[i].setShadowOffset(0.0f, 10.0f);
			mIconSlide[i].setShadowOpacity(0.5f);
			mIconSlide[i].setShadowRadius(40.0f);
			
		
			
			
			
			
			count++;
		}
		
		
	}

	@Override
	public void onAnimationEnd(String tag) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		
		if( event.getAction() == MotionEvent.ACTION_DOWN ){
			//runKeyFrameAnimation();
			mCheckedList[mProperty] = true;			
			runKeyFrameAnimation(mProperty);
			Log.i("test", mProperty+"");
			
		}
		
		requestRender();
		return true;
	}
	
	
	
	protected void removeall(){
		SVISlide mainSlide = getMainSlide();		
			
		if (mIconSlide != null){
			int  slideCount = mCurrentRowCount * mCurrentColumnCount;			
			for(int i=0; i<slideCount; i++) {
				mainSlide.removeSlide(mIconSlide[i]);
				mIconSlide[i] = null;
			}
		}
		mIconSlide = null;
	}
	
	
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub	
		String msg = item.getTitle().toString();
		Log.i("SVI", msg);
		if(msg == "Explicit") {
			isImplicit = false;
		}
		
		else if(msg == "Implicit"){
			isImplicit = true;
		}
		return super.onOptionsItemSelected(item);
	}

	public boolean[] getCheckedList() {
		return mCheckedList;
	}

	public void setCheckedList(boolean[] checkedList) {
		mCheckedList = checkedList;
	}


	public static final int REGION = 0;
	public static final int POSITION = 1;
	public static final int ZPOSITION = 2;
	public static final int ROTATION = 3;
	public static final int SCALE = 4;
	public static final int BG_COLOR = 5;
	public static final int OPACITY = 6;
	public static final int CORNER_RADIUS = 7;
	public static final int BORDER_WIDTH = 8;
	public static final int BORDER_COLOR = 9;
	public static final int SHADOW_RADIUS = 10;
	public static final int SHADOW_COLOR = 11;
	public static final int SHADOW_OPACITY = 12;
	public static final int SHADOW_OFFSET = 13;
			
	
	public static final int TEXTURE_REGION = 14;
	
	
	public static final int LIGHT_RADIUS = 15; 
	public static final int LIGHT_COLOR = 16;
	public static final int LIGHT_OPACITY = 17;
	public static final int LIGHT_ANGLE = 18;
	public static final int LIGHT_OFFSET = 19;
	public static final int LIGHT_POWER = 20;
	
			
	public static final int SCALETOFIT_REGION = 21;
	
	protected boolean isREGION = false;
	protected boolean isPOSITION = false;
	protected boolean isZPOSITION = false;
	protected boolean isROTATION = false;
	protected boolean isSCALE = false;
	protected boolean isBG_COLOR = false;
	protected boolean isOPACITY = false;
	protected boolean isCORNER_RADIUS = false;
	protected boolean isBORDER_WIDTH = false;
	protected boolean isBORDER_COLOR = false;
	protected boolean isSHADOW_RADIUS = false;
	protected boolean isSHADOW_COLOR = false;
	protected boolean isSHADOW_OPACITY = false;
	protected boolean isSHADOW_OFFSET = false;
	protected boolean isTEXTURE_REGION = false;
	protected boolean isLIGHT_RADIUS = false; 
	protected boolean isLIGHT_COLOR = false;
	protected boolean isLIGHT_OPACITY = false;
	protected boolean isLIGHT_ANGLE = false;
	protected boolean isLIGHT_OFFSET = false;
	protected boolean isLIGHT_POWER = false;
	protected boolean isSCALETOFIT_REGION = false;
	
	
	
	
	protected SVISlide[]			mIconSlide = null;

	protected int				PORTRAIT_COLUMN_COUNT = 1;
	protected int 				PORTRAIT_ROW_COUNT = 3;

	protected float 			ICON_WIDTH		= 180.0f;
	protected float 			ICON_HEIGHT		= 180.0f;
	
	protected float 			defalutLocation = 100.0f;
	
	protected int 				LANDSCAPE_COLUMN_COUNT	= PORTRAIT_ROW_COUNT;
	protected int 				LANDSCAPE_ROW_COUNT		= PORTRAIT_COLUMN_COUNT;
	
	protected float[]			defaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	protected float[]			defaultDarkColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	
	protected int 				exDuration = 5000;
	protected int				imDuration = 1000;
	
	final protected int  		offset = 0;
	final protected int 		repeatCount = 0;
	
	protected boolean 			isImplicit = false;
	
	protected int mProperty=0;
	
	
	
	
	
	Random mRandom = new Random();
	int mCurrentRowCount = 0;
	int mCurrentColumnCount = 0;
	
	protected boolean[]				mCheckedList = new boolean[SCALETOFIT_REGION - REGION + 1];
}



