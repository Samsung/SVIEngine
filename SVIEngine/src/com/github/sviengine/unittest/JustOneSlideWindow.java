package com.github.sviengine.unittest;

import android.content.Context;
import android.view.SurfaceHolder;

public class JustOneSlideWindow extends PlatformWindow {

	public JustOneSlideWindow(Context context) {
		super(context, false);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		// TODO Auto-generated method stub
		super.surfaceChanged(holder, format, w, h);
	}
	
	@Override
	public void onResize(int width, int height) {
		// TODO Auto-generated method stub
		
		if( getMainSlide() == null )
			buildSubSlide();
		
		super.onResize(width, height);
	}
	
	protected void buildSubSlide(){
		float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		super.buildSubSlide(color);
	}
}
