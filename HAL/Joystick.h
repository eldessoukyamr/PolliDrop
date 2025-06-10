/*
 * Joystick.h
 *
 *  Created on: Nov 2, 2023
 *      Author: desok
 */

#ifndef HAL_JOYSTICK_H_
#define HAL_JOYSTICK_H_

#define UP_threshold 14000
#define DOWN_threshold 2000
#define RIGHT_threshold 10000
#define LEFT_threshold 4000

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
enum _JoystickDebounceState { UP, neutral, DOWN };
typedef enum _JoystickDebounceState JoystickDebounceState;
struct _Joystick
{
    uint_fast16_t x;
    uint_fast16_t y;
bool reset;
JoystickDebounceState State;
};
typedef struct _Joystick Joystick;

/** Constructs a new joystick object, given a valid port and pin. */
Joystick Joystick_construct();

/** Given a joystick, determines if the joystick is currently pressed to the left */
bool Joystick_isPressedToUP(Joystick* Joystick);

/** Given a joystick, determines if it was "tapped" to the left - went from middle to the left */
bool Joystick_isPressedToDOWN(Joystick* Joystick);

/** Refreshes this joystick so the joystick FSM now has new outputs to interpret */
void Joystick_refresh(Joystick* Joystick);


#endif /* HAL_JOYSTICK_H_ */
