#include "microbian.h"
#include "lib.h"
#include "hardware.h"
#include <string.h>

#define PUTC 16
int BUTTON;
int GEN;

void delay(unsigned n)
{
    unsigned t = 2*n;
    while (t > 0) {
        /* 500nsec per iteration at 16MHz */
        nop(); nop(); nop();
        t--;
    }
}

void rng_setup(void) {
    RNG_START = 1;
    RNG_INTEN = BIT(RNG_INT_VALRDY);
    connect(RNG_IRQ);
    enable_irq(RNG_IRQ);

}

int pressed(int button)
{
    return (GPIO_IN & BIT(button)) == 0;
}


void get_roll(int arg) {
    message m;
    unsigned state = 0;
    while (1){
    	if ((pressed(BUTTON_A) || pressed(BUTTON_B)) && (state == 0)) {
    	   state = 1;
	   m.type = PUTC;
	   sendrec(GEN, &m);
	   printf("roll = %d\n", m.int1);
           }
        delay(200000);
        state = 0;
    	}
    
}

void rng_task(int arg){
    #define NBUF 64
    static int txbuf[NBUF];
    int bufin, bufout, bufcnt;
    bufin = 0;
    bufcnt = 0;
    bufout = 0;
    message m;
    rng_setup();

    while (1){
    	if (bufcnt == 0) {
            receive(INTERRUPT, &m);
        }
        else 
            receive(PUTC, &m);
        switch (m.type) {
            case INTERRUPT:
            	if (RNG_VALRDY & (bufcnt < NBUF)){

            	    txbuf[bufin] = RNG_VALUE/51 + 1;
            	    bufin = (bufin + 1)%NBUF;
            	    bufcnt ++;
                }
                clear_pending(RNG_IRQ);
                enable_irq(RNG_IRQ);
        	break;
            case PUTC:
            	if (bufcnt > 0){
            	    m.int1 = txbuf[bufout];
            	    bufout = (bufout + 1)%NBUF;
            	    bufcnt --;
            	    m.type = REPLY;
            	    send(BUTTON, &m);
            	}
            	break;     
        }
    }	
}

void rng_init(void) {
	GEN = start("Rng", rng_task, 0, STACK);
}
    
	

void init(void) {
    GPIO_DIR = 0xfff0;
    GPIO_PINCNF[BUTTON_A] = 0;
    GPIO_PINCNF[BUTTON_B] = 0;

    serial_init();
    timer_init();
    rng_init();
    BUTTON = start("Main", get_roll, 0, STACK);
}
