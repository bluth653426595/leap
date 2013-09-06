#include <iostream>
#include <cmath>

#include "search_listener.h"

using namespace std;
using namespace Leap;

void SearchListener::onInit(const Controller& controller)
{
    cout << "Initialized" << endl;
}

void SearchListener::onConnect(const Controller& controller)
{
    cout << "Connected" << endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SearchListener::onDisconnect(const Controller& controller)
{
    //Note: not dispatched when running in a debugger.
    cout << "Disconnected" << endl;
}

void SearchListener::onExit(const Controller& controller)
{
    cout << "Exited" << endl;
}

void SearchListener::onFrame(const Controller& controller)
{
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();

    if (frame.hands().empty())
        return;

    // cout << ", hands: " << frame.hands().count()
    //           << ", fingers: " << frame.fingers().count()
    //           << ", gestures: " << frame.gestures().count() << endl;


    //int LIFE_HIDE = 0;//hand_to_left
    //int LIFE_SHOW = 1;//hand_to_right
    //int FINGER_CIRCLE = 2;
    //int FINGER_TO_LEFT = 3;
    //int FINGER_TO_RIGHT = 4;
    //int FINGER_TO_UP = 5;
    //int FINGER_TO_DOWN = 6;
    //int FINGER_PRESS = 7;
    //int result = -2;

    //Get Screen
    //const ScreenList screen = frame.

    // Get gestures
    const GestureList gestures = frame.gestures();

    for (int g = 0; g < gestures.count(); ++g) {
        Gesture gesture = gestures[g];

        switch (gesture.type()) {
        case Gesture::TYPE_CIRCLE: {
            CircleGesture circle = gesture;

            if (frame.fingers().count() == 1
                && gesture.state() == Gesture::STATE_STOP) {
                emit_dbus_signal("FingerCircle");
            }

            break;
        }
        case Gesture::TYPE_SWIPE: {
            SwipeGesture swipe = gesture;

            Vector start = swipe.startPosition();
            Vector current = swipe.position();

            int fingers_count = frame.fingers().count();
            int gestures_count = frame.gestures().count();
            int hands_count = frame.hands().count();

            if (fingers_count == 1 && gestures_count == 1) {
                if ((start.x - current.x) > 100
                    && abs(start.y - current.y) < 100) {
                    emit_dbus_signal("FingerLeft");

                } else if ((start.x - current.x) < -100
                           && abs(start.y - current.y) < 100) {
                    emit_dbus_signal("FingerRight");
                }

                if ((start.y - current.y) < -300) {
                    emit_dbus_signal("FingerUp");
                } else if ((start.y - current.y) > 300) {
                    emit_dbus_signal("FingerDown");
                }
            } else if (fingers_count > 1 && gestures_count > 1
                       && abs(start.y - current.y) < 100) {
                if ((start.x - current.x) > 100) {
                    emit_dbus_signal("HandLeft");
                } else if ((start.x - current.x) < -100) {
                    emit_dbus_signal("HandRight");
                }
            }

            break;
        }
        case Gesture::TYPE_KEY_TAP: {
            KeyTapGesture tap = gesture;
            int fingers_count = frame.fingers().count();

            if (fingers_count == 1) {
                emit_dbus_signal("FingerClick");
            }

            break;
        }
        default:
            cout << "Unknown gesture type." << endl;
            break;
        }
    }

    /*  if (!frame.hands().empty() || !gestures.empty()) {*/
    //cout << endl;
    //}

}

void SearchListener::onFocusGained(const Controller& controller)
{
    cout << "Focus Gained" << endl;
}

void SearchListener::onFocusLost(const Controller& controller)
{
    cout << "Focus Lost" << endl;
}


void SearchListener::emit_dbus_signal(const string& signame)
{
    DBusConnection* dbus_conn = NULL;
    DBusError dbus_err;
    DBusMessage* msg = NULL;

    dbus_error_init(&dbus_err);
    dbus_conn = dbus_bus_get(DBUS_BUS_SESSION, &dbus_err);

    if (dbus_error_is_set(&dbus_err)) {
        cout << "get conn error : " << dbus_err.message << endl;
        dbus_error_free(&dbus_err);
    }

    if (dbus_conn == NULL) {
        cout << "conn NULL..." << endl;
        return ;
    }

    msg = dbus_message_new_signal("/com/deepin/lsearch",
                                  "com.deepin.lsearch", signame.c_str());

    if (msg == NULL) {
        cout << "msg NULL..." << endl;
        return ;
    }

    //send message
    if (!dbus_connection_send(dbus_conn, msg, NULL)) {
        cout << "Out of Memory !" << endl;
        return ;
    }

    dbus_connection_flush(dbus_conn);
    cout << signame << " Signal has been sent..." << endl;

    dbus_message_unref(msg);
}

