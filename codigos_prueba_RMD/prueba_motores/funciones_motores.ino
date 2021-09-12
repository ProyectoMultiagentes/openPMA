/*
Funciones para el accionamiento de los motores a través del puente H TB6612FNG

-Requisitos: Libreria analogWrite.h <https://github.com/ERROPiX/ESP32_AnalogWrite>

-Cofiguracion: Verificar sentido de giro de ambos motores.
                
*/
#include <analogWrite.h>  //Es necesario incluir esta libreria ya que no esta implementada en ESP32

/*  Pines canal A driver motor derecho*/
#define ain1  16  //invertir pines de control (ain1, ain2) en caso de que el sentido de giro no corresponda.
#define ain2  27
#define pwm_a  17 

/*  Pines canal B driver motor izquierdo*/
#define bin1  12  //invertir pines de control (bin1, bin2) en caso de que el sentido de giro no corresponda.
#define bin2  13
#define pwm_b  14

#define resolucion 1023 //Resolucion de la PWM: 0-1023 -> duty cycle: 0-100%

void motor(int PWM_motor_izq, int PWM_motor_der)
{ 
  /*
   * Funcion de motor motor(M1, M2).
   * Mueve los motores con velocidad M1 y M2 con un valor entero entre 0 y 1023
   * En caso de introducir un número negativo, el motor se mueve en sentido inverso.
  */
  
  //Configuracion pines de control del driver
  pinMode(ain1, OUTPUT); 
  pinMode(ain2, OUTPUT); 

  pinMode(bin1, OUTPUT); 
  pinMode(bin2, OUTPUT); 

  analogWriteFrequency(500); //Frecuencia de la señal PWM de ambos canales

  //Caso: Motor derecho hacia adelante
  if (PWM_motor_der > 0)
  {
    digitalWrite(ain1, HIGH); 
    digitalWrite(ain2, LOW);
    analogWrite(pwm_a, PWM_motor_der,resolucion); 
  }

  //Caso: Motor derecho hacia atras
  else if (PWM_motor_der < 0)
  {
    digitalWrite(ain1, LOW); 
    digitalWrite(ain2, HIGH);
    analogWrite(pwm_a, -PWM_motor_der,resolucion); 
  }

  //Caso: Motor derecho detenido
  else
  {
    digitalWrite(ain1, LOW); 
    digitalWrite(ain2, LOW);
    analogWrite(pwm_a, PWM_motor_der,resolucion); 
  }

  //Caso: Motor izquierdo hacia adelante
  if (PWM_motor_izq > 0)
  {
    digitalWrite(bin1, HIGH); 
    digitalWrite(bin2, LOW);
    analogWrite(pwm_b, PWM_motor_izq,resolucion); 
  }

  //Caso: Motor izquierdo hacia atras
  else if (PWM_motor_izq < 0)
  {
    digitalWrite(bin1, LOW); 
    digitalWrite(bin2, HIGH);
    analogWrite(pwm_b, -PWM_motor_izq,resolucion); 
  }

  //Caso: Motor izquierdo detenido
  else
  {
    digitalWrite(bin1, LOW); 
    digitalWrite(bin2, LOW);
    analogWrite(pwm_b, PWM_motor_izq,resolucion); 
  }
}
