#ifndef SimpleButton_LedExpander_h
#define SimpleButton_LedExpander_h

#include "Button.h"
#include "libs/GPIOExpander.h"
#include "libs/PCF8574.h"
#include "libs/PCF8575.h"
#include "libs/MCP23017.h"

enum blink_t {BLINK_ON, BLINK_OFF};

namespace simplebutton {
    class LedExpander : public Button {
        public:
            LedExpander();
            LedExpander(GPIOExpander* pcf, uint8_t pin);
            LedExpander(GPIOExpander* pcf, uint8_t pin, bool inverted);
            LedExpander(int pin);
            void blink(int on_for, int off_for);
            void blink();
            void setOnTime(int ms);
            void setOffTime(int ms);
            void setBlinkState(blink_t state);
            void blinkDelay(int d);
            void blinkOff();
            virtual ~LedExpander();

            void setup(GPIOExpander* pcf, uint8_t pin, bool inverted);

            virtual void enable();
            virtual int read();
            virtual void update();
            virtual void update(int state);
        private:
            unsigned long m_blinkTime;
            int m_onForTime;
            int m_offForTime;
            blink_t m_blinkState;
            short m_pin;
            int m_resolution;

            void reset();

        protected:
            GPIOExpander* pcf = NULL;
    };
}

#endif // ifndef SimpleButton_LedExpander_h