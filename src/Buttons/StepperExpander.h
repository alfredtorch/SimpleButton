#ifndef SimpleButton_StepperExpander_h
#define SimpleButton_StepperExpander_h

#include "Button.h"
#include "libs/GPIOExpander.h"
#include "libs/PCF8574.h"
#include "libs/PCF8575.h"
#include "libs/MCP23017.h"

namespace simplebutton {
    class StepperExpander : public Button {
        public:
            StepperExpander();
            StepperExpander(GPIOExpander* pcf, uint8_t pin);
            StepperExpander(GPIOExpander* pcf, uint8_t pin, bool inverted);
            StepperExpander(GPIOExpander* pcf, int pin1, int pin2, int pin3, int pin4, int steps, unsigned long stepdelay);
            boolean run();
            void move(int steps);
            void moveToPos(unsigned int pos);
            void moveToRev(unsigned int pos);
            int currentPosition();
            int currentRevolution();
            int stepsToGo();
            void stop();
            virtual ~StepperExpander();

            void setup(GPIOExpander* pcf, uint8_t pin1, uint8_t pin2,  uint8_t pin3, uint8_t pin4, int steps,  unsigned long stepdelay);

            virtual void enable();

            virtual int read();
            virtual void update();
            virtual void update(int state);

        protected:
            GPIOExpander* pcf = NULL;
        private:
            int phase; //the curret phase setp
            int p1; //output pin
            int p2; //output pin
            int p3; //output pin
            int p4; //output pin
            int stepsperrev; //the number of steps in one ref zero indexed.
            int currentstep; //the current step number, zero indexed.
            int currentrev; //the current rev
            int stepstogo;   //the remaining steps to complete the curret movement.
            unsigned long steptime; //the delay time between steps
            unsigned long lastStepTime; //the last step time in microseconds
            void nextStep();   //Called if there are stepstogo (!= 0)
            void stepCW();
            void stepCCW();
            void goto1();
            void goto2();
            void goto3();
            void goto4();
            void goto5();
            void goto6();
            void goto7();
            void goto0();
            void powerUp(); //po    
    };
}

#endif // ifndef SimpleButton_StepperExpander_h