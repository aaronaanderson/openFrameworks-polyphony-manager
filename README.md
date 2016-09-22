
# openFrameworks Polyphony Manager
Manages MIDI input to keep a tally of all active notes. Includes peal functionality. Will only process the audio from active notes, automatically removing completed notes from the active list.

<h2>Dependencies :</h2>

ofxMidi, ofxMaxim

<h2>Changes to ofxMaxim:</h2>
Note that I made a small addition to ofxMaxim for this to work. 
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

