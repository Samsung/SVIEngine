package com.github.sviengine.unittest;

import android.content.res.Configuration;
import android.os.Bundle;

public class JacketViewTestActivity extends UnitTestBaseActivity {
	

	@Override
	public void onCreate(Bundle savedInstanceState) {
		
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
	protected void onDestroy() {
		originWindow.release();
		originWindow = null;
		super.onDestroy();
	}


	@Override
	public boolean createWindow() {
		
		mWindow = new JacketViewTestWindow(this);
		
		originWindow = (JacketViewTestWindow)mWindow;
		
		if(mWindow == null)
			return false;
		
		return true;
	}
	
	JacketViewTestWindow originWindow = null;
}