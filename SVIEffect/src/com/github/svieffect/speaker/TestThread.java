package com.github.svieffect.speaker;

import com.github.sviengine.basetype.SVIPoint;

public class TestThread extends Thread {

	public interface TestListener {
		public void onUpdate(SVIPoint test[]);
	};
	
	
	private TestListener mHandler;
	
	private SVIPoint test[];
	
	public TestThread(TestListener listener) {
		mHandler = listener;
		
		
		test = new SVIPoint[10];
		for (int i = 0; i < 10; ++i) {
			test[i] = new SVIPoint();
		}
	}
	
	@Override
	public void run(){
		while (!isInterrupted()) {
			mHandler.onUpdate(test);
		}
	}
	
}
