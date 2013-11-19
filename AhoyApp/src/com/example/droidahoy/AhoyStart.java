package com.example.droidahoy;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import android.content.Context;
import android.hardware.*;
import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

public class AhoyStart extends Activity implements SensorEventListener {


	private Sensor sOrientation;
	
	private TextView sent_txt;

	//Rotat.
	float[] mRotationM = new float[16];
	float[] mOldRotationM = new float[16];

	float[] mOrientation = new float[3];
	float[] mOldOreintation = new float[3];
	
	int[] mOrientationint = new int[3];
	int[] mOldOreintationint = new int[3];

	String[] mOrientationString =  new String[3];
	String[] mOldOreintationString =  new String[3];
	
	String[] delta = new String[3];

	//All
	private boolean mInitialized;
	private SensorManager mSensorManager;

	// Sensitivity of app. Needs to be calibrated.
	//private final float NOISE = (float) 0.5;

	/** Called when the activity is first created. */

	public void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		mInitialized = false;

		mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		sOrientation = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);

		mSensorManager.registerListener(this, sOrientation,SensorManager.SENSOR_DELAY_NORMAL);

		Button message_txt  = (Button)findViewById(R.id.send_message);
		sent_txt = (TextView)findViewById(R.id.button_listen);


	    message_txt.setOnClickListener(new View.OnClickListener() {

	        @Override
	        public void onClick(View v) {
	            // TODO Auto-generated method stub
	        //connectSocket("Hello");
	        	if(mInitialized)
			connectSocket(delta[0] + "," + delta[1] + "," + delta[2]);

	        }
	    });
		
	}


	protected void onResume() {
		super.onResume();
		mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
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
				if(event.sensor.getType()==Sensor.TYPE_ROTATION_VECTOR){
					SensorManager.getRotationMatrixFromVector(mRotationM,event.values);

					// getOrientation values
					SensorManager.remapCoordinateSystem(mRotationM,SensorManager.AXIS_X, SensorManager.AXIS_Z, mRotationM);
					SensorManager.getOrientation(mRotationM, mOrientation);
					
					if(!mInitialized) {
						
						mOldOreintationint[0]=Math.round((float)Math.toDegrees(mOrientation[0]));
						mOldOreintationint[1]=Math.round((float)Math.toDegrees(mOrientation[1]));
						mOldOreintationint[2]=Math.round((float)Math.toDegrees(mOrientation[2]));
						
						mInitialized = true;
						
					}
					else
					{
					mOrientationint[0]=Math.round((float)Math.toDegrees(mOrientation[0]));
					mOrientationint[1]=Math.round((float)Math.toDegrees(mOrientation[1]));
					mOrientationint[2]=Math.round((float)Math.toDegrees(mOrientation[2]));

					TextView tvX = (TextView) findViewById(R.id.x_axis);
					TextView tvY = (TextView) findViewById(R.id.y_axis);
					TextView tvZ = (TextView) findViewById(R.id.z_axis);
					
					delta[0]=String.valueOf(mOldOreintationint[0]-mOrientationint[0]);
					delta[1]=String.valueOf(mOldOreintationint[1]-mOrientationint[1]);
					delta[2]=String.valueOf(mOldOreintationint[2]-mOrientationint[2]);
					

					tvX.setText(delta[0]);
					tvY.setText(delta[1]);
					tvZ.setText(delta[2]);

					ImageView iv = (ImageView) findViewById(R.id.image);

					iv.setVisibility(View.VISIBLE);
					}

				}

			}
		});
	}
	
	private void connectSocket(String a){ 

	    try { 
	        InetAddress serverAddr = InetAddress.getByName("192.168.43.200"); 
	        Log.d("TCP", "C: Connecting..."); 
	        Socket socket = new Socket(serverAddr, 4444); 

	        String message = a;

	        PrintWriter out = null;
	        BufferedReader in = null;

	        try { 
	            Log.d("TCP", "C: Sending: '" + message + "'"); 
	            out = new PrintWriter( new BufferedWriter( new OutputStreamWriter(socket.getOutputStream())),true); 
	            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));                

	            out.println(message);
	            while ((in.readLine()) != null) {
	                sent_txt.append(in.readLine());
	            }

	            Log.d("TCP", "C: Sent."); 
	            Log.d("TCP", "C: Done.");               

	        } catch(Exception e) { 
	            Log.e("TCP", "S: Error", e); 
	        } finally { 
	            socket.close(); 
	        } 

	    } catch (UnknownHostException e) { 
	        // TODO Auto-generated catch block 
	        Log.e("TCP", "C: UnknownHostException", e); 
	        e.printStackTrace(); 
	    } catch (IOException e) { 
	        // TODO Auto-generated catch block 
	        Log.e("TCP", "C: IOException", e); 
	        e.printStackTrace(); 
	    }       
	} 
	} 