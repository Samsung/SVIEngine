package com.github.sviengine.performance;


import com.github.sviengine.slide.SVISlide;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;


abstract public class TestCaseBaseActivity extends Activity {

	abstract boolean createWindow();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		if( createWindow() ) {
			setContentView(mWindow);
			boolean translucent = true;
			int depth = 16;
			int stencil = 8;
			mWindow.initialize(translucent, depth, stencil);
		}
		else
			Log.e("SVI UnitTest", "TestBed Window is not created!!!");
		
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		if( mWindow != null ) {
			int rotation = getWindowManager().getDefaultDisplay().getRotation();
			mWindow.onConfigurationChanged(rotation);
		}
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		if(mWindow!=null)
			mWindow.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if( mWindow != null )
			mWindow.onResume();
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		Log.e("SVI", "Activity Destroyed");
		SVISlide.nativeSetRootSlideSVISlide(0);
		super.onDestroy();
	}
	




	protected PlatformWindow			mWindow = null;
}
