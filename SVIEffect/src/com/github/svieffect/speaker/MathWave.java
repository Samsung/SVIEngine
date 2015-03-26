package com.github.svieffect.speaker;

import java.util.ArrayList;

import android.os.Handler;
import android.os.Message;


public class MathWave {
	
	private static final int MSG_UPDATE = 1;
	
	//Maximum Range of Radius
	public void setMaxRangeOfRadius(float size) {
		mMaxRadius = size;
	}
	
	public void setMaxFps(int fps) {
		mDelayedTime = 1000 / fps;
	}
	
	public void start() {
		boolean  fff = true;
		mUpdateHandler.sendEmptyMessage(MSG_UPDATE);
	}
	
	public void stop() {
		mUpdateHandler.removeMessages(MSG_UPDATE);
	}
	
	public class Wave{
	
		public Wave(float effectiveRadius, float radius, float x, float y, float speed, boolean isHigh){
			mCurrentRadius = radius;
			mCenterX = x;
			mCenterY = y;
			mEffectiveRadius = effectiveRadius;
			if(mEffectiveRadius <=0) mEffectiveRadius=1.0f;
			mSpeed = speed;
			mIsHigh = isHigh;
		}
		
		public Wave(Wave wave) {
			mCurrentRadius = wave.mCurrentRadius;
			mCenterX = wave.mCenterX;
			mCenterY = wave.mCenterY;
			mSpeed = wave.mSpeed;
			mEffectiveRadius = wave.mEffectiveRadius;
			mIsHigh = wave.mIsHigh;
		}
		
		public boolean update(){
			// Radious plus =  for wave animation
			mCurrentRadius += mSpeed;				
			if (mMaxRadius < mCurrentRadius) return true;
			
			// Range for Wave size
			mOuterDist = (float) Math.pow((mCurrentRadius + mEffectiveRadius * 0.5f),2);
			mInnerDist = (float) Math.pow((mCurrentRadius - mEffectiveRadius * 0.5f),2);
			return false;
		}
		
		
		public float getScale(float x, float y){
			// Calculate Radius = pitagoras theory
			float distX = x - mCenterX;
			float distY = y - mCenterY;
			float distXY=(distX * distX + distY*distY);
			if (distXY > mOuterDist) return 0.0f;
			if (distXY < mInnerDist) return 0.0f;
			
			float distance = (float)Math.sqrt(distXY);					
			//Calculate curve effect using Math.Sin fuction
			float sinvalue = (distance - (mCurrentRadius - mEffectiveRadius * 0.5f)) / mEffectiveRadius;
			return (float)Math.sin(sinvalue*3.141592f);
		}
		
		public boolean isHigh() {
			return mIsHigh;
		}
				
		
		private float mCurrentRadius;
		
		private float mCenterX;
		private float mCenterY;
		private float mEffectiveRadius;
		private float mSpeed;

		private float mOuterDist;
		private float mInnerDist;
		
		private boolean mIsHigh;
		
	}
	
	public interface MathWaveListener{	
		public void onDataChanged(ArrayList<Wave> waves);
	};	

	
	private MathWaveListener mListener;

	public ArrayList<Wave> mWaves = new ArrayList<Wave>();
	
	public void setListener(MathWaveListener listener){
		mListener = listener;
	}
	
	public MathWave() {
		mUpdateHandler = new Handler() {
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case 1 :
					updateWaves();
					
					mListener.onDataChanged(mWaves);
					
					sendEmptyMessageDelayed(MSG_UPDATE, mDelayedTime);
				}
			}
		};
	}
	
	public void updateWaves(){
		for (int n = mWaves.size()-1; n >= 0 ; n--){
			Wave wave = mWaves.get(n);
			if (wave.update()){
				mWaves.remove(n);
				wave = null;
			}
		}
	}
	
	public void addWave(float effectiveRadius, float radius, float x, float y, float speed, boolean isHigh){
		Wave wave = new Wave(effectiveRadius,radius,x,y,speed, isHigh);		
		mWaves.add(wave);
	}
	
	
	//default maximum range of radius(H screen size)
	private float mMaxRadius = 1080.0f + 1920.0f;
	//default fps = 18
	private int mDelayedTime = 1000 / 18;
	private Handler mUpdateHandler;
	
	
}