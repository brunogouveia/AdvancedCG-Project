/*
 * Inpuc.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: Bruno Gouveia
 */

#include "Input.h"

KeyCode KeyCode::Esc(27);
KeyCode KeyCode::UpArrow(256);
KeyCode KeyCode::DownArrow(257);
KeyCode KeyCode::LeftArrow(258);
KeyCode KeyCode::RightArrow(259);
KeyCode KeyCode::PageUp(260);
KeyCode KeyCode::PageDown(261);

/**
 * KeyCode constructor
 */
KeyCode::KeyCode(unsigned int v) :
		value(v) {
}

std::vector<bool> Input::keyMap;
std::vector<bool> Input::keyUpMap;
std::vector<bool> Input::keyDownMap;

void Input::init() {
	keyMap.resize(262);
	keyUpMap.resize(262);
	keyDownMap.resize(262);

	for (int i = 0; i < 256; i++) {
		keyMap[i] = false;
		keyDownMap[i] = false;
		keyUpMap[i] = false;
	}
}

/**
 * Returns true during the frame the user starts pressing down the key identified by name.
 */
bool Input::getKeyDown(KeyCode key) {
	return keyDownMap[key.value];
}

/**
 * Returns true during the frame the user releases the key identified by name.
 */
bool Input::getKeyUp(KeyCode key) {
	return keyUpMap[key.value];
}

/**
 * Returns true while the user holds down the key identified by name. Think auto fire.
 */
bool Input::getKey(KeyCode key) {
	return keyMap[key.value];
}

void Input::setKey(KeyCode key) {
	if (!keyDownMap[key.value])
		keyDownMap[key.value] = true;
	keyUpMap[key.value] = false;
	keyMap[key.value] = true;
}

void Input::resetMaps() {
	for (unsigned int i = 0; i < keyMap.size(); ++i) {
		if (keyMap[i])
			keyUpMap[i] = true;
		keyMap[i] = false;
	}
}

void Input::updateDownMap() {
	for (unsigned int i = 0; i < keyMap.size(); ++i) {
		// Check if is second frame that key is pressed
		if (keyMap[i] && keyDownMap[i]) {
			keyDownMap[i] = false;
		}
	}
}
