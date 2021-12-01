//Los sensores IR estan en los canales io4-A7-A6-A0-A3-io26 de izquierda a derecha
float sensoresMax[16];
float sensoresMin[16];


void calibrarSensores() {
  for (int i = 0; i <= 15; i++) {
    sensoresMax[i] = 0;
    sensoresMin[i] = 4095;
  }
  int vcal=200;
//  for(int j=0; j<6; j++){
  encoder_der.write(0);
  encoder_izq.write(0);
  while(abs(radio_rueda*PI*(encoder_der.read()-encoder_izq.read())*N/(CPR*l))< 4*PI){
    motor(-vcal,vcal);
    for(byte i=0;i<=15;i++){
      digitalWrite(25, i & 0x01  );
      digitalWrite(26, i & 0x02  );
      digitalWrite(2, i & 0x04  );
      digitalWrite(4, i & 0x08 );
      setMaxMin(i, analogRead(39));
    }
  }
    //vcal=-vcal;
//  }
  digitalWrite(25, 4 & 0x01  );
  digitalWrite(26, 4 & 0x02  );
  digitalWrite(2, 4 & 0x04  );
  digitalWrite(4, 4 & 0x08 );
  while((((1-2*linea)*100*(analogRead(39)-sensoresMin[2])/(sensoresMax[2]-sensoresMin[2])) + 100*linea)<80){
    //motor(abs(vcal),abs(vcal));
    motor(-vcal,vcal);
  }
  /*encoder_der.write(0);
  encoder_izq.write(0);
  while((2*PI*radio_rueda*encoder_der.read()*N/CPR) < 11.2*PI/2){
    motor(0,abs(vcal));
  }*/
  motor(0,0);
  //d_ref=sensor.readReg16Bit(sensor.RESULT_RANGE_STATUS + 10)*0.1;
  //Input_d=d_ref;
}
void setMaxMin(int i, float aread) {
  if (sensoresMax[i] < aread) {
    sensoresMax[i] = aread;
  }
  if (sensoresMin[i] > aread) {
    sensoresMin[i] = aread;
  }
}

float getposition(bool direccion){
    // VARIABLES FUNCION getposition
  float sensorNormalizado[8];
  float inte[8];
  float sum1=0;
  float area=0;
  if(direccion){
    for(byte i=0;i<=7;i++){
      digitalWrite(25, i & 0x01  );
      digitalWrite(26, i & 0x02  );
      digitalWrite(2, i & 0x04  );
      digitalWrite(4, i & 0x08 );
      sensorNormalizado[i]=((1-2*linea)*100*(analogRead(39)-sensoresMin[i])/(sensoresMax[i]-sensoresMin[i])) + 100*linea; //MAPEO DE SENSORES
      if( sensorNormalizado[i]<30){
        sensorNormalizado[i]=0;
      }
    }
  }
  else{
    for(byte i=0;i<=7;i++){
      digitalWrite(25, (i+8) & 0x01  );
      digitalWrite(26, (i+8) & 0x02  );
      digitalWrite(2, (i+8) & 0x04  );
      digitalWrite(4, (i+8) & 0x08 );
      sensorNormalizado[i]=((1-2*linea)*100*(analogRead(39)-sensoresMin[i+8])/(sensoresMax[i+8]-sensoresMin[i+8])) + 100*linea; //MAPEO DE SENSORES
      if( sensorNormalizado[i]<30){
        sensorNormalizado[i]=0;
      }
    }
  }
  for(int i=0; i<=7;i++){
    //inte[i]=2*sensorNormalizado[i] - sensorNormalizado[i]*sensorNormalizado[i]/100;
    inte[i]=8*sensorNormalizado[i]-0.04*sensorNormalizado[i]*sensorNormalizado[i];
  }  
  sum1=-28*inte[0] - 20*inte[1] - 12*inte[2] - 4*inte[3] + 4*inte[4] + 12*inte[5] + 20*inte[6] + 28*inte[7];
  area=inte[0]+inte[1]+inte[2]+inte[3]+inte[4]+inte[5]+inte[6]+inte[7];
  if(area==0){
    parar ="si";
    return 0;
  }
  else{
      return sum1/area; //centroide
  }
  
  //pose_sensor=(2.4*sensorNormalizado[0] + 1.44*sensorNormalizado[1] + 0.48*sensorNormalizado[2] - 0.48*sensorNormalizado[3] - 1.44*sensorNormalizado[4] - 2.4*sensorNormalizado[5])/100; //promedio ponderado
   /*for(int i=0; i<=5;i++){
    Serial.print(' '); 
    Serial.print(sensorNormalizado[i]);
  }
  Serial.print(' ');
  Serial.print(pose_sensor);  
  Serial.println(' '); */ 
}

/*void print_MaxMin() {
  for (int i = 0; i <= 5; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" ----> ");
    Serial.print(sensoresMin[i]);
    Serial.print(" - ");
    Serial.print(sensoresMax[i]);
    umbral[i]=(sensoresMax[i]+sensoresMin[i])/4;
    Serial.print(" - ");
    Serial.print(umbral[i]);
  }
  Serial.println(' ');
}*/

void configuracion_sensor_d(){
  // CONFIGURACION SENSOR DE DISTANCIA ////////////////////////////////////                         
  Wire.begin();                                                       ///
  sensor.setTimeout(500);                                             ///
  while (!sensor.init())                                                 ///
  {                                                                   ///
    Serial.println("Failed to detect and initialize sensor!");        ///                                                    ///
    delay(1000); 
  }                                                                   ///
  sensor.setSignalRateLimit(0.25);                                    ///
  sensor.setMeasurementTimingBudget(33000);                           ///
  sensor.startContinuous(); //toma medidas cada {Argumento} ms     ///
/////////////////////////////////////////////////////////////////////////
  }
  double distancia(){
  double dist=sensor.readReg16Bit(sensor.RESULT_RANGE_STATUS + 10)*0.100 - 4;
  if(dist>80){
    dist=80;
  }
  else if(dist<=d_ref+10 && Input_d==80 && dist_ant<100){
    dist_ant++;
    return 80;
  }
  else  if(dist<=d_ref && dist_ant<105){
    return dist;
  }
  else{
    dist_ant=0;
  }
  /*if(((dist-Input_d)>=5)&&(d_ref<=Input_d)){
    return Input_d;
  }*/
  //dist_ant=dist;
  return dist;
}

void velocidades(){
t_actual = millis() - t_svel;
if(t_actual>=10){
  v_r[1]=v_r[0];
  v_r[2]=v_r[1];
  v_r[3]=v_r[2];
  v_r[4]=v_r[3];
  v_l[1]=v_l[0];
  v_l[2]=v_l[1];
  v_l[3]=v_l[2];
  v_l[4]=v_l[3];
  v_r[0]=2*PI*radio_rueda*encoder_der.read()*N/(t_actual*0.001*CPR); // ENCODERS:28CPR---CAJA_REDUCTORA:100:1  [cm/s]
  v_l[0]=2*PI*radio_rueda*encoder_izq.read()*N/(t_actual*0.001*CPR); // ENCODERS:28CPR---CAJA_REDUCTORA:100:1  [cm/s]
  encoder_der.write(0);
  encoder_izq.write(0);
  t_svel=millis();
}
curva=2.3333*abs(((0.9*(v_r[1]+v_r[2]+v_r[3]+v_r[4])/4) + 0.1*v_r[0])-((0.9*(v_l[1]+v_l[2]+v_l[3]+v_l[4])/4) + 0.1*v_l[0]));
v_r[0]=(0.2*(v_r[1]+v_r[2]+v_r[3]+v_r[4])/4) + 0.8*v_r[0];
v_l[0]=(0.2*(v_l[1]+v_l[2]+v_l[3]+v_l[4])/4) + 0.8*v_l[0];
Input_vel=0.5*(v_r[0]+v_l[0]); // [cm/s]
ang_vel=0.5*(v_r[0]-v_l[0])/l;
t_actual = millis() - t_arco;
s_l+=abs(v_l[0]*t_actual);
s_r+=abs(v_r[0]*t_actual);
t_arco=millis();
s=0.5*(s_r+s_l);
if(s<10 && Input_vel<0.1){
  radio_curva=0;
  s_l=0;
  s_r=0;
}
else if(s>=10){
  radio_curva=abs((0.5*(s_r-s_l))/(l*s));
  s_l=0;
  s_r=0;
}
}
