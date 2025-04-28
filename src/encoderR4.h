#ifndef _ENCODERR4_H_
#define _ENCODERR4_H_

#include "FspTimer.h"

class EncoderR4{
    public:
        EncoderR4(unsigned int pinA, unsigned int pinB);

        bool begin();
        uint32_t read();
        int32_t readChange();
    private:
        unsigned int _pinA;
        unsigned int _pinB;
        FspTimer _timer{};
        uint32_t _lastValue;
};

#endif // _ENCODERR4_H_