/**
 * SGIEffectDialer.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.speaker;

import java.util.ArrayList;
import java.util.Random;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Point;
import android.util.Log;
import android.view.MotionEvent;

import com.github.svieffect.main.SVIEffectNode;
import com.github.svieffect.main.SVIEffectNodeCallback;
import com.github.svieffect.main.SVIEffectView;
import com.github.svieffect.speaker.MathWave.Wave;
import com.github.sviengine.animation.SVIAnimation;
import com.github.sviengine.animation.SVIAnimationSet;
import com.github.sviengine.animation.SVIKeyFrameAnimation;
import com.github.sviengine.animation.SVIPropertyAnimation;
import com.github.sviengine.basetype.SVIColor;
import com.github.sviengine.basetype.SVIImage;
import com.github.sviengine.basetype.SVIRect;
import com.github.sviengine.basetype.SVIVector3;
import com.github.sviengine.basetype.SVIVector4;
import com.github.sviengine.slide.SVISlide;

public class SVIEffectSpeaker extends SVIEffectView implements MathWave.MathWaveListener {
	
	private boolean mIsAddBlend = false;
	
	//2013/01/31 rareboy0112@facebook.com
	//add skin mode
	/**
     * Skin type
     */
    public interface SkinType {
        public static final int 	SKIN_CLASSIC = 0;
        public static final int 	SKIN_FUNKY = 1;
        public static final int		SKIN_WAVE = 2;
    }
	
    public interface MaskType {
    	public static final int		MASK_A = 0;
    	public static final int		MASK_B = 1;
    	public static final int		MASK_C = 2;
    	public static final int		MASK_D = 3;
    	public static final int		MASK_E = 4;
    	public static final int		MASK_COUNT = 5;
    }
    
    public interface MusicType {
    	public static final int		MUSIC_BASE = 0;
		public static final int		MUSIC_LOW = 1;
		public static final int		MUSIC_MID = 2;
		public static final int 	MUSIC_HIGH = 3;
		public static final int 	MUSIC_COUNT = 4;
    }
    
    public class MaskInfo {
    	public float mBottomWidth;
    	public float mBottomHeight;
    	
    	public float mFxWidth;
    	public float mFxHeight;
    	
    	public float mTopWidth;
    	public float mTopHeight;
    	
    	public float mFirstScale;
    	public Point mFirstCenter;
    	
    	public float mSecondScale;
    	public Point mSecondCenter;
    	
    	public float mThirdScale;
    	public Point mThirdCenter;
    	
    	public float mFourthScale;
    	public Point mFourthCenter;
    	
    	public Bitmap mBottomBase;
    	public Bitmap mFX_First;
    	public Bitmap mFX_Second;
    	public Bitmap mFX_Third;
    	public Bitmap mFX_Fourth;
    	public Bitmap mFX_Top;
    	public Bitmap mTopBase;
    	
    	public float mOtherFirstRotate;
    	public float mOtherSecondRotate;
    	public float mOtherThirdRotate;
    	public float mOtherFourthRotate;
    	
    	public MaskInfo() {}
    };
    
    private MaskInfo mInfoOfMaskA = null;
    private MaskInfo mInfoOfMaskB = null;
    private MaskInfo mInfoOfMaskC = null;
    private MaskInfo mInfoOfMaskD = null;
    private MaskInfo mInfoOfMaskE = null;
    
    private int mCurrentSkin = SkinType.SKIN_CLASSIC;
    
    private int mMaskType = MaskType.MASK_A;
    
    private boolean mChangedMaskType = false;
    
    private static final float 		SENSOR_SCALE = 0.7f;
	private static final float 		SENSOR_SCALE_CENTER = 1.1f;
	private static final float 		SENSOR_SCALE_RUBBER = 0.5f;
	
	private final float 			OTHERS_BOTTOM_FX_SCALE = 2.8f;
	
	private interface DefaultSkinIndex {
		public static final int 	IDX_BASE = 0;
		public static final int 	IDX_RUBBERIN = 2;
		public static final int 	IDX_RUBBEROUT = 1;
		public static final int 	IDX_CENTERCAP = 3;
		public static final int 	IDX_COVER = 4;
	}
	
	private interface DJSkinIndex {
		@SuppressWarnings("unused")
		public static final int		IDX_ALL_BASE = 0;
		
		public static final int		IDX_BASE_FX = 1;
		public static final int		IDX_LOW_FX = 2;
		
		public static final int		IDX_OTHER_FIRST = 3;
		public static final int		IDX_OTHER_SECOND = 4;
		public static final int		IDX_OTHER_THIRD = 5;
		public static final int		IDX_OTHER_FOURTH = 6;
		
		public static final int		IDX_MID_FX = 7;
		public static final int		IDX_HIGH_FX = 8;
		public static final int		IDX_TOP_BASE = 9;
		public static final int		IDX_TOP_FX = 10;
		
		public static final int		IDX_MASK = 11;
		
		public static final int		IDX_FLASH = 12;
	}
	
	private final int FLASH_ANIMATION_DURATION = 350;
	
	private float mMusicBase = 0.0f;
	private float mMusicLow = 0.0f;
	private float mMusicMid = 0.0f;
	private float mMusicHigh = 0.0f;
	
	private static final int		RECOVER_ANIMATION_DURATION = 25;
	
	private int mAnimationDuration = RECOVER_ANIMATION_DURATION;
	
	private float mRotateZ = 0.0f;
	
	private float mOtherFirstRotateZ = 0.0f;
	private float mOtherSecondRotateZ = 0.0f;
	private float mOtherThirdRotateZ = 0.0f;
	private float mOtherFourthRotateZ = 0.0f;
	
	private float mEffectScale = 0.9f;
	private float mAnimationSpeed = 1.5f;
	
	private float mLastFrequencyDepth = 0.0f;
	private float mNextFrequencyDepth = 1.0f;

	public boolean mUseDepthAnimation = true;
	
	private SVIImage[] mImages;
	
	
	private class WavePoint{
		public float mHeight;
		public float mX;
		public float mY;
		
		public WavePoint(float value, float x, float y){
			mHeight = value;
			mX = x;
			mY = y;
		}
	}
	
	//2013-08-07 rareboy0112@facebook.com
	//default max slide count = 250
	public int maxSlidesCount = 250;
	
	//2013-07-22 rareboy0112@facebook.com
	//use Wave mode
	//why make class? Only to improve readability.
	private class WaveField {
		public int slideXCount;
		public int slideYCount;
		
		public ArrayList<WavePoint> scaleGrid;
		public ArrayList<SVISlide> slideGrid;
		
		public int maxSlidesCount;
		
		public WaveField() {
			//Define Base Particle size = 14x28
			slideXCount = WAVE_COUNT_X;
			slideYCount = WAVE_COUNT_Y;
			slideGrid = new ArrayList<SVISlide>();
			scaleGrid = new ArrayList<WavePoint>();
		}
		
		public void release() {
			// offset for Base particles
			if (mPannelNodes != null && !mPannelNodes.isEmpty() && mPannelNodes.get(0) != null) {
				SVISlide baseSlide = mPannelNodes.get(0).getSlide();
				if (baseSlide != null) {
					for (SVISlide slide : slideGrid) {
						baseSlide.removeSlide(slide);
					}
				}
			}
			
			slideGrid.clear();
			slideGrid = null;
			
			scaleGrid.clear();
			scaleGrid = null;
		}
	}
	
	
	WaveField mWaveField;
	//for calculate wave
	MathWave mMathWave;
	
	//range 0.0 ~ 1.0f
	float mWaveColors[];
	
	private int WAVE_COUNT_X = 14;
	private int WAVE_COUNT_Y = 28;
	
	//2013-08-09 jdev.nam
	private boolean particleFitForSquare = true;
	private Random mRandom = new Random();
	
	public SVIEffectSpeaker(Context context) {
		super(context);
		//enableShowFPS(true);
		//enablePostEffect(true);
		
		//SVIPostEffectManager.enable(false);
		//SVIPostEffectManager.setType(SVIPostEffectManager.POST_EFFECT_BLOOM);
		
		setSVIPreserveEGLContext(true);
		setZOrderOnTop(false);
		this.setAnimationCallback(mAnimationCallback);
	}
		
		
	public void initialize(){
		setRenderFPS(18);
		super.initialize();
	}
	
	public void onPause() {
		super.onPause();
		//2013-02-14 rareboy0112@facebook.com
		//should be when pause
		if(mCurrentSkin == SkinType.SKIN_FUNKY) {
			if (mPannelNodes.size() < DJSkinIndex.IDX_FLASH) {
				return;
			}
			
			SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_FLASH).getSlide();
			slide.setOpacity(0.0f);
			mChangedMaskType = false;
		}
		//2013-08-07 rareboy0112@facebook.com
		//stop wave update handler
		else if(mCurrentSkin == SkinType.SKIN_WAVE) {
			if (mMathWave != null) {
				mMathWave.stop();
			}
		}
	}
	
	//2013-08-07 rareboy0112@facebook.com
	//start wave update handler
	public void onResume() {
		super.onResume();
		if(mCurrentSkin == SkinType.SKIN_WAVE) {
			if (mMathWave != null) {
				mMathWave.start();
			}
		}
	}
	
	private void releaseFunkySkin() {
		mInfoOfMaskA = null;
	    mInfoOfMaskB = null;
	    mInfoOfMaskC = null;
	    mInfoOfMaskD = null;
	    mInfoOfMaskE = null;
	}
	
	//2013-08-02 minw83.kim
	// create wave particle images
	public void setWaveTextures(Bitmap bitmaps[]) {
		mImages = new SVIImage[bitmaps.length];
		for (int i = 0; i < bitmaps.length; i++) {
			mImages[i] = new SVIImage();
			mImages[i].setBitmap(bitmaps[i], SVIImage.AlphaType.NORMAL);
		}
	}
	
	public void setWaveMaxSlide(int slideCount) {
		maxSlidesCount = slideCount;
	}
	//2013-08-09 jdev.nam
	public void enableParticleFitForRectangle(){
		particleFitForSquare = false;
	}
		
	//2013-07-22 rareboy0112@facebook.com
	//create wave particle slide
	private void buildWaveSkin() {
		if (mWaveField != null) {
			
			mWaveField.release();
			mWaveField = null;
		}
		
		mWaveField = new WaveField();
		mWaveField.maxSlidesCount = maxSlidesCount;
		
		// offset for Base particles
		SVIEffectNode node = mPannelNodes.get(0);
		SVISlide baseSlide = node.getSlide();
		
		float width = node.getRegion().mSize.mWidth;
		float height = node.getRegion().mSize.mHeight;
		
		float xstep = (float)width / (float)mWaveField.slideXCount;
		float ystep = (float)height / (float)mWaveField.slideYCount;
		
		// 2013-08-08 jdev.nam
		float halfYCount = (float)mWaveField.slideYCount * 0.5f;
		float yOffset = height*0.5f-(ystep*0.5f);
		float addingYOffset = 0.0f;
		

		//2013-08-09 jdev.nam
		float particleWidth = xstep;
		float particleHeigh = ystep;
		
		//2013-08-09 jdev.nam
		if(mWaveOrientLandscape == true)
		{
			if(particleFitForSquare == true){
				if(xstep > ystep)
					addingYOffset  = xstep - ystep;
				else
					addingYOffset  = ystep - xstep;
				particleHeigh = xstep;
			}
		}
		else if(mWaveOrientPortraits == true)
		{
			if(particleFitForSquare == true){
				if(xstep > ystep)
					addingYOffset  = xstep - ystep;
				else
					addingYOffset  = ystep - xstep;
				particleWidth = ystep;
			}
		}
		
					
		// Create Base Particle 14x28
		// Set init scale = 0;
		float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		baseSlide.setBackgroundColor(color);
		// 2013-08-08 jdev.nam
		for (int n = 0; n < mWaveField.maxSlidesCount; n++) {
			//SVISlide tempslide = new SVISlide(0, baseSlide.getNativeSlideHandle(), (float)0 * xstep, (float)0 * ystep, xstep, ystep, color);
			SVISlide tempslide = new SVISlide(0, baseSlide.getNativeSlideHandle(), (float)0 * xstep, (float)0 * ystep, particleWidth, particleHeigh, color);
			
			tempslide.setScale(0.0f, 0.0f, 1.0f);
			SVIImage image = mImages[mRandom.nextInt(mImages.length)];
			tempslide.setImage(image);
			tempslide.setBlendType(SVISlide.BlendType.BLEND_MULTIPLY);
			mWaveField.slideGrid.add(tempslide);
		}
				
		// 2013-08-08 jdev.nam
		for (int y = 0; y < mWaveField.slideYCount; y++) {
			for (int x = 0; x < mWaveField.slideXCount; x++) {
				//mWaveField.scaleGrid.add(new WavePoint(0.0f, (float)x * xstep + xstep * 0.5f, (ystep * halfYCount - ystep *(float)y)+yOffset));
				mWaveField.scaleGrid.add(new WavePoint(0.0f, (float)x * xstep + xstep * 0.5f, (ystep * halfYCount - ystep *(float)y)+yOffset+addingYOffset));
				//Log.d("TAG","jdev ["+(float)x * xstep + xstep * 0.5f+"]     ["+(ystep * halfYCount - ystep *(float)y)+yOffset);
			}
		}
	}
	// 				//mWaveField.scaleGrid.add(new WavePoint(0.0f, (float)x * xstep + xstep * 0.5f, (float)y * ystep + ystep * 0.5f));
	//2013-07-22 rareboy0112@facebook.com
	//Override finalize method. Because must WaveUpdater stop.
	@Override
	public void finalize() {
		if (mMathWave != null) {
			mMathWave.stop();
			mMathWave = null;
		}
		if (mWaveField != null) {
			mWaveField.release();
			mWaveField = null;
		}
		
		super.finalize();
	}

	public void enableAddBlend(boolean aValue) {
		mIsAddBlend = aValue;
	}
	
	@Override
	public void onDataChanged(ArrayList<Wave> waves) {
		
		if (mWaveField == null || waves == null) {
			return;
		}
		
		if (waves.isEmpty()) {
			return;
		}
		
		for (int n = 0; n < mWaveField.maxSlidesCount; ++n) {
			SVISlide slide = mWaveField.slideGrid.get(n);
			if (!slide.getHidden()) {
				slide.setHidden(true);
			}
		}
		
		int count = mWaveField.slideXCount * mWaveField.slideYCount;
		int slideIdx = 0;
		boolean isHigh = false;
		
		for (int n = 0; n < count; ++n) {
			float scale = 0.0f;
			for (int k = 0; k < waves.size(); ++k) {
				if (mWaveField != null && mWaveField.scaleGrid != null && mWaveField.scaleGrid.get(n) != null) {
					WavePoint point = mWaveField.scaleGrid.get(n);
					scale = waves.get(k).getScale(point.mX, point.mY);
					isHigh = waves.get(k).isHigh();
					// 1 piece of the Particles is setting any value
					if (scale > 0.0f) break;
				}
			}
			
			WavePoint point = mWaveField.scaleGrid.get(n);
			if (scale <= 0) {										
				point.mHeight = 0.0f;
				continue;
			}
			
			if (Math.abs(scale - point.mHeight) < 0.001f)
				continue;

			slideIdx = n % mWaveField.maxSlidesCount;
			SVISlide slide = mWaveField.slideGrid.get(slideIdx);
			slide.setHidden(false);
			
			point.mHeight = scale;
			slide.setPosition(point.mX, point.mY);
			slide.setScale(scale, scale, 1.0f);
			
			float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			int index = 0;
			if (isHigh) {
				// 2013-08-12 teja.kim
				color[0] = 1.0f - mWaveColors[0];
				color[1] = 1.0f - mWaveColors[1];
				color[2] = 1.0f - mWaveColors[2];
				
//				color[0] = 1.0f - mWaveColors[3];
	//			color[1] = 1.0f - mWaveColors[4];
		//		color[2] = 1.0f - mWaveColors[5];
				slide.setBackgroundColor(color);
			}
			else {
				int colorCount = mWaveColors.length / 3 - 1;
				index = Math.abs(mRandom.nextInt()) % colorCount + 1;
				for (int i = 0; i < 3; ++i) {
					color[i] = mWaveColors[index * 3 + i];
				}
				slide.setBackgroundColor(color);
			}
		}
	}
	
	
	//2013-07-22 rareboy0112@facebook.com
	//2013-08-07 rareboy0112@facebook.com
	//changed method name
	//create low wave
	public void addLowWave(float effectiveRadius, float radius, float x, float y, float speed) {
		if (mCurrentSkin == SkinType.SKIN_WAVE && mMathWave != null) {
			mMathWave.addWave(effectiveRadius, radius, x, y, speed, false);
		}
	}
	
	//2013-08-07 rareboy0112@facebook.com
	//create high wave
	public void addHighWave(float effectiveRadius, float radius, float x, float y, float speed) {
		if (mCurrentSkin == SkinType.SKIN_WAVE && mMathWave != null) {
			mMathWave.addWave(effectiveRadius, radius, x, y, speed, true);
		}
	}
	
	
	//2013-07-22 rareboy0112@facebook.com
	//wave stop
	public void setWaveHidden() {
		// offset for Base particles
		SVISlide slide = mPannelNodes.get(0).getSlide();
		slide.setOpacity(0.0f);
	}
	
	public void setWaveShow() {
		// offset for Base particles
		SVISlide slide = mPannelNodes.get(0).getSlide();
		slide.setOpacity(1.0f);
	}
	
	//2013-07-22 rareboy0112@facebook.com
	//set wave colors
	public void setWaveColors(int colors[]) {
		mWaveColors = null;
		mWaveColors = new float[colors.length];
		for (int i = 0; i < colors.length; ++i) {
			mWaveColors[i] = (float)(colors[i]) / 255.0f;
		}
		
		if (mCurrentSkin == SkinType.SKIN_WAVE) {
			// offset for Base particles
			SVIEffectNode node = mPannelNodes.get(0);
			SVISlide baseSlide = node.getSlide();
			
			int index = 0;
			float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
			// 2013-08-12 teja.kim
//			color[0] = mWaveColors[0];
//			color[1] = mWaveColors[1];
//			color[2] = mWaveColors[2];
//			
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
			
			
			//set screen-background color
			baseSlide.setBackgroundColor(color);
			
			for (int n = 0; n < mWaveField.slideGrid.size(); ++n) {
				int colorCount = mWaveColors.length / 3 - 1;
				index = Math.abs(mRandom.nextInt()) % colorCount + 1;
				for (int i = 0; i < 3; ++i) {
					
					color[i] = mWaveColors[index * 3 + i];
				}				
				mWaveField.slideGrid.get(n).setBackgroundColor(color);
			}
		}
	}
	
	//jdev.nam 2013-09-06 
	//Wave effect needs orientation setting.
	boolean mWaveOrientLandscape = false;
	public void setWaveLandscape()
	{
		WAVE_COUNT_X = 28;
		WAVE_COUNT_Y = 14;
		
		mWaveOrientLandscape = true;
		mWaveOrientPortraits = false;
	}
	
	//jdev.nam 2013-09-06
	//Wave effect needs orientation setting.
	boolean mWaveOrientPortraits = false;
	public void setWavePortraits()
	{
		WAVE_COUNT_X = 14;
		WAVE_COUNT_Y = 28;
		
		mWaveOrientPortraits = true;
		mWaveOrientLandscape = false;
	}
	public void setSkinMode(int aSkinMode) {
		mChangedMaskType = false;
		
		if(mCurrentSkin != aSkinMode) {
			
			if (mMathWave != null) {
				mMathWave.stop();
			}
			
			if(mCurrentSkin == SkinType.SKIN_FUNKY) {
				//2013-02-19 rareboy0112@facebook.com
				//should be when pause
				if (mPannelNodes.size() >= DJSkinIndex.IDX_FLASH) {
					SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_FLASH).getSlide();
					slide.initOpacity(0.0f);
				}
				
				releaseFunkySkin();
			}
			
			mAnimationSpeed = 1.0f;
			
			//2013-03-08 masterkeaton27@gmail.com
			if (aSkinMode == SkinType.SKIN_CLASSIC){
				mAnimationSpeed = 1.25f;
			}
			
			mCurrentSkin = aSkinMode;
			
			//2013-07-22 rareboy0112@facebook.com
			//create wave particle
			if (mCurrentSkin == SkinType.SKIN_WAVE) {
				buildWaveSkin();
				
				if (mMathWave == null) {
					mMathWave = new MathWave();
				}
				
				SVIEffectNode node = mPannelNodes.get(0);
				mMathWave.setMaxRangeOfRadius(node.getRegion().mSize.mWidth + node.getRegion().mSize.mHeight);
				mMathWave.setListener(this);
				mMathWave.setMaxFps(18);
				mMathWave.start();
			}
		}
	}
	
	public void setMaskType(int aMaskType, Bitmap aMaskBitmap) {
		mMaskType = aMaskType;
		
		if(mChangedMaskType == true) {
			if (mPannelNodes.size() < DJSkinIndex.IDX_FLASH) {
				return;
			}
			
			SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_FLASH).getSlide();
			
			SVIKeyFrameAnimation keyOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
			keyOpacityAni.setDuration((int)(FLASH_ANIMATION_DURATION * mAnimationSpeed));
			keyOpacityAni.setRepeatCount(0);
			keyOpacityAni.setAutoReverse(false);
			keyOpacityAni.setOffset(0);
			keyOpacityAni.addKeyProperty(0.0f, 1.0f);
			keyOpacityAni.addKeyProperty(1.0f, 0.0f);
			
			slide.startAnimation(keyOpacityAni);
		}
		mChangedMaskType = true;
		
		setOthersCircle(mMaskType, aMaskBitmap);
	}
	
	public void setMusiBase(float aValue) {
		mMusicBase = aValue;
	}
	
	public void setMusiLow(float aValue) {
		mMusicLow = aValue;
	}
	
	public void setMusiMid(float aValue) {
		mMusicMid = aValue;
	}
	
	public void setMusiHigh(float aValue) {
		mMusicHigh = aValue;
	}
	
	public void setMaskInfomation(int aMaskType, MaskInfo info) {
		
		switch (aMaskType) {
		case MaskType.MASK_A:
			if(mInfoOfMaskA == null)
				mInfoOfMaskA = new MaskInfo();
			
			mInfoOfMaskA.mBottomWidth = info.mBottomWidth; 	mInfoOfMaskA.mBottomHeight = info.mBottomHeight;
			mInfoOfMaskA.mFxWidth = info.mFxWidth; 			mInfoOfMaskA.mFxHeight = info.mFxHeight;
			mInfoOfMaskA.mTopWidth = info.mTopWidth; 		mInfoOfMaskA.mTopHeight = info.mTopHeight;
			mInfoOfMaskA.mBottomBase = info.mBottomBase; 	mInfoOfMaskA.mTopBase = info.mTopBase; 
			mInfoOfMaskA.mFX_First = info.mFX_First; 		mInfoOfMaskA.mFX_Second = info.mFX_Second;
			mInfoOfMaskA.mFX_Third = info.mFX_Third; 		mInfoOfMaskA.mFX_Fourth = info.mFX_Fourth;
			mInfoOfMaskA.mFX_Top = info.mFX_Top;
			mInfoOfMaskA.mFirstScale = info.mFirstScale; 	mInfoOfMaskA.mFirstCenter = info.mFirstCenter;
			mInfoOfMaskA.mSecondScale = info.mSecondScale; 	mInfoOfMaskA.mSecondCenter = info.mSecondCenter;
			mInfoOfMaskA.mThirdScale = info.mThirdScale; 	mInfoOfMaskA.mThirdCenter = info.mThirdCenter;
			mInfoOfMaskA.mFourthScale = info.mFourthScale; 	mInfoOfMaskA.mFourthCenter = info.mFourthCenter;
			
			mInfoOfMaskA.mOtherFirstRotate = info.mOtherFirstRotate;
			mInfoOfMaskA.mOtherSecondRotate = info.mOtherSecondRotate;
			mInfoOfMaskA.mOtherThirdRotate = info.mOtherThirdRotate;
			mInfoOfMaskA.mOtherFourthRotate = info.mOtherFourthRotate;
			
			break;
			
		case MaskType.MASK_B:
			if(mInfoOfMaskB == null)
				mInfoOfMaskB = new MaskInfo();
			
			mInfoOfMaskB.mBottomWidth = info.mBottomWidth; 	mInfoOfMaskB.mBottomHeight = info.mBottomHeight;
			mInfoOfMaskB.mFxWidth = info.mFxWidth; 			mInfoOfMaskB.mFxHeight = info.mFxHeight;
			mInfoOfMaskB.mTopWidth = info.mTopWidth; 		mInfoOfMaskB.mTopHeight = info.mTopHeight;
			mInfoOfMaskB.mBottomBase = info.mBottomBase; 	mInfoOfMaskB.mTopBase = info.mTopBase; 
			mInfoOfMaskB.mFX_First = info.mFX_First; 		mInfoOfMaskB.mFX_Second = info.mFX_Second;
			mInfoOfMaskB.mFX_Third = info.mFX_Third; 		mInfoOfMaskB.mFX_Fourth = info.mFX_Fourth;
			mInfoOfMaskB.mFX_Top = info.mFX_Top;
			mInfoOfMaskB.mFirstScale = info.mFirstScale; 	mInfoOfMaskB.mFirstCenter = info.mFirstCenter;
			mInfoOfMaskB.mSecondScale = info.mSecondScale; 	mInfoOfMaskB.mSecondCenter = info.mSecondCenter;
			mInfoOfMaskB.mThirdScale = info.mThirdScale; 	mInfoOfMaskB.mThirdCenter = info.mThirdCenter;
			mInfoOfMaskB.mFourthScale = info.mFourthScale; 	mInfoOfMaskB.mFourthCenter = info.mFourthCenter;
			
			mInfoOfMaskB.mOtherFirstRotate = info.mOtherFirstRotate;
			mInfoOfMaskB.mOtherSecondRotate = info.mOtherSecondRotate;
			mInfoOfMaskB.mOtherThirdRotate = info.mOtherThirdRotate;
			mInfoOfMaskB.mOtherFourthRotate = info.mOtherFourthRotate;
			
			break;
			
		case MaskType.MASK_C:
			if(mInfoOfMaskC == null)
				mInfoOfMaskC = new MaskInfo();
			
			mInfoOfMaskC.mBottomWidth = info.mBottomWidth; 	mInfoOfMaskC.mBottomHeight = info.mBottomHeight;
			mInfoOfMaskC.mFxWidth = info.mFxWidth; 			mInfoOfMaskC.mFxHeight = info.mFxHeight;
			mInfoOfMaskC.mTopWidth = info.mTopWidth; 		mInfoOfMaskC.mTopHeight = info.mTopHeight;
			mInfoOfMaskC.mBottomBase = info.mBottomBase; 	mInfoOfMaskC.mTopBase = info.mTopBase; 
			mInfoOfMaskC.mFX_First = info.mFX_First; 		mInfoOfMaskC.mFX_Second = info.mFX_Second;
			mInfoOfMaskC.mFX_Third = info.mFX_Third; 		mInfoOfMaskC.mFX_Fourth = info.mFX_Fourth;
			mInfoOfMaskC.mFX_Top = info.mFX_Top;
			mInfoOfMaskC.mFirstScale = info.mFirstScale; 	mInfoOfMaskC.mFirstCenter = info.mFirstCenter;
			mInfoOfMaskC.mSecondScale = info.mSecondScale; 	mInfoOfMaskC.mSecondCenter = info.mSecondCenter;
			mInfoOfMaskC.mThirdScale = info.mThirdScale; 	mInfoOfMaskC.mThirdCenter = info.mThirdCenter;
			mInfoOfMaskC.mFourthScale = info.mFourthScale; 	mInfoOfMaskC.mFourthCenter = info.mFourthCenter;
			
			mInfoOfMaskC.mOtherFirstRotate = info.mOtherFirstRotate;
			mInfoOfMaskC.mOtherSecondRotate = info.mOtherSecondRotate;
			mInfoOfMaskC.mOtherThirdRotate = info.mOtherThirdRotate;
			mInfoOfMaskC.mOtherFourthRotate = info.mOtherFourthRotate;
			
			break;
			
		case MaskType.MASK_D:
			if(mInfoOfMaskD == null)
				mInfoOfMaskD = new MaskInfo();
			
			mInfoOfMaskD.mBottomWidth = info.mBottomWidth; 	mInfoOfMaskD.mBottomHeight = info.mBottomHeight;
			mInfoOfMaskD.mFxWidth = info.mFxWidth; 			mInfoOfMaskD.mFxHeight = info.mFxHeight;
			mInfoOfMaskD.mTopWidth = info.mTopWidth; 		mInfoOfMaskD.mTopHeight = info.mTopHeight;
			mInfoOfMaskD.mBottomBase = info.mBottomBase; 	mInfoOfMaskD.mTopBase = info.mTopBase; 
			mInfoOfMaskD.mFX_First = info.mFX_First; 		mInfoOfMaskD.mFX_Second = info.mFX_Second;
			mInfoOfMaskD.mFX_Third = info.mFX_Third; 		mInfoOfMaskD.mFX_Fourth = info.mFX_Fourth;
			mInfoOfMaskD.mFX_Top = info.mFX_Top;
			mInfoOfMaskD.mFirstScale = info.mFirstScale; 	mInfoOfMaskD.mFirstCenter = info.mFirstCenter;
			mInfoOfMaskD.mSecondScale = info.mSecondScale; 	mInfoOfMaskD.mSecondCenter = info.mSecondCenter;
			mInfoOfMaskD.mThirdScale = info.mThirdScale; 	mInfoOfMaskD.mThirdCenter = info.mThirdCenter;
			mInfoOfMaskD.mFourthScale = info.mFourthScale; 	mInfoOfMaskD.mFourthCenter = info.mFourthCenter;
			
			mInfoOfMaskD.mOtherFirstRotate = info.mOtherFirstRotate;
			mInfoOfMaskD.mOtherSecondRotate = info.mOtherSecondRotate;
			mInfoOfMaskD.mOtherThirdRotate = info.mOtherThirdRotate;
			mInfoOfMaskD.mOtherFourthRotate = info.mOtherFourthRotate;
			
			break;
			
		case MaskType.MASK_E:
			if(mInfoOfMaskE == null)
				mInfoOfMaskE = new MaskInfo();
			
			mInfoOfMaskE.mBottomWidth = info.mBottomWidth; 	mInfoOfMaskE.mBottomHeight = info.mBottomHeight;
			mInfoOfMaskE.mFxWidth = info.mFxWidth; 			mInfoOfMaskE.mFxHeight = info.mFxHeight;
			mInfoOfMaskE.mTopWidth = info.mTopWidth; 		mInfoOfMaskE.mTopHeight = info.mTopHeight;
			mInfoOfMaskE.mBottomBase = info.mBottomBase; 	mInfoOfMaskE.mTopBase = info.mTopBase; 
			mInfoOfMaskE.mFX_First = info.mFX_First; 		mInfoOfMaskE.mFX_Second = info.mFX_Second;
			mInfoOfMaskE.mFX_Third = info.mFX_Third; 		mInfoOfMaskE.mFX_Fourth = info.mFX_Fourth;
			mInfoOfMaskE.mFX_Top = info.mFX_Top;
			mInfoOfMaskE.mFirstScale = info.mFirstScale; 	mInfoOfMaskE.mFirstCenter = info.mFirstCenter;
			mInfoOfMaskE.mSecondScale = info.mSecondScale; 	mInfoOfMaskE.mSecondCenter = info.mSecondCenter;
			mInfoOfMaskE.mThirdScale = info.mThirdScale; 	mInfoOfMaskE.mThirdCenter = info.mThirdCenter;
			mInfoOfMaskE.mFourthScale = info.mFourthScale; 	mInfoOfMaskE.mFourthCenter = info.mFourthCenter;
			
			mInfoOfMaskE.mOtherFirstRotate = info.mOtherFirstRotate;
			mInfoOfMaskE.mOtherSecondRotate = info.mOtherSecondRotate;
			mInfoOfMaskE.mOtherThirdRotate = info.mOtherThirdRotate;
			mInfoOfMaskE.mOtherFourthRotate = info.mOtherFourthRotate;
			
			break;

		default:
			break;
		}
	}
	
	public void createOthersCircleFX() {
		
		//set first circle
		{
			SVIEffectNode first_node = new SVIEffectNode();
			mPannelNodes.add(first_node);
			SVISlide root_slide = new SVISlide();
			mRootSlide.addSlide(root_slide);
			first_node.setSlide(root_slide);
			root_slide.setPivotPoint(0.5f, 0.5f);
			root_slide.setOpacity(1.0f);
			root_slide.setOrthogonal(false);
			root_slide.setClipSubSlides(true);
			
			//create bottom_fx_slide
			SVISlide bottom_fx_slide = new SVISlide();
			root_slide.addSlide(bottom_fx_slide);
			
			bottom_fx_slide.setPivotPoint(0.5f, 0.5f);
			bottom_fx_slide.setOpacity(1.0f);
			bottom_fx_slide.setOrthogonal(false);
				
			//create top_fx slide
			SVISlide top_fx_slide = new SVISlide();
			root_slide.addSlide(top_fx_slide);
			
			top_fx_slide.setPivotPoint(0.5f, 0.5f);
			top_fx_slide.setOpacity(1.0f);
			top_fx_slide.setOrthogonal(false);
			
			top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
		}
		
		//set second circle
		{
			SVIEffectNode second_node = new SVIEffectNode();
			mPannelNodes.add(second_node);
			SVISlide root_slide = new SVISlide();
			mRootSlide.addSlide(root_slide);
			second_node.setSlide(root_slide);
			root_slide.setPivotPoint(0.5f, 0.5f);
			root_slide.setOpacity(1.0f);	
			root_slide.setOrthogonal(false);
			root_slide.setClipSubSlides(true);
			
			//create bottom_fx_slide
			SVISlide bottom_fx_slide = new SVISlide();
			root_slide.addSlide(bottom_fx_slide);
			
			bottom_fx_slide.setPivotPoint(0.5f, 0.5f);
			bottom_fx_slide.setOpacity(1.0f);	
			bottom_fx_slide.setOrthogonal(false);
				
			//create top_fx slide
			SVISlide top_fx_slide = new SVISlide();
			root_slide.addSlide(top_fx_slide);
			
			top_fx_slide.setPivotPoint(0.5f, 0.5f);
			top_fx_slide.setOpacity(1.0f);	
			top_fx_slide.setOrthogonal(false);
			
			top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
		}
		
		//set third circle
		{
			SVIEffectNode third_node = new SVIEffectNode();
			mPannelNodes.add(third_node);
			SVISlide root_slide = new SVISlide();
			mRootSlide.addSlide(root_slide);
			third_node.setSlide(root_slide);
			root_slide.setPivotPoint(0.5f, 0.5f);
			root_slide.setOpacity(1.0f);	
			root_slide.setOrthogonal(false);
			root_slide.setClipSubSlides(true);
			
			//create bottom_fx_slide
			SVISlide bottom_fx_slide = new SVISlide();
			root_slide.addSlide(bottom_fx_slide);
			
			bottom_fx_slide.setPivotPoint(0.5f, 0.5f);
			bottom_fx_slide.setOpacity(1.0f);	
			bottom_fx_slide.setOrthogonal(false);
				
			//create top_fx slide
			SVISlide top_fx_slide = new SVISlide();
			root_slide.addSlide(top_fx_slide);
			
			top_fx_slide.setPivotPoint(0.5f, 0.5f);
			top_fx_slide.setOpacity(1.0f);	
			top_fx_slide.setOrthogonal(false);
			
			top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
		}
		
		//set fourth circle
		{
			SVIEffectNode fourth_node = new SVIEffectNode();
			mPannelNodes.add(fourth_node);
			SVISlide root_slide = new SVISlide();
			mRootSlide.addSlide(root_slide);
			fourth_node.setSlide(root_slide);
			root_slide.setPivotPoint(0.5f, 0.5f);
			root_slide.setOpacity(1.0f);	
			root_slide.setOrthogonal(false);
			root_slide.setClipSubSlides(true);
			
			//create bottom_fx_slide
			SVISlide bottom_fx_slide = new SVISlide();
			root_slide.addSlide(bottom_fx_slide);
			
			bottom_fx_slide.setPivotPoint(0.5f, 0.5f);
			bottom_fx_slide.setOpacity(1.0f);	
			bottom_fx_slide.setOrthogonal(false);
				
			//create top_fx slide
			SVISlide top_fx_slide = new SVISlide();
			root_slide.addSlide(top_fx_slide);
			
			top_fx_slide.setPivotPoint(0.5f, 0.5f);
			top_fx_slide.setOpacity(1.0f);	
			top_fx_slide.setOrthogonal(false);
			
			top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
		}
	}
	
	
	public void createOthersCircleBase() {
		/*for(int i = 0; i < 4; ++i) {
			//create top_base slide
			SVIEffectNode top_base_node = new SVIEffectNode();
			mPannelNodes.add(top_base_node);
				
			SVISlide topBaseSlide = new SVISlide();
			top_base_node.setSlide(topBaseSlide);
			
			topBaseSlide.setPivotPoint(0.5f, 0.5f);
			topBaseSlide.setOpacity(1.0f);	
			topBaseSlide.setOrthogonal(false);
			mRootSlide.addSlide(topBaseSlide);
		}*/
	}
	
	
	private MaskInfo getCurrentMaskInfo() {
		MaskInfo info = null;
		
		switch (mMaskType) {
		case MaskType.MASK_A:
			info = mInfoOfMaskA;
			break;
		case MaskType.MASK_B:
			info = mInfoOfMaskB;
			break;
		case MaskType.MASK_C:
			info = mInfoOfMaskC;
			break;
		case MaskType.MASK_D:
			info = mInfoOfMaskD;
			break;
		case MaskType.MASK_E:
			info = mInfoOfMaskE;
			break;
		default:
			break;
		}
		
		if(info == null) {
			return null;
		}
		
		return info;
	}
	
	
	private void setOthersCircle(int aMaskType, Bitmap aMaskBitmap) {
		MaskInfo info = null;
		
		info = getCurrentMaskInfo();
		
		if(info == null) {
			Log.e("SVIEffect","Mask Info null, please check mask info");
			return;
		}
		
		SVIRect bottomRegion = new SVIRect(0.0f, 0.0f, info.mBottomWidth + 10.0f, info.mBottomHeight + 10.0f);
		SVIRect fxRegion = new SVIRect(0.0f, 0.0f, info.mFxWidth, info.mFxHeight);
		//SVIRect topRegion = new SVIRect(0.0f, 0.0f, info.mTopWidth, info.mTopHeight);
		
		float bottom_width, bottom_height;
		float top_fx_width, top_fx_height;
		float bottom_fx_width, bottom_fx_height;
		
		float pos_x, pos_y;
		
		//set first circle
		{
			SVIRect rect;
			
			//bottom_base node
			SVIEffectNode bottom_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FIRST);
			bottom_base_node.setRegion(bottomRegion);
			rect = bottom_base_node.getRegion();
			SVISlide bottom_base_slide = bottom_base_node.getSlide();
			
			bottom_width = rect.mSize.mWidth * info.mFirstScale;
			bottom_height = rect.mSize.mHeight * info.mFirstScale;
			
			bottom_base_slide.setCornerRadius(bottom_width * 0.5f);
			
			bottom_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, bottom_width, bottom_height);
			bottom_base_slide.setPosition(info.mFirstCenter.x, info.mFirstCenter.y);
			bottom_base_slide.setImage(info.mBottomBase);
			
			//bottom_fx_slide
			SVISlide bottom_fx_slide = bottom_base_slide.getSubSlide(0);
			
			top_fx_width = fxRegion.mSize.mWidth * info.mFirstScale;
			top_fx_height = fxRegion.mSize.mHeight * info.mFirstScale;
			bottom_fx_slide.setRegion(fxRegion.mOrigin.mY, fxRegion.mOrigin.mY, top_fx_width, top_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			bottom_fx_slide.setPosition(pos_x, pos_y);
			
			bottom_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			bottom_fx_slide.setImage(info.mFX_First);
			if(mIsAddBlend) {
				bottom_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			//top_fx_slide
			SVISlide top_fx_slide = bottom_base_slide.getSubSlide(1);
			
			bottom_fx_width = fxRegion.mSize.mWidth * info.mFirstScale;
			bottom_fx_height = fxRegion.mSize.mHeight * info.mFirstScale;
			top_fx_slide.setRegion(fxRegion.mOrigin.mX, fxRegion.mOrigin.mY, bottom_fx_width, bottom_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			top_fx_slide.setPosition(pos_x, pos_y);
			
			top_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			
			top_fx_slide.setImage(info.mFX_Top);
			if(mIsAddBlend) {
				top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			/*//top_base_slide
			SVIEffectNode top_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FIRST_TOP_BASE);
			top_base_node.setRegion(topRegion);
			rect = top_base_node.getRegion();
			SVISlide top_base_slide = top_base_node.getSlide();
			top_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth * info.mFirstScale, rect.mSize.mHeight * info.mFirstScale);
			top_base_slide.setPosition(info.mFirstCenter.x, info.mFirstCenter.y);
			top_base_slide.setScale(0.68f, 0.68f, 1.0f);
			top_base_slide.setImage(info.mTopBase);*/
		}
		
		//set second circle
		{
			SVIRect rect;
			
			//bottom_base node
			SVIEffectNode bottom_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_SECOND);
			bottom_base_node.setRegion(bottomRegion);
			rect = bottom_base_node.getRegion();
			SVISlide bottom_base_slide = bottom_base_node.getSlide();
			
			bottom_width = rect.mSize.mWidth * info.mSecondScale;
			bottom_height = rect.mSize.mHeight * info.mSecondScale;
			
			bottom_base_slide.setCornerRadius(bottom_width * 0.5f);
			
			bottom_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, bottom_width, bottom_height);
			bottom_base_slide.setPosition(info.mSecondCenter.x, info.mSecondCenter.y);
			bottom_base_slide.setImage(info.mBottomBase);
			
			//bottom_fx_slide
			SVISlide bottom_fx_slide = bottom_base_slide.getSubSlide(0);
			
			top_fx_width = fxRegion.mSize.mWidth * info.mSecondScale;
			top_fx_height = fxRegion.mSize.mHeight * info.mSecondScale;
			bottom_fx_slide.setRegion(fxRegion.mOrigin.mY, fxRegion.mOrigin.mY, top_fx_width, top_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			bottom_fx_slide.setPosition(pos_x, pos_y);
			
			bottom_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			bottom_fx_slide.setImage(info.mFX_Second);
			if(mIsAddBlend) {
				bottom_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			//top_fx_slide
			SVISlide top_fx_slide = bottom_base_slide.getSubSlide(1);
			
			bottom_fx_width = fxRegion.mSize.mWidth * info.mSecondScale;
			bottom_fx_height = fxRegion.mSize.mHeight * info.mSecondScale;
			top_fx_slide.setRegion(fxRegion.mOrigin.mX, fxRegion.mOrigin.mY, bottom_fx_width, bottom_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			top_fx_slide.setPosition(pos_x, pos_y);
			
			top_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			
			top_fx_slide.setImage(info.mFX_Top);
			if(mIsAddBlend) {
				top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			/*//top_base_slide
			SVIEffectNode top_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_SECOND_TOP_BASE);
			top_base_node.setRegion(topRegion);
			rect = top_base_node.getRegion();
			SVISlide top_base_slide = top_base_node.getSlide();
			top_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth * info.mSecondScale, rect.mSize.mHeight * info.mSecondScale);
			top_base_slide.setPosition(info.mSecondCenter.x, info.mSecondCenter.y);
			top_base_slide.setScale(0.68f, 0.68f, 1.0f);
			top_base_slide.setImage(info.mTopBase);*/
		}
		
		//set third circle
		{
			SVIRect rect;
			
			//bottom_base node
			SVIEffectNode bottom_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_THIRD);
			bottom_base_node.setRegion(bottomRegion);
			rect = bottom_base_node.getRegion();
			SVISlide bottom_base_slide = bottom_base_node.getSlide();
			
			bottom_width = rect.mSize.mWidth * info.mThirdScale;
			bottom_height = rect.mSize.mHeight * info.mThirdScale;
			
			bottom_base_slide.setCornerRadius(bottom_width * 0.5f);
			
			bottom_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, bottom_width, bottom_height);
			bottom_base_slide.setPosition(info.mThirdCenter.x, info.mThirdCenter.y);
			bottom_base_slide.setImage(info.mBottomBase);
			
			//bottom_fx_slide
			SVISlide bottom_fx_slide = bottom_base_slide.getSubSlide(0);
			
			top_fx_width = fxRegion.mSize.mWidth * info.mThirdScale;
			top_fx_height = fxRegion.mSize.mHeight * info.mThirdScale;
			bottom_fx_slide.setRegion(fxRegion.mOrigin.mY, fxRegion.mOrigin.mY, top_fx_width, top_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			bottom_fx_slide.setPosition(pos_x, pos_y);
			
			bottom_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			
			//bottom_fx_slide.setScale(OTHERS_BOTTOM_FX_SCALE, OTHERS_BOTTOM_FX_SCALE, 1.0f);
			
			bottom_fx_slide.setImage(info.mFX_Third);
			if(mIsAddBlend) {
				bottom_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			//top_fx_slide
			SVISlide top_fx_slide = bottom_base_slide.getSubSlide(1);
			
			bottom_fx_width = fxRegion.mSize.mWidth * info.mThirdScale;
			bottom_fx_height = fxRegion.mSize.mHeight * info.mThirdScale;
			top_fx_slide.setRegion(fxRegion.mOrigin.mX, fxRegion.mOrigin.mY, bottom_fx_width, bottom_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			top_fx_slide.setPosition(pos_x, pos_y);
			
			top_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			
			top_fx_slide.setImage(info.mFX_Top);
			if(mIsAddBlend) {
				top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			/*//top_base_slide
			SVIEffectNode top_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_THIRD_TOP_BASE);
			top_base_node.setRegion(topRegion);
			rect = top_base_node.getRegion();
			SVISlide top_base_slide = top_base_node.getSlide();
			top_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth * info.mThirdScale, rect.mSize.mHeight * info.mThirdScale);
			top_base_slide.setPosition(info.mThirdCenter.x, info.mThirdCenter.y);
			top_base_slide.setScale(0.68f, 0.68f, 1.0f);
			top_base_slide.setImage(info.mTopBase);*/
		}
		
		//set fourth circle
		{
			SVIRect rect;
			
			//bottom_base node
			SVIEffectNode bottom_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FOURTH);
			bottom_base_node.setRegion(bottomRegion);
			rect = bottom_base_node.getRegion();
			SVISlide bottom_base_slide = bottom_base_node.getSlide();
			
			bottom_width = rect.mSize.mWidth * info.mFourthScale;
			bottom_height = rect.mSize.mHeight * info.mFourthScale;
			
			bottom_base_slide.setCornerRadius(bottom_width * 0.5f);
			
			bottom_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, bottom_width, bottom_height);
			bottom_base_slide.setPosition(info.mFourthCenter.x, info.mFourthCenter.y);
			bottom_base_slide.setImage(info.mBottomBase);
			
			//bottom_fx_slide
			SVISlide bottom_fx_slide = bottom_base_slide.getSubSlide(0);
			
			top_fx_width = fxRegion.mSize.mWidth * info.mFourthScale;
			top_fx_height = fxRegion.mSize.mHeight * info.mFourthScale;
			bottom_fx_slide.setRegion(fxRegion.mOrigin.mY, fxRegion.mOrigin.mY, top_fx_width, top_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			bottom_fx_slide.setPosition(pos_x, pos_y);
			
			bottom_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			
			bottom_fx_slide.setImage(info.mFX_Fourth);
			if(mIsAddBlend) {
				bottom_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			//top_fx_slide
			SVISlide top_fx_slide = bottom_base_slide.getSubSlide(1);
			
			bottom_fx_width = fxRegion.mSize.mWidth * info.mFourthScale;
			bottom_fx_height = fxRegion.mSize.mHeight * info.mFourthScale;
			top_fx_slide.setRegion(fxRegion.mOrigin.mX, fxRegion.mOrigin.mY, bottom_fx_width, bottom_fx_height);
			
			pos_x = bottom_width / 2.0f;
			pos_y = bottom_height / 2.0f;
			top_fx_slide.setPosition(pos_x, pos_y);
			
			top_fx_slide.setScale(0.68f, 0.68f, 1.0f);
			top_fx_slide.setImage(info.mFX_Top);
			top_fx_slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			
			/*//top_base_slide
			SVIEffectNode top_base_node = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FOURTH_TOP_BASE);
			top_base_node.setRegion(topRegion);
			rect = top_base_node.getRegion();
			SVISlide top_base_slide = top_base_node.getSlide();
			top_base_slide.setRegion(rect.mOrigin.mX, rect.mOrigin.mY, rect.mSize.mWidth * info.mFourthScale, rect.mSize.mHeight * info.mFourthScale);
			top_base_slide.setPosition(info.mFourthCenter.x, info.mFourthCenter.y);
			top_base_slide.setScale(0.68f, 0.68f, 1.0f);
			top_base_slide.setImage(info.mTopBase);*/
		}
		
		SVISlide mask_slide = mPannelNodes.get(DJSkinIndex.IDX_MASK).getSlide();
		mask_slide.setImage(aMaskBitmap);
	}
	
	public void setAnimationDuration(int duration){
		mAnimationDuration = duration;
	}
	
	@Deprecated
	public void setMovementScale(float scale){
		//mEffectScale = scale;
	}

	@Deprecated
	public void setAnimationSpeed(float speed){
		//mAnimationSpeed = speed;
	}
	
	public float getAnimationSpeed(){
		return mAnimationSpeed;
	}
	
	public void setTimeDelta(long aTimeDelta) {
		if(mCurrentSkin == SkinType.SKIN_FUNKY) {
			//2013/01/31 rareboy0112@facebook.com
			//Rotate 540 degrees for 1 minute.
			float rotateZ;
			float deltaZ = (540.0f / 60.0f) / (1000.0f / (float)aTimeDelta);
			rotateZ = mRotateZ + deltaZ;
			if(rotateZ > 360.0f) {
				rotateZ -= 360.0f;
			}
			else if(rotateZ < -360.0f) {
				rotateZ += 360.0f;
			}
			
			mRotateZ = rotateZ;
			
			SVISlide slide;
			
			if (mPannelNodes.size() <= DJSkinIndex.IDX_TOP_FX) return;
			slide = mPannelNodes.get(DJSkinIndex.IDX_TOP_FX).getSlide();
			
			//rotate clockwise direction.
			slide.setRotation(0.0f, 0.0f, mRotateZ, 1.0f);
			
			if (mPannelNodes.size() <= DJSkinIndex.IDX_HIGH_FX) return;
			slide = mPannelNodes.get(DJSkinIndex.IDX_HIGH_FX).getSlide();
			//rotate counter-clockwise direction.
			slide.setRotation(0.0f, 0.0f, -mRotateZ, 1.0f);
			
			if (mPannelNodes.size() <= DJSkinIndex.IDX_MID_FX) return;
			slide = mPannelNodes.get(DJSkinIndex.IDX_MID_FX).getSlide();
			//rotate clockwise direction.
			slide.setRotation(0.0f, 0.0f, mRotateZ, 1.0f);
			
			
			MaskInfo info = null;
			info = getCurrentMaskInfo();
			
			if(info == null) {
				return;
				
			}
			
			//first_other_slide
			deltaZ = (info.mOtherFirstRotate / 60.0f) / (1000.0f / (float)aTimeDelta);
			rotateZ = mOtherFirstRotateZ + deltaZ;
			if(rotateZ > 360.0f) {
				rotateZ -= 360.0f;
			}
			else if(rotateZ < -360.0f) {
				rotateZ += 360.0f;
			}
			
			SVISlide first_other_slide = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FIRST).getSlide().getSubSlide(0);
			first_other_slide.setRotation(0.0f, 0.0f, mOtherFirstRotateZ, 1.0f);
			
			//second_other_slide
			deltaZ = (info.mOtherSecondRotate / 60.0f) / (1000.0f / (float)aTimeDelta);
			rotateZ = mOtherSecondRotateZ + deltaZ;
			if(rotateZ > 360.0f) {
				rotateZ -= 360.0f;
			}
			else if(rotateZ < -360.0f) {
				rotateZ += 360.0f;
			}
			
			SVISlide second_other_slide = mPannelNodes.get(DJSkinIndex.IDX_OTHER_SECOND).getSlide().getSubSlide(0);
			second_other_slide.setRotation(0.0f, 0.0f, mOtherSecondRotateZ, 1.0f);
			
			//third_other_slide
			deltaZ = (info.mOtherThirdRotate / 60.0f) / (1000.0f / (float)aTimeDelta);
			rotateZ = mOtherThirdRotateZ + deltaZ;
			if(rotateZ > 360.0f) {
				rotateZ -= 360.0f;
			}
			else if(rotateZ < -360.0f) {
				rotateZ += 360.0f;
			}
			SVISlide third_other_slide = mPannelNodes.get(DJSkinIndex.IDX_OTHER_THIRD).getSlide().getSubSlide(0);
			third_other_slide.setRotation(0.0f, 0.0f, mOtherThirdRotateZ, 1.0f);
			
			//fourth_other_slide
			deltaZ = (info.mOtherFourthRotate / 60.0f) / (1000.0f / (float)aTimeDelta);
			rotateZ = mOtherFourthRotateZ + deltaZ;
			if(rotateZ > 360.0f) {
				rotateZ -= 360.0f;
			}
			else if(rotateZ < -360.0f) {
				rotateZ += 360.0f;
			}
			SVISlide fourth_other_slide = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FOURTH).getSlide().getSubSlide(0);
			fourth_other_slide.setRotation(0.0f, 0.0f, mOtherFourthRotateZ, 1.0f);
		}
	}
	
	public void setDecibel(float fDB){
		float filteredDB = (fDB + 25.0f) / 40.0f;			
		mNextFrequencyDepth = filteredDB;
		setPannelDepthAnimation((int)(mAnimationDuration * mAnimationSpeed));
		mLastFrequencyDepth = mNextFrequencyDepth;
	}
	
	public void setDecibelRatio(float fDBRatio){
		float filteredDB = fDBRatio;
		if(mCurrentSkin == SkinType.SKIN_FUNKY) {
			filteredDB = fDBRatio;
			if(filteredDB > 1.0f) {
				filteredDB = 1.0f;
			}
		}
		
		mNextFrequencyDepth = filteredDB;
		setPannelDepthAnimation((int)(mAnimationDuration * mAnimationSpeed));
		mLastFrequencyDepth = mNextFrequencyDepth;
	}
	
	public int getAnimationDuration(){
		return mAnimationDuration;
	}
	
	
	private void setPannelDepthAnimation(int aDuration){
		
		if (mPannelNodes.size() == 0) return;
		
		if(mCurrentSkin == SkinType.SKIN_CLASSIC) {
			animateDefaultSkin(aDuration);
		}
		else if(mCurrentSkin == SkinType.SKIN_FUNKY) {
			animateDJSkin(aDuration);
		}
	}
	
	
	private void animateDefaultSkin(int aDuration) {
		animateCenter(aDuration);
		animateBase(aDuration);
		animateRubberIn(aDuration);
		animateRubberOut(aDuration);
	}
	
	
	private void animateDJSkin(int aDuration) {
		animateBaseFX(aDuration);
		animateLowFX(aDuration);
		animateMidFX(aDuration);
		animateHighFX(aDuration);
		animateTopBase(aDuration);
		animateTopFX(aDuration);
		animateOtherFirstCircle(aDuration);
		animateOtherSecondCircle(aDuration);
		animateOtherThirdCircle(aDuration);
		animateOtherFourthCircle(aDuration);
	}
	
	private void animateBaseFX(int aDuration) {
		if (mPannelNodes.size() <= DJSkinIndex.IDX_BASE_FX) return;
		SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_BASE_FX).getSlide();
		
		float toScale, toOpacity;
		toScale = mMusicBase * 2.4f + 2.8f;
		toOpacity = mMusicBase * 2.0f;
		
		slide.setScale(toScale, toScale, 1.0f, aDuration);
		slide.setOpacity(toOpacity, aDuration);
	}
	
	private void animateLowFX(int aDuration) {
		if (mPannelNodes.size() <= DJSkinIndex.IDX_LOW_FX) return;
		SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_LOW_FX).getSlide();
		
		float toScale, toOpacity;
		toScale = mMusicLow * 0.8f + 2.6f;
		toOpacity = mMusicLow * 0.7f + 0.5f;
		
		slide.setScale(toScale, toScale, 1.0f, aDuration);
		slide.setOpacity(toOpacity, aDuration);
	}

	private void animateMidFX(int aDuration) {
		if (mPannelNodes.size() <= DJSkinIndex.IDX_MID_FX) return;
		SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_MID_FX).getSlide();
		
		float toScale, toOpacity;
		toScale = mMusicMid * 8.0f + 2.4f;
		toOpacity = mMusicMid;
		
		slide.setScale(toScale, toScale, 1.0f, aDuration);
		slide.setOpacity(toOpacity, aDuration);
	}

	private void animateHighFX(int aDuration) {
		if (mPannelNodes.size() <= DJSkinIndex.IDX_HIGH_FX) return;
		SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_HIGH_FX).getSlide();
		
		float toScale, toOpacity;
		toScale = mMusicHigh * 8.0f + 1.2f;
		toOpacity = mMusicHigh * 1.4f + 0.3f;
				
		slide.setScale(toScale, toScale, 1.0f, aDuration);
		slide.setOpacity(toOpacity, aDuration);
	}
	
	private void animateTopBase(int aDuration) {
		if (mPannelNodes.size() <= DJSkinIndex.IDX_TOP_BASE) return;
		SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_TOP_BASE).getSlide();
		
		float toScale;
		if(mNextFrequencyDepth <= 0.35f) {
			toScale = 1.0f;
		}
		else {
			toScale = (mNextFrequencyDepth - 0.35f) * 0.07f + 1.0f;
		}
		
		slide.setScale(toScale, toScale, 1.0f, aDuration);
	}
	
	private void animateTopFX(int aDuration) {
		if (mPannelNodes.size() <= DJSkinIndex.IDX_TOP_FX) return;
		SVISlide slide = mPannelNodes.get(DJSkinIndex.IDX_TOP_FX).getSlide();
		
		float toScale;
		if(mNextFrequencyDepth <= 0.35f) {
			toScale = 1.0f;
		}
		else {
			toScale = (mNextFrequencyDepth - 0.35f) * 0.07f + 1.0f;
		}
		
		slide.setScale(toScale, toScale, 1.0f, aDuration);
		slide.setOpacity(mNextFrequencyDepth, aDuration);
	}
	
	private void animateOtherFirstCircle(int aDuration) {
		
		float toScale, toOpacity;
		//bottom_fx
		SVISlide bottom_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FIRST).getSlide().getSubSlide(0);
		toScale = mMusicBase * 2.5f + OTHERS_BOTTOM_FX_SCALE;
		toOpacity = mMusicBase * 2.8f;
		
		bottom_fx.setScale(toScale, toScale, 1.0f, aDuration);
		bottom_fx.setOpacity(toOpacity, aDuration);
		
		//top_fx
		SVISlide top_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FIRST).getSlide().getSubSlide(1);
		toScale = mMusicBase * 3.9f + 1.2f;
		toOpacity = mMusicBase * 0.7f + 0.5f;
		
		top_fx.setScale(toScale, toScale, 1.0f, aDuration);
		top_fx.setOpacity(toOpacity, aDuration);
		
		/*//top_base
		SVISlide top_base = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FIRST_TOP_BASE).getSlide();
		if(mMusicBase <= 0.45f) {
			toScale = 0.68f;
		}
		else {
			toScale = (mMusicBase - 0.45f) * 0.1f + 0.68f;
		}
		
		top_base.setScale(toScale, toScale, 1.0f, aDuration);*/
	}
	
	private void animateOtherSecondCircle(int aDuration) {
		
		float toScale, toOpacity;
		//bottom_fx
		SVISlide bottom_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_SECOND).getSlide().getSubSlide(0);
		toScale = mMusicLow * 2.5f + OTHERS_BOTTOM_FX_SCALE;
		toOpacity = mMusicLow * 2.8f;
		
		bottom_fx.setScale(toScale, toScale, 1.0f, aDuration);
		bottom_fx.setOpacity(toOpacity, aDuration);
		
		//top_fx
		SVISlide top_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_SECOND).getSlide().getSubSlide(1);
		toScale = mMusicLow * 3.9f + 1.2f;
		toOpacity = mMusicLow * 0.7f + 0.5f;
		
		top_fx.setScale(toScale, toScale, 1.0f, aDuration);
		top_fx.setOpacity(toOpacity, aDuration);
		
		/*//top_base
		SVISlide top_base = mPannelNodes.get(DJSkinIndex.IDX_OTHER_SECOND_TOP_BASE).getSlide();
		if(mMusicLow <= 0.45f) {
			toScale = 0.68f;
		}
		else {
			toScale = (mMusicLow - 0.45f) * 0.1f + 0.68f;
		}
		
		top_base.setScale(toScale, toScale, 1.0f, aDuration);*/
	}
	
	private void animateOtherThirdCircle(int aDuration) {
		
		float toScale, toOpacity;
		
		//bottom_fx
		SVISlide bottom_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_THIRD).getSlide().getSubSlide(0);
		toScale = mMusicMid * 2.5f + OTHERS_BOTTOM_FX_SCALE;
		toOpacity = mMusicMid * 2.8f;
		
		bottom_fx.setScale(toScale, toScale, 1.0f, aDuration);
		bottom_fx.setOpacity(toOpacity, aDuration);
		
		//top_fx
		SVISlide top_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_THIRD).getSlide().getSubSlide(1);
		toScale = mMusicMid * 3.9f + 1.2f;
		toOpacity = mMusicMid * 0.7f + 0.5f;
		
		top_fx.setScale(toScale, toScale, 1.0f, aDuration);
		top_fx.setOpacity(toOpacity, aDuration);
		
		/*//top_base
		SVISlide top_base = mPannelNodes.get(DJSkinIndex.IDX_OTHER_THIRD_TOP_BASE).getSlide();
		if(mMusicMid <= 0.45f) {
			toScale = 0.68f;
		}
		else {
			toScale = (mMusicMid - 0.45f) * 0.1f + 0.68f;
		}
		
		top_base.setScale(toScale, toScale, 1.0f, aDuration);*/
	}
	
	private void animateOtherFourthCircle(int aDuration) {
		
		float toScale, toOpacity;
		
		//bottom_fx
		SVISlide bottom_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FOURTH).getSlide().getSubSlide(0);
		toScale = mMusicHigh * 2.5f + OTHERS_BOTTOM_FX_SCALE;
		toOpacity = mMusicHigh * 2.8f;
		
		bottom_fx.setScale(toScale, toScale, 1.0f, aDuration);
		bottom_fx.setOpacity(toOpacity, aDuration);
		
		//top_fx
		SVISlide top_fx = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FOURTH).getSlide().getSubSlide(1);
		toScale = mMusicHigh * 3.9f + 1.2f;
		toOpacity = mMusicHigh * 0.7f + 0.5f;
		
		top_fx.setScale(toScale, toScale, 1.0f, aDuration);
		top_fx.setOpacity(toOpacity, aDuration);
		
		/*//top_base
		SVISlide top_base = mPannelNodes.get(DJSkinIndex.IDX_OTHER_FOURTH_TOP_BASE).getSlide();
		if(mMusicHigh <= 0.45f) {
			toScale = 0.68f;
		}
		else {
			toScale = (mMusicHigh - 0.45f) * 0.1f + 0.68f;
		}
		
		top_base.setScale(toScale, toScale, 1.0f, aDuration);*/
	}
	
	private void animateDefaultSkinObject(SVISlide slide, int duration, float startbase, float tobase){
		float startScale = startbase + (mLastFrequencyDepth-0.5f) * tobase * mEffectScale;
		float endScale = startbase + (mNextFrequencyDepth-0.5f) * tobase * mEffectScale;
				
		SVIAnimationSet aniSet = new SVIAnimationSet();
		aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		aniSet.setDuration(duration);
		aniSet.setRepeatCount(0);
		aniSet.setAutoReverse(false);
		aniSet.setOffset(0);			
				
		SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
		keyScaleAni.setDuration(duration);
		keyScaleAni.setRepeatCount(0);
		keyScaleAni.setAutoReverse(false);
		keyScaleAni.setOffset(0);
		keyScaleAni.addKeyProperty(0.0f, new SVIVector3(startScale,startScale,startScale));
		keyScaleAni.addKeyProperty(1.0f, new SVIVector3(endScale, endScale, endScale));
		aniSet.addAnimation(keyScaleAni);
		
		
		if (mUseDepthAnimation){
			SVIKeyFrameAnimation keyPowerAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.LIGHT_POWER);
			keyPowerAni.setDuration(duration);
			keyPowerAni.setRepeatCount(0);
			keyPowerAni.setAutoReverse(false);
			keyPowerAni.setOffset(0);
			keyPowerAni.addKeyProperty(0.0f, -0.7f + -3.0f * mLastFrequencyDepth * mEffectScale);
			keyPowerAni.addKeyProperty(1.0f, -0.7f + -3.0f * mNextFrequencyDepth * mEffectScale);
			aniSet.addAnimation(keyPowerAni);		
		}else{
			slide.setLightPower(-1.3f);
		}
		
		slide.setOpacity(1.0f);
		slide.startAnimation(aniSet);
	}
	
	
	/*private void animateDJSkinObject(SVISlide slide, int duration, float fromScale, float toScale, float fromOpacity, float toOpacity){
		SVIAnimationSet aniSet = new SVIAnimationSet();
		aniSet.setAnimationSetInterpolator(SVIAnimation.InterpolatorType.ACCELERATE_DECELERATE);
		aniSet.setDuration(duration);
		aniSet.setRepeatCount(0);
		aniSet.setAutoReverse(false);
		aniSet.setOffset(0);			
				
		SVIKeyFrameAnimation keyScaleAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.SCALE);
		keyScaleAni.setDuration(duration);
		keyScaleAni.setRepeatCount(0);
		keyScaleAni.setAutoReverse(false);
		keyScaleAni.setOffset(0);
		keyScaleAni.addKeyProperty(0.0f, new SVIVector3(fromScale,fromScale,1.0f));
		keyScaleAni.addKeyProperty(1.0f, new SVIVector3(toScale, toScale, 1.0f));
		aniSet.addAnimation(keyScaleAni);
		
		SVIKeyFrameAnimation keyOpacityAni = new SVIKeyFrameAnimation(SVIPropertyAnimation.PropertyAnimationType.OPACITY);
		keyOpacityAni.setDuration(duration);
		keyOpacityAni.setRepeatCount(0);
		keyOpacityAni.setAutoReverse(false);
		keyOpacityAni.setOffset(0);
		keyOpacityAni.addKeyProperty(0.0f, fromOpacity);
		keyOpacityAni.addKeyProperty(1.0f, toOpacity);
		aniSet.addAnimation(keyOpacityAni);
		
		slide.setOpacity(toOpacity);
		slide.startAnimation(aniSet);
	}*/
	
	
	private void animateCenter(int duration){
		if (mPannelNodes.size() <= DefaultSkinIndex.IDX_CENTERCAP) return;
		SVISlide slide = mPannelNodes.get(DefaultSkinIndex.IDX_CENTERCAP).getSlide();		
		animateDefaultSkinObject(slide, duration, 1.4f, 0.6f);	
	}
	
	private void animateBase(int duration){
		if (mPannelNodes.size() <= DefaultSkinIndex.IDX_BASE) return;
		SVISlide slide = mPannelNodes.get(DefaultSkinIndex.IDX_BASE).getSlide();
		animateDefaultSkinObject(slide, duration, 1.3f, 0.46f);
	}
	
	private void animateRubberIn(int duration){
		if (mPannelNodes.size() <= DefaultSkinIndex.IDX_RUBBERIN) return;
		SVISlide slide = mPannelNodes.get(DefaultSkinIndex.IDX_RUBBERIN).getSlide();
		animateDefaultSkinObject(slide, duration, 1.09f, 0.28f);		
	}
	
	private void animateRubberOut(int duration){
		if (mPannelNodes.size() <= DefaultSkinIndex.IDX_RUBBEROUT) return;
		SVISlide slide = mPannelNodes.get(DefaultSkinIndex.IDX_RUBBEROUT).getSlide();
		animateDefaultSkinObject(slide, duration, 1.05f, 0.14f);
	}
	
	class SVIEffectSpeakerAnimCallback extends SVIEffectNodeCallback{		
				
		public SVIEffectSpeakerAnimCallback(){}	
		
		@Override
		public boolean callbackInitialize(SVIEffectNode node){
			if(mCurrentSkin == SkinType.SKIN_CLASSIC) {
				initNodeDefaultSkin(node);
			}
			else if(mCurrentSkin == SkinType.SKIN_FUNKY) {
				initNodeDJSkin(node);
			}
			
			return false;
		}
		
		private void initNodeDefaultSkin(SVIEffectNode node) {
			SVISlide slide = node.getSlide();
			slide.setOpacity(1.0f);
			slide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,0.0f));
			slide.setLightColor(new float[]{1.0f, 1.0f, 1.0f, 64.0f});	
			slide.setLightPower(-1.0f);
		}
		
		private void initNodeDJSkin(SVIEffectNode node) {
			SVISlide slide = node.getSlide();
			slide.setOpacity(1.0f);
		}
		
		@Override
		public boolean callbackPannelInitialize(SVIEffectNode node) {
			if(mCurrentSkin == SkinType.SKIN_CLASSIC) {
				initPannelNodeDefaultSkin(node);
			}
			else if(mCurrentSkin == SkinType.SKIN_FUNKY) {
				initPannelNodeDJSkin(node);
			}
			
			return false;
		}
		
		private void initPannelNodeDefaultSkin(SVIEffectNode node) {
			SVISlide slide = node.getSlide();
			
			//g : 0.55 b : 0.6
			slide.setLightColor(new float[]{0.001f, -0.1f, 0.8f, 11.0f});
			slide.setViewDirection(new SVIVector4(0.0f, 0.5f, 1.0f, 1.0f));
			
			if (mPannelNodes.size() > DefaultSkinIndex.IDX_COVER && node == mPannelNodes.get(DefaultSkinIndex.IDX_COVER)){
				slide.setLightColor(new float[]{0.001f, 0.0f, 0.8f, 9.8f});
				slide.setViewDirection(new SVIVector4(0.0f, 0.55f, 1.0f, 1.0f));		
				slide.setBackgroundColor(new SVIColor(1.0f,1.0f,1.0f,1.0f));
			}else{
				slide.setBackgroundColor(new SVIColor(0.4f,0.4f,0.4f,1.0f));
			}					
			
			if (mPannelNodes.size() > DefaultSkinIndex.IDX_RUBBERIN && node == mPannelNodes.get(DefaultSkinIndex.IDX_RUBBERIN)){
				slide.setLightColor(new float[]{0.001f, -0.1f, 0.8f, 10.5f});
				slide.setViewDirection(new SVIVector4(0.0f, 0.5f, 1.0f, 1.0f));		
				slide.setBackgroundColor(new SVIColor(0.6f,0.6f,0.6f,1.0f));
			}
			
			if (mPannelNodes.size() > DefaultSkinIndex.IDX_RUBBEROUT && node == mPannelNodes.get(DefaultSkinIndex.IDX_RUBBEROUT)){
				slide.setLightColor(new float[]{0.001f, -0.1f, 0.8f, 10.5f});
				slide.setViewDirection(new SVIVector4(0.0f, 0.5f, 1.0f, 1.0f));		
				slide.setBackgroundColor(new SVIColor(0.6f,0.6f,0.6f,1.0f));
			}
			
			if (mPannelNodes.size() > DefaultSkinIndex.IDX_CENTERCAP && node == mPannelNodes.get(DefaultSkinIndex.IDX_CENTERCAP)){
				slide.setLightColor(new float[]{0.001f, -0.1f, 0.8f, 11.0f});				
			}
		}
		
		private void initPannelNodeDJSkin(SVIEffectNode node) {
			SVISlide slide = node.getSlide();
			
			if (mPannelNodes.size() > DJSkinIndex.IDX_BASE_FX && node == mPannelNodes.get(DJSkinIndex.IDX_BASE_FX)){
				slide.setOpacity(0.0f);
				slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			if (mPannelNodes.size() > DJSkinIndex.IDX_LOW_FX && node == mPannelNodes.get(DJSkinIndex.IDX_LOW_FX)){
				slide.setOpacity(0.0f);
				slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			if (mPannelNodes.size() > DJSkinIndex.IDX_MID_FX && node == mPannelNodes.get(DJSkinIndex.IDX_MID_FX)){
				slide.setOpacity(0.0f);
				slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			if (mPannelNodes.size() > DJSkinIndex.IDX_HIGH_FX && node == mPannelNodes.get(DJSkinIndex.IDX_HIGH_FX)){
				slide.setOpacity(0.0f);
				slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			if (mPannelNodes.size() > DJSkinIndex.IDX_TOP_FX && node == mPannelNodes.get(DJSkinIndex.IDX_TOP_FX)){
				slide.setOpacity(0.0f);
				slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
			
			if (mPannelNodes.size() > DJSkinIndex.IDX_FLASH && node == mPannelNodes.get(DJSkinIndex.IDX_FLASH)){
				slide.setOpacity(0.0f);
				slide.setBlendType(SVISlide.BlendType.BLEND_ONE);
			}
		}
		
		@Override
		public int callbackExtraTask(SVIEffectNode node, MotionEvent event, int index) {
			
			// TODO Auto-generated method stub
			return 0;
		}
				
		@Override
		public boolean callbackClickAnimation(SVIEffectNode node, MotionEvent event, int index){ 
			
			return false; 
		}
		
		@Override
		public boolean callbackLongPressAnimation(SVIEffectNode node, MotionEvent event){ 
			
			return false; 
		}
		
		@Override
		public boolean callbackHoveringAnimation(SVIEffectNode node, MotionEvent event){ 
			
			return false; 
		}
				
		float mSensorX = 10.0f;
		float mSensorY = 10.0f;
		@Override
		public boolean callbackSensor(SVISlide slide, float [] value , int type){
			//2013/01/31 rareboy0112@facebook.com
			//SKINE_DEFAULT mode only support sensor.
			if(mCurrentSkin != SkinType.SKIN_CLASSIC)
				return false;
			
			float x = value[0];
			float y = value[1];
			float z = value[2];
						
			if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
			if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;			
			
			slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
			//slide.setRotation(value[3] , value[4], 0.0f , 0.0f);
			return false;
		}

		@Override
		public boolean callbackSensor(SVIEffectNode node, float [] value , int type){
			//2013/01/31 rareboy0112@facebook.com
			//SKINE_DEFAULT mode only support sensor.
			if(mCurrentSkin != SkinType.SKIN_CLASSIC)
				return false;
			
			SVISlide slide = node.getSlide();
			float x = value[0];
			float y = value[1];
			float z = value[2];
			
						
			if (y < 7.0f*SENSOR_SCALE_CENTER) y = 7.0f*SENSOR_SCALE_CENTER;
			if (z < 7.0f*SENSOR_SCALE_CENTER) z = 7.0f*SENSOR_SCALE_CENTER;			
			
			slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
			
			
			SVIEffectNode rubberinnode = mPannelNodes.get(DefaultSkinIndex.IDX_RUBBEROUT);
			if (node == rubberinnode){
				
				x = value[0];
				y = value[1];
				z = value[2];
											
				if (y < 7.0f*SENSOR_SCALE_RUBBER) y = 7.0f*SENSOR_SCALE_RUBBER;
				if (z < 7.0f*SENSOR_SCALE_RUBBER) z = 7.0f*SENSOR_SCALE_RUBBER;			
								
				slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
				slide.setViewDirection(new SVIVector4(x, y, z, 1.0f));
			}
			SVIEffectNode rubberoutnode = mPannelNodes.get(DefaultSkinIndex.IDX_RUBBEROUT);
			if (node == rubberoutnode){
				
				x = value[0];
				y = value[1];
				z = value[2];
											
				if (y < 7.0f*SENSOR_SCALE_RUBBER) y = 7.0f*SENSOR_SCALE_RUBBER;
				if (z < 7.0f*SENSOR_SCALE_RUBBER) z = 7.0f*SENSOR_SCALE_RUBBER;			
								
				slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
				slide.setViewDirection(new SVIVector4(x, y, z, 1.0f));	
			}
			
			if (mPannelNodes.size() > DefaultSkinIndex.IDX_COVER){
				SVIEffectNode lastnode = mPannelNodes.get(DefaultSkinIndex.IDX_COVER);
				if (node == lastnode){
					
					x = value[0];
					y = value[1];
					z = value[2];
												
					if (y < 7.0f*SENSOR_SCALE) y = 7.0f*SENSOR_SCALE;
					if (z < 7.0f*SENSOR_SCALE) z = 7.0f*SENSOR_SCALE;			
									
					slide.setLightDirection(new SVIVector4(x, y, z, 1.0f));
					slide.setViewDirection(new SVIVector4(x, y, z, 1.0f));	
				}
			}
					
			//slide.setRotation(value[3] , value[4], 0.0f , 0.0f);
			return false;
		}
	}
	
	private SVIEffectSpeakerAnimCallback mAnimationCallback = new SVIEffectSpeakerAnimCallback();

}
