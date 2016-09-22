#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//MIDI stuff
	midiIn.openPort(0);
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);

	//set all envelope values
	for (int i = 0; i < numSlots; i++) {
		envelopes[i].setAttack(500);
		envelopes[i].setDecay(1);
		envelopes[i].setSustain(1);
		envelopes[i].setRelease(1000);
	}

	
	//clear all slots
	for (int i = 0;i < numSlots;i++) { envelopes[i].trigger = 0;}

	//fill table for mtof
	for (int i = 0; i < numSlots; i++) {
		mtof[i] = (pow(2, (i - 69) / 12.f)*440.f);
	}

	ofSoundStreamSetup(6, 0, 48000, 512, 12);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}
void ofApp::audioOut(float *buffer, int bufferSize, int nChannels) {
	for (int i = 0; i < bufferSize; i++) {

		float currentSample = 0;

		for (int j = 0; j < activeList.size(); j++) {//for all active notes
			int note = activeList[j];
			currentSample += envelopes[note].adsr(
				oscillators[note].sinewave(mtof[note]),//input
				envelopes[note].trigger)*0.01;
		}
		
		buffer[i*nChannels] = currentSample;
		buffer[i*nChannels + 1] = currentSample;

	}
	//--------------------------------------------------------------------------Control Rate

	//check active list for finished evenlopes;
	for (int j = killList.size()-1; j >= 0 ; j--) {//for every member of the kill list
		//remove from active list
		if (envelopes[killList[j]].isFinished()) {//if this particular member of the kill list is finished
			//erase the note from the active list
			for (int k = activeList.size()-1; k >= 0; k--) {
				if (activeList[k] == killList[j]) {
					activeList.erase(activeList.begin() + k);//erase the member from the active list
					//----------------------------------------report new active list to console
					cout << "active notes ";
					for (int i = 0; i < activeList.size(); i++) {
						cout << activeList[i] << " ";
					}
					cout << endl;
				}   //---------------------------------------------
			}
			killList.erase(killList.begin() + j);//erase the member from the kill list
		}
	}
}
//--------------------------------------------------------------

void ofApp::newMidiMessage(ofxMidiMessage &msg) {
	midiMessage = msg;
	int note = midiMessage.pitch;

	if (midiMessage.velocity > 0) {//if note-on
		envelopes[note].trigger = 1;//set trigger
		bool alreadyActive = false;
		for (int i = 0; i < activeList.size(); i++) {
			if (activeList[i] == note) { //make sure it's not already active
				alreadyActive = true; 
				break;//no need to continue
			}
		}
		if (alreadyActive == false) {//if it's not already active
			activeList.push_back(note);//add it to the active list
			//-----------------------------------------report to console
			cout << "active notes ";
			for (int i = 0; i < activeList.size(); i++) {
				cout << activeList[i] << " ";
			}
			cout << endl;
		}   //-------------------------------------------------
	}
	else {//if it was a note-Off message
		if (sustainOn) {//if the sustain pedal is down
			bool alreadyOnList = false;
			for (int i = 0; i < sustainList.size(); i++) {
				if (sustainList[i] == note) {//make sure it's not already on the kill list
					alreadyOnList = true;
					break;//no need to continue
				}
			}
			if (alreadyOnList == false) {//if it wasn't on the kill list
				sustainList.push_back(note);//add it to the kill list
			}
		}
		else {//if sustain pedal is not down
			envelopes[note].trigger = 0;//sit trigger to off
			bool alreadyOnList = false;
			for (int i = 0; i < killList.size(); i++) {
				if (killList[i] == note) {//make sure it's not already on the kill list
					alreadyOnList = true;
					break;//no need to continue
				}
			}
			if (alreadyOnList == false) {//if it wasn't on the kill list
				killList.push_back(note);//add it to the kill list
			}
		}
	}

	if (midiMessage.control == 64) {//if sustain pedal
		if (midiMessage.value == 127) {//if sustain pedal is down
			sustainOn = true;
		}
		else {//if sustain pedal was released
			sustainOn = false;
			//add sustain collection to kill list
			for (int i = 0; i < sustainList.size(); i++) {
				envelopes[sustainList[i]].trigger = 0;//sit trigger to off
				bool alreadyOnList = false;
				for (int i = 0; i < killList.size(); i++) {
					if (killList[i] == note) {//make sure it's not already on the kill list
						alreadyOnList = true;
						break;//no need to continue
					}
				}
				if (alreadyOnList == false) {//if it wasn't on the kill list
					killList.push_back(sustainList[i]);//add it to the kill list
				}
			}
			sustainList.clear();
		}
	}

}
//--------------------------------------------------------------
void ofApp::exit() {
	midiIn.closePort();
	midiIn.removeListener(this);
	ofSoundStreamClose();
}