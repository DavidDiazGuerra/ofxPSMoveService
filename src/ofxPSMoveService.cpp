#include "ofxPSMoveService.h"

PSMoveEventInfo::PSMoveEventInfo(int idx, PSMButton butt, PSMButtonState butt_state) {
	controller_idx = idx;
	button = butt;
	button_state = butt_state;
}


ofxPSMoveServiceClient::~ofxPSMoveServiceClient() {
	this->close();
}

void ofxPSMoveServiceClient::initialize(const char* server_address, const char* server_port) {
	PSMResult result = PSM_Initialize(server_address, server_port, PSM_DEFAULT_TIMEOUT);
	if (result != PSMResult_Success) ofLog(OF_LOG_WARNING, "Couldn't connect to PSMoveServer.");
}

int ofxPSMoveServiceClient::register_connected_controllers(bool enable_tracking) {
	unsigned int my_flags = PSMStreamFlags_defaultStreamOptions |
		PSMStreamFlags_includeRawSensorData |
		PSMStreamFlags_includeCalibratedSensorData |
		PSMStreamFlags_includePhysicsData;
	if (enable_tracking) {
		my_flags |= PSMStreamFlags_includePositionData;
		tracking_enabled = true;
	}

	PSM_GetControllerList(&controller_list, PSM_DEFAULT_TIMEOUT);
	n_controllers = controller_list.count;

	for (int i = 0; i < n_controllers; i++) {
		PSM_StartControllerDataStream(controller_list.controller_id[i], my_flags, PSM_DEFAULT_TIMEOUT);
		PSM_AllocateControllerListener(controller_list.controller_id[i]);
		controllers.push_back(PSM_GetController(controller_list.controller_id[i]));
		controllers_state.push_back(&controllers[i]->ControllerState.PSMoveState);
		buttons_state.push_back(array<PSMButtonState, N_BUTTONS>());
	}

	return n_controllers;
}

void ofxPSMoveServiceClient::start()
{
	ofAddListener(ofEvents().update, this, &ofxPSMoveServiceClient::update);
}

void ofxPSMoveServiceClient::close() {
	for (int i = 0; i < controller_list.count; i++) {
		PSM_FreeControllerListener(controller_list.controller_id[i]);
	}
	PSM_Shutdown();
	ofRemoveListener(ofEvents().update, this, &ofxPSMoveServiceClient::update);
}

PSMButtonState ofxPSMoveServiceClient::get_button_state(int controller, PSMButton button) {
	return buttons_state[controller][button];
}

unsigned char ofxPSMoveServiceClient::get_trigger_value(int controller)
{
	return controllers_state[controller]->TriggerValue;
}

ofVec3f ofxPSMoveServiceClient::get_raw_accelerometer(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->RawSensorData.Accelerometer);
}

ofVec3f ofxPSMoveServiceClient::get_raw_gyroscope(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->RawSensorData.Gyroscope);
}

ofVec3f ofxPSMoveServiceClient::get_raw_magnetometer(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->RawSensorData.Magnetometer);
}

ofVec3f ofxPSMoveServiceClient::get_calibrated_accelerometer(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->CalibratedSensorData.Accelerometer);
}

ofVec3f ofxPSMoveServiceClient::get_calibrated_gyroscope(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->CalibratedSensorData.Gyroscope);
}

ofVec3f ofxPSMoveServiceClient::get_calibrated_magnetometer(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->CalibratedSensorData.Magnetometer);
}

ofVec3f ofxPSMoveServiceClient::get_position(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->Pose.Position);
}

ofVec4f ofxPSMoveServiceClient::get_orientation(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->Pose.Orientation);
}

ofVec3f ofxPSMoveServiceClient::get_linear_velocity(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->PhysicsData.LinearVelocityCmPerSec);
}

ofVec3f ofxPSMoveServiceClient::get_linear_acceleration(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->PhysicsData.LinearAccelerationCmPerSecSqr);
}

ofVec3f ofxPSMoveServiceClient::get_angular_velocity(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->PhysicsData.AngularVelocityRadPerSec);
}

ofVec3f ofxPSMoveServiceClient::get_angular_acceleration(int controller)
{
	return psmVec_to_ofVec(controllers_state[controller]->PhysicsData.AngularAccelerationRadPerSecSqr);
}

bool ofxPSMoveServiceClient::is_orientation_valid(int controller)
{
	return controllers_state[controller]->bIsOrientationValid;
}

bool ofxPSMoveServiceClient::is_position_valid(int controller)
{
	return controllers_state[controller]->bIsPositionValid;
}

float ofxPSMoveServiceClient::get_rumble_fraction(int controller) 
{
	float fraction;
	PSM_GetControllerRumble(controller, PSMControllerRumbleChannel_All, &fraction);
	return fraction;
}

void ofxPSMoveServiceClient::set_rumble_fraction(int controller, float fraction)
{
	PSM_SetControllerRumble(controller, PSMControllerRumbleChannel_All, fraction);
}

void ofxPSMoveServiceClient::set_LED_color(int controller, unsigned char r, unsigned char g, unsigned char b)
{
	if (tracking_enabled) ofLog(OF_LOG_WARNING, "Overriding the tracking LED will stop the tracking.");
	PSM_SetControllerLEDOverrideColor(controller, r, g, b);
}

void ofxPSMoveServiceClient::set_LED_color(int controller, ofColor color)
{
	set_LED_color(controller, color.r, color.g, color.b);
}

void ofxPSMoveServiceClient::reset_LED_color(int controller)
{
	set_LED_color(controller, 0, 0, 0);
}

void ofxPSMoveServiceClient::reset_orientation(int controller, ofVec4f orientation)
{
	PSM_ResetControllerOrientation(controller, &ofVec_to_psmVec(orientation), PSM_DEFAULT_TIMEOUT);
}

void ofxPSMoveServiceClient::reset_orientation(int controller)
{
	reset_orientation(controller, ofVec4f(0, 0, 0, 1));
}

void ofxPSMoveServiceClient::update(ofEventArgs& args) {
	PSM_Update();

	for (int controller = 0; controller < n_controllers; controller++) {
		for (int button = 0; button < N_BUTTONS; button++) {
			if (button_pressed(controller, (PSMButton)button)) {
				buttons_state[controller][button] = PSMButtonState_PRESSED;
				PSMoveEventInfo eventInfo = PSMoveEventInfo(controller, (PSMButton)button, PSMButtonState_PRESSED);
				ofNotifyEvent(buttonPressedEvent, eventInfo);
			}
			else if (get_button_server_state(controller, (PSMButton)button) == PSMButtonState_DOWN) {
				buttons_state[controller][button] = PSMButtonState_DOWN;
			}
			else if (button_released(controller, (PSMButton)button)) {
				buttons_state[controller][button] = PSMButtonState_RELEASED;
				PSMoveEventInfo eventInfo = PSMoveEventInfo(controller, (PSMButton)button, PSMButtonState_RELEASED);
				ofNotifyEvent(buttonReleasedEvent, eventInfo);
			}
			else if (get_button_server_state(controller, (PSMButton)button) == PSMButtonState_UP) {
				buttons_state[controller][button] = PSMButtonState_UP;
			}
		}
	}
}

inline PSMButtonState ofxPSMoveServiceClient::get_button_server_state(int controller, PSMButton button) {
	switch (button) {
	case PSMOVE_TRIANGLE_BUTTON: 	return controllers_state[controller]->TriangleButton;
	case PSMOVE_CIRCLE_BUTTON:		return controllers_state[controller]->CircleButton;
	case PSMOVE_CROSS_BUTTON:		return controllers_state[controller]->CrossButton;
	case PSMOVE_SQUARE_BUTTON: 		return controllers_state[controller]->SquareButton;
	case PSMOVE_SELECT_BUTTON:		return controllers_state[controller]->SelectButton;
	case PSMOVE_START_BUTTON:		return controllers_state[controller]->StartButton;
	case PSMOVE_PS_BUTTON:			return controllers_state[controller]->PSButton;
	case PSMOVE_MOVE_BUTTON:		return controllers_state[controller]->MoveButton;
	case PSMOVE_TRIGGER_BUTTON:		return controllers_state[controller]->TriggerButton;
	}
}

inline bool ofxPSMoveServiceClient::button_pressed(int controller, PSMButton button) {
	return (get_button_server_state(controller, button) == PSMButtonState_PRESSED) ||
		(get_button_server_state(controller, button) == PSMButtonState_DOWN && buttons_state[controller][button] == PSMButtonState_UP);
}

inline bool ofxPSMoveServiceClient::button_released(int controller, PSMButton button) {
	return (get_button_server_state(controller, button) == PSMButtonState_RELEASED) ||
		(get_button_server_state(controller, button) == PSMButtonState_UP && buttons_state[controller][button] == PSMButtonState_DOWN);
}

inline ofVec2f ofxPSMoveServiceClient::psmVec_to_ofVec(PSMVector2f vector)
{
	return ofVec2f(vector.x, vector.y);
}

inline ofVec3f ofxPSMoveServiceClient::psmVec_to_ofVec(PSMVector3f vector)
{
	return ofVec3f(vector.x, vector.y, vector.z);
}

inline ofVec3f ofxPSMoveServiceClient::psmVec_to_ofVec(PSMVector3i vector)
{
	return ofVec3f(vector.x, vector.y, vector.z);
}

inline ofVec4f ofxPSMoveServiceClient::psmVec_to_ofVec(PSMQuatf vector)
{
	return ofVec4f(vector.x, vector.y, vector.z, vector.w);
}

inline PSMQuatf ofxPSMoveServiceClient::ofVec_to_psmVec(ofVec4f vector)
{
	PSMQuatf quat;
	quat.w = vector.w;
	quat.x = vector.x;
	quat.y = vector.y;
	quat.z = vector.z;
	return quat;
}
