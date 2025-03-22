// lab4-phos/today.c
// Copyright (c) 2018 J. M. Spivey

#include "microbian.h"
#include "lib.h"
#include <string.h>

//What actually happens when you run? The output gets jumbled, something like BnRoE XdIeTa lM EiAsN Sb eBtRtEeXrI Tt!
//This is because, when the MAY process is called, it calls put_string, which calls serial_putc on one character. This generates an interrupt, and then moves to the FARAGE process, prints one character, and the process repeats.

//The code in this document outlines how to make the program such that the politician pass solutions to the interviewer - see ex-today2.c for the alternative solution with the interviewer giving them permission to speak in turn. 

int MAY;
int FARAGE;
int INTERVIEWER;
    

void put_string(char *s) {
    for (char *p = s; *p != '\0'; p++)
        serial_putc(*p);
}

void interviewer(int n) {
    message m;
    while (1) {
    	receive(ANY, &m);
    	put_string(m.ptr1);
    }
}

char *slogan[] = {
    "no deal is better than a bad deal\n",
    "BREXIT MEANS BREXIT!\n"
};

void process(int n) {
    message m;
    while (1) {
        m.ptr1 = slogan[n];
        send(INTERVIEWER, &m);
    }
}

void init(void) {
    serial_init();
    INTERVIEWER = start("interviewer", interviewer, 0, STACK);
    MAY = start("May", process, 0, STACK);
    FARAGE = start("Farage", process, 1, STACK);
}
