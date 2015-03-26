package com.github.svieffect.sample;

import android.content.res.Resources;
import android.graphics.Bitmap;


public class SpeakerSkin {
	public void loadSpeakerImages(Resources aRes) {
		
	}
	
	public void loadBgImages(Resources aRes) {
		
	}
	
	public void releaseBitmaps() {
		
	}
	
	protected void releaseBitmap(Bitmap aBitmap){
		if (aBitmap != null){
			aBitmap.recycle();
			aBitmap = null;
		}
	}
}
