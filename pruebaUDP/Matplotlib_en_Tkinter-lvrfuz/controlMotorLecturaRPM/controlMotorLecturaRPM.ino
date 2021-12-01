///////////////// PINES ARDUINO ///////////////////////

const int AIN1 = 5;
const int AIN2 = 6;
const int encoderPin = 2;  //Pin 2, donde se conecta el encoder   

//////////////// VARIABLES DE TRABAJO ///////////////////////
float rpm = 0.0;  // Revoluciones por minuto calculadas.
int pwm =0; // Ciclo de trabajo
volatile byte pulses = 0;       // Número de pulsos leidos por el Arduino
const float pulsesperturn = 20.0;   // Número de muescas que tiene el disco del encoder.
volatile unsigned long lastTimeDebounce = 0; // Tiempo antirebote.

/////////////////// VARIABLES TIEMPO DE MUESTREO ///////////////
unsigned long lastTime = 0, sampleTime = 100; // Tiempo de muestreo

////////////////// VARIABLES COMUNICACION SERIAL //////////////
String inputString = "";  // String para contener datos entrantes
bool stringComplete = false; // Bandera que indica la recepcion de un dato nuevo



void setup() {
  
  ////////// CONFIGURACIONES DE PINES //////////////////
  pinMode(AIN2,OUTPUT);
  attachInterrupt(0,counter, RISING); // Configuración de la interrupción 0.

  //////////// CONFIGURACION PUERTO SERIAL //////////////
  Serial.begin(9600); 

  ////////// CONDICION INICIAL DE PINES ///////////////
  digitalWrite(AIN2,0);

}

void loop() {

  /////////// DATOS RECIBIDOS /////////////
  if (stringComplete) 
  {
      pwm = inputString.toInt();
      analogWrite(AIN1,pwm);
      inputString = "";
      stringComplete = false;
  }

  ////////// ENVIAR DATOS //////////////
  if (millis()-lastTime>=sampleTime){
      
      noInterrupts(); //Desconectamos la interrupción para que no actué en esta parte del programa.
      rpm = 0.1*((pulses*60000.0)/((millis()-lastTime)*pulsesperturn))+0.9*rpm;  // Calculamos las revoluciones por minuto
      //http://www.conectados-online.com/encoder-de-cuadratura-para-robotica/
      lastTime = millis();// Almacenamos el tiempo actual.
      pulses = 0;  // Reset pulses.
      Serial.println(rpm);
      interrupts(); // Restart the interrupt processing // Reiniciamos la interrupción
  }

}

//////////////////////// FUNCION PARA RECIBIR DATOS /////////////////////
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

///////////////////////////FUNCION QUE CUENTA PULSOS/////////////////////
void counter()
{
  if(digitalRead(encoderPin) && (micros()-lastTimeDebounce >= 500)) // Vuelve a comprobar que el encoder envia una señal buena y luego comprueba que el tiempo es superior a 1000 microsegundos y vuelve a comprobar que la señal es correcta.
  { 
    lastTimeDebounce = micros(); // Almacena el tiempo para comprobar que no contamos el rebote que hay en la señal.
    pulses++;
  }
} 
