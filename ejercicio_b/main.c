#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "configbits.h"

#define _XTAL_FREQ 4000000

#define modo_standby 0
#define modo_estrellita 1
#define modo_notas 2

char frec, ciclodet, presc;

int frecuencia, tiempo_on, estado=0;

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

void estrellita(){

        
        int doo=523, re=587, mi=659, fa=698, sol=784, la=880, si=988, do_sost=1047;
        
        pwmOFF();
        __delay_ms(1000);
        
   
        
        
        //---INICIO----
        pwmON(doo, 50);
        __delay_ms(1000);
        
        pwmON(doo, 50);
        __delay_ms(1000);
        
        pwmON(sol, 50);
        __delay_ms(1000);
        
        pwmON(sol, 50);
        __delay_ms(1000);
        
        pwmON(la, 50);
        __delay_ms(1000);
        
        pwmON(la, 50);
        __delay_ms(1000);
        
        pwmON(sol, 50);
        __delay_ms(1000);
        
        pwmOFF();
        __delay_ms(500);
        
        pwmON(fa, 50);
        __delay_ms(1000);
        
        pwmON(fa, 50);
        __delay_ms(1000);
        
        pwmON(mi, 50);
        __delay_ms(1000);
        
        pwmON(mi, 50);
        __delay_ms(1000);
        
        pwmON(re, 50);
        __delay_ms(1000);
        
        pwmON(re, 50);
        __delay_ms(1000);
        
        pwmON(doo, 50);
        __delay_ms(1000);
        

        estado=modo_standby;

}

void notas_musicales(){     
        int doo=523, re=587, mi=659, fa=698, sol=784, la=880, si=988, do_sost=1047;
        
        pwmOFF();
        __delay_ms(1000);
        
   
        
        
        //---INICIO----
        pwmON(doo, 50);
        __delay_ms(1000);
        
        pwmON(re, 50);
        __delay_ms(1000);
        
        pwmON(mi, 50);
        __delay_ms(1000);
        
        pwmON(fa, 50);
        __delay_ms(1000);
        
        pwmON(sol, 50);
        __delay_ms(1000);
        
        pwmON(la, 50);
        __delay_ms(1000);
        
        pwmON(si, 50);
        __delay_ms(1000);
       
        pwmON(do_sost, 50);
        __delay_ms(1000);
        
        
        estado=modo_standby;

}


int flag=0;
void main() {
    
    PWMinit();
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    
    
    while(1){
        pwmOFF();
        
    
        if (PORTBbits.RB0 == 0 && PORTBbits.RB1==0){
            flag=0;
        }


        if (PORTBbits.RB0 == 1 && flag==0 && estado==modo_standby){
            flag=1;
            estado=modo_estrellita;
        }
        if (PORTBbits.RB1 == 1 && flag==0 && estado==modo_standby){
            flag=1;
            estado=modo_notas;
        }
    
    
    
    
        if(estado==modo_estrellita){
            estrellita();
        }
        if(estado==modo_notas){
            notas_musicales();
        }
    }
    

}
  