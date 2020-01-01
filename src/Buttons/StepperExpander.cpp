#include "StepperExpander.h"

namespace simplebutton {
    StepperExpander::StepperExpander() {
      //  setup(NULL, 255, false);
    }

    StepperExpander::StepperExpander(GPIOExpander* pcf, uint8_t pin) {
     //   setup(pcf, pin, false);
    }

    StepperExpander::StepperExpander(GPIOExpander* pcf, uint8_t pin, bool inverted) {
      //  setup(pcf, pin, inverted);
    }
    StepperExpander::StepperExpander(GPIOExpander* pcf, int pin1,int pin2, int pin3, int pin4, int steps, unsigned long stepdelay)
    {
        setup(pcf, pin1, pin2, pin3, pin4, steps, stepdelay);

    p1=pin1;
    p2=pin2;
    p3=pin3;
    p4=pin4;
 //   pinMode(p1, OUTPUT);
 //   pinMode(p2, OUTPUT);
 //   pinMode(p3, OUTPUT);
 //   pinMode(p4, OUTPUT);
    phase = 0;
    steptime = stepdelay;
    stepsperrev = steps;
    currentstep = 0;
    stepstogo = 0;
    }
    boolean StepperExpander::run()
    {
    if (!stepstogo) // will be true if zero (!false = true)
    {//we're done
        return true;
    } else {
        nextStep();
    }
    }

// Called because we still have to move a stepper. Do a time check to see if
// the next step is due and determine the sequence of phases that we need.
    void StepperExpander::nextStep()
    {
    // Do time check first
    unsigned long time = micros();
        if (time - lastStepTime >= steptime)  //this should work (not overflow), but test.
        {
            if (stepstogo > 0)
            {//clockwise
            stepCW();
            stepstogo--;
            if (stepstogo == 0) stop(); // set pins low. Less humm and less energy.
            }
            else
            {//counter-clockwise
            stepCCW();
            stepstogo++;
            if (stepstogo == 0) stop(); // set pins low. Less humm and less energy.
            }
            lastStepTime = time;
        }
    }

    // Setup a movement. Set stepstogo.
    void StepperExpander::move(int steps){
    powerUp();
    stepstogo = steps;
    }

    // Setup a movement to position. Calculate and set stepstogo. There may be a
    // more elegant way to calculate the shortest route.
    void StepperExpander::moveTo(unsigned int pos){
    powerUp();
    stepstogo = pos - currentstep;
    if (abs(stepstogo) > stepsperrev / 2)
        {
            stepstogo = (stepstogo > 0 ) ? stepstogo - stepsperrev : stepstogo + stepsperrev ;
        }
    }

    // Inherits phase, calls for clockwise movement phase sequence
    void StepperExpander::stepCW()
    {
        switch(phase)//gofromthisphase
        {
        case 0:
            goto7();
        break;
        case 1:
            goto0();
        break;
        case 2:
            goto1();
        break;
        case 3:
            goto2();
        break;
        case 4:
            goto3();
        break;
        case 5:
            goto4();
        break;
            case 6:
            goto5();
        break;
        case 7:
            goto6();
        break;
        default:
            goto0();
        break;
        }

        currentstep++;
        if (currentstep == stepsperrev) currentstep = 0;

        }

    // Inherits phase, calls for counter-clockwise movement phase sequence
    void StepperExpander::stepCCW()
    {
    switch(phase)//gofromthisphase
    {
    case 0:
        goto1();
    break;
    case 1:
        goto2();
    break;
    case 2:
        goto3();
    break;
    case 3:
        goto4();
    break;
    case 4:
        goto5();
    break;
    case 5:
        goto6();
    break;
        case 6:
        goto7();
    break;
    case 7:
        goto0();
    break;
    default:
        goto0();
    break;
    }

    currentstep--;
    if (currentstep < 0) currentstep = stepsperrev - 1;

    }

    //individual steps
    void StepperExpander::goto1()
    {
    pcf->write(p1, LOW);
    pcf->write(p2, LOW);
    pcf->write(p3, LOW);
    pcf->write(p4, HIGH);
    phase=1;
    }
    void StepperExpander::goto2()
    {
    pcf->write(p1, LOW);
    pcf->write(p2, LOW);
    pcf->write(p3, HIGH);
    pcf->write(p4, HIGH);
    phase=2;
    }
    void StepperExpander::goto3()
    {
    pcf->write(p1, LOW);
    pcf->write(p2, LOW);
    pcf->write(p3, HIGH);
    pcf->write(p4, LOW);
    phase=3;
    }
    void StepperExpander::goto4()
    {
    pcf->write(p1, LOW);
    pcf->write(p2, HIGH);
    pcf->write(p3, HIGH);
    pcf->write(p4, LOW);
    phase=4;
    }
    void StepperExpander::goto5()
    {
    pcf->write(p1, LOW);
    pcf->write(p2, HIGH);
    pcf->write(p3, LOW);
    pcf->write(p4, LOW);
    phase=5;
    }
    void StepperExpander::goto6()
    {
    pcf->write(p1, HIGH);
    pcf->write(p2, HIGH);
    pcf->write(p3, LOW);
    pcf->write(p4, LOW);
    phase=6;
    }
    void StepperExpander::goto7()
    {
    pcf->write(p1, HIGH);
    pcf->write(p2, LOW);
    pcf->write(p3, LOW);
    pcf->write(p4, LOW);
    phase=7;
    }
    void StepperExpander::goto0()
    {
    pcf->write(p1, HIGH);
    pcf->write(p2, LOW);
    pcf->write(p3, LOW);
    pcf->write(p4, HIGH);
    phase=0;
    }

    // Returns current step
    int StepperExpander::currentPosition()
    {
    return currentstep;
    }

    // Returns steps to go
    int StepperExpander::stepsToGo()
    {
    return stepstogo;
    }

    // Pulls pins low to save power and avoid heat build up when not moving.
    void StepperExpander::stop()
    {
    stepstogo = 0;
    pcf->write(p1, LOW);
    pcf->write(p2, LOW);
    pcf->write(p3, LOW);
    pcf->write(p4, LOW);
    }

    // Powers pins up again at current phase to get ready to move. This prevents
    // stalling and improves the response to CCW movement, allowing faster speeds
    // (lower step delays)
    void StepperExpander::powerUp()
    {
        switch(phase)//gofromthisphase
        {
        case 0:
            goto0();
        break;
        case 1:
            goto1();
        break;
        case 2:
            goto2();
        break;
        case 3:
            goto3();
        break;
        case 4:
            goto4();
        break;
        case 5:
            goto5();
        break;
            case 6:
            goto6();
        break;
        case 7:
            goto7();
        break;
        default:
            goto0();
        break;
        }
    }
    StepperExpander::~StepperExpander() {}

    //StepperExpander::StepperExpander(GPIOExpander* pcf, int pin1,int pin2, int pin3, int pin4, int steps, unsigned long stepdelay)

    void StepperExpander::setup(GPIOExpander* pcf, uint8_t pin1, uint8_t pin2,  uint8_t pin3, uint8_t pin4, int steps,  unsigned long stepdelay) {
        this->pcf     = pcf;
        this->p1      = pin1;
        this->p2      = pin2;
        this->p3      = pin3;
        this->p4      = pin4;
       // this->steps     = steps;
       // this->stepdelay = stepdelay;
        enable();
    }

    void StepperExpander::enable() {
        button_enabled = true;

        if (pcf) {
            pcf->write(button_pin, 0);
            if (pcf->connected()) button_setup = true;
        }
    }

    int StepperExpander::read() {
        bool currentState = false;

        if (button_enabled && button_setup) {
            currentState = pcf->read(button_pin) > 0;

            if (button_inverted) currentState = !currentState;
        }

        return (int)currentState;
    }

    void StepperExpander::update() {
        if (button_enabled && button_setup) {
            update(read());
        }
    }

    void StepperExpander::update(int state) {
        Button::update(state);
    }
}
