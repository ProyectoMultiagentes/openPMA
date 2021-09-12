/*

Funciones para el accionamiento de los motores a través del puente H TB6612FNG

-Requisitos: Archivo funciones_motores.ino

-Cofiguracion: Verificar sentido de giro de ambos motores.
                
*/
void setup() {
}

void loop() {
  motor(-300,300); 
  /*
   * El primer y segundo argumento corresponden al motor izquierdo y derecho respectivamente. 
   * Los argumentos estan en el rango de [-1023,1023] que corresponden al ciclo de trabajo de la señal PWM que controlan su velocidad.
   * El signo del argumento cambia el sentido de giro de los motores.
  */
}

  
