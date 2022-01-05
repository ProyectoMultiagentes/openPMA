/*
Este programa permite el accionamiento de los motores a través del puente H TB6612FNG
-Requisitos: Libreria analogWrite.h <https://github.com/ERROPiX/ESP32_AnalogWrite>
             Archivo actuadores.ino
-Cofiguracion: Verificar sentido de giro de ambos motores.
*/
#include <analogWrite.h>

/*  Pines canal A driver motor derecho*/
#define ain1  16  //invertir pines de control (ain1, ain2) en caso de que el sentido de giro no corresponda.
#define ain2  27
#define pwm_a  17

/*  Pines canal B driver motor izquierdo*/
#define bin1  13  //invertir pines de control (bin1, bin2) en caso de que el sentido de giro no corresponda.
#define bin2  12
#define pwm_b  14

#define resolucionPWM 1023 //Resolucion de la PWM: 0-1023 -> duty cycle: 0-100%

void setup() {
}

void loop() {
  motor(-200,200);
  /*
   * El primer y segundo argumento corresponden al motor izquierdo y derecho respectivamente. 
   * Los argumentos estan en el rango de [-1023,1023] que corresponden al ciclo de trabajo de la señal PWM que controlan su velocidad.
   * El signo del argumento cambia el sentido de giro de los motores.
  */
}

  
