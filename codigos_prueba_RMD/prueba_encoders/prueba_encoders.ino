/*
Este programa permite la lectura de los pulsos de salida de los encoders
dispuestos en cada una de las ruedas del RMD.

-Requisitos: Libreria Encoder.h <http://www.pjrc.com/teensy/td_libs_Encoder.html>

-Cofiguracion: Al visualizar por consola los valores entregados por los encoders, verificar si el incremento/disminucion observado coincide 
                con el sentido de giro de las ruedas del Robot (adelante/atras).

*/
#include <Encoder.h>

Encoder encoder_der(23, 5);          //Corresponde a la estructura "encoder_der(PIN1, PIN2)" que lee los pulsos desde del encoder de la rueda derecha (invertir PINES de ser necesario)
Encoder encoder_izq(19, 18);         //Corresponde a la estructura "encoder_izq(PIN1, PIN2)" que lee los pulsos desde del encoder de la rueda izquierda (invertir PINES de ser necesario)

void setup() {
  Serial.begin(9600);
}

void loop() {                       //Se imprime en pantalla las cuentas de los encoder utilizando la funcion encoder.read()
  Serial.print(encoder_izq.read()); 
  Serial.print(" - ");
  Serial.print(encoder_der.read());
  Serial.println();
}
