/*
 * ADC_HAL.c
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <HAL/ADC_HAL.h>
/////////////////set threshold for joysticks.
#define LEFT_THRESHOLD 1380
#define RIGHT_THRESHOLD 15000
#define DOWN_THRESHOLD 1000
#define UP_THRESHOLD 15000

#define UP_leftx1 1610///////////for left joysticks
#define UP_lefty1 14900
#define UP_leftx2 5510
#define UP_lefty2 12000

/////////////for right joysticks
#define UP_rightx1 14900
#define UP_righty1 14900
#define UP_rightx2 11000
#define UP_righty2 12000

#define still_THRESHOLDd 1100/////////////for the original opsition threshold
#define still_THRESHOLDu 11000
#define still_THRESHOLDr 10000
#define still_THRESHOLDl 1400


// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    // This sets the conversion clock to 3MHz
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 up to ADC_MEM1. Each conversion will
    // thus use two channels.
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
    // TODO: which one of GPIO_PRIMARY_MODULE_FUNCTION, or
    //                    GPIO_SECONDARY_MODULE_FUNCTION, or
    //                    GPIO_TERTIARY_MODULE_FUNCTION
    // should be used in place of 0 as the last argument?
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    ADC14_configureConversionMemory(ADC_MEM1,
                                   ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                   ADC_INPUT_A9,                 // joystick y
                                   ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);




}





void getSampleJoyStick(unsigned *X, unsigned *Y) {//////////get the numbers from the joystick
    // ADC runs in continuous mode, we just read the conversion buffers
    *X = ADC14_getResult(ADC_MEM0);

    // TODO: Read the Y channel
    *Y = ADC14_getResult(ADC_MEM1);
}


//
//
//
//
Joystick Joystick_construct(){
    Joystick joystick;
    initADC();
    initJoyStick();
    startADC();
 ////////////////////////initialize all states
    joystick.leftState = notleft;
    joystick.rightState = notright;
    joystick.upState = notup;
    joystick.downState = notdown;
    joystick.upleftState = notupleft;
    joystick.uprightState = notupright;
    joystick.stillState = still;
    return(joystick);

}



//////////////////////////////////////////////////////////////////////////////////////
void Joystick_refresh(Joystick *joystick_q) {
    joystick_q->x = ADC14_getResult(ADC_MEM0);

    // TODO: Read the Y channel
    joystick_q->y = ADC14_getResult(ADC_MEM1);




    ///////////////////////////////////////////////////////left

    joystick_q->isTappedToLeft = false;///////////////set the range of left joystick
    if (joystick_q->leftState == notleft)
    {
        if (joystick_q->x < LEFT_THRESHOLD)
        {
            joystick_q->isTappedToLeft = true;
            joystick_q->leftState = left;
        }


    }
    else
    {
        if (joystick_q->x > LEFT_THRESHOLD){
            joystick_q->leftState = notleft;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////set the range of right joystick
    joystick_q->isTappedToRight = false;
    if (joystick_q->rightState == notright)
    {
        if (joystick_q->x < RIGHT_THRESHOLD)
        {
            joystick_q->isTappedToRight = true;
            joystick_q->rightState = right;
        }


    }
    else
    {
        if (joystick_q->x > RIGHT_THRESHOLD){
            joystick_q->rightState = notright;
        }
    }

    /////////////////////////////////////////////////////////////////set the range of down joystick
    joystick_q->isTappedToDown = false;
        if (joystick_q->downState == notdown)
        {
            if (joystick_q->y < DOWN_THRESHOLD)
            {
                joystick_q->isTappedToDown = true;
                joystick_q->downState = down;
            }


        }
        else
        {
            if (joystick_q->y > DOWN_THRESHOLD){
                joystick_q->downState = notdown;
            }
        }

        ///////////////////////////////////////////////////////////////////////set the range of up joystick

        joystick_q->isTappedToUp = false;
        if (joystick_q->upState == notup)
        {
            if (joystick_q->y < UP_THRESHOLD)
            {
                joystick_q->isTappedToUp = true;
                joystick_q->upState = up;
            }


        }
        else
        {
            if (joystick_q->y > UP_THRESHOLD){
                joystick_q->upState = notup;
            }
        }

}
//////////////////////////////////////////////////////////////////////////push button range for each directions
bool Joystick_isPushedToLeft(Joystick * joystick_q) {
    return (joystick_q->x < LEFT_THRESHOLD);

}

bool Joystick_isPushedToRight(Joystick * joystick_q) {
    return (joystick_q->x > RIGHT_THRESHOLD);

}

bool Joystick_isPushedToUp(Joystick * joystick_q) {
    return (joystick_q->y > UP_THRESHOLD);

}

bool Joystick_isStill(Joystick * joystick_q) {
    if ((joystick_q->x < still_THRESHOLDr) && (joystick_q->x > still_THRESHOLDl) && (joystick_q->y < still_THRESHOLDu) && (joystick_q->y > still_THRESHOLDd))
    return true;
    else
        return false;
}

bool Joystick_isPushedToUpleft(Joystick * joystick_q) {
    if((joystick_q->y < UP_lefty1) && (joystick_q->y > UP_lefty2) && (joystick_q->x > UP_leftx1) && (joystick_q->x < UP_leftx2))
    return true;

    else
        return false;

}

bool Joystick_isPushedToUpright(Joystick * joystick_q) {
    if((joystick_q->y < UP_righty1) && (joystick_q->y > UP_righty2) && (joystick_q->x < UP_rightx1) && (joystick_q->x > UP_rightx2))
        return true;

        else
            return false;

}

/////////////////////////////////////////////////////////////////////////////////////////////////tap button
bool Joystick_isTappedToLeft(Joystick * joystick_q){

        return (joystick_q->isTappedToLeft);

}

bool Joystick_isTappedToRight(Joystick * joystick_q){

        return (joystick_q->isTappedToRight);

}

bool Joystick_isTappedToDown(Joystick * joystick_q){

        return (joystick_q->isTappedToDown);

}


bool Joystick_isTappedToUp(Joystick * joystick_q){

        return (joystick_q->isTappedToUp);

}
