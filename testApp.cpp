#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    soundCount = 0;
    all = 8000;
    v = 0;
    
    colors[0].set(255, 246, 201); //노랑
    colors[1].set(200, 100, 100); //분홍
    colors[2].set(201, 243, 255); //연두
    
    
    video.setVerbose(true);
    video.setDeviceID(0);
    video.setDesiredFrameRate(30);
    video.initGrabber(640, 480);
    video.listDevices();
    
    sounds[0] = "FLOW_depapepe";
    sounds[1] = "DangDangDang_supreme";
    sounds[2] = "MYLO_XYLOTO_coldplay";
    sounds[3] = "SHAKE_IT_OFF_tylor_swift";
    sounds[4] = "BangBang_Ariana_Grande";
    sounds[5] = "I_Believe_daishi_dance";
    sounds[6] = "HI_D_depapepe";
    sounds[7] = "Baby_Ariana_Grande";
    sounds[8] = "HOME_daishi_dance";
    sounds[9] = "Popular_Mika";
    sounds[10] = "MTMM_depapepe";
    sounds[11] = "MAKE_IT_MINE_jason_mraz";
    sounds[12] = "Happy_Parrel_Williams";
    
    
    sound.loadSound("beat.wav");
    sound.setLoop(true);
    sound.setVolume(v);
    sound.setMultiPlay(true);
    sound.play();
    
    font.loadFont("sanha.ttf", 20);
    
    camera.loadSound("camera.wav");
    camera.setVolume(1);
    
    snapTime = ofGetFrameNum();
    bSnapshot = false;
    snapCount = 0;
    
    px = ofGetWidth() - 100;
    py = ofGetHeight() - 100;
    vx = 1;
    vy = 0.1;
    
    fftSmoothed = new float[8192];
    for (int i=0; i< 8192; i++) {
        fftSmoothed[i] = 0;
    }
    nBandsToGet = 128;
    
    
//	video.loadMovie("ballet1-640-360.mov");	//Load the video file
//	video.play();						//Start the video to play
}

//--------------------------------------------------------------
void testApp::update(){
	video.update();		//Decode the new frame if needed
	//Do computing only if the new frame was obtained
	if ( video.isFrameNew() ) {
		//Store the previous frame, if it exists till now
		if ( grayImage.bAllocated ) {
			grayImagePrev = grayImage;
		}

		//Getting a new frame
		image.setFromPixels( video.getPixelsRef() );
		grayImage = image;	//Convert to grayscale image

		//Do processing if grayImagePrev is inited
		if ( grayImagePrev.bAllocated ) {
			//Get absolute difference
			diff.absDiff( grayImage, grayImagePrev );

			//We want to amplify the difference to obtain
			//better visibility of motion
			//We do it by multiplication. But to do it, we
			//need to convert diff to float image first
			diffFloat = diff;	//Convert to float image
			diffFloat *= 5.0;	//Amplify the pixel values

			//Update the accumulation buffer
			if ( !bufferFloat.bAllocated ) {
				//If the buffer is not initialized, then
				//just set it equal to diffFloat
				bufferFloat = diffFloat;
			}
			else {
				//Slow damping the buffer to zero
				bufferFloat *= 0.85;
				//Add current difference image to the buffer
				bufferFloat += diffFloat;
			}
		}	
	}
    px+=vx;
    py+=vy;
//
    int x = grayImage.width*3/2;
    int y = 0;
    int w = ofGetWidth()-x;
    int h = ofGetHeight()-y;
    
    if(px < x){
        px = x;
        vx*=-1;
        
    }else if(px > x+w){
        px = x+w;
        vx*=-1;
    }
    
    if(py < y){
        py = y;
        vy = 30;
        soundCount++;
        sound.loadSound("song/"+sounds[soundCount%13]+".wav");
        sound.play();
    }else if(py > y+h){
        py = y+h;
        vy*=-1;
    }
    vy*=0.96;
    
    

    float * val = ofSoundGetSpectrum(nBandsToGet);
    for(int i=0; i< nBandsToGet; i++){
        fftSmoothed[i] *= 0.96f;
        if(fftSmoothed[i]<val[i]) fftSmoothed[i] = val[i];
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    float volume = 0.0;
	ofBackground( 0, 0, 0 );	//Set the background color

	//Draw only if diffFloat image is ready.
	//It happens when the second frame from the video is obtained
	if ( diffFloat.bAllocated ) {
		//Get image dimensions
		int w = grayImage.width;	
		int h = grayImage.height;
		ofSetColor( 255, 255, 255 );

		//Draw images grayImage,  diffFloat, bufferFloat
		grayImage.draw( 0, 0, w/2, h/2);
		diffFloat.draw( w/2, 0, w/2, h/2);
		bufferFloat.draw( 0, h/2, w/2, h/2);

		//Shift and scale the coordinate system
		ofPushMatrix();
		ofTranslate( w/2, h/2);
		ofScale( 0.5, 0.5 );

		//Draw bounding rectangle
		ofSetColor(0, 0, 0);
		ofNoFill();
		ofRect( -1, -1, w+2, h+2 );

		//Get bufferFloat pixels
		float *pixels = bufferFloat.getPixelsAsFloats();
		//Scan all pixels
		for (int y=0; y<h; y++) {
			for (int x=0; x<w; x++) {
				//Get the pixel value
				float value = pixels[ x + w * y ];
				//If value exceed threshold, then draw pixel
				if ( value >= 0.9 ) {
                    volume +=0.1;
				}
			}
		}
		ofPopMatrix();	//Restore the coordinate system
        v = volume/all;
        

         ofSetColor(200, 100, 100);
        if (v > 1) {
            sound.setVolume(1);
            font.drawString("GOOD \n now don't act", w/2+10, h/2+150);
            int nowTime  = ofGetFrameNum();
            //60 = 1cho
            if(nowTime - snapTime > 120 ){
                snapTime = nowTime;
//                cout << ofGetFrameNum() << endl;
                bSnapshot = true;
                vy = -5;
            }
        }else{
            sound.setVolume(v);
//            v = 0~1
            font.drawString("More Act Make Louder! \n sound:"+ofToString((int)(v*100))+"%", w/2+10, h/2+150);

        }
        
        ofSoundUpdate();
        if(bSnapshot == true){
            bSnapshot = false;
            camera.play();
            img.grabScreen(w/2, 0, w/2, h/2);
            string fileName = "photo/snapShot_"+ofToString(10000+snapCount)+".png";
            img.saveImage(fileName);
            imgs.push_back(img);
            snapCount++;
        }
        
        //images print
        if(snapCount > 0){
            vector<ofImage>::iterator itor;
            int count = snapCount;
            for(itor = imgs.begin(); itor != imgs.end(); ++itor){
                count--;
                ofSetColor(colors[count%3]);
                itor->draw(w,count*h/2, w/2, h/2);
            }
        }
    
        //sound bar
        ofSetColor(119, 122, 166);
        float width = (float)(5*128) /nBandsToGet;
        for (int i=0; i<nBandsToGet; i++) {
            ofRect(10+i*width, ofGetHeight()-5, width, MAX(-(fftSmoothed[i]*200), h-ofGetHeight()+10));
        }
        
        //message
        font.drawString("When Circle is on top \n music change ", w*3/2+10, 100);
        font.drawString(sounds[soundCount%12], w*3/2+10, 400);
        
        //circle
        ofEnableAlphaBlending();
        ofSetColor(119,122,166,80);
//        ofFill();
        ofCircle(px, py,50);
        ofDisableAlphaBlending();
        
        ofSetColor(119, 122, 166);
//        ofSetHexColor(0xffffff);
        ofCircle(px, py,8);
    
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'x'){
        int nowTime  = ofGetFrameNum();
        //60 = 1cho
        if(nowTime - snapTime > 60 ){
            snapTime = nowTime;
            cout << ofGetFrameNum() << endl;
            bSnapshot = true;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
