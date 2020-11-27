# ofxPSMoveService (BETA)
PSMoveService client to use PSMove controllers in openFrameworks apps

In order to use it, you need to have a PSMoveService running with the controllers paired. It can be in the computer that is running the OF app or in other one in the same network.

The addon is still in beta stage, it works only on Windows systems and compiled for x64 plataform. I haven't written a nice documentation yet, but you can find the list of the public functions in [ofxPSMoveService.h](https://github.com/DavidDiazGuerra/ofxPSMoveService/blob/master/src/ofxPSMoveService.h), I think their names are obvious enough. It allows you to read the information about the state of the PSMove controllers connected to the PSMoveService, including their position if you have trackers running with your server, and modify the color of their LEDs and make them rumble.

## Example
The example included with the addon should show the status of the controller as in the following image:

<img src="example/screenshot.png" title="Example screenshot">

If you have several controllers, you can select which one you want to see in the screen with the arrow keys. The position information will be "NO VALID" if you don't have trackers or they aren't properly calibrated, but will work fine if you have them. I couldn't get the linear and angular velocity and acceleration yet.

In addition, if you press the Move button of your controller its LED should turn white, the trigger should activate (and regulate) the rumbling, and pressing the SELECT button should reset the orientation to the "identity" pose (pointing down the -Z axis).
