package com.github.sviengine.unittest;

import java.io.InputStream;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.util.Log;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.github.sviengine.R;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIParticleAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVISize;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.particle.SVIKeyFrameParticleEffect;
import com.github.sviengine.particle.SVIParticles;
import com.github.sviengine.slide.SVISlide;
import com.github.sviengine.slide.SVISlide.SlideSwipeType;

public class SparkleSampleWindow extends PlatformWindow{

	
	protected Handler 	mParticleHandler = null;
	protected int		mParticleCount = 0;
	protected float		mParticleGenX = 0.0f;
	
	public SparkleSampleWindow(Context context) {
		super(context, false);
		Log.i("SVI", "Sparkle Sample Window(Context context)");
		Activity activity = (Activity)context;
		InputStream is;
		
		
		is = activity.getResources().openRawResource(R.drawable.particle);
		mParticleBitmap = BitmapFactory.decodeStream(is);
		
		mParticleImage = new SVIImage();
		mParticleImage.setBitmap(mParticleBitmap);
		
		is = activity.getResources().openRawResource(R.drawable.art_1);
		mSlideBitmap = BitmapFactory.decodeStream(is);
		mSlideImage = new SVIImage();
		mSlideImage.setBitmap(mSlideBitmap);
		
		is = activity.getResources().openRawResource(R.drawable.art_3);
		mBackSlideBitmap = BitmapFactory.decodeStream(is);
		mBackSlideImage = new SVIImage();
		mBackSlideImage.setBitmap(mBackSlideBitmap);
						
		if( getMainSlide() == null )
			buildSubSlide();
				
		/*mParticleHandler = new Handler() {
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);				
				
				SVISize slideSize = mLinearSlide.getRegion().mSize;	
				
				float yOffset = -slideSize.mHeight * 0.5f;
				float xOffset = -slideSize.mWidth * 0.5f;
				for (int n = 0; n < 13; n++){
					float yRange = slideSize.mHeight * mRandom.nextFloat();
					runMoveParticleEffect(mLinearSlide, mParticleGenX+xOffset, yOffset + yRange);
				}												
								
				mParticleGenX += (slideSize.mWidth / 50.0f) ;
								
				mParticleCount++;
				if (mParticleCount >= 50) {
					mParticleCount = 0;
					removeMessages(0);
					return;
				}
				sendEmptyMessageDelayed(0, 20);
			}
		};*/
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
			runRotationSlide(mLinearSlide);
		}
		
		return true;
	}
	
	
	private boolean mModeSelected = false;
	private boolean mDirectionSelected = false;
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {		
		return true;
	}
	
		
	protected void runRotationSlide(SVISlide slide){
		int aniSetDuration, swipeDuration, particleDuration;
		aniSetDuration = 3000;
		swipeDuration = 2000;
		particleDuration = 700;
		
		SVIAnimationSet slideAniSet = new SVIAnimationSet();
		slideAniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.LINEAR);
		slideAniSet.setDuration(aniSetDuration);
		slideAniSet.setOffset(0);
		slideAniSet.setRepeatCount(0);
		slideAniSet.setAutoReverse(false);
				
		SVIKeyFrameAnimation swipeAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SWIPE_RATIO);
		swipeAni.setDuration(swipeDuration);
		swipeAni.setRepeatCount(0);
		swipeAni.setOffset(0);
		swipeAni.setAutoReverse(false);
		swipeAni.addKeyProperty(0.0f, new SVIVector3(0.0f, 0.0f, mDirectionSelected ? 1.0f : 0.0f));
		swipeAni.addKeyProperty(1.0f, new SVIVector3(1.0f, 0.05f, mDirectionSelected ? 1.0f : 0.0f));
		
		
		//! particle effect setting
		slide.setPivotPoint(0.0f, 0.0f);
		SVISize slideSize = slide.getRegion().mSize;
		slide.setPivotPoint(0.0f, 0.0f);
		SVIKeyFrameParticleEffect effect = new SVIKeyFrameParticleEffect(this);
		effect.setDuration(aniSetDuration);
		effect.setGeneratorPositionKeyFrameAnimationProperty(swipeDuration, SVIAnimation.InterpolatorType.LINEAR, true);
		effect.addGeneratorPositionKeyFrame( 0.0f, new SVIVector3(0.0f, 0.0f, 0.0f) );
		effect.addGeneratorPositionKeyFrame( 1.0f, new SVIVector3(slideSize.mWidth, 0.0f, 0.0f) );
		
		SVIParticles particles = new SVIParticles(this);
		
		particles.setParticleCount(100);
		particles.setMaxDuration(particleDuration);
		
		particles.setDefaultPosition( slideSize.mWidth / 20.0f, slideSize.mHeight / 2.0f - 20.0f, 0.0f );
		particles.setRandomPosition( slideSize.mWidth / 20.0f, slideSize.mHeight / 2.0f + 20.0f, 0.0f );
		
		particles.setDefaultGravity(0.0f, 0.1f, 0.0f);
		particles.setRandomGravity(0.0f, 0.1f, 0.0f);
		
		particles.setDefaultForce(-0.3f, 0.0f, 0.0f);
		particles.setRandomForce(1.0f, 1.0f, 0.0f);
		
		particles.setMaxMass(3.0f);
		particles.setRandomMass(2.0f);
		
		float particleSize = slideSize.mWidth / 40.0f;
		particles.setMaxSize(particleSize, particleSize, particleSize);
		particles.setRandomSize(particleSize / 10.0f, particleSize / 10.0f, particleSize / 10.0f);
		
		particles.setMaxColor(0.7f, 0.8f, 0.9f, 1.0f);
		particles.setRandomColor(0.0f, 0.0f, 0.1f, 0.0f);
		
		particles.setTextureFile("particle", mParticleImage);
		
		/*float moveX = slideSize.mWidth / ( (float)aniDuration / particleDuration );
		particles.setPositionKeyFrameAnimationProperty(aniDuration, SVIAnimation.InterpolatorType.LINEAR, true);
		particles.addPositionKeyFrame(0.0f, new SVIVector3(0.0f, 0.0f, 0.0f));
		particles.addPositionKeyFrame(1.0f, new SVIVector3(moveX, 0.0f, 0.0f));*/
		
		particles.setScaleKeyFrameAnimationProperty(particleDuration, SVIAnimation.InterpolatorType.LINEAR, true);
		particles.addScaleKeyFrame(0.0f, new SVIVector3(1.0f, 1.0f, 1.0f));
		particles.addScaleKeyFrame(0.5f, new SVIVector3(2.0f, 2.0f, 2.0f));
		particles.addScaleKeyFrame(0.7f, new SVIVector3(1.0f, 1.0f, 1.0f));
		particles.addScaleKeyFrame(1.0f, new SVIVector3(0.3f, 0.3f, 0.3f));
				
		/*particles.setColorKeyFrameAnimationProperty(particleDuration, SVIAnimation.InterpolatorType.LINEAR, true);
		particles.addColorKeyFrame(0.0f, new SVIColor(1.0f, 1.0f, 1.0f, 1.0f));
		particles.addColorKeyFrame(1.0f, new SVIColor(1.0f, 1.0f, 1.0f, 0.0f));
		*/
		effect.addParticles(0.0f, particles);
		effect.addParticles(0.025f, particles);
		effect.addParticles(0.05f, particles);
		effect.addParticles(0.075f, particles);
		effect.addParticles(0.1f, particles);
		effect.addParticles(0.125f, particles);
		effect.addParticles(0.15f, particles);
		effect.addParticles(0.175f, particles);
		effect.addParticles(0.2f, particles);
		effect.addParticles(0.225f, particles);
		effect.addParticles(0.25f, particles);
		effect.addParticles(0.275f, particles);
		effect.addParticles(0.3f, particles);
		effect.addParticles(0.325f, particles);
		effect.addParticles(0.35f, particles);
		effect.addParticles(0.375f, particles);
		effect.addParticles(0.40f, particles);
		effect.addParticles(0.425f, particles);
		effect.addParticles(0.45f, particles);
		effect.addParticles(0.475f, particles);
		effect.addParticles(0.5f, particles);
		effect.addParticles(0.525f, particles);
		effect.addParticles(0.55f, particles);
		effect.addParticles(0.575f, particles);
		
		particles.setDefaultPosition( 0.0f, slideSize.mHeight / 2.0f - 20.0f, 0.0f );
		
		effect.addParticles(0.6f, particles);
		effect.addParticles(0.63f, particles);
		particles.setMaxDuration(700);
		effect.addParticles(0.66f, particles);
		
		SVIParticleAnimation particleAni = new SVIParticleAnimation();
		//particleAni.setListener(this);
		particleAni.setDuration(aniSetDuration);
		particleAni.setTag("ParticleEffect");
		particleAni.setParticleEffect(effect);
		
		slideAniSet.addAnimation(swipeAni);
		slideAniSet.addAnimation(particleAni);
		
		slide.startAnimation(slideAniSet);
	}
	
	@Override
	protected void buildSubSlide() {
		super.buildSubSlide();
		
		float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		
		SVISlide mainSlide = getMainSlide();
		SVISize mainSize = mainSlide.getRegion().mSize;			
		
		mainSlide.setBackgroundColor(color);
		mainSlide.setHoldOpacity(true);
		mainSlide.setOrthogonal(true);	
			
		
		mBackSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
				mainSize.mWidth * 0.1f, mainSize.mHeight * 0.1f,  
				mainSize.mWidth * 0.8f, mainSize.mHeight * 0.8f, color);
		mBackSlide.setImage(mBackSlideImage);
		
		mLinearSlide = new SVISlide(0, mainSlide.getNativeSlideHandle(), 
					mainSize.mWidth * 0.1f, mainSize.mHeight * 0.1f,  
					mainSize.mWidth * 0.8f, mainSize.mHeight * 0.8f, color);
			
		
		mLinearSlide.setImage(mSlideImage);
		mLinearSlide.setSwipeType(SlideSwipeType.SWIPE_LINEAR);
		
		
		
		
		//mLinearSlide.setOpacity(0.8f);
		//mRadialSlide.setOpacity(0.8f);
			
	}
	
	public void onStop(){
		clearRemainTextures();
	}
		
	private Random mRandom = new Random();
	private SVISlide mLinearSlide = null;
	private SVISlide mBackSlide = null;
	
	private SVIImage mSlideImage = null;
	private Bitmap	mSlideBitmap = null;
	
	private SVIImage mBackSlideImage = null;
	private Bitmap	mBackSlideBitmap = null;
	
	private SVIImage mParticleImage = null;
	private Bitmap	mParticleBitmap = null;
	
}
