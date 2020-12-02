#pragma once

#include "PSMoveClient_CAPI.h"
#include "ClientGeometry_CAPI.h"
#include "ofMain.h"


static const int N_BUTTONS = 9;

typedef enum {
	PSMOVE_TRIANGLE_BUTTON,
	PSMOVE_CIRCLE_BUTTON,
	PSMOVE_CROSS_BUTTON,
	PSMOVE_SQUARE_BUTTON,
	PSMOVE_SELECT_BUTTON,
	PSMOVE_START_BUTTON,
	PSMOVE_PS_BUTTON,
	PSMOVE_MOVE_BUTTON,
	PSMOVE_TRIGGER_BUTTON
} PSMButton;


class PSMoveEventInfo {
public:
	PSMoveEventInfo(int idx, PSMButton butt, PSMButtonState butt_state);

	int controller_idx;
	PSMButton button;
	PSMButtonState button_state;
};


class ofxPSMoveServiceClient {
public:
	~ofxPSMoveServiceClient();

	/** \brief Initializes a connection to PSMoveService.
	Initializes the connection to PSMService at the given address and port.
	This function must be called before calling any other client functions.

	\param server_address The address that PSMoveService is running at, "localhost" by default.
	\param port The port that PSMoveSerive is running at, 9512 by default.
	*/
	void initialize(const char* server_address = "localhost", const char* server_port = PSMOVESERVICE_DEFAULT_PORT);
	
	/** \brief Register all the controllers available at PSMoveService.
	Start a data stream of each controller including information about the sensors (raw and 
	calibrated) and the linear and angular velocity and acceleration. If enable_tracking is
	true, it turns on tracking lights and also includes the information about the pose and 
	the position of the controllers.
	
	\param enable_tracking Include pose and position information (turns on tracking lights).
	*/
	int register_connected_controllers(bool enable_tracking = false);

	/** \brief Start updating the state of the registered controllers.
	Start updating the state of the registered controllers. 
	*/
	void start();

	/** \brief Close the connection to PSMoveService.
	Close the connection to PSMoveService.
	*/
	void close();

	PSMButtonState get_button_state(int controller, PSMButton button);
	unsigned char get_trigger_value(int controller);

	ofVec3f get_raw_accelerometer(int controller);
	ofVec3f get_raw_gyroscope(int controller);
	ofVec3f get_raw_magnetometer(int controller);

	ofVec3f get_calibrated_accelerometer(int controller);
	ofVec3f get_calibrated_gyroscope(int controller);
	ofVec3f get_calibrated_magnetometer(int controller);

	ofVec3f get_position(int controller);
	ofVec4f get_orientation(int controller);

	ofVec3f get_orientationAngles(int controller);
	ofVec3f get_navigationAngles(int controller);
	ofVec3f get_orientationVector(int controller);

	ofVec3f get_linear_velocity(int controller);		// cm/s
	ofVec3f get_linear_acceleration(int controller);	// cm/s2
	ofVec3f get_angular_velocity(int controller);		// rad/s
	ofVec3f get_angular_acceleration(int controller);	// rad/s2

	bool is_orientation_valid(int controller);
	bool is_position_valid(int controller);

	float get_rumble_fraction(int controller);
	void set_rumble_fraction(int controller, float fraction);

	void set_LED_color(int controller, unsigned char r, unsigned char g, unsigned char b);
	void set_LED_color(int controller, ofColor color);
	void reset_LED_color(int controller);

	void reset_orientation(int controller, ofVec4f orientation);
	void reset_orientation(int controller);

	ofEvent<PSMoveEventInfo> buttonPressedEvent;
	ofEvent<PSMoveEventInfo> buttonReleasedEvent;

	int n_controllers = 0;
	bool tracking_enabled = false;

private:
	void update(ofEventArgs& args);

	inline PSMButtonState get_button_server_state(int controller, PSMButton button);
	inline bool button_pressed(int controller, PSMButton button);
	inline bool button_released(int controller, PSMButton button);

	inline ofVec2f psmVec_to_ofVec(PSMVector2f vector);
	inline ofVec3f psmVec_to_ofVec(PSMVector3f vector);
	inline ofVec3f psmVec_to_ofVec(PSMVector3i vector);
	inline ofVec4f psmVec_to_ofVec(PSMQuatf vector);
	
	inline PSMQuatf ofVec_to_psmVec(ofVec4f vector);

	ofVec3f quaternion_to_euler(ofVec4f q);

	PSMControllerList controller_list;
	vector<PSMController*> controllers;
	vector<PSMPSMove*> controllers_state;

	vector<array<PSMButtonState, N_BUTTONS>> buttons_state;

};

