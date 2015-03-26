package com.github.svieffect.sample;

import java.io.InputStream;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.github.svieffect.R;

public class SpeakerSkinClassic extends SpeakerSkin{
public Bitmap	mBgDiffuse = null;
	
	public Bitmap	mBaseDiffuse = null;
	public Bitmap	mBaseNormal = null;
	public Bitmap	mBaseWeight = null;
	
	public Bitmap	mCenterDiffuse = null;
	public Bitmap	mCenterNormal = null;
	public Bitmap	mCenterWeight = null;
	
	public Bitmap	mRubberInsideDiffuse = null;
	public Bitmap	mRubberInsideNormal = null;
	public Bitmap	mRubberInsideWeight = null;
	
	public Bitmap	mRubberOutsideDiffuse = null;
	public Bitmap	mRubberOutsideNormal = null;
	public Bitmap	mRubberOutsideWeight = null;
	
	public Bitmap   mSplashBitmap = null;
		
	@Override
	public void loadBgImages(Resources res) {
		if (mBgDiffuse != null) return;
		
		super.loadSpeakerImages(res);
		InputStream is;
		is = res.openRawResource(R.drawable.speaker_bg_diffuse);
		mBgDiffuse = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.speaker_splash);
		mSplashBitmap = BitmapFactory.decodeStream(is);
	}
	
	@Override
	public void loadSpeakerImages(Resources res) {
		
		if (mBaseDiffuse != null) return;
		
		super.loadSpeakerImages(res);
		
		InputStream is;
						
		is = res.openRawResource(R.drawable.speaker_base_diffuse);
		mBaseDiffuse = BitmapFactory.decodeStream(is);		
		is = res.openRawResource(R.drawable.speaker_base_normal);
		mBaseNormal = BitmapFactory.decodeStream(is);		
		is = res.openRawResource(R.drawable.speaker_base_weight);
		mBaseWeight = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.speaker_center_diffuse);
		mCenterDiffuse = BitmapFactory.decodeStream(is);		
		is = res.openRawResource(R.drawable.speaker_center_normal);
		mCenterNormal = BitmapFactory.decodeStream(is);		
		is = res.openRawResource(R.drawable.speaker_center_weight);
		mCenterWeight = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.speaker_rubber_outside_diffuse);
		mRubberOutsideDiffuse = BitmapFactory.decodeStream(is);			
		is = res.openRawResource(R.drawable.speaker_rubber_outside_normal);
		mRubberOutsideNormal = BitmapFactory.decodeStream(is);		
		is = res.openRawResource(R.drawable.speaker_rubber_outside_weight);
		mRubberOutsideWeight = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.speaker_rubber_inside_diffuse);
		mRubberInsideDiffuse = BitmapFactory.decodeStream(is);			
		is = res.openRawResource(R.drawable.speaker_rubber_inside_normal);
		mRubberInsideNormal = BitmapFactory.decodeStream(is);		
		is = res.openRawResource(R.drawable.speaker_rubber_inside_weight);
		mRubberInsideWeight = BitmapFactory.decodeStream(is);
	}



	@Override
	public void releaseBitmaps() {
		super.releaseBitmaps();
		
		releaseBitmap(mSplashBitmap);
		releaseBitmap(mBgDiffuse);
		
		releaseBitmap(mBaseDiffuse);
		releaseBitmap(mBaseNormal);
		releaseBitmap(mBaseWeight);
		releaseBitmap(mCenterDiffuse);
		releaseBitmap(mCenterNormal);
		releaseBitmap(mCenterWeight);
		releaseBitmap(mRubberOutsideDiffuse);
		releaseBitmap(mRubberOutsideNormal);
		releaseBitmap(mRubberOutsideWeight);
		releaseBitmap(mRubberInsideDiffuse);
		releaseBitmap(mRubberInsideNormal);
		releaseBitmap(mRubberInsideWeight);
	}



	@Override
	public void finalize() {
		releaseBitmaps();
	}
	
}
