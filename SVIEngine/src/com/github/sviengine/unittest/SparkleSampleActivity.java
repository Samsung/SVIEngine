package com.github.sviengine.unittest;

import android.content.res.Configuration;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class SparkleSampleActivity extends UnitTestBaseActivity{
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
	protected void onDestroy() {
		
		
		mOriginWindow.release();
		mOriginWindow = null;
		super.onDestroy();
	}
	
	
	@Override
	public void onStop(){
		super.onStop();
		((SparkleSampleWindow)mWindow).onStop();
	}
	
	
	@Override
	public boolean createWindow() {
		// TODO Auto-generated method stub
		mWindow = new SparkleSampleWindow(this);
		mOriginWindow = (SparkleSampleWindow)mWindow;
		if( mWindow == null )
			return false;
		
		return true;
	}
	
	@Override
	public boolean onCreateOptionsMenu (Menu menu) {
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected (MenuItem item) {
		mWindow.onOptionsItemSelected(item);
		return true;
	}

	
	
	SparkleSampleWindow mOriginWindow = null;
}
