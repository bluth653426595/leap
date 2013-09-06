#include <iostream>
#include "Leap.h"
#include "SearchListener"
using namespace std;
using namespace Leap;

int main() {
    // Create a sample listener and controller
    SearchListener listener;
    Controller controller;

    // Have the sample listener receive events from the controller
    controller.addListener(listener);

    // Keep this process running until Enter is pressed
    cout << "Press Enter to quit..." << endl;
    cin.get();

    // Remove the sample listener when done
    controller.removeListener(listener);

    return 0;
}

