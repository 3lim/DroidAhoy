package com.example.droidahoy;

import android.content.Context;
import android.hardware.*;
import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.view.View;
import android.widget.ImageView;

public class AhoyStart extends Activity implements SensorEventListener {

	//Accel.
	private float mLastX, mLastY, mLastZ;
	private Sensor mAccelerometer;
	private Sensor sOrientation;
	
	//Rotat.
    float[] mRotationM = new float[16];
    float[] mInclinationM = new float[16];
    float[] mOrientation = new float[3];
    float[] mOldOreintation = new float[3];
    
    String[] mRotation =  new String[16];
    String[] mInclination =  new String[16];
    String[] mOrientationString =  new String[3];
    String[] mOldOreintationString =  new String[3];
    
    //All
	private boolean mInitialized;
	private SensorManager mSensorManager;

	// Sensitivity of app. Needs to be calibrated.
	private final float NOISE = (float) 0.5;

	/** Called when the activity is first created. */

	public void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		mInitialized = false;

		mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		sOrientation = mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);
		//Accel.
	     /*
		mAccelerometer = mSensorManager
				.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		mSensorManager.registerListener(this, mAccelerometer,
				SensorManager.SENSOR_DELAY_NORMAL);
		*/
				
		
		
	}

	protected void onResume() {
		super.onResume();
		mSensorManager.registerListener(this, mAccelerometer,SensorManager.SENSOR_DELAY_NORMAL);
		mSensorManager.registerListener(this, sOrientation, SensorManager.SENSOR_DELAY_NORMAL);
	}

	protected void onPause() {
		super.onPause();
		mSensorManager.unregisterListener(this);
	}

	// Apparently not useful yet, ignore for now for now
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}

	public void onSensorChanged(SensorEvent event_in) {
		final SensorEvent event = event_in;
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				
				//Orient.
			    float[] mGravs = new float[3];
			    float[] mGeoMags = new float[3];
				boolean success = SensorManager.getRotationMatrix(mRotationM, mInclinationM, mGravs, mGeoMags);
                if (success) {
                    // getOrientation values
                    SensorManager.getOrientation(mRotationM, mOrientation);

                      for(int i=0; i<2; i++){
                          mOrientationString[i] = Float.toString(mOrientation[i]);
                          mOldOreintationString[i] = Float.toString(mOldOreintation[i]);
                      }
                }
                      
                TextView tvX = (TextView)findViewById(R.id.xaxisOrientation);
                tvX.setText(mOrientationString[0]);    
                TextView tvY = (TextView)findViewById(R.id.yaxisOrientation);
                tvY.setText(mOrientationString[1]);
                TextView tvZ = (TextView)findViewById(R.id.);
                tvZ.setText(mOrientationString[2]);
                      
                //end Orient.
				
				//Accel.
                /*	
			  	TextView tvX = (TextView) findViewById(R.id.x_axis);
				TextView tvY = (TextView) findViewById(R.id.y_axis);
				TextView tvZ = (TextView) findViewById(R.id.z_axis);
				*/
                
                

				ImageView iv = (ImageView) findViewById(R.id.image);

				float x = event.values[0];
				float y = event.values[1];
				float z = event.values[2];

				if (!mInitialized) {

					// Keep track of old values
					mLastX = x;
					mLastY = y;
					mLastZ = z;

					// Initialize
					tvX.setText("0.0");
					tvY.setText("0.0");
					tvZ.setText("0.0");

					mInitialized = true;

				} else {

					float deltaX = Math.abs(mLastX - x);
					float deltaY = Math.abs(mLastY - y);
					float deltaZ = Math.abs(mLastZ - z);

					if (deltaX < NOISE)
						deltaX = (float) 0.0;
					if (deltaY < NOISE)
						deltaY = (float) 0.0;
					if (deltaZ < NOISE)
						deltaZ = (float) 0.0;

					mLastX = x;
					mLastY = y;
					mLastZ = z;

					tvX.setText(Float.toString(deltaX));
					tvY.setText(Float.toString(deltaY));
					tvZ.setText(Float.toString(deltaZ));

					iv.setVisibility(View.VISIBLE);

					if (deltaX > deltaY) {

						iv.setImageResource(R.drawable.horizontal);

					} else if (deltaY > deltaX) {

						iv.setImageResource(R.drawable.vertical);

					} else {

						iv.setVisibility(View.INVISIBLE);

					}
				}
			}
			
		});
	}
}