package com.github.svieffect.sample;

import java.io.InputStream;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.github.svieffect.R;
import com.github.svieffect.speaker.SVIEffectSpeaker;

public class SpeakerSkinFuncky extends SpeakerSkin{
	public Bitmap	mBottomBase = null;
	
	public Bitmap	mTopFX = null;
	public Bitmap	mHighFX = null;
	public Bitmap	mMidFX = null;
	public Bitmap	mLowFX = null;
	public Bitmap	mBaseFX = null;
	
	public Bitmap	mTopBase = null;
	public Bitmap	mAllBase = null;
	
	public Bitmap	mMaskA = null;
	public Bitmap	mMaskB = null;
	public Bitmap	mMaskC = null;
	public Bitmap	mMaskD = null;
	public Bitmap	mMaskE = null;
	
	//2013-02-26 rareboy0112@facebook.com
	//add bitmap resources
	public Bitmap 	mOtherFX_A = null;
	public Bitmap 	mOtherFX_B = null;
	public Bitmap	mOtherFX_C = null;
	public Bitmap	mOtherFX_D = null;
	public Bitmap	mOtherFX_Top = null;
	public Bitmap	mOtherTopBase = null;
	
	//2013/02/04 rareboy0112@facebook.com
	//don't recycle mMask
	public Bitmap 	mMask = null;
	
	private int 	mCurrentMaskType = SVIEffectSpeaker.MaskType.MASK_A;
	
	
	@Override
	public void loadBgImages(Resources res) {
		super.loadSpeakerImages(res);
		InputStream is = null;
		
		if(mMaskA == null) {
			is = res.openRawResource(R.drawable.mask_a);
			mMaskA = BitmapFactory.decodeStream(is);
		}
		
		if(mMaskB == null) {
			is = res.openRawResource(R.drawable.mask_b);
			mMaskB = BitmapFactory.decodeStream(is);
		}
		
		if(mMaskC == null) {
			is = res.openRawResource(R.drawable.mask_c);
			mMaskC = BitmapFactory.decodeStream(is);
		}
		
		if(mMaskD == null) {
			is = res.openRawResource(R.drawable.mask_d);
			mMaskD = BitmapFactory.decodeStream(is);
		}
		
		if(mMaskE == null) {
			is = res.openRawResource(R.drawable.mask_e);
			mMaskE = BitmapFactory.decodeStream(is);
		}
		
		mMask = mMaskA;
	}
	
	//2013/02/04 rareboy0112@facebook.com
	//change mask image
	public void changeBgImage(Resources res, int maskType) {
		if(mCurrentMaskType != maskType && maskType < SVIEffectSpeaker.MaskType.MASK_COUNT) {
			
			switch (maskType) {
			case SVIEffectSpeaker.MaskType.MASK_A:
				mMask = mMaskA;
				break;
			case SVIEffectSpeaker.MaskType.MASK_B:
				mMask = mMaskB;
				break;
			case SVIEffectSpeaker.MaskType.MASK_C:
				mMask = mMaskC;
				break;
			case SVIEffectSpeaker.MaskType.MASK_D:
				mMask = mMaskD;
				break;
			case SVIEffectSpeaker.MaskType.MASK_E:
				mMask = mMaskE;
				break;
			}
			
			mCurrentMaskType = maskType;
		}
	}
	
	@Override
	public void loadSpeakerImages(Resources res) {
		super.loadSpeakerImages(res);
		
		if (mBottomBase != null) return;
		
		InputStream is;
				
		is = res.openRawResource(R.drawable.bottom_base);
		mBottomBase = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_top);
		mTopFX = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_high);
		mHighFX = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_mid);
		mMidFX = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_low);
		mLowFX = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_base);
		mBaseFX = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.top_base);
		mTopBase = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_s_a);
		mOtherFX_A = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_s_b);
		mOtherFX_B = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_s_c);
		mOtherFX_C = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_s_d);
		mOtherFX_D = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.fx_s_top);
		mOtherFX_Top = BitmapFactory.decodeStream(is);
		
		is = res.openRawResource(R.drawable.top_base);
		mOtherTopBase = BitmapFactory.decodeStream(is);
	}


	@Override
	public void releaseBitmaps() {
		super.releaseBitmaps();
		
		releaseBitmap(mBottomBase);
		releaseBitmap(mTopFX);
		releaseBitmap(mHighFX);
		releaseBitmap(mMidFX);
		releaseBitmap(mLowFX);
		releaseBitmap(mBaseFX);
		
		releaseBitmap(mTopBase);
		releaseBitmap(mAllBase);
		
		mMask = null;
		releaseBitmap(mMaskA);
		releaseBitmap(mMaskB);
		releaseBitmap(mMaskC);
		releaseBitmap(mMaskD);
		releaseBitmap(mMaskE);
		
		releaseBitmap(mOtherFX_A);
		releaseBitmap(mOtherFX_B);
		releaseBitmap(mOtherFX_C);
		releaseBitmap(mOtherFX_D);
		releaseBitmap(mOtherFX_Top);
		releaseBitmap(mOtherTopBase);
	}



	@Override
	public void finalize() {
		releaseBitmaps();
	}
	
}
