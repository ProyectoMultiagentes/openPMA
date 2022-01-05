/*
Este programa permite la lectura de distancia que mide el sensor VL53L0X, 
ubicado en la parte frontal del DDMR.
-Requisitos: Libreria Wire.h <https://github.com/espressif/arduino-esp32/tree/master/libraries/Wire>
             Libreria VL53L0X.h <https://github.com/pololu/vl53l0x-arduino##readme>
             Archivo sensores.ino
-Cofiguracion: Al visualizar por consola los valores entregados por el sensor, verificar si el valor minimo que se obtiene
               al poner un objeto justo en frente del DDMR corresponede a 0[cm] (de ser necesario ajustar "offset_sensor").
*/
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

double Input_d=0;         //Variable que almacena la distancia medida por el sensor
int cuenta=0;             //Variable que cuenta las veces que se repite una medición para discriminar su validez
#define offset_sensor 4   //Valor de ajuste del sensor en [cm]

void setup()
{
  Serial.begin(115200);
  configuracion_sensor_d(); //Fucion que configura el sensor VL53L0X
}
void loop()
{
  Input_d = distancia();   //Obtención de la distancia en centimetros
  Serial.println(Input_d);
}
