#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  int winX, winY, winW, winH;

  ofSetLogLevel(LOGLEVEL);
  ofLogToConsole();
  ofLogVerbose() << "Starting...";
  ofLogVerbose() << "ofApp::setup";

  ofBackground(128, 128, 128, 255);

  cursorImage.loadImage("cursor.png");
}

//--------------------------------------------------------------
void ofApp::update() {
  WiiMouse.processEvents();
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255,255,255);
  cursorImage.draw(mouseX, mouseY);
}

void ofApp::exit() {
  ofLog() << "Exiting...";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
// Mouse moved doesn't fire when mouse dragged does.
  ofLog(OF_LOG_VERBOSE, "mouseMoved x: %d, y: %d", x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
// Mouse drag is mouseMoved with button pressed
  ofLog(OF_LOG_VERBOSE, "mouseDragged x: %d, y: %d, button: %d", x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  ofLog(OF_LOG_VERBOSE, "mousePressed x: %d, y: %d, button: %d", x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  ofLog(OF_LOG_VERBOSE, "mouseReleased x: %d, y: %d, button: %d", x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::wiiMouseConnect() {
  ofLog(OF_LOG_VERBOSE, "Press A + B on wiimote.");
}

void ofApp::wiiMouseConnected(int connected, int found) {
  ofLog(OF_LOG_VERBOSE, "Connected to %i wiimotes (of %i found).", connected, found);
}

//--------------------------------------------------------------
void ofApp::wiiMouseMoved(int x, int y) {
  ofNotifyMouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::wiiMouseDragged(int x, int y, int button) {
  ofNotifyMouseDragged(x, y, button);
  //ofLog(OF_LOG_VERBOSE, "wiiMouseDragged x: %d, y: %d, button: %d", x, y, button);
}

//--------------------------------------------------------------
void ofApp::wiiMousePressed(int x, int y, int button) {
  if (button == WIIMOTE_BUTTON_B) {
    ofNotifyMousePressed(x, y, OF_MOUSE_BUTTON_LEFT);
  } else if (button == WIIMOTE_BUTTON_A) {
    ofNotifyMousePressed(x, y, OF_MOUSE_BUTTON_RIGHT);
  } else  if (button == WIIMOTE_BUTTON_HOME) {
    ofNotifyMousePressed(x, y, OF_MOUSE_BUTTON_MIDDLE);
  }
}

//--------------------------------------------------------------
void ofApp::wiiMouseReleased(int x, int y, int button) {
  if (button == WIIMOTE_BUTTON_B) {
    ofNotifyMouseReleased(x, y, OF_MOUSE_BUTTON_LEFT);
  } else if (button == WIIMOTE_BUTTON_A) {
    ofNotifyMouseReleased(x, y, OF_MOUSE_BUTTON_RIGHT);
  } else  if (button == WIIMOTE_BUTTON_HOME) {
    ofNotifyMouseReleased(x, y, OF_MOUSE_BUTTON_MIDDLE);
  }
}

//--------------------------------------------------------------
void ofApp::wiiMouseButtonHeld(int x, int y, int button) {
}

