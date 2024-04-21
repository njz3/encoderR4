#ifndef _ENCODERR4_H_
#define _ENCODERR4_H_

#include "FspTimer.h"

class EncoderR4{
    public:
        EncoderR4(unsigned int pinA, unsigned int pinB);

        bool begin();
        uint16_t read();
        int16_t readChange();
    private:
        unsigned int _pinA;
        unsigned int _pinB;
        FspTimer _timer{};
        uint16_t _lastValue;
};

#endif // _ENCODERR4_H_