package com.github.svieffect.speaker;

import java.util.ArrayList;


public class MathWaveThread extends Thread{
	float mMaxRadius = 1080.0f + 1920.0f;
	
	//Maximum Range of Radius
	public void setMaxRangeOfRadius(float size) {
		mMaxRadius = size;
	}
	
	public class Wave{
		
		public Wave(float effectiveRadius, float radius, float x, float y, float speed){
			mCurrentRadius = radius;
			mCenterX = x;
			mCenterY = y;
			mEffectiveRadius = effectiveRadius;
			if(mEffectiveRadius <=0) mEffectiveRadius=1.0f;
			mSpeed = speed;
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
				
		
		private float mCurrentRadius;
		
		private float mCenterX;
		private float mCenterY;
		private float mEffectiveRadius;
		private float mSpeed;

		private float mOuterDist;
		private float mInnerDist;
		
		
	}
	
	public interface MathWaveListener{	
		public void onDataChanged(ArrayList<Wave> waves);
	};	

	
	private MathWaveListener mListener;

	public ArrayList<Wave> mWaves = new ArrayList<Wave>();
	private int mSkipIndex=0;
	
	public void setListener(MathWaveListener listener){
		mListener = listener;
	}
	
	public MathWaveThread() {

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
	
	public void addWave(float effectiveRadius, float radius, float x, float y, float speed){
		Wave wave = new Wave(effectiveRadius,radius,x,y,speed);		
		mWaves.add(wave);
	}
	
	@SuppressWarnings("unchecked")
	public void run(){
		while (!isInterrupted()) {
			updateWaves();
			mListener.onDataChanged((ArrayList<Wave>)mWaves.clone());
		}
	}
	
	
	
}