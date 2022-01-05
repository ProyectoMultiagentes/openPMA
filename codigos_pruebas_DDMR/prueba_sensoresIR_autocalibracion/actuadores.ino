/*
Funciones para el accionamiento de los motores a través del puente H TB6612FNG          
*/
void motor(int Velocidad_motor_izq, int Velocidad_motor_der)
{ 
  /*
   * Función de motor motor(M1, M2).
   * Mueve los motores con velocidad proporcional a M1 y M2, con un entre 0 y 1023
   * En caso de introducir un número negativo, el motor se mueve en sentido inverso.
  */

  //Configuracion pines de control del driver
  pinMode(ain1, OUTPUT); 
  pinMode(ain2, OUTPUT); 

  pinMode(bin1, OUTPUT); 
  pinMode(bin2, OUTPUT); 

  analogWriteFrequency(500);//Frecuencia de la señal PWM de ambos canales

  if (Velocidad_motor_der >= resolucionPWM){
    Velocidad_motor_der=resolucionPWM;  
  }
  else if(Velocidad_motor_der <= -resolucionPWM){
    Velocidad_motor_der=-resolucionPWM;
  }
  if (Velocidad_motor_izq >= resolucionPWM){
    Velocidad_motor_izq=resolucionPWM;  
  }
  else if(Velocidad_motor_izq <= -resolucionPWM){
    Velocidad_motor_izq=-resolucionPWM;
  }
  
  //Caso: Motor derecho hacia adelante
  if (Velocidad_motor_der > 0)
  {
    digitalWrite(ain1, HIGH); 
    digitalWrite(ain2, LOW);
    analogWrite(pwm_a, Velocidad_motor_der,resolucionPWM); 
  }

  //Caso: Motor derecho hacia atras
  else if (Velocidad_motor_der < 0)
  {
    digitalWrite(ain1, LOW); 
    digitalWrite(ain2, HIGH);
    analogWrite(pwm_a, -Velocidad_motor_der,resolucionPWM); 
  }

  //Caso: Motor derecho detenido
  else
  {
    digitalWrite(ain1, LOW); 
    digitalWrite(ain2, LOW);
    analogWrite(pwm_a, Velocidad_motor_der,resolucionPWM); 
  }

  //Caso: Motor izquierdo hacia adelante
  if (Velocidad_motor_izq > 0)
  {
    digitalWrite(bin1, HIGH); 
    digitalWrite(bin2, LOW);
    analogWrite(pwm_b, Velocidad_motor_izq,resolucionPWM); 
  }

  //Caso: Motor izquierdo hacia atras
  else if (Velocidad_motor_izq < 0)
  {
    digitalWrite(bin1, LOW); 
    digitalWrite(bin2, HIGH);
    analogWrite(pwm_b, -Velocidad_motor_izq,resolucionPWM); 
  }

  //Caso: Motor izquierdo detenido
  else
  {
    digitalWrite(bin1, LOW); 
    digitalWrite(bin2, LOW);
    analogWrite(pwm_b, Velocidad_motor_izq,resolucionPWM); 
  }
}
