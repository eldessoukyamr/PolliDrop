/*
 * joystick.c
 *
 *  Created on: Nov 2, 2023
 *      Author: desok
 */
#include <HAL/Joystick.h>
#define UP_threshold 9100
#define DOWN_threshold 7200
// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    ADC14_initModule(ADC_CLOCKSOURCE_SYSOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                     0
    );

    // This configures the ADC to store output results
    // in ADC_MEM0 for joystick X.
    // Todo: if we want to add joystick Y, then, we have to use more memory locations
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
    // Starts the ADC with the first conversion
    // in repeat-mode, subsequent conversions run automatically
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A15,                 // joystick X
                                    ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM1,
                                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A9,                 // joystick y
                                    ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    // should be used in place of 0 as the last argument?
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


}
/**
 * Constructs a joystick
 * Initializes the output FSMs.
 *

 */
Joystick Joystick_construct()
{
    // The joystick object which will be returned at the end of construction
    Joystick joystick;

    initADC();
    initJoyStick();
    startADC();

    // Initialize all buffered outputs of the joystick
    //    joystick.pushState = RELEASED;
    //    joystick.isTapped = false;

    // Return the constructed joystick object to the user
    return joystick;
}
/**
 * pin value and advancing the debouncing FSM by one step.
 *
 * @param joystcik:   The joystcik object to refresh
 */
void Joystick_refresh(Joystick* joystick_p)
{
    joystick_p->x = ADC14_getResult(ADC_MEM0);
    joystick_p->y = ADC14_getResult(ADC_MEM1);


    if(!joystick_p->reset&&joystick_p->y> UP_threshold){
              joystick_p->State = UP;
          }
    else if (!joystick_p->reset&&joystick_p->y< DOWN_threshold){
                  joystick_p->State = DOWN;
          }
    else if (joystick_p->reset && joystick_p->y < UP_threshold && joystick_p->y > DOWN_threshold){
                  joystick_p->State = neutral;
                  joystick_p->reset = false;
              }

}

//to detrmine if joystick is pressed
bool Joystick_isPressedToUP(Joystick* joystick_p){
bool result = false;
if(!joystick_p->reset&&joystick_p->State == UP)
{
    result = true;
    joystick_p->reset = true;
}
return result;
}
/** Given a joystick, determines if it was "tapped" to the DOWN - went from middle to the DOWN */
bool Joystick_isPressedToDOWN(Joystick* joystick_p){
    bool result = false;
    if(!joystick_p->reset&&joystick_p->State == DOWN)
    {
        result = true;
        joystick_p->reset = true;
    }
    return result;

}
