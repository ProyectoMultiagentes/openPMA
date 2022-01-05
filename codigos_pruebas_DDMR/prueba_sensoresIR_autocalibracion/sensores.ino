/*
Funciones para la calibracion de sensores IR y medicion de la orientacion del DDMR respecto a la trayectoria de navegacion      
*/

float sensoresMax[16]; //Variable global que almacena el valor de lectura maximo de cada uno de los sensores IR (frontal y posterior)
float sensoresMin[16]; //Variable global que almacena el valor de lectura minimo de cada uno de los sensores IR (frontal y posterior)

void calibrarSensores() {         //Funcion de calibracion (se ejecuta una sola vez)
  for (int i = 0; i <= 15; i++) { //Inicializacion de maximos y minimos
    sensoresMax[i] = 0;
    sensoresMin[i] = 4095;
  }
  int vcal=200;                   //Ciclo de trabajo 20% PWM (0->1023) para mover los motores del DDMR
  encoder_der.write(0);           //Cuentas de los encoders inicializadas en 0 cuentas 
  encoder_izq.write(0);           
  while(abs(radio_rueda*PI*(encoder_der.read()-encoder_izq.read())*N/(CPR*l))< 4*PI){ //El lado izquierdo corresponde a la posicion angular del DDMR en [rad] (es decir mientras el robot no de 2 vueltas)
    motor(-vcal,vcal);                //Permite hacer girar al DDMR en torno a su eje de giro
    for(byte i=0;i<=15;i++){          //Se recorre cada uno de los sensores IR (MUX1)
      digitalWrite(25, i & 0x01  );
      digitalWrite(26, i & 0x02  );
      digitalWrite(2, i & 0x04  );
      digitalWrite(4, i & 0x08 );
      setMaxMin(i, analogRead(39));   //Esta funcion determina si el valor medido por el sensor conectado al canal i+1, es un maximo o minimo
    }
  }
  digitalWrite(25, 4 & 0x01  );       //Luego de las 2 vueltas del robot, se selecciona el canal 5 del DDMR (sensor central del arreglo IR frontal)
  digitalWrite(26, 4 & 0x02  );
  digitalWrite(2, 4 & 0x04  );
  digitalWrite(4, 4 & 0x08 );
  while((((1-2*linea)*100*(analogRead(39)-sensoresMin[4])/(sensoresMax[4]-sensoresMin[4])) + 100*linea)<80){ //Mientras el sensor IR (canal 5) no este posicinado en la trayectoria 
    motor(-vcal,vcal);                //El DDMR sigue rotando                                                                          
  }                                                                                                          
  motor(0,0);                         //Detiene el robot alineado con la trayectoria
}

void setMaxMin(int i, float aread) {  //Funcion que determina valores maximos y minimos de los sensores IR (se ejecuta una sola vez)
  if (sensoresMax[i] < aread) {
    sensoresMax[i] = aread;
  }
  if (sensoresMin[i] > aread) {
    sensoresMin[i] = aread;
  }
}
                                      //Funcion que determina la posicion de la trayectoria respecto del centro del arreglo de sensores en [mm],
float getposition(bool direccion){    //si el argumento es 1 se calcula utilizando el arreglo de sensores frontal, pero si el argumento es 0 se utiliza el arreglo de sensores posterior.
  float sensorNormalizado[8];
  float inte[8];                      
  float sum1=0;
  float area=0;
  if(direccion){                     //Calcula el valor normalizado de cada sensor del arreglo frontal en una escala comun
    for(byte i=0;i<=7;i++){
      digitalWrite(25, i & 0x01  );
      digitalWrite(26, i & 0x02  );
      digitalWrite(2, i & 0x04  );
      digitalWrite(4, i & 0x08 );
      sensorNormalizado[i]=((1-2*linea)*100*(analogRead(39)-sensoresMin[i])/(sensoresMax[i]-sensoresMin[i])) + 100*linea; //Mapeo del sensor de 0 a 100, o viceversa (segun "linea")
      if( sensorNormalizado[i]<30){
        sensorNormalizado[i]=0;
      }
    }
  }
  else{                              //Calcula el valor normalizado de cada sensor del arreglo posterior en una escala comun
    for(byte i=0;i<=7;i++){
      digitalWrite(25, (i+8) & 0x01  );
      digitalWrite(26, (i+8) & 0x02  );
      digitalWrite(2, (i+8) & 0x04  );
      digitalWrite(4, (i+8) & 0x08 );
      sensorNormalizado[i]=((1-2*linea)*100*(analogRead(39)-sensoresMin[i+8])/(sensoresMax[i+8]-sensoresMin[i+8])) + 100*linea; //Mapeo del sensor de 0 a 100, o viceversa (segun "linea")
      if( sensorNormalizado[i]<30){       //Control del piso de ruido (se vuelve mas preciso el algoritmo que calcula la posicion de la trayectoria)
        sensorNormalizado[i]=0;
      }
    }
  }
  for(int i=0; i<=7;i++){
    inte[i]=8*sensorNormalizado[i]-0.04*sensorNormalizado[i]*sensorNormalizado[i];  //Calculo de un area (inte) para cada sensor, que depende del nivel del sensor normalizado.
  }  
  sum1=-28*inte[0] - 20*inte[1] - 12*inte[2] - 4*inte[3] + 4*inte[4] + 12*inte[5] + 20*inte[6] + 28*inte[7];  //Suma de las areas de cada sensor ponderadas por su ubicacion espacial medida desde el centro del arreglo en [mm] 
  area=inte[0]+inte[1]+inte[2]+inte[3]+inte[4]+inte[5]+inte[6]+inte[7];                                       //Suma total de las areas
  if(area==0){    //Si el area es cero, implica que el robot se salio de la trayectoria
    parar ="si";
    return 0;
  }
  else{
      return sum1/area; //Se retorna el promedio del nivel del sensor normalizado, poderado por la ubicacion espacial de cada sensor (Retorna la posicion de la trayectoria respecto del centro del arreglo de sensores en [mm])
  }                     //Rango +- 28[mm]
}
