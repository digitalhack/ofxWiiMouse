#include "ofxWiiMouse.h"

//--------------------------------------------------------------
ofxWiiMouse::ofxWiiMouse() {
  ofAddListener(connect, this, &ofxWiiMouse::wiiMouseConnect);
  ofAddListener(connected, this, &ofxWiiMouse::wiiMouseConnected);
  ofAddListener(moved, this, &ofxWiiMouse::wiiMouseMoved);
  ofAddListener(dragged, this, &ofxWiiMouse::wiiMouseDragged);
  ofAddListener(pressed, this, &ofxWiiMouse::wiiMousePressed);
  ofAddListener(held, this, &ofxWiiMouse::wiiMouseButtonHeld);
  ofAddListener(released, this, &ofxWiiMouse::wiiMouseReleased);

  dragButton = 0;

  startThread(true, false);    // blocking, non verbose
}

//--------------------------------------------------------------
ofxWiiMouse::~ofxWiiMouse() {
  stopThread();

  ofRemoveListener(connect, this, &ofxWiiMouse::wiiMouseConnect);
  ofRemoveListener(connected, this, &ofxWiiMouse::wiiMouseConnected);
  ofRemoveListener(moved, this, &ofxWiiMouse::wiiMouseMoved);
  ofRemoveListener(dragged, this, &ofxWiiMouse::wiiMouseDragged);
  ofRemoveListener(pressed, this, &ofxWiiMouse::wiiMousePressed);
  ofRemoveListener(released, this, &ofxWiiMouse::wiiMouseReleased);
  ofRemoveListener(held, this, &ofxWiiMouse::wiiMouseButtonHeld);
}

//--------------------------------------------------------------
void ofxWiiMouse::threadedFunction() {
  bool found;
  bool connected;
  ofxWiiMouseEvent wmEvent;

  wiiMouse =  wiiuse_init(ONE_WIIMOTE);
  connected = false;

  pointerX = 0;
  pointerY = 0;

  oldPointerX = 0;
  oldPointerY = 0;

  while(isThreadRunning()) {
    if (!connected) {
      wmEvent.event = OFXWE_CONNECT_WIIMOTE;
		  lock();
        wmEvents.push(wmEvent);
      unlock();
      found = wiiuse_find(wiiMouse, ONE_WIIMOTE, 5);
      if (found) {
        connected = wiiuse_connect(wiiMouse, ONE_WIIMOTE);
        if (connected) {
       		wiiuse_set_ir(wiiMouse[0], 1);
       		wiiuse_set_ir_vres(wiiMouse[0], ofGetWindowWidth(), ofGetWindowHeight());
          wmEvent.event = OFXWE_WIIMOTE_CONNECTED;
          wmEvent.arg[0] = connected;
          wmEvent.arg[1] = found;
          lock();
            wmEvents.push(wmEvent);
          unlock();
        }
      }
    } else {
      if (wiiMouse[0] && WIIMOTE_IS_CONNECTED(wiiMouse[0])) {
        if (wiiuse_poll(wiiMouse, ONE_WIIMOTE)) {
          if ((wiiMouse[0])->event == WIIUSE_EVENT) {
            eventHandler(wiiMouse[0]);
          }
        }
      }
    }
  }
  wiiuse_cleanup(wiiMouse, ONE_WIIMOTE);
}

//--------------------------------------------------------------
void ofxWiiMouse::eventHandler(wiimote* wm) {
  ofxWiiMouseEvent wmEvent;

	if (WIIUSE_USING_IR(wm)) {
    lock();
      pointerX = wm->ir.x;
      pointerY = wm->ir.y;
    unlock();
	}

	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_A);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_B);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_UP);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_DOWN);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_LEFT);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_RIGHT);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_MINUS);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_PLUS);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_ONE);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_TWO);
	OFXWM_BUTTON_PRESS_EVENT(wm, WIIMOTE_BUTTON_HOME);

	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_A);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_B);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_UP);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_DOWN);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_LEFT);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_RIGHT);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_MINUS);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_PLUS);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_ONE);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_TWO);
	OFXWM_BUTTON_RELEASE_EVENT(wm, WIIMOTE_BUTTON_HOME);

	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_A);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_B);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_UP);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_DOWN);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_LEFT);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_RIGHT);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_MINUS);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_PLUS);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_ONE);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_TWO);
	OFXWM_BUTTON_HELD_EVENT(wm, WIIMOTE_BUTTON_HOME);

}

void ofxWiiMouse::processEvents() {
  static ofxWiiMouseEventArgs wiiMouseEventArgs;
  ofxWiiMouseEvent wmEvent;
  bool moreEvents;

  lock();
    if ((pointerX != oldPointerX) || (pointerY != oldPointerY)) {
      oldPointerX = pointerX;
      oldPointerY = pointerY;

      wiiMouseEventArgs.x = pointerX;
      wiiMouseEventArgs.y = pointerY;

      if (dragButton) {
        wiiMouseEventArgs.button = dragButton;
        ofNotifyEvent(dragged, wiiMouseEventArgs);
      } else {
        wiiMouseEventArgs.button = 0;
        ofNotifyEvent(moved, wiiMouseEventArgs);
      }
    }
  unlock();

  moreEvents = true;
  while (moreEvents) {
    lock();
    if (!wmEvents.empty()) {
      wmEvent = wmEvents.front();
      wmEvents.pop();
    } else {
      moreEvents = false;
    }
    unlock();
    if (moreEvents) {
      switch (wmEvent.event) {
        case OFXWE_CONNECT_WIIMOTE:
          ofNotifyEvent(connect, wiiMouseEventArgs);
          break;
        case OFXWE_WIIMOTE_CONNECTED:
          wiiMouseEventArgs.p0 = wmEvent.arg[0];
          wiiMouseEventArgs.p1 = wmEvent.arg[1];
          ofNotifyEvent(connected, wiiMouseEventArgs);
          break;
        case OFXWE_BUTTON_PRESS:
          wiiMouseEventArgs.button = wmEvent.arg[0];
          ofNotifyEvent(pressed, wiiMouseEventArgs);
          break;
        case OFXWE_BUTTON_RELEASE:
          if (dragButton == wmEvent.arg[0]) {
            dragButton = 0;
          }
          wiiMouseEventArgs.button = wmEvent.arg[0];
          ofNotifyEvent(released, wiiMouseEventArgs);
          break;
        case OFXWE_BUTTON_HELD:
          dragButton = wmEvent.arg[0];
          wiiMouseEventArgs.button = wmEvent.arg[0];
          ofNotifyEvent(held, wiiMouseEventArgs);
          break;
        default:
          ofLogVerbose() << "unknown event";
          break;
      }
    }
  }
}

//--------------------------------------------------------------
void ofxWiiMouse::wiiMouseConnect(ofxWiiMouseEventArgs & wiiMouseEventArgs) {
  ((ofxWiiMouseApp*)ofGetAppPtr())->wiiMouseConnect();
}

//--------------------------------------------------------------
void ofxWiiMouse::wiiMouseConnected(ofxWiiMouseEventArgs & wiiMouseEventArgs) {
  ((ofxWiiMouseApp*)ofGetAppPtr())->wiiMouseConnected(wiiMouseEventArgs.p0, wiiMouseEventArgs.p1);
}

//--------------------------------------------------------------
void ofxWiiMouse::wiiMouseMoved(ofxWiiMouseEventArgs & wiiMouseEventArgs) {
  ((ofxWiiMouseApp*)ofGetAppPtr())->wiiMouseMoved(wiiMouseEventArgs.x, wiiMouseEventArgs.y);
}

void ofxWiiMouse::wiiMouseButtonHeld(ofxWiiMouseEventArgs & wiiMouseEventArgs) {
  ((ofxWiiMouseApp*)ofGetAppPtr())->wiiMouseButtonHeld(wiiMouseEventArgs.x, wiiMouseEventArgs.y, wiiMouseEventArgs.button);
}

//--------------------------------------------------------------
void ofxWiiMouse::wiiMousePressed(ofxWiiMouseEventArgs & wiiMouseEventArgs) {
  ((ofxWiiMouseApp*)ofGetAppPtr())->wiiMousePressed(wiiMouseEventArgs.x, wiiMouseEventArgs.y, wiiMouseEventArgs.button);
}

//--------------------------------------------------------------
void ofxWiiMouse::wiiMouseDragged(ofxWiiMouseEventArgs & wiiMouseEventArgs) {
  ((ofxWiiMouseApp*)ofGetAppPtr())->wiiMouseDragged(wiiMouseEventArgs.x, wiiMouseEventArgs.y, wiiMouseEventArgs.button);
}

//--------------------------------------------------------------
void ofxWiiMouse::wiiMouseReleased(ofxWiiMouseEventArgs & wiiMouseEventArgs) {
  ((ofxWiiMouseApp*)ofGetAppPtr())->wiiMouseReleased(wiiMouseEventArgs.x, wiiMouseEventArgs.y, wiiMouseEventArgs.button);
}

