
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "configbits.h"

#define _XTAL_FREQ 4000000

char frec, ciclodet, presc;

int frecuencia, tiempo_on;

void pwmON(frec, ciclodet){
    T2CONbits.TMR2ON = 1;

    presc=16;
    //PARA FRECUENCIA
    frecuencia=(1000000)/((frec*presc))-1;
    if (frecuencia < 256){
        PR2 = frecuencia;
    }
    else{
        presc=4;
        frecuencia=(1000000)/((frec*presc))-1;
        if (frecuencia < 256){
            PR2 = frecuencia;
        }
        else{
            presc=1;
            frecuencia=(1000000)/((frec*presc))-1;
            if (frecuencia < 256){
                PR2 = frecuencia;
            }
        }
    }

    
    //PARA DUTY CICLE (FRECUENCIA)
    tiempo_on=(40000*ciclodet)/(frec*presc);
    CCPR1L = tiempo_on>>2;
    CCP1CONbits.DC1B = tiempo_on & 0b00000011;
    
    return;
    
}

void pwmOFF(){
    T2CONbits.TMR2ON = 0;
}

void PWMinit(){
    
    //CONFIGURAR EL MODO PWM en el pin RC2
    CCP1CON = 0b00001100;
    //ESTABLECER LA FRECUENCIA DE OPERACIÓN (UTILIZANDO LA FÓRMULA)
    
    PR2 = frecuencia;
    //ESTABLECER EL PREESCALER, EN ESTE CASO 1:16

    T2CONbits.T2CKPS = 0b11;

    //ACTIVAR EL TIMER2
    T2CONbits.TMR2ON = 1;

    //ESTABLECER EL CICLO DE TRABAJO,10 BITS EN TOTAL
    CCPR1L = tiempo_on>>2;
    CCP1CONbits.DC1B = tiempo_on & 0b00000011;
  
    //CONFIGURAR PUERTO C COMO SALIDA (SÓLO SE UTILIZARÁ RC2)
    TRISC = 0;
    
}


void main() {
    
    while(1){
        PWMinit();
        pwmON(2000, 20);
    }
    
    
}

