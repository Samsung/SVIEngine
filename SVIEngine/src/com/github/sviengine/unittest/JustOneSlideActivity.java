package com.github.sviengine.unittest;

import android.content.res.Configuration;
import android.os.Bundle;

public class JustOneSlideActivity extends UnitTestBaseActivity{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
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
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}

	@Override
	public boolean createWindow() {
		// TODO Auto-generated method stub
		mWindow = new JustOneSlideWindow(this);
		
		if( mWindow == null )
			return false;
		
		return true;
	}

}
