/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.example.finalanswer;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.content.Context;
import java.util.Random;
//
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
//
import android.view.View;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Display;
import android.view.WindowManager;
import android.view.Surface;
import android.view.SurfaceHolder;




class MyRenderer implements GLSurfaceView.Renderer
{
	//Random random = new Random();

	public MyRenderer() {
		Log.i("finalanswer", "@MyRenderer");
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.i("finalanswer", "@onSurfaceCreated");
		gl.glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
	}

	@Override
    public void onSurfaceChanged(GL10 gl, int w, int h) {
		Log.i("finalanswer", "@onSurfaceChanged");
        gl.glViewport(0, 0, w, h);
    }

	@Override
    public void onDrawFrame(GL10 gl) {
		Log.i("finalanswer", "@onDrawFrame");
		//Read();

		//float b = random.nextInt(256) / (float)256.0;
		//float g = random.nextInt(256) / (float)256.0;
		//float r = random.nextInt(256) / (float)256.0;
		//Log.i("finalanswer", b+","+g+","+r);

		//gl.glClearColor(b, g, r, 0.0f);
		gl.glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
    }
}
class FinalAnswerView extends GLSurfaceView {
	private static native void Start(Surface s);
	private static native void Stop();
	private static native void Read();
	private final MyRenderer renderer;

	public FinalAnswerView(Context context) {
		super(context);
		Log.i("finalanswer", "@FinalAnswerView");

		setEGLContextClientVersion(2);

		renderer = new MyRenderer();
		setRenderer(renderer);
	}

	public void surfaceCreated(SurfaceHolder holder) {
		Log.i("finalanswer", "@SurfaceCreated");
		//Start(holder.getSurface());
	}

	public void surfaceChanged(SurfaceHolder holder) {
		Log.i("finalanswer", "@surfaceChanged");
	}

	public void surfaceDestoryed(SurfaceHolder holder) {
		Log.i("finalanswer", "@surfaceDestoryed");
	}

	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getActionMasked();
		int index = event.getActionIndex();
		int count = event.getPointerCount();
		Log.i("finalanswer","("+action+","+index+","+count+")");
		return true;
	}
}
public class FinalAnswer extends Activity
{
	private GLSurfaceView view;
	static {
		System.loadLibrary("finalanswer");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i("finalanswer", "@onCreate");

		view = new FinalAnswerView(this);
		setContentView(view);
	}

	@Override
	protected void onPause() {
		super.onPause();
		view.onPause();
		Log.i("finalanswer", "@onPause");
	}

	@Override
	protected void onResume() {
		super.onResume();
		view.onResume();
		Log.i("finalanswer", "@onResume");
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			Log.i("finalanswer","KeyEvent.KEYCODE_BACK");
			finish();
			System.exit(0);
		}
		return super.onKeyDown(keyCode, event);
	}
}