void ParticleSystemApp::setupMIDI()
{
	if (mInput.getNumPorts() > 0){
		mInput.listPorts();
		mInput.openPort(0);
		console() << "Opening MIDI port 0" << std::endl;
	}
	else {
		console() << "No MIDI Ports found!!!!" << std::endl;
	}
    
    mInput.midiSignal.connect(boost::bind(&ParticleSystemApp::midiListener, this, boost::arg<1>::arg()));
}


void ParticleSystemApp::midiListener(midi::Message msg){
	switch (msg.status)
	{
        case MIDI_NOTE_ON:
            break;
        case MIDI_NOTE_OFF:
            break;
        case MIDI_CONTROL_CHANGE:
            break;
        default:
            break;
	}
    
}
