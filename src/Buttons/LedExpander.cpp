#include "LedExpander.h"

namespace simplebutton {
    LedExpander::LedExpander() {
        setup(NULL, 255, false);
    }

    LedExpander::LedExpander(GPIOExpander* pcf, uint8_t pin) {
        setup(pcf, pin, false);
    }

    LedExpander::LedExpander(GPIOExpander* pcf, uint8_t pin, bool inverted) {
        setup(pcf, pin, inverted);
    }

    LedExpander::~LedExpander() {}

    void LedExpander::setup(GPIOExpander* pcf, uint8_t pin, bool inverted) {
        this->pcf             = pcf;
        this->button_pin      = pin;
        this->button_inverted = inverted;
        enable();
    }

    void LedExpander::enable() {
        button_enabled = true;

        if (pcf) {
            pcf->write(button_pin, 0);
            if (pcf->connected()) button_setup = true;
        }
    }

    int LedExpander::read() {
        bool currentState = false;

        if (button_enabled && button_setup) {
            currentState = pcf->read(button_pin) > 0;

            if (button_inverted) currentState = !currentState;
        }

        return (int)currentState;
    }
    void LedExpander::reset() {
        m_blinkTime   = 0UL;
        m_onForTime   = -1;
        m_offForTime  = -1;
        m_blinkState  = BLINK_OFF;
        m_resolution  = 100;
    }
    LedExpander::LedExpander(int pin) {
        m_pin = pin;
        reset();
    }
    void LedExpander::setOnTime(int ms) {
        if (ms>0) { // no=op if ms is <= 0
            m_onForTime = ms;
            if (m_offForTime>0) {
            m_resolution = min(m_onForTime,m_offForTime)/10;
            }
        }
    }
    void LedExpander::setOffTime(int ms) {
        if (ms>0) { // no=op if ms is <= 0
            m_offForTime = ms;
            if (m_onForTime>0) {
            m_resolution = min(m_onForTime,m_offForTime);
            }
        }
    }
    void LedExpander::setBlinkState(blink_t state)
    {
        pcf->write(m_pin, (state==BLINK_ON) ? HIGH : LOW);
        //digitalWrite(m_pin, (state==BLINK_ON) ? HIGH : LOW);
        m_blinkState = state;
        m_blinkTime  = millis();
    }
    void LedExpander::blink(int on_for, int off_for) {
        unsigned long ct = millis();
        if (m_blinkTime==0UL) m_blinkTime=ct;
        unsigned long diff = abs(ct - m_blinkTime);
        short set_to = -1;

        setOnTime(on_for);
        setOffTime(off_for);

        if (m_blinkState==BLINK_OFF) {
            if (m_offForTime>0 && diff>m_offForTime) {
            setBlinkState(BLINK_ON);
            }
        } else {
            if (m_onForTime>0 && diff>m_onForTime) {
            setBlinkState(BLINK_OFF);
            }
        }
    }
    /*
    * Call often to blink.
    */
    void LedExpander::blink() {
    blink(-1,-1);
    }

    /*
    * Equivalent to delay(d), but updates the blink.
    */
    void LedExpander::blinkDelay(int d)
    {
    unsigned long ct = millis();
    while (millis()-ct<d) {
        blink();
        delay(m_resolution);
    }
    }

    /*
    * Turns off the blink.
    */
    void LedExpander::blinkOff() {
    reset();
    pcf->write(m_pin, (state==BLINK_ON) ? HIGH : LOW);

    //digitalWrite(m_pin, LOW);
    }

    void LedExpander::update() {
        if (button_enabled && button_setup) {
            update(read());
        }
    }

    void LedExpander::update(int state) {
        Button::update(state);
    }
}
