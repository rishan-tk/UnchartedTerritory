#include "Timing.h"

#include <SDL/SDL.h>

namespace GameEngine2D {

	FPSLimiter::FPSLimiter(){

	}

	void FPSLimiter::initialise(float maxFPS){
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS){
		_maxFPS = maxFPS;
	}

	void FPSLimiter::begin(){
		_startTicks = (float)SDL_GetTicks();
	}

	float FPSLimiter::end(){
		calculateFPS();

		float frameTicks = (float)SDL_GetTicks() - _startTicks;

		//Limit the fps to max fps if it needs limiting
		if (1000.0f / _maxFPS > frameTicks)
			SDL_Delay((Uint32)(1000.0f / _maxFPS - frameTicks));

		return _fps;
	}

	void FPSLimiter::calculateFPS(){
		//Number of frames we are going to average over
		static const int NUM_SAMPLES = 10;

		//Frame time buffer
		static float frameTimes[NUM_SAMPLES];

		static int currentFrame = 0;

		//Get previous and current ticks;
		static float prevTicks = (float)SDL_GetTicks();

		float currentTicks;
		currentTicks = (float)SDL_GetTicks();

		//Calculate the frametime
		_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame%NUM_SAMPLES] = _frameTime;

		//Set prevTicks
		prevTicks = currentTicks;

		int count;

		if (currentFrame++ < NUM_SAMPLES)
			count = currentFrame;
		else
			count = NUM_SAMPLES;


		float frameTimeAvg = 0;

		for (int i = 0; i < (int)count; i++)
			frameTimeAvg += (float)frameTimes[i];

		//Get the average frame time
		frameTimeAvg /= count;

		//As long as frame time average is greater than 0 calculate the FPS
		if (frameTimeAvg > 0)
			_fps = 1000.0f / frameTimeAvg;
		else
			_fps = 60.0f;

	}

};