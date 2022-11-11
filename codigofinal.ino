
const int Trigger = D7;   
const int Echo = D8;   
const int buzzer = D6; 

#include <DHT.h>
float tiempo;
float distancia;
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#define DHTPIN D2
#include <Firebase_ESP_Client.h> 
#define DHTTYPE DHT11
#include <addons/RTDBHelper.h>
#define WIFI_SSID "Azulejo2"
#define WIFI_PASSWORD "3994.dasala"
#define DATABASE_URL "heatertech-f3dd5-default-rtdb.firebaseio.com"
#define DATABASE_SECRET "tHjSebolExLaQ8Q9RVwIaGxHper8bl2CADRjdRbc"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

DHT dht(DHTPIN, DHTTYPE);
// se usa para loguear en una red y una bas de datos ademas de inicializar los pines 
 void setup()
 {
pinMode(buzzer, OUTPUT); 
 
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);  
  digitalWrite(Trigger, LOW);

  
    dht.begin();
  Serial.begin(115200);
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 Serial.print ("buscando red");
 while (WiFi.status()!= WL_CONNECTED)
 {
  Serial.print(".");
  delay(30);
  }
  Serial.println();
  Serial.print("conexion exitosa ip:");
  Serial.println(WiFi.localIP());
  Serial.print("firebase");
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Firebase.reconnectWiFi(true);
  Firebase.begin(&config,&auth);

  pinMode(D4,OUTPUT);
  
  
  
 }

 
String dato2="";
String dato="";
String dato3="";




void loop() 





{








  
  
if (Firebase.RTDB.getString(&fbdo, "/Estado")){
  
dato = fbdo.stringData();
}
if (Firebase.RTDB.getString(&fbdo, "/seg")){
  
dato2 = fbdo.stringData();
}

if(dato=="0") digitalWrite(D4,LOW);
else if(dato2=="0")digitalWrite(D4,LOW);

else if(dato=="1")digitalWrite(D4,HIGH);








 
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
 

 
 
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Índice de calor: ");

  Serial.print(" *C ");
 
  Serial.println(" *F");
 //Firebase.set(fbdo, "/temperatura",t);
 //Firebase.set(fbdo, "/farenheit",f);
//fbdo.dataType("/farenheit",f);
Firebase.RTDB.set(&fbdo, "/humedad",h);
Firebase.RTDB.set(&fbdo, "/farenheit",f);
Firebase.RTDB.set(&fbdo, "/temperatura",t);
// se suben las variables de temperatura humedad a la base de datos 
Firebase.RTDB.set(&fbdo, "/seg","0");
delay(3000);
//Firebase.setInt(firebaseData,"/humedad",h;

 long ti; //timepo que demora en leer la distancia 
  long d; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          
  digitalWrite(Trigger, LOW);
  
  ti = pulseIn(Echo, HIGH); 
  d = ti/59;             
  
  Serial.print("Distancia: ");
  Serial.print(d);    
  Serial.print("cm");
  Serial.println();
  delay(100);         
Firebase.RTDB.set(&fbdo, "/distancia",d);


if (Firebase.RTDB.getString(&fbdo, "/buz")){
  
dato3 = fbdo.stringData();
}
if(dato3=="0")  noTone(buzzer);     

else if (dato3=="1") tone(buzzer, 1000); 
}
