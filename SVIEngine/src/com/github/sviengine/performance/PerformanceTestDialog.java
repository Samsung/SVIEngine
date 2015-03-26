package com.github.sviengine.performance;

import android.app.AlertDialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

import com.github.sviengine.R;

public class PerformanceTestDialog extends AlertDialog implements OnCheckedChangeListener, android.widget.CompoundButton.OnCheckedChangeListener{
	
	public PerformanceTestDialog(Context context, int theme) {
		super(context, theme);
		// TODO Auto-generated constructor stub
	}

	public PerformanceTestDialog(Context context, int theme, OnClickListener callback) {
        super(context, theme);
		// TODO Auto-generated constructor stub
		
    	LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    	View layout = inflater.inflate(R.layout.performancetest_dialog, null);
    	
    	mDurationText = (TextView)layout.findViewById(R.id.duration);
    	mIconSizeText = (TextView)layout.findViewById(R.id.iconsize);
    	mCornerRadText = (TextView)layout.findViewById(R.id.cornerradius);
    	
    	mDurationBar = (SeekBar)layout.findViewById(R.id.durationbar);
    	mDurationBar.setMax(durationMax);
    	mDurationBar.incrementProgressBy(durationIncrease);
    	mDurationBar.setProgress(defaultDuration);
    	mDurationBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				mDuration = progress;
				mDurationText.setText(mDuration+"");
			}
		});
    	
    	
    	
    	
    	mIconSizeBar = (SeekBar)layout.findViewById(R.id.iconsizebar);
    	mIconSizeBar.setMax(durationMax/100);
    	mIconSizeBar.incrementProgressBy(durationIncrease/100);
    	mIconSizeBar.setProgress(defaultIconSize);
    	
    	mIconSizeBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				mIconSize = progress;
				mIconSizeText.setText(mIconSize+"");
			}
		});
    	
    	mCornerRadBar = (SeekBar)layout.findViewById(R.id.cornerradbar);
    	mCornerRadBar.setMax(durationMax/100);
    	mCornerRadBar.incrementProgressBy(durationIncrease/100);
    	mCornerRadBar.setProgress(defaultCornerRad);
    	
    	mCornerRadBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				mCornerRad = progress;
				mCornerRadText.setText(mCornerRad+" %");
				
			}
		});
    	
    	
    	mQuantityGroup = (RadioGroup)layout.findViewById(R.id.quntityGroup);
    	mQuantityGroup.setOnCheckedChangeListener(this);
    	
    	mOne = (RadioButton)layout.findViewById(R.id.one);
    	mOne.setChecked(true);
    	
    	mShadowSet = (CheckBox)layout.findViewById(R.id.shadowcheck);
    	mLightSet = (CheckBox)layout.findViewById(R.id.lightcheck);
    	mBlendSet = (CheckBox)layout.findViewById(R.id.blendaddcheck);
    	mScaleFitSet = (CheckBox)layout.findViewById(R.id.scaletofit);
    	mBgColorSet = (CheckBox)layout.findViewById(R.id.bgcheck);
    	
    	mShadowSet.setOnCheckedChangeListener(this);
    	mLightSet.setOnCheckedChangeListener(this);
    	mBlendSet.setOnCheckedChangeListener(this);
    	mScaleFitSet.setOnCheckedChangeListener(this);
    	mBgColorSet.setOnCheckedChangeListener(this);
    	
    	
    	setView(layout);
    	setButton("OK", callback);
	}
	
	
	@Override
	public void onCheckedChanged(RadioGroup group, int checkedId) {
		if(group== mQuantityGroup){
			if(checkedId == R.id.one){
				mQuantity = 1;
			}
			else if(checkedId == R.id.fifty){
				mQuantity = 50;
			}else if(checkedId == R.id.oneh){
				mQuantity = 100;
			}else if(checkedId == R.id.twoh){
				mQuantity = 200;
			}else if(checkedId == R.id.threeh){
				mQuantity = 300;
			}else{
				mQuantity = 1;
			}
		}
	}
	
	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		
		if(buttonView == mShadowSet){
			if(isChecked)
				isShadowSet = true;
			else
				isShadowSet = false;
		}
		if(buttonView == mLightSet){
			if(isChecked)
				isLightSet = true;
			else
				isLightSet = false;
		}
		if(buttonView == mBlendSet){
			if(isChecked)
				isBlendSet = true;
			else
				isBlendSet = false;
		}
		if(buttonView == mScaleFitSet){
			if(isChecked)
				isScaletoFitMat = true;
			else
				isScaletoFitMat = false;
		}
		if(buttonView == mBgColorSet){
			if(isChecked)
				isBgColorSet = true;
			else
				isBgColorSet = false;
		}
		
		
	}
	
	
	
	private TextView mDurationText;
	private TextView mIconSizeText;
	private TextView mCornerRadText;
	
	private SeekBar mDurationBar;
	private SeekBar mIconSizeBar;
	private SeekBar mCornerRadBar;
	
	
	private RadioGroup mQuantityGroup;
	
	private RadioButton mOne;
	
	private CheckBox mShadowSet;
	private CheckBox mLightSet;
	private CheckBox mBlendSet;
	private CheckBox mScaleFitSet;
	private CheckBox mBgColorSet;
	
	protected boolean isShadowSet=false;
	protected boolean isLightSet=false;
	protected boolean isBlendSet=false;
	protected boolean isScaletoFitMat=false;
	protected boolean isBgColorSet=false;
	
	protected int mDuration=5000;
	protected int mIconSize=72;
	protected int mCornerRad=0;
	
	protected int mQuantity=1;
	
	final protected int durationMax = 10000;
	final protected int durationIncrease = 100;
	final protected int defaultDuration = 5000;
	final protected int defaultIconSize = 72;
	final protected int defaultCornerRad = 0;
	
	

}
