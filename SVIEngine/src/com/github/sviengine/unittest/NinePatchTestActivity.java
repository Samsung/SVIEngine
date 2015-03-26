package com.github.sviengine.unittest;

import android.content.res.Configuration;
import android.os.Bundle;

public class NinePatchTestActivity extends UnitTestBaseActivity{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}

	@Override
	protected void onPause() {
		mWindow.onPause();
		super.onPause();
	}

	@Override
	protected void onResume() {
		mWindow.onResume();
		super.onResume();
	}

	
	@Override
	protected void onDestroy() {
		mWindow.release();
		mWindow = null;
		super.onDestroy();
	}
	
	
	@Override
	public boolean createWindow() {
		mWindow = new NinePatchTestWindow(this);		
		if( mWindow == null )
			return false;
		
		return true;
	}
}