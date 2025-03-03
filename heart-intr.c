// lab2-gpio/heart-intr.c
// Copyright (c) 2018 J. M. Spivey

#include "hardware.h"

#define TICK1 5               // One timer interrupt per 5 millisec
#define TICK2 500           // Switch between small and large every 500 ms   

/* heart -- GPIO values for heart image */
const unsigned heart[] = {
    0x28f0, 0x5e00, 0x8060
};

/* small -- GPIO values for small heart */
const unsigned small[] = {
    0x2df0, 0x5fb0, 0x8af0
};


static unsigned row = 0;
static unsigned mode = 0;

void advance(void) {
    row = (row + 1)%3;
    if (mode == 0){ 
    	GPIO_OUT = heart[row];
    }
    else {
    	GPIO_OUT = small[row];	
    }
    
}

void timer1_handler(void) {
    if (TIMER1_COMPARE[0]) {
        advance();
        TIMER1_COMPARE[0] = 0;
    }
}

void timer2_handler(void) {
    if (TIMER2_COMPARE[0]) {
        mode = (mode + 1)%2;
        TIMER2_COMPARE[0] = 0;
    }
}

void init_timer(void) {
    TIMER1_STOP = 1;
    TIMER1_MODE = TIMER_MODE_Timer;
    TIMER1_BITMODE = TIMER_BITMODE_16Bit;
    TIMER1_PRESCALER = 4;      // 1MHz = 16MHz / 2^4
    TIMER1_CLEAR = 1;
    TIMER1_CC[0] = 1000 * TICK1;
    TIMER1_SHORTS = BIT(TIMER_COMPARE0_CLEAR);
    TIMER1_INTENSET = BIT(TIMER_INT_COMPARE0);
    TIMER1_START = 1;
    enable_irq(TIMER1_IRQ);
    
    TIMER2_STOP = 1;
    TIMER2_MODE = TIMER_MODE_Timer;
    TIMER2_BITMODE = TIMER_BITMODE_16Bit;
    TIMER2_PRESCALER = 6;      // 1MHz = 16MHz / 2^4
    TIMER2_CLEAR = 1;
    TIMER2_CC[0] = 1000 * TICK2;
    TIMER2_SHORTS = BIT(TIMER_COMPARE0_CLEAR);
    TIMER2_INTENSET = BIT(TIMER_INT_COMPARE0);
    TIMER2_START = 1;
    enable_irq(TIMER2_IRQ);
}

void init(void) {
    GPIO_DIR = 0xfff0;
    GPIO_PINCNF[BUTTON_A] = 0;
    GPIO_PINCNF[BUTTON_B] = 0;
    GPIO_OUT = heart[0];
    
    init_timer();

    while (1) {
        pause();
    }
}
