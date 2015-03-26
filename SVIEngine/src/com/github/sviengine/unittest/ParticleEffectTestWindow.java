package com.github.sviengine.unittest;

import java.io.InputStream;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimation.SVIAnimationListener;
import com.github.sviengine.animation.SVIParticleAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.particle.SVIKeyFrameParticleEffect;
import com.github.sviengine.particle.SVIParticleEffect;
import com.github.sviengine.particle.SVIParticles;
import com.github.sviengine.slide.SVISlide;

public class ParticleEffectTestWindow extends PlatformWindow implements SVIAnimationListener {

	public ParticleEffectTestWindow(Context context) {
		super(context, false);
		Log.i("SVI", "ParticleEffectTestWindow(Context context)");
		Activity activity = (Activity)context;
		InputStream is;
		
		is = activity.getResources().openRawResource(R.drawable.particle);
		mParticleBitmap = BitmapFactory.decodeStream(is);
		
		mParticleImage = new SVIImage();
		mParticleImage.setBitmap(mParticleBitmap);
		
		//is = activity.getResources().openRawResource(R.drawable.);
		//mCloudBitmap = BitmapFactory.decodeStream(is);		
		//mCloudImage = new SVIImage(this);
		//mCloudImage.setBitmap(mCloudBitmap);
		
		if( getMainSlide() == null )
			buildSubSlide();
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		super.onResize(width, height);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if( event.getAction() == MotionEvent.ACTION_UP ) {
			runParticleEffect(event.getX(), event.getY());
			//runParticleEffect(event.getX(), event.getY());
			//runKeyFrameParticleEffect(event.getX(), event.getY());
			//runParticleAnimation(event.getX(),event.getY());
			//runKeyFrameParticleAnimation(event.getX(),event.getY());
		}else if( event.getAction() == MotionEvent.ACTION_MOVE ) {
			//runMoveParticleEffect(event.getX(),event.getY());
		}			
		return true;
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		
		SVISlide mainSlide = getMainSlide();
		
		mainSlide.setBackgroundColor(color);
		mainSlide.setHoldOpacity(true);
		mainSlide.setOrthogonal(true);
	}
		
	private void runMoveParticleEffect(float x, float y) {
		/*mSkipIndex++;		
		//if (mSkipIndex % 2 == 0) return;
		
		SVISlide mainSlide = getMainSlide();
		
		SVIParticleEffect effectInfo = new SVIParticleEffect();
		effectInfo.setDuration(1500);
		
		SVIParticles info = new SVIParticles();
		
		info.mParticleCount = 10;
		info.mMaxDuration = 1000;
		info.mRandomDuration = 10;
		
		info.mDefaultPosition.mX = x - mainSlide.getRegion().mSize.mWidth * 0.5f;
		info.mDefaultPosition.mY = y - mainSlide.getRegion().mSize.mHeight * 0.5f;
		
		info.mMaxMass = 2.0f;
		info.mRandomMass = 2.5f;
		
		info.mRandomColor.mR = 0.1f;
		info.mRandomColor.mG = 0.1f;
		info.mRandomColor.mB = 0.1f;
		info.mRandomColor.mA = 0.3f;
		
		info.mMaxColor.mR = 0.9f;
		info.mMaxColor.mG = 0.5f;
		info.mMaxColor.mB = 0.4f;
		info.mMaxColor.mA = 0.5f;
		
		info.mDefaultGravity.mX = 0.0f;
		info.mDefaultGravity.mY = 0.0f;
		info.mDefaultGravity.mZ = -0.98f;
		
		info.mRandomForce.mX = 2.0f;
		info.mRandomForce.mY = 2.0f;
		info.mRandomForce.mZ = -2.0f;
		
		info.mRandomPosition.mX = 10.0f;
		info.mRandomPosition.mY = 10.0f;
		info.mRandomPosition.mZ = 10.0f;
						
		info.mMaxParticleSize.mX = 30.0f;
		info.mMaxParticleSize.mY = 30.0f;
		info.mMaxParticleSize.mZ = 30.0f;
		info.mRandomParticleSize.mX = 10.0f;
		info.mRandomParticleSize.mY = 10.0f;
		info.mRandomParticleSize.mZ = 10.0f;
		
		info.setTexture("particle", mParticleImage);
		
		info.mScaleKeyFrameDuration = 1000;
		info.mScaleKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		
		info.addScaleKeyFrameFromInfo(0.0f, new SVIVector3(0.3f, 0.3f, 0.3f));
		info.addScaleKeyFrameFromInfo(0.1f, new SVIVector3(0.6f, 0.6f, 0.6f));
		info.addScaleKeyFrameFromInfo(1.0f, new SVIVector3(1.0f, 1.0f, 1.0f));					
		
		info.mColorKeyFrameDuration = 1000;
		info.mColorKeyFrameInterpolaterType = SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE;
		
		info.addColorKeyFrameFromInfo(0.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		info.addColorKeyFrameFromInfo(1.0f, new SVIColor(0.0f, 0.0f, 0.0f, 0.0f));
		
		effectInfo.addParticlesFromInfo(info);
		mainSlide.addParticleEffectFromInfo(effectInfo);*/
	}
	
	
	private void runParticleEffect(float x, float y) {
				
		SVISlide mainSlide = getMainSlide();
		
		SVIParticleEffect effect = new SVIParticleEffect(this);
		effect.setDuration(1500);
		
		SVIParticles particles = new SVIParticles(this);
		
		particles.setParticleCount(400);
		particles.setMaxDuration(1500);
		
		float posX = x - mainSlide.getRegion().mSize.mWidth * 0.5f;
		float posY = y - mainSlide.getRegion().mSize.mHeight * 0.5f;
		
		particles.setDefaultPosition(posX, posY, 0.0f);
		particles.setRandomPosition(10.0f, 10.0f, 10.0f);
		
		particles.setMaxMass(8.0f);
		particles.setRandomMass(4.0f);
		
		particles.setMaxColor(0.9f, 0.4f, 0.3f, 0.6f);
		particles.setRandomColor(0.1f, 0.1f, 0.1f, 0.15f);
		
		particles.setDefaultGravity(0.0f, 0.0f, -0.98f);
		
		particles.setRandomForce(5.0f, 5.0f, 5.0f);
		
		particles.setMaxSize(7.0f, 7.0f, 7.0f);
		particles.setRandomSize(0.5f, 0.5f, 0.5f);
		
		particles.setTextureFile("particle", mParticleImage);
		
		particles.setScaleKeyFrameAnimationProperty(1500, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addScaleKeyFrame(0.0f, new SVIVector3(1.0f, 1.0f, 1.0f));
		particles.addScaleKeyFrame(0.8f, new SVIVector3(2.0f, 2.0f, 2.0f));
		particles.addScaleKeyFrame(1.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		particles.addScaleKeyFrame(0.3f, new SVIVector3(1.5f, 1.5f, 1.5f));
				
		particles.setColorKeyFrameAnimationProperty(1500, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addColorKeyFrame(0.0f, new SVIColor(0.0f, 0.0f, 0.0f, 0.0f));
		particles.addColorKeyFrame(0.2f, new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		particles.addColorKeyFrame(1.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		
		effect.addParticles(particles);		
			
		particles.setParticleCount(5);
		particles.setMaxDuration(200);
		particles.setRandomDuration(50);
		
		particles.setMaxColor(1.0f, 1.0f, 1.0f, 1.0f);
		
		particles.setMaxSize(40.0f, 40.0f, 40.0f);
		particles.setRandomSize(10.0f, 10.0f, 10.0f);
		
		particles.setRandomPosition(150.0f, 150.0f, 150.0f);
		
		particles.setTextureFile("particle", mParticleImage);

		particles.setScaleKeyFrameAnimationProperty(200, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addScaleKeyFrame(0.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		particles.addScaleKeyFrame(0.2f, new SVIVector3(5.0f, 5.0f, 5.0f));
		particles.addScaleKeyFrame(1.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
				
		particles.setColorKeyFrameAnimationProperty(200, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		
		particles.addColorKeyFrame(0.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		particles.addColorKeyFrame(0.5f, new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		particles.addColorKeyFrame(1.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		
		effect.addParticles(particles);
		
		mainSlide.addParticleEffect(effect);
	}
	
	/****************************************************************************
	 * same runParticleEffect
	 * Slide Particle Effect - (ParticleEffect = KeyFrameParticleEffect)
	****************************************************************************/
	private void runKeyFrameParticleEffect(float x, float y) {
		
		SVISlide mainSlide = getMainSlide();
		
		SVIKeyFrameParticleEffect effect = new SVIKeyFrameParticleEffect(this);
		effect.setDuration(1500);
		
		SVIParticles particles = new SVIParticles(this);
		
		particles.setParticleCount(400);
		particles.setMaxDuration(1500);
		
		float posX = x - mainSlide.getRegion().mSize.mWidth * 0.5f;
		float posY = y - mainSlide.getRegion().mSize.mHeight * 0.5f;
		
		particles.setDefaultPosition(posX, posY, 0.0f);
		particles.setRandomPosition(10.0f, 10.0f, 10.0f);
		
		particles.setMaxMass(8.0f);
		particles.setRandomMass(4.0f);
		
		particles.setMaxColor(0.9f, 0.4f, 0.3f, 0.6f);
		particles.setRandomColor(0.1f, 0.1f, 0.1f, 0.15f);
		
		particles.setDefaultGravity(0.0f, 0.0f, -0.98f);
		
		particles.setRandomForce(5.0f, 5.0f, 5.0f);
		
		particles.setMaxSize(7.0f, 7.0f, 7.0f);
		particles.setRandomSize(0.5f, 0.5f, 0.5f);
		
		particles.setTextureFile("particle", mParticleImage);
		
		particles.setScaleKeyFrameAnimationProperty(1500, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addScaleKeyFrame(0.0f, new SVIVector3(1.0f, 1.0f, 1.0f));
		particles.addScaleKeyFrame(0.8f, new SVIVector3(2.0f, 2.0f, 2.0f));
		particles.addScaleKeyFrame(1.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		particles.addScaleKeyFrame(0.3f, new SVIVector3(1.5f, 1.5f, 1.5f));
				
		particles.setColorKeyFrameAnimationProperty(1500, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addColorKeyFrame(0.0f, new SVIColor(0.0f, 0.0f, 0.0f, 0.0f));
		particles.addColorKeyFrame(0.2f, new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		particles.addColorKeyFrame(1.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		
		effect.addParticles(0.0f, particles);		
			
		particles.setParticleCount(5);
		particles.setMaxDuration(200);
		particles.setRandomDuration(50);
		
		particles.setMaxColor(1.0f, 1.0f, 1.0f, 1.0f);
		
		particles.setMaxSize(40.0f, 40.0f, 40.0f);
		particles.setRandomSize(10.0f, 10.0f, 10.0f);
		
		particles.setRandomPosition(150.0f, 150.0f, 150.0f);
		
		particles.setTextureFile("particle", mParticleImage);

		particles.setScaleKeyFrameAnimationProperty(200, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addScaleKeyFrame(0.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		particles.addScaleKeyFrame(0.2f, new SVIVector3(5.0f, 5.0f, 5.0f));
		particles.addScaleKeyFrame(1.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
				
		particles.setColorKeyFrameAnimationProperty(200, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		
		particles.addColorKeyFrame(0.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		particles.addColorKeyFrame(0.5f, new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		particles.addColorKeyFrame(1.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		
		effect.addParticles(0.5f, particles);
		
		mainSlide.addParticleEffect(effect);
	}
	
	
	private void runParticleAnimation(float x, float y) {
		if(mAnimating) {
			return;
		}
		
		SVISlide mainSlide = getMainSlide();
		
		SVIParticleEffect effect = new SVIParticleEffect(this);
		effect.setDuration(1500);
		effect.setPositionKeyFrameAnimationProperty(1500, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		effect.addPositionKeyFrame(0.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		effect.addPositionKeyFrame(1.0f, new SVIVector3(500.0f, 0.0f, 0.0f));
		
		SVIParticles particles = new SVIParticles(this);
		
		particles.setParticleCount(400);
		particles.setMaxDuration(1500);
		
		float posX = x - mainSlide.getRegion().mSize.mWidth * 0.5f;
		float posY = y - mainSlide.getRegion().mSize.mHeight * 0.5f;
		
		particles.setDefaultPosition(posX, posY, 0.0f);
		particles.setRandomPosition(10.0f, 10.0f, 10.0f);
		
		particles.setMaxColor(0.9f, 0.4f, 0.3f, 0.6f);
		particles.setRandomColor(0.1f, 0.1f, 0.1f, 0.15f);
		
		particles.setMaxSize(7.0f, 7.0f, 7.0f);
		particles.setRandomSize(0.5f, 0.5f, 0.5f);
		
		particles.setTextureFile("particle", mParticleImage);
		
		particles.setScaleKeyFrameAnimationProperty(1500, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addScaleKeyFrame(0.0f, new SVIVector3(1.0f, 1.0f, 1.0f));
		particles.addScaleKeyFrame(0.8f, new SVIVector3(2.0f, 2.0f, 2.0f));
		particles.addScaleKeyFrame(1.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		particles.addScaleKeyFrame(0.3f, new SVIVector3(1.5f, 1.5f, 1.5f));
				
		particles.setColorKeyFrameAnimationProperty(1500, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addColorKeyFrame(0.0f, new SVIColor(0.0f, 0.0f, 0.0f, 0.0f));
		particles.addColorKeyFrame(0.2f, new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		particles.addColorKeyFrame(1.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		
		effect.addParticles(particles);
		
		SVIParticleAnimation particleAni = new SVIParticleAnimation();
		particleAni.setListener(this);
		particleAni.setDuration(1500);
		particleAni.setTag("ParticleEffect");
		
		particleAni.setParticleEffect(effect);
		
		mainSlide.startAnimation(particleAni);
	}
	
	
	/****************************************************************************
	 * different runParticleAnimation
	 * Particle Animation - (ParticleEffect != KeyFrameParticleEffect)
	****************************************************************************/
	private void runKeyFrameParticleAnimation(float x, float y) {
		if(mAnimating) {
			return;
		}
		
		SVISlide mainSlide = getMainSlide();
		
		//! particle effect setting
		mainSlide.setPivotPoint(0.0f, 0.0f);
		SVISize slideSize = mainSlide.getRegion().mSize;
		SVIKeyFrameParticleEffect effect = new SVIKeyFrameParticleEffect(this);
		effect.setDuration(2000);
		effect.setGeneratorPositionKeyFrameAnimationProperty(2000, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		effect.addGeneratorPositionKeyFrame(0.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		effect.addGeneratorPositionKeyFrame(1.0f, new SVIVector3(slideSize.mWidth, 0.0f, 0.0f));
		
		SVIParticles particles = new SVIParticles(this);
		
		particles.setParticleCount(100);
		particles.setMaxDuration(2000);
		
		particles.setDefaultPosition(500.0f, 300.0f, 0.0f);
		particles.setRandomPosition(slideSize.mWidth / 20.0f, slideSize.mHeight, 10.0f);
		
		particles.setDefaultForce(-0.1f, 0.0f, 0.0f);
		
		particles.setMaxMass(3.0f);
		particles.setRandomMass(2.0f);
		
		float particleSize = slideSize.mWidth / 20.0f;
		particles.setMaxSize(particleSize, particleSize, particleSize);
		particles.setRandomSize(particleSize / 10.0f, particleSize / 10.0f, particleSize / 10.0f);
		
		particles.setTextureFile("particle", mParticleImage);
		
		particles.setScaleKeyFrameAnimationProperty(2000, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addScaleKeyFrame(0.0f, new SVIVector3(1.0f, 1.0f, 1.0f));
		particles.addScaleKeyFrame(0.5f, new SVIVector3(2.0f, 2.0f, 2.0f));
		particles.addScaleKeyFrame(1.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
				
		particles.setColorKeyFrameAnimationProperty(2000, SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE, true);
		particles.addColorKeyFrame(0.0f, new SVIColor(0.0f, 0.0f, 0.0f, 0.0f));
		particles.addColorKeyFrame(0.2f, new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		particles.addColorKeyFrame(1.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		
		particles.setMaxColor(0.9f, 0.9f, 0.9f, 0.9f);
		particles.setRandomColor(0.0f, 0.0f, 0.0f, 0.1f);
		
		effect.addParticles(0.0f, particles);
		effect.addParticles(0.05f, particles);
		effect.addParticles(0.1f, particles);
		effect.addParticles(0.15f, particles);
		effect.addParticles(0.2f, particles);
		effect.addParticles(0.25f, particles);
		effect.addParticles(0.3f, particles);
		effect.addParticles(0.35f, particles);
		effect.addParticles(0.40f, particles);
		effect.addParticles(0.45f, particles);
		effect.addParticles(0.5f, particles);
		effect.addParticles(0.55f, particles);
		effect.addParticles(0.6f, particles);
		effect.addParticles(0.65f, particles);
		effect.addParticles(0.7f, particles);
		effect.addParticles(0.75f, particles);
		effect.addParticles(0.8f, particles);
		effect.addParticles(0.85f, particles);
		effect.addParticles(0.90f, particles);
		effect.addParticles(0.95f, particles);
		
		SVIParticleAnimation particleAni = new SVIParticleAnimation();
		particleAni.setListener(this);
		particleAni.setDuration(2000);
		particleAni.setTag("ParticleEffect");
		particleAni.setParticleEffect(effect);
		
		mainSlide.startAnimation(particleAni);
	}
	
	
	public void onStop(){
		clearRemainTextures();
	}
	
	
	@Override
	public void onAnimationEnd(String tag) {
		// TODO Auto-generated method stub
		Log.i("ParticleEffectTest", "AnimationEnd(Tag:" + tag +")");
		SVISlide mainSlide = getMainSlide();
		//mainSlide.setBackgroundColor(new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		mAnimating = false;
	}

	@Override
	public void onAnimationRepeat(String tag) {
		// TODO Auto-generated method stub
		Log.i("ParticleEffectTest", "AnimationRepeat(Tag:" + tag +")");
	}

	@Override
	public void onAnimationStart(String tag) {
		// TODO Auto-generated method stub
		Log.i("ParticleEffectTest", "AnimationStart(Tag:" + tag +")");
		SVISlide mainSlide = getMainSlide();
		//mainSlide.setBackgroundColor(new SVIColor(0.0f, 0.0f, 0.0f, 1.0f));
		mAnimating = true;
	}

	
	private int 	mSkipIndex = 0;
	private SVIImage mParticleImage = null;
	private Bitmap	mParticleBitmap = null;
	
	private SVIImage mCloudImage = null;
	private Bitmap	mCloudBitmap = null;
	
	private boolean	mAnimating = false;
}

