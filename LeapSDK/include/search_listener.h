#ifndef _SEARCH_LISENTER_H_
#define _SEARCH_LISENTER_H_

#include <string>

#include "Leap.h"
#include <dbus/dbus.h>

class SearchListener : public Leap::Listener {
public:
    virtual void onInit(const Leap::Controller&);
    virtual void onConnect(const Leap::Controller&);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onExit(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
    virtual void onFocusGained(const Leap::Controller&);
    virtual void onFocusLost(const Leap::Controller&);
    void emit_dbus_signal(const std::string& signame);
};

#endif /* end of include guard: _SEARCH_LISENTER_H_ */

