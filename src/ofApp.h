#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxMidi.h"

#define numSlots 128

class ofApp : public ofBaseApp, public ofxMidiListener{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void audioOut(float *buffer, int bufferSize, int nChannels);
		void newMidiMessage(ofxMidiMessage& eventArgs);

		ofxMidiIn midiIn;
		ofxMidiMessage midiMessage;

		maxiOsc oscillators[numSlots];
		maxiEnv envelopes[numSlots];
		float mtof[numSlots];//stpre conversion

		double attack, decay, sustain, release;
		vector<int> activeList;//all notes currently sounding
		vector<int> killList;//notes that have been triggered off, but aren't done sounding
		vector<int> sustainList;//notes triggered while sustain on
		bool sustainOn;//sustain pedal status
		
};
