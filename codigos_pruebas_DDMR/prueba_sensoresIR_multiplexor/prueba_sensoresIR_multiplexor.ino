/*
Este programa permite la lectura de los sensores IR conectados a traves del multiplexor CD74HC4067
-Requisitos:
-Cofiguracion: Verificar la lectura de los arreglos de sensores IR, observando por consola los valores (12-bits)
               desplegados en forma de columnas (1-16), las primeras 8 columnas corresponden a los primeros 8 canales del multiplexor (MUX1),
               donde esta conectado el arreglo de sensores frontal, el resto de columnas corresponden a las mediciones del arreglo de sensores
               posterior, que esta conectado entre los canales 8-15 del MUX1.
*/
void setup() {
  Serial.begin(115200);
                           //Pines de control del multiplexor
  pinMode(25, OUTPUT);     //PIN S0
  pinMode(26, OUTPUT);     //PIN S1
  pinMode(2, OUTPUT);      //PIN S2
  pinMode(4, OUTPUT);      //PIN S3
}

void loop() {
  for(byte i=0;i<=15;i++){         //Ciclo que permite recorrer el i+1 canal del multiplexor
    digitalWrite(25, i & 0x01  );  //Permite extraer el bit 2^0 del numero i, el valor se asigna al pin de control S0
    digitalWrite(26, i & 0x02  );  //Permite extraer el bit 2^1 del numero i, el valor se asigna al pin de control S1
    digitalWrite(2, i & 0x04  );   //Permite extraer el bit 2^2 del numero i, el valor se asigna al pin de control S2
    digitalWrite(4, i & 0x08 );    //Permite extraer el bit 2^3 del numero i, el valor se asigna al pin de control S3
    Serial.print(analogRead(39));  //Imprime en pantalla la lectura analogica del pin IO39 el cual esta conectado al pin de salida "SIG" del MUX1
    Serial.print(" - ");
  }
  Serial.println();
}
