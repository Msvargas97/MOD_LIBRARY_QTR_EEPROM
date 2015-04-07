#include <QTRSensors.h>
/*
 -------------------------------------------------------------------
|Este Sketch da un ejemplo de como guardar y restaurar los valores |
|calibrados de los sensores QTR en la EEPROM de los ATMega         |
|                                                                  |
|Version1.0 MOD QTR_EEPROM By ©MichaelVargas97                     |   
|contact me: msvargas97@gmail.com                                  |
 -------------------------------------------------------------------
Bitacora->
07/04/2015 MOD Initial V1.0

*/

//Ejemplo con sensores RC ( resistencia condesador) funciona igual con los analogos

/*
############NOTA##########
Si desea visualizar el proceso de Guardar y Restaurar los valores 
calibrados a travez del monitor serial, ir a la siguiente Ruta ../Documentos\Arduino\libraries\QTRSensors
Y buscar y abrir el archivo QTRSensors.cpp, abrirlo y posteriormente quitar los comentarios
donde dice #define MICHAEL_DEBUG 1, una vez hecho esto porfavor cargar de nuevo el programa
*/

#define NUM_SENSORS   8     // numero de sensores a usar
#define TIMEOUT       2500  
#define EMITTER_PIN   QTR_NO_EMITTER_PIN

QTRSensorsRC qtrrc((unsigned char[]) {12,13,14,15,16,17,18,19},// Asigne los pines correspondientes a los sensores
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];


void setup()
{
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    
  for (int i = 0; i < 200; i++)  //Calibracion de sensores durante 5 segundos
  {
    qtrrc.calibrate();       
  }
  digitalWrite(13, LOW);     
// Imprimir resultados de calibracion
Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
//### FUNCIONES AÑADIDAS
qtrrc.recordCalibration(); //1. con esta funcion puede guardar los datos despues de hacer la calibracion devuelve un 1 si se realiza
delay(2000); // delay de 2s
qtrrc.resetCalibration(); // Esta funcion asigna un valor de 0 a todos los datos de calibracion es para comprobar que se restauran usando la EEPROM
   delay(2000); // Podria usar botones para indicar cuando quiere restaurar o calibrar los sensores :)
 qtrrc.restoreCalibration(); //Restaura los valores calibrados desde la EEPROM, podria saltarse la calibracion ejecutando esta funcion y asi no tener que calibrar nuevamente
  Serial.println();
 delay(2000);
 //qtrrc.clearEEPROM(); // Esta funcion elimina o borra los datos calibrados 
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtrrc.readLine(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position); // comment this line out if you are using raw values
  
  delay(250);
}
