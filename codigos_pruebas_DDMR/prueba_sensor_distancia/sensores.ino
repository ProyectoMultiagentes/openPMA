/*
Funciones para la configuracion y uso del sensor de distancia VL53L0X              
*/
void configuracion_sensor_d(){ //Funcion de configuracion              
  Wire.begin();                                                       
  sensor.setTimeout(500);                                             
  while (!sensor.init())                                              
  {                                                                   
    Serial.println("Falla de conexión VL53L0X");                                                            
    delay(1000);                                                      
  }                                                                   
  sensor.setSignalRateLimit(0.25);                                    
  sensor.setMeasurementTimingBudget(33000);        //Actualiza la medición cada {Argumento}[us]                      
  sensor.startContinuous();                        //toma medidas cada {Argumento}[ms]      
  }
  
double distancia(){
  double dist = sensor.readReg16Bit(sensor.RESULT_RANGE_STATUS + 10)*0.100 - offset_sensor; //Lee el registro que almacena las mediciones del sensor, se realiza una
                                                                                            //operacion matematica para ajustar la medicion y expresarla en centimetros
  if(dist>80){ //Saturación de la medicion en 80[cm]
    cuenta=0;
    dist=80;
  }
  else if(dist<=60 && Input_d==80 && cuenta<200){ //Discriminacion de una muestra valida
    cuenta++;
    return 80;
  }
  else  if(dist<=60 && cuenta<205){
    return dist;
  }
  else{
    cuenta=0;
  }
  return dist;
}
