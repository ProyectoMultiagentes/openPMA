/*
Este programa permite la auto-calibracion de los arreglos de sensores IR, y permite la obtencion del angulo de orientacion entre el centro del DDMR y la trayectoria de navegacion
-Requisitos: Libreria Encoder.h <http://www.pjrc.com/teensy/td_libs_Encoder.html>
             Libreria analogWrite.h <https://github.com/ERROPiX/ESP32_AnalogWrite>
             Archivo actuadores.ino
             Archivo sensores.ino
-Cofiguracion: Antes de encender el DDMR desconectar el cable USB o cuidar que este no se enrede (el robot realizara dos vueltas sobre su eje de giro)
               En una zona recta de la trayectoria, posicionar el robot de forma ortogonal, es decir, con sus dos ruedas activas sobre la trayectoria, luego encender el DDMR
               El robot comenzará a calibrarse a medida que realiza los giros, luego se detendra alineado con la trayectoria, finalmente comenzara a imprimir por consola el valor
               del angulo de orientacion medido con el arreglo de sensores frontal y posterior, por lo que es necesario abrir la consola y conectar el robot (USB) de ser necesario.
               Segun el color de la superficie y trayectoria de navegacion a utilzar es necesario modificar el parametro "linea".
*/
#include <Encoder.h>
#include <analogWrite.h>

/*  Pines canal A driver motor derecho*/
#define ain1  16  //Invertir pines de control (ain1, ain2) en caso de que el sentido de giro no corresponda.
#define ain2  27
#define pwm_a  17

/*  Pines canal B driver motor izquierdo*/
#define bin1  13  //Invertir pines de control (bin1, bin2) en caso de que el sentido de giro no corresponda.
#define bin2  12
#define pwm_b  14

#define resolucionPWM 1023 //Resolucion de la PWM: 0-1023 -> duty cycle: 0-100%

#define radio_rueda 2.15   //Radio ruedas en [cm]
#define l 5.6              //Distancia entre las ruedas en [cm]
#define N 0.01             //Modulo de reduccion caja reductora
#define CPR 28             //Cuentas por revolucion encoders
#define d1 52              //Distancia desde el centro de giro del DDMR al centro del arreglo IR frontal en [mm]
#define d2 32              //Distancia desde el centro de giro del DDMR al centro del arreglo IR posterior en [mm]

#define linea 0  //Valor logico 1 corresponde a una pista con trayectoria color blanco; Valor lógico 0 corresponde a una pista con trayectoria color negro. 

Encoder encoder_der(5, 23);    //Corresponde a la estructura "encoder_der(PIN1, PIN2)" que lee los pulsos desde del encoder de la rueda derecha (invertir orden de ser necesario)
Encoder encoder_izq(18, 19);   //Corresponde a la estructura "encoder_izq(PIN1, PIN2)" que lee los pulsos desde del encoder de la rueda izquierda (invertir orden de ser necesario)

String parar = "si"; //Variable que permite saber si el robot se salió de la pista

void setup() {
  Serial.begin(115200);
                           //Pines de control del multiplexor
  pinMode(25, OUTPUT);     //PIN S0
  pinMode(26, OUTPUT);     //PIN S1
  pinMode(2, OUTPUT);      //PIN S2
  pinMode(4, OUTPUT);      //PIN S3
  calibrarSensores();      //Funcion que calibra los sensores IR
}

void loop() {
  Serial.print(getposition(1)/d1);    //Imprime en pantalla una aproximación del angulo de orientacion del DDMR medido con el arreglo frontal en [rad] : rango +- 0.54[rad]
  Serial.print("   ---   ");
  Serial.println(-getposition(0)/d2); //Imprime en pantalla una aproximación del angulo de orientacion del DDMR medido con el arreglo posterior en [rad] : rango +- 0.88[rad]
}
