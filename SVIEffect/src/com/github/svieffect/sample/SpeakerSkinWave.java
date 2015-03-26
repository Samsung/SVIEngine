package com.github.svieffect.sample;

import java.io.InputStream;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.github.svieffect.R;

public class SpeakerSkinWave extends SpeakerSkin{
	
	public Bitmap[]	mBitmap = null;
	
	@Override
	public void loadBgImages(Resources res) {
		super.loadSpeakerImages(res);
		InputStream is = null;
		int testImage[] = {
				R.drawable.particle_png_000,
				R.drawable.particle_png_001,
				R.drawable.particle_png_002,
				R.drawable.particle_png_003,
				R.drawable.particle_png_004,
				R.drawable.particle_png_005,
				R.drawable.particle_png_006,
				R.drawable.particle_png_007,
				R.drawable.particle_png_008,
				R.drawable.particle_png_009
		};
		mBitmap = new Bitmap[testImage.length];
		for(int i=0; i<testImage.length; i++) {
			is = res.openRawResource(testImage[i]);			
			mBitmap[i] = BitmapFactory.decodeStream(is);
		}		
	}
	
	@Override
	public void loadSpeakerImages(Resources res) {
		super.loadSpeakerImages(res);
	}


	@Override
	public void releaseBitmaps() {
		super.releaseBitmaps();
		if (mBitmap != null) {
			for(int i = 0; i < mBitmap.length; i++) {
				releaseBitmap(mBitmap[i]);
			}
			mBitmap = null;
		}
	}



	@Override
	public void finalize() {
		releaseBitmaps();
	}
	
}
