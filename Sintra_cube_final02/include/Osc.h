
void ParticleSystemApp::oscSetup()
{
	listener.setup(PORT);
}

void ParticleSystemApp::oscUpdate()
{
	while (listener.hasWaitingMessages()) {
		ci::osc::Message message;
		listener.getNextMessage(&message);
        
        console() << "New message received" << std::endl;
        console() << "Address: " << message.getAddress() << std::endl;
        console() << "Num Arg: " << message.getNumArgs() << std::endl;
        
		for (int i = 0; i < message.getNumArgs(); i++) {
			//		console() << "-- Argument " << i << std::endl;
			//		console() << "---- type: " << message.getArgTypeName(i) << std::endl;
            if (message.getAddress() == "/NewAwareness"){
                if (message.getArgType(i) == ci::osc::TYPE_INT32) {
					try {
                        console() << "new A: "<<message.getArgAsInt32(i)<<std::endl;
					}
					catch (...) {
						console() << "Exception reading argument as float" << std::endl;
					}
				}
                
                if (message.getArgType(i) == ci::osc::TYPE_STRING) {
					try {
                        console() << "new sA: "<<message.getArgAsString(i)<<std::endl;
					}
					catch (...) {
						console() << "Exception reading argument as float" << std::endl;
					}
				}
                
                console() << "NewAwareness" << std::endl;
            }
            
            /*  if (message.getAddress() == "/instrumentos"){
             
             console() << "instrumentos" << std::endl;
             }
             
             */
            
            if (message.getAddress() == "/amp"){
				if (message.getArgType(i) == ci::osc::TYPE_FLOAT) {
					try {
						//    console() << "------ value: " << message.getArgAsFloat(i) << std::endl;
						mAmp = ci::lmap<float>(message.getArgAsFloat(i), 20, 20000, 0, 1.0);
                        console() << "amp"<<std::endl;
					}
					catch (...) {
						console() << "Exception reading argument as float" << std::endl;
					}
				}
			}
            
			if (message.getAddress() == "/pitch"){
				if (message.getArgType(i) == ci::osc::TYPE_FLOAT) {
					try {
						//    console() << "------ value: " << message.getArgAsFloat(i) << std::endl;
						mFreq = ci::lmap<float>(message.getArgAsFloat(i), 20, 20000, 0, 1.0);
					}
					catch (...) {
						console() << "Exception reading argument as float" << std::endl;
					}
				}
			}
			if (message.getAddress() == "/onset"){
				mOnSet = !mOnSet;
			}
		}
	}
}