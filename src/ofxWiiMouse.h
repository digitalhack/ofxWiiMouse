#pragma once

#include <queue>

#include "ofMain.h"
#include "wiiuse.h"

#define ONE_WIIMOTE 1

#define OFXWE_CONNECT_WIIMOTE 1
#define OFXWE_WIIMOTE_CONNECTED 2
#define OFXWE_BUTTON_PRESS 3
#define OFXWE_BUTTON_RELEASE 4
#define OFXWE_BUTTON_HELD 5

#define OFXWM_BUTTON_PRESS_EVENT(d, b)  if (IS_JUST_PRESSED(d, b)) { \
                                                wmEvent.event = OFXWE_BUTTON_PRESS; \
                                                wmEvent.arg[0] = b; \
		                                            lock(); \
                                                wmEvents.push(wmEvent); \
                                                unlock(); \
                                              }

#define OFXWM_BUTTON_RELEASE_EVENT(d, b)  if (IS_RELEASED(d, b)) { \
                                                  wmEvent.event = OFXWE_BUTTON_RELEASE; \
                                                  wmEvent.arg[0] = b; \
		                                              lock(); \
                                                  wmEvents.push(wmEvent); \
                                                  unlock(); \
                                                }

#define OFXWM_BUTTON_HELD_EVENT(d, b)  if (IS_HELD(d, b)) { \
                                                  wmEvent.event = OFXWE_BUTTON_HELD; \
                                                  wmEvent.arg[0] = b; \
		                                              lock(); \
                                                  wmEvents.push(wmEvent); \
                                                  unlock(); \
                                                }

struct ofxWiiMouseEvent {
  int event;
  int arg[3];
};

struct ofxWiiMouseEventArgs {
  union {
    struct {
      int x;
      int y;
      int button;
    };
    struct {
      int p0;
      int p1;
      int p2;
    };
  };
};

class ofxWiiMouseApp : public ofBaseApp {
  public:
    virtual void wiiMouseConnect() {}
    virtual void wiiMouseConnected(int connected, int found) {}
    virtual void wiiMouseMoved(int x, int y) {}
    virtual void wiiMousePressed(int x, int y, int button) {}
    virtual void wiiMouseReleased(int x, int y, int button) {}
    virtual void wiiMouseButtonHeld(int x, int y, int button) {}
    virtual void wiiMouseDragged(int x, int y, int button) {}
};

class ofxWiiMouse : public ofThread {
  public:
    ofxWiiMouse();
    ~ofxWiiMouse();
    void processEvents();

    ofEvent<ofxWiiMouseEventArgs> connect;
    ofEvent<ofxWiiMouseEventArgs> connected;
    ofEvent<ofxWiiMouseEventArgs> moved;
    ofEvent<ofxWiiMouseEventArgs> pressed;
    ofEvent<ofxWiiMouseEventArgs> released;
    ofEvent<ofxWiiMouseEventArgs> held;
    ofEvent<ofxWiiMouseEventArgs> dragged;

  private:
    wiimote** wiiMouse;
    int pointerX, pointerY;
    int oldPointerX, oldPointerY;
    int dragButton;
    std::queue<ofxWiiMouseEvent> wmEvents;

    void threadedFunction();
    void eventHandler(wiimote* wm);

    void wiiMouseConnect(ofxWiiMouseEventArgs & wiiMouseEventArgs);
    void wiiMouseConnected(ofxWiiMouseEventArgs & wiiMouseEventArgs);
    void wiiMouseMoved(ofxWiiMouseEventArgs & wiiMouseEventArgs);
    void wiiMousePressed(ofxWiiMouseEventArgs & wiiMouseEventArgs);
    void wiiMouseReleased(ofxWiiMouseEventArgs & wiiMouseEventArgs);
    void wiiMouseButtonHeld(ofxWiiMouseEventArgs & wiiMouseEventArgs);
    void wiiMouseDragged(ofxWiiMouseEventArgs & wiiMouseEventArgs);
};
