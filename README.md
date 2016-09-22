# openFrameworks-polyphony-manager

Dependencies: ofxMIDI, ofxMaxim

Note that I made a small additio to ofxMaxim for this to work. 
In the Maximilian.h file:

to the maxiEnv class
bool isFinished();
added: bool noteFinished;

In the Maximilian.cpp file;

Add to the "double maxiEnv::adsr(double input, int trigger)" function:

	if(releasephase ==1 && amplitude <=0.001){//if the amplitude is -60dBFS
		noteFinished = true;
	}
	
add the folowing function anywhere in Maximilian.cpp:

bool maxiEnv::isFinished() {
	return noteFinished;
}

