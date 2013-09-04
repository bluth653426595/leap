/******************************************************************************\
* Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include "Leap.h"
#include "cmath"
#include <dbus/dbus.h>
using namespace std;
using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    void emit_dbus_signal(const char *signame );
};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  //Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  // std::cout << ", hands: " << frame.hands().count()
  //           << ", fingers: " << frame.fingers().count()
  //           << ", gestures: " << frame.gestures().count() << std::endl;


    int LEAP_INVALIABLE = -1;
    int LIFE_HIDE = 0;//hand_to_left
    int LIFE_SHOW = 1;//hand_to_right
    int FINGER_CIRCLE = 2;
    int FINGER_TO_LEFT = 3;
    int FINGER_TO_RIGHT = 4;
    int FINGER_TO_UP = 5;
    int FINGER_TO_DOWN = 6;
    int FINGER_PRESS = 7;


    int result = -2;

 // Get gestures
  const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        // std::cout <<"circle----------";
        CircleGesture circle = gesture;
        std::string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        // Calculate angle swept since last frame
        // float sweptAngle = 0;
        // if (circle.state() != Gesture::STATE_START) {
        //   CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
        //   sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        // }
        // float angle = sweptAngle * RAD_TO_DEG;
        // std::cout << "angle: " << angle << std::endl;

        if(frame.fingers().count() == 1)
        {
          // std::cout << "fingers circle " << std::endl;
          result = FINGER_CIRCLE;
          emit_dbus_signal("FINGER_CIRCLE");
        }

        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        // std::cout <<"swipe,";
        SwipeGesture swipe = gesture;
        // std::cout << "startPosition:" << swipe.startPosition() << std::endl;
        // std::cout << "currentPosition:" << swipe.position() << std::endl;

        float start_x = swipe.startPosition().x;
        float current_x = swipe.position().x;

        float start_y = swipe.startPosition().y;
        float current_y = swipe.position().y;

        int fingers_count = frame.fingers().count();
        int gestures_count = frame.gestures().count();
        int hands_count = frame.hands().count();

        if( hands_count > -1 && fingers_count == 1 && gestures_count == 1 )
        {
          // std::cout << "fingers------" ;
          if((start_x - current_x) > 100 && std::abs(start_y - current_y) < 100)
          {
            // std::cout << "finger to left " << std::endl;
            result = FINGER_TO_LEFT;
            emit_dbus_signal("FINGER_TO_LEFT");

          }
          else if ((start_x - current_x) < -100 && std::abs(start_y - current_y) < 100){
            // std::cout << "finger to right " << std::endl;
            result = FINGER_TO_RIGHT;
            emit_dbus_signal("FINGER_TO_RIGHT");
          }

          if((start_y - current_y) < -100)
          {
            // std::cout << "finger to up " << std::endl;
            result = FINGER_TO_UP;
            emit_dbus_signal("FINGER_TO_UP");
          }
          else if ((start_y - current_y) > 100){
            // std::cout << "finger to down " << std::endl;
            result = FINGER_TO_DOWN;
            emit_dbus_signal("FINGER_TO_DOWN");
          }

        }
        else if (hands_count > 0 && fingers_count > 1 && gestures_count > 1)
        {
          // std::cout << "hand-------" ;
          if((start_x - current_x) > 100 && std::abs(start_y - current_y) < 100)
          {
            // std::cout << "hand to left " << std::endl;
            result = LIFE_HIDE;
            emit_dbus_signal("LIFE_HIDE");
          }
          else if ((start_x - current_x) < -100 && std::abs(start_y - current_y) < 100){
            // std::cout << "hand to right " << std::endl;
            result = LIFE_SHOW;
            emit_dbus_signal("LIFE_SHOW");
          }

        }

        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        // std::cout <<"key_tap";
        KeyTapGesture tap = gesture;
        result = FINGER_PRESS;
        emit_dbus_signal("FINGER_PRESS");
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        std::cout <<"screen_tap";
        ScreenTapGesture screentap = gesture;
        break;
      }
      default:
        std::cout << "Unknown gesture type." << std::endl;
        result = LEAP_INVALIABLE;
        // emit_dbus_signal("LEAP_INVALIABLE");
        break;
    }
  }

  if (!frame.hands().empty() || !gestures.empty()) {
    std::cout << std::endl;
  }

  
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

int main() {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}

void SampleListener::emit_dbus_signal(const char *signame )
{
    DBusConnection *dbus_conn = NULL;
    DBusError dbus_err;
    DBusMessage *msg = NULL;
    // DBusMessageIter args;

    dbus_error_init(&dbus_err);
    dbus_conn = dbus_bus_get(DBUS_BUS_SESSION, &dbus_err);

    if ( dbus_error_is_set(&dbus_err) ) {
        cout<<"get conn error : "<<dbus_err.message<<endl;
        dbus_error_free(&dbus_err);
    }

    if ( dbus_conn == NULL ) {
        cout<<"conn NULL...\n"<<endl;
        return ;
    }

    msg = dbus_message_new_signal ("/com/deepn/lsearch", 
      "com.deepn.lsearch", signame);

    if ( msg == NULL ) {
        cout<<"msg NULL...\n"<<endl;
        return ;
    }

    //send message
    if ( !dbus_connection_send(dbus_conn, msg, NULL) ) {
        cout<<"Out of Memory !"<<endl;
        return ;
    }

    dbus_connection_flush(dbus_conn);
    cout<< signame<<" Signal has been sent..."<<endl;

    dbus_message_unref(msg);
}