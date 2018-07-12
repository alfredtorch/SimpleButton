#ifndef ButtonPullup_h
#define ButtonPullup_h

#include "Button.h"

namespace simpleButton {
    class ButtonPullup : public Button {
        public:
            ButtonPullup();
            ButtonPullup(uint8_t pin);
            ~ButtonPullup();

            void enable();
            void update();

            bool isEnabled();

        private:
            uint8_t pin = 255;
    };
}

#endif // ifndef ButtonPullup_h