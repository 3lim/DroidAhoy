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
	float[] mOldOrientation = new float[3];
	
	float[] delta = new float[3];
	
	boolean showinfo = false;
	boolean WantConnection = false;
	boolean connected = false;

	//All
	private boolean mInitialized;
	private SensorManager mSensorManager;
	public Socket socket;

	// Sensitivity of app. Needs to be calibrated.
	//private final float NOISE = (float) 0.5;

	/** Called when the activity is first created. */

	public void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		mInitialized = false;

		mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		sOrientation = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);

		mSensorManager.registerListener(this,sOrientation,SensorManager.SENSOR_DELAY_NORMAL);

		Button message_txt  = (Button)findViewById(R.id.send_message);
		//sent_txt = (TextView)findViewById(R.id.button_listen);
		
		Button button_listen  = (Button)findViewById(R.id.button_listen);
		
		Button reset  = (Button)findViewById(R.id.reset);
		
		Button show  = (Button)findViewById(R.id.show);

	    message_txt.setOnClickListener(new View.OnClickListener() {

	        @Override
	        public void onClick(View v) {
	        	
	        		WantConnection = true;

	        }
	    });
	    
	        button_listen.setOnClickListener(new View.OnClickListener() {

		        @Override
		        public void onClick(View v) {
			        //sendData(Float.toString(delta[0])+""+Float.toString(delta[1])+""+Float.toString(delta[2]));
		        	
		        	float testFl = 0.0f;
		        	connectSocket("192.168.43.201");
		        	sendData(Float.toString(testFl)+" "+Float.toString(testFl)+" "+Float.toString(testFl));
		        	WantConnection = false;
		        }

	    });
	        
	        reset.setOnClickListener(new View.OnClickListener() {

		        @Override
		        public void onClick(View v) {
		        	
					mOldOrientation[0]=(float)Math.toDegrees(mOrientation[0]);
					mOldOrientation[1]=(float)Math.toDegrees(mOrientation[1]);
					mOldOrientation[2]=(float)Math.toDegrees(mOrientation[2]);

		        }
		    });
	        
	        show.setOnClickListener(new View.OnClickListener() {

		        @Override
		        public void onClick(View v) {
		        	
		        	if(!showinfo)
		        		showinfo = true;
		        	else
		        		showinfo = false;

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
					
					if(!mInitialized || !WantConnection) {
						
						mOldOrientation[0]=(float)Math.toDegrees(mOrientation[0]);
						mOldOrientation[1]=(float)Math.toDegrees(mOrientation[1]);
						mOldOrientation[2]=(float)Math.toDegrees(mOrientation[2]);
						
						mInitialized = true;
						
						if(showinfo){
							TextView tvX = (TextView) findViewById(R.id.x_axis);
							TextView tvY = (TextView) findViewById(R.id.y_axis);
							TextView tvZ = (TextView) findViewById(R.id.z_axis);
							
							
							tvX.setText(Float.toString((float)Math.toDegrees(mOrientation[0])-mOldOrientation[0]));
							tvY.setText(Float.toString((float)Math.toDegrees(mOrientation[1])-mOldOrientation[1]));
							tvZ.setText(Float.toString((float)Math.toDegrees(mOrientation[2])-mOldOrientation[2]));
							}
						
					}
					else
					{
					
						//Skriver ut grader i app:n
					
					delta[0]=(float)Math.toDegrees(mOrientation[0])-mOldOrientation[0];
					delta[1]=(float)Math.toDegrees(mOrientation[1])-mOldOrientation[1];
					delta[2]=(float)Math.toDegrees(mOrientation[2])-mOldOrientation[2];
					
					if(showinfo){
					TextView tvX = (TextView) findViewById(R.id.x_axis);
					TextView tvY = (TextView) findViewById(R.id.y_axis);
					TextView tvZ = (TextView) findViewById(R.id.z_axis);
					
					
					tvX.setText(Float.toString(delta[0]));
					tvY.setText(Float.toString(delta[1]));
					tvZ.setText(Float.toString(delta[2]));
					}
					
					
					/*
					tvX.setText(Float.toString(mOrientation[0]));
					tvY.setText(Float.toString(mOrientation[1]));
					tvZ.setText(Float.toString(mOrientation[2]));
					*/
					
					//Slut "Skriver ut grader i app:n"
					
					connectSocket("192.168.43.201");

					sendData(Float.toString(delta[0])+" "+Float.toString(delta[1])+" "+Float.toString(delta[2]));
					//sendData(Float.toString(mOrientation[0])+" "+Float.toString(mOrientation[1])+" "+Float.toString(mOrientation[2]));
					
					ImageView iv = (ImageView) findViewById(R.id.image);

					iv.setVisibility(View.VISIBLE);
					
					
					}

				}

			}
		});
	}
	
	private boolean connectSocket(String serverAddr){ 

		try {
			InetAddress.getByName(serverAddr);
			
	        Log.d("TCP", "C: Connecting..."); 
	        socket = new Socket(serverAddr, 4444);
	       
	        
	        return true;
	        
	        } 

	     catch (UnknownHostException e) { 
	        // TODO Auto-generated catch block 
	        Log.e("TCP", "C: UnknownHostException", e); 
	        e.printStackTrace();
	        return false;
	    } catch (IOException e) { 
	        // TODO Auto-generated catch block 
	        Log.e("TCP", "C: IOException", e); 
	        e.printStackTrace(); 
	        return false;
	    } 
		}


	
	private void sendData (String message){
		
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

        } 
		catch(NullPointerException e) {
			Log.e("TCP", "Error: NullPointerException"); 
			WantConnection = false;
		}
		
		catch(Exception e) { 
            Log.e("TCP", "S: Error", e); 
        }
		
	}
	
	} 