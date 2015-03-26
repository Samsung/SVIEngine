/**
 * SpeakerSample.java 
 * @author Joonyong Park
 * @email masterkeaton27@gmail.com
 */

package com.github.svieffect.sample;

import java.io.InputStream;
import java.util.Calendar;
import java.util.Random;

import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.audiofx.Equalizer;
import android.media.audiofx.Visualizer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;

import com.github.svieffect.R;
import com.github.svieffect.main.SVIEffectListener;
import com.github.svieffect.speaker.SVIEffectSpeaker;
import com.github.svieffect.speaker.findColor.CollectColor;

//2012-12-04 masterkeaton27@gmail.com
//NOTE GRAPHICS: 
//should implements from SGIEffectListener interface for to check first render finished.
public class SpeakerSample extends Activity implements SensorEventListener, SVIEffectListener {
	
	private final String TAG = "SVIGroupPlay";
	
	private SVIEffectSpeaker mEffectView = null;
	private Sensor gravitymeter = null;	
	private SensorManager mSensorManager = null;
	private float[] mGravity = new float[]{0.0f, 0.0f, 0.0f};
 
	
	//2013-02-01 rareboy0112@facebook.com
	private SpeakerSkinClassic 	mSkinClassic = new SpeakerSkinClassic();
	private SpeakerSkinFuncky 	mSkinFuncky = new SpeakerSkinFuncky();
	//2013-07-22 rareboy0112@facebook.com
	private SpeakerSkinWave		mSkinWave = new SpeakerSkinWave();
	
		
	private float	mWidth = 720.0f;
	private float	mHeight = 1280.0f;
	private float	mMovementScale = 0.7f;
	
	private FrameLayout mLayout = null;
	
	//2013-02-01 rareboy0112@facebook.com
	private AbsoluteLayout mLayoutClassic = null;	
	private AbsoluteLayout mLayoutFuncky = null;
	//2013-07-22 rareboy0112@facebook.com
	private AbsoluteLayout mLayoutWave = null;
	
	//2013-02-01 rareboy0112@facebook.com
	private int mCurrentSkin = SVIEffectSpeaker.SkinType.SKIN_CLASSIC;
	private int mCurrentMask = SVIEffectSpeaker.MaskType.MASK_A;
	
	//2013-02-14 rareboy0112@facebook.com
	//changed keep time
	//change time value 2000 -> 8000
	private final long KEEP_MASK_TYPE = 8000;
	
	private Visualizer mVisualizer;
	private Equalizer mEqualizer;
	private AudioManager mAudioManager;
	private MediaPlayer mMusicPlayer;
		
	private boolean mUseSound = true;
	
	//2013-01-23 masterkeaton27@gmail.com
	private int FREQUENCY_MILLIHERTZ = 1000000;
	private ImageView mCoverView = null;
		
	//2013-01-23 masterkeaton27@gmail.com
	//splash view to avoid initialization blink
	private ImageView mSplashView = null;
	
	//2013-02-06 rareboy0112@facebook.com
	private float mDbValues[] = null;
	
	//2013-07-22 rareboy0112@facebook.com
	//use Wave mode
	private float mPrevDbValues[] = null;
	
	//2013-07-22 rareboy0112@facebook.com
	//use Wave mode
	private Point mWaveLowPos, mWaveMidPos, mWaveHighPos;
	
	//2013-07-22 rareboy0112@facebook.com
	//use Wave mode(find color)
	private CollectColor mCollectColor;
	
	//2013-08-08 teja.kim
	private int[] mColors = {
			0,0,0,
			242,64,197,
			166,49,234,
			234,56,188,
			195,44,221,
			1,61,244,
			
			223,57,176,
			203,1,86,
			185,1,210,
			188,2,82,
			159,17,216,
			
			241,5,125,
			2,47,179//,
		//	163,29,127
		};

	
	
	private int mFindColors[];
	
	//2013-02-01 rareboy0112@facebook.com
	//2012-12-21 masterkeaton27@gmail.com
	public View buildViews_Classic(Context context){				
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.speaker_layout_classic, null);
		return layoutView;
	}	
	
	
	//2013-02-01 rareboy0112@facebook.com
	//2013-01-23 masterkeaton27@gmail.com
	//due to orientation changing, need to inflate landscape layout
	public View buildViews_Funcky(Context context){			
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.speaker_layout_funcky, null);
		return layoutView;
	}
	
	//2013-07-22 rareboy0112@facebook.com
	public View buildViews_Wave(Context context) {
		LayoutInflater factory = LayoutInflater.from(context);
		View layoutView = factory.inflate(R.layout.speaker_layout_wave, null);
		return layoutView;
	}
	
	private double mRMS;
	private Calendar prevTime;
	private int count = 0;
	private double accRMS = 0;
	private long mChangedTime = 0;
	public void updateVisualizer(byte[] bytes)
	{
		//Get RMS
		double max = 0;
		for (int i = 0; i < bytes.length - 1; i++){
			max += bytes[i] * bytes[i];
		}
		
		if (max > 0){
			final double amplitude = max / (bytes.length - 1);

			double rms = Math.sqrt(amplitude);
			accRMS += rms;
			count++;

			if (prevTime == null){
				prevTime = Calendar.getInstance();
				return;
			}
			Calendar curTime = Calendar.getInstance();
			long deltaTime = curTime.getTimeInMillis() - prevTime.getTimeInMillis();
			if (deltaTime == 0)
				return;

			float aproxFps = 1000 / deltaTime;
			if (aproxFps > 20){
				return;
			}
			prevTime = curTime;

			//Update value
			mRMS = accRMS / count;
			accRMS = 0;
			count = 0;
			
			float ratio = (float)(Math.abs(mRMS - 128.0f) / 64.0f);
			
			//2013-02-06 rareboy0112@facebook.com
			if(mEffectView == null) {
				return;
			}
			
			//2013-01-07 masterkeaton27@gmail.com
			//NOTE: application developer can choose 2 method to set desibel
			mEffectView.setDecibelRatio(ratio); // min 0.0 max 1.0
			
			//mEffectView.setDesibel(); // min -25.0 max 20.0
			
			//2013-01-07 masterkeaton27@gmail.com
			//NOTE: effect movement scale // min 1.0 max unlimited (1.1 ~ 1.4)
			mEffectView.setMovementScale(mMovementScale);		
						
			//2013-02-01 rareboy0112@facebook.com
			//NOTE: don't forget this
			mEffectView.setTimeDelta(deltaTime);
			
			//2013-02-06 rareboy0112@facebook.com
			//NOTE: change mask image logic.(not used Android-ImageView)
			mChangedTime += deltaTime;
			if (mCurrentSkin == SVIEffectSpeaker.SkinType.SKIN_FUNKY && mChangedTime > KEEP_MASK_TYPE && ratio > 0.6f ) {
				
				int curMask = mCurrentMask; 
				Random random = new Random();
				
				while(curMask == mCurrentMask) {
					curMask = random.nextInt(SVIEffectSpeaker.MaskType.MASK_COUNT);
				}
				
				mCurrentMask = curMask;
				
				mSkinFuncky.changeBgImage(getResources(), curMask);
				
				mEffectView.setMaskType(curMask, mSkinFuncky.mMask);
				
				mChangedTime = 0;
			}
		}
	}
	
	public long prevTimeFFT = 0;
	
	public void updateVisualizerFFT(byte[] bytes) {
		if(mDbValues == null || mEffectView == null) {
			return;
		}
		
		byte rfk, ifk;
		float magnitude = 0.0f;
		int dbValue = 0;
		
		if(bytes.length < SVIEffectSpeaker.MusicType.MUSIC_COUNT * 2 + 2) {
			return;
		}
		
		for (int i = 0; i < SVIEffectSpeaker.MusicType.MUSIC_COUNT; i++) {
			rfk = bytes[2 * i + 2];
			ifk = bytes[2 * i + 1 + 2];
			magnitude = (rfk * rfk + ifk * ifk);

			dbValue = 0;

			if (magnitude != 0.0f) {
				dbValue = (int) (10 * Math.log10(magnitude));
			}
			
			mDbValues[i] = (float)dbValue * 2.0f / 100.0f;
		}
		
		//2013-02-06 rareboy0112@facebook.com
		//Base Channel (min 0.0 max 1.0)
		mEffectView.setMusiBase(mDbValues[0]);
		//Low Channel (min 0.0 max 1.0)
		mEffectView.setMusiLow(mDbValues[1]);
		//Middle Channel (min 0.0 max 1.0)
		mEffectView.setMusiMid(mDbValues[2]);
		//High Channel (min 0.0 max 1.0)
		mEffectView.setMusiHigh(mDbValues[3]);
		
		//2013-07-22 rareboy0112@facebook.com
		//NOTE UX:
		//change the this logic.
		
		if (System.currentTimeMillis() - prevTimeFFT > 300) {
			//check Low
			if (mDbValues[1] > mPrevDbValues[1]) {
				//Add 1 wave   : addLowWave(waveSize, startWavePoint, currunt X, current Y, Wave speed)
				mEffectView.addLowWave(250.0f, 100.0f, mWaveLowPos.x, mWaveLowPos.y, 8.0f*7.0f);
			}
			
			
			//not used mid wave
			/*
			if (mDbValues[2] > mPrevDbValues[2]) {
				//Add 1 wave   : addWave(waveSize, startWavePoint, currunt X, current Y, Wave speed)
				mEffectView.addWave(120.0f, 80.0f, mWaveMidPos.x, mWaveMidPos.y, 7.0f);
			}*/
			
			//check High
			if (mDbValues[3] > mPrevDbValues[3]) {
				//Add 1 wave   : addHighWave(waveSize, startWavePoint, currunt X, current Y, Wave speed)
				mEffectView.addHighWave(120.0f, 100.0f, mWaveHighPos.x, mWaveHighPos.y, 15.0f*7.0f);
			}
			
			prevTimeFFT = System.currentTimeMillis();
		}
		
		for (int i = 0; i < SVIEffectSpeaker.MusicType.MUSIC_COUNT; ++i) {
			mPrevDbValues[i] = mDbValues[i];
		}
	}
	
	protected void loadBGM(){
		
		try{
			
			mMusicPlayer = MediaPlayer.create(this, R.raw.bgm);
			mMusicPlayer.prepare();
			mMusicPlayer.setLooping(true);
			mMusicPlayer.start();
			
		} catch(Exception e){
		}
		
	}
	
	protected void createAudioModules(){
		mAudioManager = (AudioManager) getSystemService(AUDIO_SERVICE);
		
		//2013-02-06 rareboy0112@facebook.com
		//changed create equalizer code.
		int sessionId = mMusicPlayer != null ? mMusicPlayer.getAudioSessionId() : 0;
		mEqualizer = new Equalizer(0, sessionId);
		mEqualizer.setEnabled(true);
		
		mVisualizer = new Visualizer(0);
		if (mVisualizer.getEnabled())
			mVisualizer.setEnabled(false);
		mVisualizer.setCaptureSize(Visualizer.getCaptureSizeRange()[1] / 8);
		
        //2013-02-06 rareboy0112@facebook.com
		//changed create listener code.
		Visualizer.OnDataCaptureListener captureListener = new Visualizer.OnDataCaptureListener() {
			public void onWaveFormDataCapture(Visualizer visualizer, byte[] bytes, int samplingRate)
			{
				updateVisualizer(bytes);
			}

			public void onFftDataCapture(Visualizer visualizer, byte[] bytes, int samplingRate)
			{
				//2013/02-06 rareboy0112@facebook.com
				//NOTE GRAPHICS: 
				//only use sample code.
				//need to apply the correct value from the App.
				updateVisualizerFFT(bytes);
			}
		};
		
		mVisualizer.setDataCaptureListener(captureListener, Visualizer.getMaxCaptureRate() / 2, true, true);
		mVisualizer.setEnabled(true);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		//2012-12-04 masterkeaton27@gmail.com
		//NOTE GRAPHICS: 
		//set screen size to full, only for the demo.		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
				
		if (mUseSound){
			//2013-02-06 rareboy0112@facebook.com
			//Changed the order of method calls.
			loadBGM();
			createAudioModules();
		}
		
		mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
	    gravitymeter = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);	    
	    
	    mWidth = getWindowManager().getDefaultDisplay().getWidth();
		mHeight= getWindowManager().getDefaultDisplay().getHeight();
		
		//2013-02-01 masterkeaton27@gmail.com
		//funky mode is default.
		//setSkinMode(SVIEffectSpeaker.SkinType.SKIN_FUNKY);
		setSkinMode(SVIEffectSpeaker.SkinType.SKIN_WAVE);
		
		//2013-02-06 rareboy0112@facebook.com
		//frequency data
		mDbValues = new float[SVIEffectSpeaker.MusicType.MUSIC_COUNT];
		
		//2013-07-22 rareboy0112@facebook.com
		//use Wave mode
		mPrevDbValues = new float[SVIEffectSpeaker.MusicType.MUSIC_COUNT];
		for (int i = 0; i < SVIEffectSpeaker.MusicType.MUSIC_COUNT; ++i) {
			mPrevDbValues[i] = 0.0f;
		}
		
		//2013-07-22 rareboy0112@facebook.com
		//wave position
		mWaveLowPos = new Point();
		mWaveLowPos.x = 1080 / 2;
		mWaveLowPos.y = 1920 - (1920 / 4);
		
		mWaveMidPos = new Point();
		mWaveMidPos.x = 1080 / 4;
		mWaveMidPos.y = 1920 / 2;
		
		mWaveHighPos = new Point();
		mWaveHighPos.x = 1080;
		mWaveHighPos.y = 0;
		
		//2013-07-22 rareboy0112@facebook.com
		//find Album jacket colors
		mCollectColor = new CollectColor(getResources());
		
		
		// 2013-08-08 teja.kim
		mCollectColor.setDefaultColor(mColors);
	}	
	
	
	//2013-02-01 masterkeaton27@gmail.com
	//2013-02-01 rareboy0112@facebook.com
	//set mode for effect
	public void setSkinMode(int type){
		mChangedTime = 0;
		if (type == SVIEffectSpeaker.SkinType.SKIN_CLASSIC){
			mCurrentSkin = SVIEffectSpeaker.SkinType.SKIN_CLASSIC;
			buildLayoutForClassic();
		}
		else if (type == SVIEffectSpeaker.SkinType.SKIN_FUNKY){
			mCurrentSkin = SVIEffectSpeaker.SkinType.SKIN_FUNKY;
			//2013-02-14 rareboy0112@facebook.com
			//blend mode change support.
			if(mEffectView != null) mEffectView.enableAddBlend(false);
			buildLayoutForFuncky();	
		}
		else if (type == SVIEffectSpeaker.SkinType.SKIN_WAVE) {
			mCurrentSkin = SVIEffectSpeaker.SkinType.SKIN_WAVE;
			
			buildLayoutForWave();
		}
	}
		
	
	//2012-12-04 masterkeaton27@gmail.com
	//NOTE GRAPHICS: 
	//DON'T FORGET THIS!!
	@Override 
	protected void onDestroy(){
				
		if (mEffectView != null){
			mEffectView.finalize();
			mEffectView = null;
		}
		
		if (mMusicPlayer != null){
			mMusicPlayer.release();
			mMusicPlayer = null;
		}	
				
		//2013-02-01 masterkeaton27@gmail.com
		mSkinClassic.releaseBitmaps();
		mSkinFuncky.releaseBitmaps();
		mSkinWave.releaseBitmaps();
		
		//2013-02-05 rareboy0112@facebook.com
		if (mVisualizer != null) {
			mVisualizer.setEnabled(false);
			mVisualizer.release();
			mVisualizer = null;
		}
		
		//2013-02-06 rareboy0112@facebook.com
		mDbValues = null;
		
		super.onDestroy();
		
	}
	
	//2013-08-07 rareboy0112@facebook.com
	//moved start high wave position
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		mWaveHighPos.x = (int) event.getRawX();
		mWaveHighPos.y = (int) event.getRawY();
		
		return super.onTouchEvent(event);
	}


	//2013-02-01 masterkeaton27@gmail.com
	public void buildViews(AbsoluteLayout layout, boolean isPortraits){
		
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();
		}
				
		mLayout = new FrameLayout(this);		
				
		//2013-01-23 masterkeaton27@gmail.com
		mCoverView = new ImageView(this);
		
		if( mCurrentSkin == SVIEffectSpeaker.SkinType.SKIN_CLASSIC) {
			mCoverView.setImageBitmap(mSkinClassic.mBgDiffuse);
			mCoverView.setScaleType(ScaleType.FIT_XY);
			
			//2013-01-23 masterkeaton27@gmail.com
			mSplashView = new ImageView(this);
			mSplashView.setImageBitmap(mSkinClassic.mSplashBitmap);
			mSplashView.setScaleType(ScaleType.FIT_XY);
			
			mLayout.addView(mSplashView);		
		} 
		else if(mCurrentSkin == SVIEffectSpeaker.SkinType.SKIN_FUNKY) {
			//mCoverView.setImageBitmap(mSkinFuncky.mMask);
			//mCoverView.setScaleType(ScaleType.FIT_XY);
			mCoverView.setImageBitmap(null);
		}
		else if (mCurrentSkin == SVIEffectSpeaker.SkinType.SKIN_WAVE) {
			mCoverView.setImageBitmap(null);
		}
		
		mLayout.addView(createEffectView());
		mLayout.addView(mCoverView);
		
		mLayout.addView(layout);	
		setContentView(mLayout);
	}
				
	//2013-02-01 masterkeaton27@gmail.com
	private void buildClassicSkin(View layout) {
		if (mEffectView == null) return;
			
		mEffectView.clearNodes();		
		//mEffectView.enablePostEffect(false);
		mEffectView.setSkinMode(SVIEffectSpeaker.SkinType.SKIN_CLASSIC);
		
		View baseViewFromXML = (View)layout.findViewById(R.id.base_outside_port);
		View rubberOutsideViewFromXML = (View)layout.findViewById(R.id.rubber_outside_port);
		View rubberInsideViewFromXML = (View)layout.findViewById(R.id.rubber_inside_port);
		View centerViewFromXML = (View)layout.findViewById(R.id.center_circle_port);
		
		//2013-01-23 masterkeaton27@gmail.com
		//based on base GUI object's layout, let's set splash view's size
		FrameLayout.LayoutParams layoutParams = (FrameLayout.LayoutParams)mSplashView.getLayoutParams();
		layoutParams.width = baseViewFromXML.getWidth();
		layoutParams.height = baseViewFromXML.getHeight();
		layoutParams.leftMargin = baseViewFromXML.getPaddingLeft();
		layoutParams.topMargin = baseViewFromXML.getPaddingTop();		
		mSplashView.setLayoutParams(layoutParams);
		
		mEffectView.addPannel(
				baseViewFromXML.getPaddingLeft(), baseViewFromXML.getPaddingTop(), baseViewFromXML.getWidth(), baseViewFromXML.getHeight(),
				mSkinClassic.mBaseDiffuse, mSkinClassic.mBaseNormal, mSkinClassic.mBaseWeight,
				-1.0f);
				
		mEffectView.addPannel(
				rubberOutsideViewFromXML.getPaddingLeft(), rubberOutsideViewFromXML.getPaddingTop(), rubberOutsideViewFromXML.getWidth(), rubberOutsideViewFromXML.getHeight(),
				mSkinClassic.mRubberOutsideDiffuse, mSkinClassic.mRubberOutsideNormal, mSkinClassic.mRubberOutsideWeight,
				-1.0f);
				
		mEffectView.addPannel(
				rubberInsideViewFromXML.getPaddingLeft(), rubberInsideViewFromXML.getPaddingTop(), rubberInsideViewFromXML.getWidth(), rubberInsideViewFromXML.getHeight(),
				mSkinClassic.mRubberInsideDiffuse, mSkinClassic.mRubberInsideNormal, mSkinClassic.mRubberInsideWeight,
				-1.0f);
		
		mEffectView.addPannel(
				centerViewFromXML.getPaddingLeft(), centerViewFromXML.getPaddingTop(), centerViewFromXML.getWidth(), centerViewFromXML.getHeight(),
				mSkinClassic.mCenterDiffuse, mSkinClassic.mCenterNormal, mSkinClassic.mCenterWeight,
				-1.0f);
		
		mEffectView.validateOnSensorChange(new float[]{-3,3,7}, 0);
		
	}
	
	//2013-02-14 rareboy0112@facebook.com
	//Method is changed.
	private void buildFunckySkin(View layout) {
		if (mEffectView == null) return;
		
		mEffectView.clearNodes();
		
		//mEffectView.enablePostEffect(false);
		
		mEffectView.setSkinMode(SVIEffectSpeaker.SkinType.SKIN_FUNKY);
		
		//2013-02-06 rareboy0112@facebook.com
		//add fxView
		View fxView = (View)layout.findViewById(R.id.fx_size);
		View bottomBaseView = (View)layout.findViewById(R.id.bottom_base);
		View topBaseView = (View)layout.findViewById(R.id.top_base);
		View allBaseView = (View)layout.findViewById(R.id.all_base);
		
		//XML data of Mask-A
		View maskA_one_center = (View)layout.findViewById(R.id.mask_a_first_center);
		View maskA_two_center = (View)layout.findViewById(R.id.mask_a_second_center);
		View maskA_three_center = (View)layout.findViewById(R.id.mask_a_third_center);
		View maskA_four_center = (View)layout.findViewById(R.id.mask_a_fourth_center);
		
		//XML data of Mask-B
		View maskB_one_center = (View)layout.findViewById(R.id.mask_b_first_center);
		View maskB_two_center = (View)layout.findViewById(R.id.mask_b_second_center);
		View maskB_three_center = (View)layout.findViewById(R.id.mask_b_third_center);
		View maskB_four_center = (View)layout.findViewById(R.id.mask_b_fourth_center);
		
		//XML data of Mask-C
		View maskC_one_center = (View)layout.findViewById(R.id.mask_c_first_center);
		View maskC_two_center = (View)layout.findViewById(R.id.mask_c_second_center);
		View maskC_three_center = (View)layout.findViewById(R.id.mask_c_third_center);
		View maskC_four_center = (View)layout.findViewById(R.id.mask_c_fourth_center);
		
		//XML data of Mask-D
		View maskD_one_center = (View)layout.findViewById(R.id.mask_d_first_center);
		View maskD_two_center = (View)layout.findViewById(R.id.mask_d_second_center);
		View maskD_three_center = (View)layout.findViewById(R.id.mask_d_third_center);
		View maskD_four_center = (View)layout.findViewById(R.id.mask_d_fourth_center);
		
		//XML data of Mask-E
		View maskE_one_center = (View)layout.findViewById(R.id.mask_e_first_center);
		View maskE_two_center = (View)layout.findViewById(R.id.mask_e_second_center);
		View maskE_three_center = (View)layout.findViewById(R.id.mask_e_third_center);
		View maskE_four_center = (View)layout.findViewById(R.id.mask_e_fourth_center);
		
		
		//all_base
		//0.035f = #09
		float color = 0.035f;
		mEffectView.addPannel(
				allBaseView.getPaddingLeft(), allBaseView.getPaddingTop(), allBaseView.getWidth(), allBaseView.getHeight(),
				color, color, color, 1.0f);
		
		//fx_base
		mEffectView.addPannel(
				fxView.getPaddingLeft(), fxView.getPaddingTop(), fxView.getWidth(), fxView.getHeight(),
				mSkinFuncky.mBaseFX);
		
		//fx_low
		mEffectView.addPannel(
				fxView.getPaddingLeft(), fxView.getPaddingTop(), fxView.getWidth(), fxView.getHeight(),
				mSkinFuncky.mLowFX);
		
		//2013/01/31 rareboy0112@facebook.com
		//NOTE GRAPHICS:
		//must here only one call
		mEffectView.createOthersCircleFX();
		
		//fx_mid
		mEffectView.addPannel(
				fxView.getPaddingLeft(), fxView.getPaddingTop(), fxView.getWidth(), fxView.getHeight(),
				mSkinFuncky.mMidFX);
		
		//fx_high
		mEffectView.addPannel(
				fxView.getPaddingLeft(), fxView.getPaddingTop(), fxView.getWidth(), fxView.getHeight(),
				mSkinFuncky.mHighFX);
		
		//top_base
		mEffectView.addPannel(
				topBaseView.getPaddingLeft(), topBaseView.getPaddingTop(), topBaseView.getWidth(), topBaseView.getHeight(),
				mSkinFuncky.mTopBase);
		
		//fx_top
		mEffectView.addPannel(
				topBaseView.getPaddingLeft(), topBaseView.getPaddingTop(), topBaseView.getWidth(), topBaseView.getHeight(),
				mSkinFuncky.mTopFX);
		
		SVIEffectSpeaker.MaskInfo info = (mEffectView).new MaskInfo();
		
		info.mBottomWidth = bottomBaseView.getWidth();
		info.mBottomHeight = bottomBaseView.getHeight();
		info.mFxWidth = fxView.getWidth();
		info.mFxHeight = fxView.getHeight();
		info.mTopWidth = topBaseView.getWidth();
		info.mTopHeight = topBaseView.getHeight();
		info.mBottomBase = mSkinFuncky.mBottomBase;
		info.mTopBase = mSkinFuncky.mOtherTopBase;
		info.mFirstScale = 0.63f;
		info.mFirstCenter = new Point(maskA_one_center.getPaddingLeft(), maskA_one_center.getPaddingTop());
		info.mSecondScale = 0.78f;
		info.mSecondCenter = new Point(maskA_two_center.getPaddingLeft(), maskA_two_center.getPaddingTop());
		info.mThirdScale = 0.64f;
		info.mThirdCenter = new Point(maskA_three_center.getPaddingLeft(), maskA_three_center.getPaddingTop());
		info.mFourthScale = 0.30f;
		info.mFourthCenter = new Point(maskA_four_center.getPaddingLeft(), maskA_four_center.getPaddingTop());
		
		info.mOtherFirstRotate = 145.0f; 
		info.mOtherSecondRotate = 30.0f; 
		info.mOtherThirdRotate = -65.0f; 
		info.mOtherFourthRotate = 0.0f;
		
		info.mFX_First = mSkinFuncky.mOtherFX_B;
		info.mFX_Second = mSkinFuncky.mOtherFX_D;
		info.mFX_Third = mSkinFuncky.mOtherFX_A;
		info.mFX_Fourth = mSkinFuncky.mOtherFX_C;
		
		//Mask-A info
		mEffectView.setMaskInfomation(SVIEffectSpeaker.MaskType.MASK_A, info);
		
		info.mFX_First = mSkinFuncky.mOtherFX_B;
		info.mFX_Second = mSkinFuncky.mOtherFX_D;
		info.mFX_Third = mSkinFuncky.mOtherFX_A;
		info.mFX_Fourth = mSkinFuncky.mOtherFX_C;
		
		info.mFirstScale = 0.87f;
		info.mFirstCenter = new Point(maskB_one_center.getPaddingLeft(), maskB_one_center.getPaddingTop());
		info.mSecondScale = 0.52f;
		info.mSecondCenter = new Point(maskB_two_center.getPaddingLeft(), maskB_two_center.getPaddingTop());
		info.mThirdScale = 0.29f;
		info.mThirdCenter = new Point(maskB_three_center.getPaddingLeft(), maskB_three_center.getPaddingTop());
		info.mFourthScale = 0.75f;
		info.mFourthCenter = new Point(maskB_four_center.getPaddingLeft(), maskB_four_center.getPaddingTop());
		
		info.mOtherFirstRotate = -145.0f;
		info.mOtherSecondRotate = 210.0f;
		info.mOtherThirdRotate = -75.0f;
		info.mOtherFourthRotate = -25.0f;
		
		info.mFX_First = mSkinFuncky.mOtherFX_B;
		info.mFX_Second = mSkinFuncky.mOtherFX_D;
		info.mFX_Third = mSkinFuncky.mOtherFX_C;
		info.mFX_Fourth = mSkinFuncky.mOtherFX_A;
		
		//Mask-B info
		mEffectView.setMaskInfomation(SVIEffectSpeaker.MaskType.MASK_B, info);
		
		info.mFirstScale = 0.60f;
		info.mFirstCenter = new Point(maskC_one_center.getPaddingLeft(), maskC_one_center.getPaddingTop());
		info.mSecondScale = 0.39f;
		info.mSecondCenter = new Point(maskC_two_center.getPaddingLeft(), maskC_two_center.getPaddingTop());
		info.mThirdScale = 0.66f;
		info.mThirdCenter = new Point(maskC_three_center.getPaddingLeft(), maskC_three_center.getPaddingTop());
		info.mFourthScale = 1.0f;
		info.mFourthCenter = new Point(maskC_four_center.getPaddingLeft(), maskC_four_center.getPaddingTop());
		
		info.mOtherFirstRotate = -180.0f;
		info.mOtherSecondRotate = 120.0f;
		info.mOtherThirdRotate = 70.0f;
		info.mOtherFourthRotate = -35.0f;
		
		info.mFX_First = mSkinFuncky.mOtherFX_B;
		info.mFX_Second = mSkinFuncky.mOtherFX_D;
		info.mFX_Third = mSkinFuncky.mOtherFX_A;
		info.mFX_Fourth = mSkinFuncky.mOtherFX_C;
		
		//Mask-C info
		mEffectView.setMaskInfomation(SVIEffectSpeaker.MaskType.MASK_C, info);
		
		info.mFirstScale = 0.83f;
		info.mFirstCenter = new Point(maskD_one_center.getPaddingLeft(), maskD_one_center.getPaddingTop());
		info.mSecondScale = 1.00f;
		info.mSecondCenter = new Point(maskD_two_center.getPaddingLeft(), maskD_two_center.getPaddingTop());
		info.mThirdScale = 0.45f;
		info.mThirdCenter = new Point(maskD_three_center.getPaddingLeft(), maskD_three_center.getPaddingTop());
		info.mFourthScale = 0.26f;
		info.mFourthCenter = new Point(maskD_four_center.getPaddingLeft(), maskD_four_center.getPaddingTop());
		
		info.mOtherFirstRotate = -145.0f;
		info.mOtherSecondRotate = 0.0f;
		info.mOtherThirdRotate = -85.0f;
		info.mOtherFourthRotate = -30.0f;
		
		info.mFX_First = mSkinFuncky.mOtherFX_B;
		info.mFX_Second = mSkinFuncky.mOtherFX_A;
		info.mFX_Third = mSkinFuncky.mOtherFX_C;
		info.mFX_Fourth = mSkinFuncky.mOtherFX_D;
		
		//Mask-D info
		mEffectView.setMaskInfomation(SVIEffectSpeaker.MaskType.MASK_D, info);
		
		info.mFirstScale = 0.44f;
		info.mFirstCenter = new Point(maskE_one_center.getPaddingLeft(), maskE_one_center.getPaddingTop());
		info.mSecondScale = 0.21f;
		info.mSecondCenter = new Point(maskE_two_center.getPaddingLeft(), maskE_two_center.getPaddingTop());
		info.mThirdScale = 0.52f;
		info.mThirdCenter = new Point(maskE_three_center.getPaddingLeft(), maskE_three_center.getPaddingTop());
		info.mFourthScale = 0.94f;
		info.mFourthCenter = new Point(maskE_four_center.getPaddingLeft(), maskE_four_center.getPaddingTop());
		
		info.mOtherFirstRotate = 140.0f;
		info.mOtherSecondRotate = 110.0f;
		info.mOtherThirdRotate = 25.0f;
		info.mOtherFourthRotate = -25.0f;
		
		info.mFX_First = mSkinFuncky.mOtherFX_D;
		info.mFX_Second = mSkinFuncky.mOtherFX_C;
		info.mFX_Third = mSkinFuncky.mOtherFX_B;
		info.mFX_Fourth = mSkinFuncky.mOtherFX_A;
		
		//Mask-E info
		mEffectView.setMaskInfomation(SVIEffectSpeaker.MaskType.MASK_E, info);
		
		//2013-01-31 rareboy0112@facebook.com
		//NOTE GRAPHICS:
		//must here only one call
		mEffectView.createOthersCircleBase();
		
		//Mask
		DisplayMetrics dm = new DisplayMetrics(); 
		getWindowManager().getDefaultDisplay().getMetrics(dm);
		mEffectView.addPannel(0, 0, dm.widthPixels, dm.heightPixels, mSkinFuncky.mMask);
		
		//2013-02-06 rareboy0112@facebook.com
		//flash layout
		//color = #00c6ff
		float red, green, blue;
		red = 0.0f;
		green = 198.0f / 255.0f;
		blue = 1.0f;
		mEffectView.addPannel(
				allBaseView.getPaddingLeft(), allBaseView.getPaddingTop(), allBaseView.getWidth(), allBaseView.getHeight(),
				red, green, blue, 1.0f);
		
		//2013-02-06 rareboy0112@facebook.com
		//NOTE GRAPHICS:
		//must here once call
		mEffectView.setMaskType(SVIEffectSpeaker.MaskType.MASK_A, mSkinFuncky.mMask);
	}
	
	//2013-07-22 rareboy0112@facebook.com
	private void buildWaveSkin(View layout) {
		if (mEffectView == null) return;
			
		mEffectView.clearNodes();
		
		//base
		//NOTE UX:
		//width and height full screen size(sample fit Gallexy S4)
		float width = 1080.0f;
		float height = 1920.0f;
		float color = 0.0f;
		mEffectView.addPannel(0, 0, width, height, color, color, color, 1.0f);
		
		//Set Wace Texure from images (2013-08-02 minw83.kim)
		mEffectView.setWaveTextures(mSkinWave.mBitmap);
		

		//2013-08-09 jdev.nam whether particle fit to itself for rectangle not square.
		//mEffectView.enableParticleFitForRectangle();

		//must here once call
		//2013-08-07 rareboy0112@facebook.com
		//Should be called before setSkinMode method.(default count = 250)
		//@params slideCount : create slide count
		// 2013-08-08 jdev.nam
		mEffectView.setWaveMaxSlide(370);
		mEffectView.setSkinMode(SVIEffectSpeaker.SkinType.SKIN_WAVE);
		
		//2013-07-22 rareboy0112@facebook.com
		//@params resultCount : find color number
		// mFindColors = mCollectColor.setImage(R.drawable.album_jacket1, 13);
		//mFindColors = mCollectColor.setImage(R.drawable.test, 13);
		//mFindColors = mCollectColor.setImage(null,	13);
		//mFindColors = mCollectColor.setImage(R.drawable.default_sample_color_new200x200, 13);
		
		
		Bitmap bitmap;
		InputStream is;
		is = this.getResources().openRawResource(R.drawable.default_sample_color_new200x200);
		bitmap = BitmapFactory.decodeStream(is);
		
		Log.d("teja.kim", "start_make albumjacket array");
		mFindColors = mCollectColor.setImage(bitmap, 13	);
		Log.d("teja.kim", "end_make albumjacket array");
		
		//must call after change mode.
		mEffectView.setWaveColors(mFindColors.clone());
	}
		
	//2013-02-01 masterkeaton27@gmail.com
	public void buildLayoutForClassic(){
		
		mSkinClassic.loadBgImages(getResources());
		mSkinClassic.loadSpeakerImages(getResources());
				
		//get layout view from xml
		mLayoutClassic = (AbsoluteLayout)buildViews_Classic(this);
		
		//build legacy views & inflate all.
		buildViews(mLayoutClassic, true);
		
		//get GUI information from layout
		buildViewBasedOnXML_Classic();
	}
	
	//2013-02-01 masterkeaton27@gmail.com
	public void buildLayoutForFuncky(){
		
		mSkinFuncky.loadBgImages(getResources());
		mSkinFuncky.loadSpeakerImages(getResources());
				
		//get layout view from xml
		mLayoutFuncky = (AbsoluteLayout)buildViews_Funcky(this);
		
		//build legacy views & inflate all.
		buildViews(mLayoutFuncky, false);
		
		//get GUI information from layout
		buildViewBasedOnXML_Funcky();
	}			
	
	//2013-07-22 rareboy0112@facebook.com
	public void buildLayoutForWave() {
		
		mSkinWave.loadBgImages(getResources());
		mSkinWave.loadSpeakerImages(getResources());
				
		//get layout view from xml
		mLayoutWave = (AbsoluteLayout)buildViews_Wave(this);
		
		//build legacy views & inflate all.
		buildViews(mLayoutWave, false);
		
		//get GUI information from layout
		buildViewBasedOnXML_Wave();
	}
	
	//2013-01-23 masterkeaton27@gmail.com
	//function call (parameter) changed.
	private OnGlobalLayoutListener mListenerClassic = new OnGlobalLayoutListener(){
	  	public void onGlobalLayout() {		  		
	  		buildClassicSkin(mLayoutClassic);	  		
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerClassic);
	  		mLayout.removeView(mLayoutClassic);
	  	}
	};
	
	
	//2013-01-23 masterkeaton27@gmail.com
	//function call (parameter) changed.
	private OnGlobalLayoutListener mListenerFuncky = new OnGlobalLayoutListener(){
	  	public void onGlobalLayout() {		  		
	  		buildFunckySkin(mLayoutFuncky); 
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerFuncky);
	  		mLayout.removeView(mLayoutFuncky);
	  	}
	};
	
	//2013-07-22 rareboy0112@facebook.com
	//function call (parameter) changed.
	private OnGlobalLayoutListener mListenerWave = new OnGlobalLayoutListener(){
	  	public void onGlobalLayout() {
	  		buildWaveSkin(mLayoutWave);
	  		ViewTreeObserver vto = mLayout.getViewTreeObserver();
	  		vto.removeGlobalOnLayoutListener(mListenerWave);
	  		mLayout.removeView(mLayoutWave);
	  	}
	};
	
	
	//2012-12-21 masterkeaton27@gmail.com
	public void buildViewBasedOnXML_Classic(){		
		if(mLayoutClassic == null)
	  		return;
		
	  	//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: need Calculate layout first Time.
		ViewTreeObserver vto = mLayout.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(mListenerClassic);
	}
	
	//2012-12-21 masterkeaton27@gmail.com
	public void buildViewBasedOnXML_Funcky(){		
	  	if(mLayoutFuncky == null)
	  		return;
	  	    
	  	//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: need Calculate layout first Time.
		ViewTreeObserver vto = mLayout.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(mListenerFuncky);	    	    
	}
	
	//2013-07-22 rareboy0112@facebook.com
	public void buildViewBasedOnXML_Wave(){		
	  	if(mLayoutWave == null)
	  		return;
	  	    
	  	//2012-12-12 phyuzion@gmail.com
		//NOTE GRAPHICS: need Calculate layout first Time.
		ViewTreeObserver vto = mLayout.getViewTreeObserver();
		vto.addOnGlobalLayoutListener(mListenerWave);	    	    
	}
	
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		if (mode == 1){
			mSensorManager.unregisterListener(this, gravitymeter);
		}
	    //2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mEffectView != null )
	    	mEffectView.onPause();
	   	    
	    if (mMusicPlayer != null && mUseSound)
	    	mMusicPlayer.pause();
	}

	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	
		
		if (mode == 1){
			mSensorManager.registerListener(this, gravitymeter, SensorManager.SENSOR_DELAY_UI);
		}
		
		//2012-12-04 masterkeaton27@gmail.com
	  	//NOTE GRAPHICS: 
	    //DON'T FORGET THIS!!!!
	    if( mEffectView != null )
	    	mEffectView.onResume();
	    	   
	    
	    if (mMusicPlayer != null && mUseSound)
	    	mMusicPlayer.start();
	    
	    //2012-02-14 rareboy0112@facebook.com
	    //time reset
	    mChangedTime = 0;
	}
	
	public SVIEffectSpeaker createEffectView() {
		if (mEffectView == null){
			mEffectView = new SVIEffectSpeaker(this);		
		}else{
			mEffectView.destroy();
		}		
		
		mEffectView.setListener(this);
		mEffectView.initialize();
		mEffectView.setZOrderOnTop(false);
				
		return mEffectView;
	}
	
	@Override
	public void onBackPressed(){
		super.onBackPressed();
		if (mLayout != null){
			mLayout.removeAllViews();
			mLayout.removeAllViewsInLayout();
		}
		
		if (mMusicPlayer != null && mUseSound)
	    	mMusicPlayer.stop();
		
		if (mEffectView != null) mEffectView.destroy();
	}
		
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {				
		menu.add("MODE Funcky");
		menu.add("MODE Classic");
		menu.add("MODE Wave");
		//menu.add("HDR ON");
		//menu.add("HDR OFF");

		/*SubMenu subMenu = menu.addSubMenu("FPS");
		subMenu.add(0, 101, 0, "5-FPS");
		subMenu.add(0, 102, 0, "10-FPS");
		subMenu.add(0, 103, 0, "15-FPS");
		subMenu.add(0, 104, 0, "20-FPS");
		subMenu.add(0, 105, 0, "25-FPS");
		subMenu.add(0, 106, 0, "30-FPS");
		subMenu.add(0, 107, 0, "35-FPS");
		subMenu.add(0, 108, 0, "40-FPS");
		subMenu.add(0, 109, 0, "45-FPS");
		subMenu.add(0, 110, 0, "50-FPS");
		subMenu.add(0, 111, 0, "55-FPS");
		subMenu.add(0, 112, 0, "60-FPS");*/
		
		/*
		subMenu = menu.addSubMenu("Animation Speed");
		subMenu.add(0, 201, 0, "x0.1");
		subMenu.add(0, 202, 0, "x0.2");
		subMenu.add(0, 203, 0, "x0.3");
		subMenu.add(0, 204, 0, "x0.4");
		subMenu.add(0, 205, 0, "x0.5");
		subMenu.add(0, 206, 0, "x0.6");
		subMenu.add(0, 207, 0, "x0.7");
		subMenu.add(0, 208, 0, "x0.8");
		subMenu.add(0, 209, 0, "x0.9");
		subMenu.add(0, 210, 0, "x1.0");
		subMenu.add(0, 211, 0, "x2.0");
		subMenu.add(0, 212, 0, "x3.0");
		subMenu.add(0, 213, 0, "x4.0");
		subMenu.add(0, 214, 0, "x5.0");
		subMenu.add(0, 215, 0, "x6.0");
		subMenu.add(0, 216, 0, "x7.0");
		subMenu.add(0, 217, 0, "x8.0");
		subMenu.add(0, 218, 0, "x9.0");
		subMenu.add(0, 219, 0, "x10.0");
		*/
		return true;
	}
	
		
	int mode = 0;
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		if (item.getTitle() == "MODE Funcky"){
			mode = 0;
			mSensorManager.unregisterListener(this, gravitymeter);
			setSkinMode(SVIEffectSpeaker.SkinType.SKIN_FUNKY);
		} else if (item.getTitle() == "MODE Classic"){
			mode = 1;
			mSensorManager.registerListener(this, gravitymeter, SensorManager.SENSOR_DELAY_UI);
			setSkinMode(SVIEffectSpeaker.SkinType.SKIN_CLASSIC);
		} else if (item.getTitle() == "MODE Wave") {
			mode = 1;
			mSensorManager.unregisterListener(this, gravitymeter);
			setSkinMode(SVIEffectSpeaker.SkinType.SKIN_WAVE);
		}
		
		
		/*else if (item.getTitle() == "HDR ON"){
			mEffectView.enablePostEffect(false);
		} else if (item.getTitle() == "HDR OFF"){
			mEffectView.enablePostEffect(false);
		}*/
		
		/*boolean bFPS = false;
		boolean bAnimationDuration = false;

		switch (item.getItemId()) {
		case 101: mEffectView.setRenderFPS(5); bFPS = true; break;
		case 102: mEffectView.setRenderFPS(10); bFPS = true; break;
		case 103: mEffectView.setRenderFPS(15); bFPS = true; break;
		case 104: mEffectView.setRenderFPS(20); bFPS = true; break;
		case 105: mEffectView.setRenderFPS(25); bFPS = true; break;
		case 106: mEffectView.setRenderFPS(30); bFPS = true; break;
		case 107: mEffectView.setRenderFPS(35); bFPS = true; break;
		case 108: mEffectView.setRenderFPS(40); bFPS = true; break;
		case 109: mEffectView.setRenderFPS(45); bFPS = true; break;
		case 110: mEffectView.setRenderFPS(50); bFPS = true; break;
		case 111: mEffectView.setRenderFPS(55); bFPS = true; break;
		case 112: mEffectView.setRenderFPS(60); bFPS = true; break;
		

		
		case 201: mEffectView.setAnimationSpeed(0.1f); bAnimationDuration = true; break;
		case 202: mEffectView.setAnimationSpeed(0.2f); bAnimationDuration = true; break;
		case 203: mEffectView.setAnimationSpeed(0.3f); bAnimationDuration = true; break;
		case 204: mEffectView.setAnimationSpeed(0.4f); bAnimationDuration = true; break;
		case 205: mEffectView.setAnimationSpeed(0.5f); bAnimationDuration = true; break;
		case 206: mEffectView.setAnimationSpeed(0.6f); bAnimationDuration = true; break;
		case 207: mEffectView.setAnimationSpeed(0.7f); bAnimationDuration = true; break;
		case 208: mEffectView.setAnimationSpeed(0.8f); bAnimationDuration = true; break;
		case 209: mEffectView.setAnimationSpeed(0.9f); bAnimationDuration = true; break;
		case 210: mEffectView.setAnimationSpeed(1.0f); bAnimationDuration = true; break;
		case 211: mEffectView.setAnimationSpeed(2.0f); bAnimationDuration = true; break;
		case 212: mEffectView.setAnimationSpeed(3.0f); bAnimationDuration = true; break;
		case 213: mEffectView.setAnimationSpeed(4.0f); bAnimationDuration = true; break;
		case 214: mEffectView.setAnimationSpeed(5.0f); bAnimationDuration = true; break;
		case 215: mEffectView.setAnimationSpeed(6.0f); bAnimationDuration = true; break;
		case 216: mEffectView.setAnimationSpeed(7.0f); bAnimationDuration = true; break;
		case 217: mEffectView.setAnimationSpeed(8.0f); bAnimationDuration = true; break;
		case 218: mEffectView.setAnimationSpeed(9.0f); bAnimationDuration = true; break;
		case 219: mEffectView.setAnimationSpeed(10.0f); bAnimationDuration = true; break;
		
		}

		if (bFPS) {
			Toast.makeText(this, item.getTitle(), Toast.LENGTH_SHORT).show();
		}
		else if (bAnimationDuration) {
			Toast.makeText(this, "Animation Speed: " + item.getTitle(), Toast.LENGTH_SHORT).show();
		}*/
		
		return true;
	}

	
	//2013-01-23 masterkeaton27@gmail.com
	//remove splash view after first rendering finished.
	private Handler mRemoveSplashHandler = new Handler(){
		@Override
		public void handleMessage(Message message) {
			if (mSplashView != null) mLayout.removeView(mSplashView);
		}
	};
	
	public void onFirstRenderFinished() {
		//2013-01-23 masterkeaton27@gmail.com
		//after get in this function, it's under control of GL Thread.
		//so if we want remove splash view, should send a remove message to main thread.
		mRemoveSplashHandler.sendEmptyMessage(0);
	}

	public void onAnimateHideFinished() {
		// TODO Auto-generated method stub
		
	}

	public void onAnimateShowFinished() {
		// TODO Auto-generated method stub
		
	}

	public void onAccuracyChanged(Sensor arg0, int arg1) {
		// TODO Auto-generated method stub
		
	}

	public void onSensorChanged(SensorEvent event) {		
		
		boolean isGravityChanged = false;
			
		if (event.sensor.getType() == Sensor.TYPE_GRAVITY) {	       	        
	        float fDistance = 
					(event.values[0] - mGravity[0]) * (event.values[0] - mGravity[0]) +
					(event.values[1] - mGravity[1]) * (event.values[1] - mGravity[1]) +
					(event.values[2] - mGravity[2]) * (event.values[2] - mGravity[2]);
			
			if (Math.sqrt(fDistance) > 0.5f){
				mGravity[0] = event.values[0];
				mGravity[1] = event.values[1];
				mGravity[2] = event.values[2];
				
				isGravityChanged = true;
				
			}			
	    }			    
		
		if (isGravityChanged){		
						
			if( mEffectView != null ){				
											
				float initialx = 0.0f /*+ vectorx*/ - mGravity[0] + 2;
				float initialy = 0.0f /*+ vectory*/ + mGravity[1];
				float initialz = 0.0f /*+ vectorz*/ + mGravity[2];
				
				//2012-12-04 masterkeaton27@gmail.com
			  	//NOTE GRAPHICS: 
			    //send gravity sensor data to sgi effect view for calculating lights and so on.				
				
				mEffectView.validateOnSensorChange(new float[]{initialx, initialy, initialz, 0.0f, 0.0f , 0.0f}, 0);	
			}
		}
	    
	}


}
