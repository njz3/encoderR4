
#include "encoderR4.h"

#include <variant.h>

static const auto CHANNEL_COUNT = 14;
static R_GPT0_Type * channel2GPT[CHANNEL_COUNT] = {
    R_GPT0, R_GPT1, R_GPT2, R_GPT3, R_GPT4, 
    R_GPT5, R_GPT6, R_GPT7, R_GPT8, R_GPT9,
    R_GPT10, R_GPT11, R_GPT12, R_GPT13
};

EncoderR4::EncoderR4(unsigned int pinA, unsigned int pinB)
    : _pinA{pinA}
    , _pinB{pinB}
{ }

bool EncoderR4::begin()
{
    bool ret = true;

    ret = ret && _pinA < PINS_COUNT;
    ret = ret && _pinB < PINS_COUNT;

    if(!ret){
        return ret;
    }

    auto cfgA = getPinCfgs(_pinA, PIN_CFG_REQ_PWM);
    auto cfgB = getPinCfgs(_pinB, PIN_CFG_REQ_PWM);

    ret = ret && IS_PIN_GPT_PWM(cfgA[0]);
    ret = ret && IS_PIN_GPT_PWM(cfgB[0]);

    auto channel = GET_CHANNEL(cfgA[0]);
    ret = ret && channel < CHANNEL_COUNT;
    ret = ret && channel == GET_CHANNEL(cfgB[0]);

    auto hasChannelA = IS_PWM_ON_A(cfgA[0]) || IS_PWM_ON_A(cfgB[0]);
    auto hasChannelB = IS_PWM_ON_B(cfgA[0]) || IS_PWM_ON_B(cfgB[0]);

    ret = ret && hasChannelA && hasChannelB;

    _timer.force_use_of_pwm_reserved_timer();
	ret = ret && _timer.begin(TIMER_MODE_PERIODIC, GPT_TIMER, channel, 0, 0, TIMER_SOURCE_DIV_1);
    ret = ret && _timer.open();

    if(!ret){
        return ret;
    }

    // All pre-checks passed. Now start configuring

    auto rpinA = g_pin_cfg[_pinA].pin;
    auto rpinB = g_pin_cfg[_pinB].pin;

    uint32_t pcfg = IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_GPT1;
    R_IOPORT_PinCfg(NULL, rpinA, pcfg);
    R_IOPORT_PinCfg(NULL, rpinB, pcfg);

    auto gpt = channel2GPT[channel];
    if(IS_PWM_ON_A(cfgA[0])){
        // configure count up
        gpt->GTUPSR_b.USCAFBL = 0b1;
        gpt->GTUPSR_b.USCARBH = 0b1;
        gpt->GTUPSR_b.USCBRAL = 0b1;
        gpt->GTUPSR_b.USCBFAH = 0b1;

        // configure count down
        gpt->GTDNSR_b.DSCAFBH = 0b1;
        gpt->GTDNSR_b.DSCARBL = 0b1;
        gpt->GTDNSR_b.DSCBRAH = 0b1;
        gpt->GTDNSR_b.DSCBFAL = 0b1;
    } else {
        // configure count up
        gpt->GTUPSR_b.USCBFAL = 0b1;
        gpt->GTUPSR_b.USCBRAH = 0b1;
        gpt->GTUPSR_b.USCARBL = 0b1;
        gpt->GTUPSR_b.USCAFBH = 0b1;

        // configure count down
        gpt->GTDNSR_b.DSCBFAH = 0b1;
        gpt->GTDNSR_b.DSCBRAL = 0b1;
        gpt->GTDNSR_b.DSCARBH = 0b1;
        gpt->GTDNSR_b.DSCAFBL = 0b1;
    }

    ret = ret && _timer.start();

    _lastValue = read();

    return ret;
}

uint32_t EncoderR4::read()
{
    return _timer.get_counter();
}

int32_t EncoderR4::readChange()
{
    uint32_t v = read();
    int32_t ret = v - _lastValue;
    _lastValue = v;

    return ret;
}
