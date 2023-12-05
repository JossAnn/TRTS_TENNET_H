#include <Wire.h> 
#include <BH1750.h>
#include <DHT.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>

BH1750 lightMeter(0x23);
DHT dht1(15, DHT11);
DHT dht2(16, DHT11);
Servo elServo;
const int ledPin = 4;//2; (Estaba en el 2 pero parece que se fundió el del azul del ESP32)
bool countServo = false;
int rInf = 75;
int rSup = 90;

const char *ssid = "Politecnica";//"Joss Inifix";//"paco"; //"Tiburoncin 2";
const char *password = "";//"JossPatoo";//"charmander04"; //"RcBaR135";
const char *serverIP = "10.11.0.220";//"192.168.125.198";//"192.168.50.198"; //"192.168.0.34";//"44.196.3.52";
const int serverPort = 7000;
WiFiUDP udp;

void setup(){
    Serial.begin(9600);

    Serial.println("Iniciando ...");
    WiFi.begin(ssid, password);
    Serial.print("Conectando a la red Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConexión Wi-Fi establecida!");

    Wire.begin();
    pinMode(ledPin, OUTPUT);
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)){Serial.println(F("Sensor BH1750 iniciado"));}
    else{Serial.println(F("Error al iniciar el sensor BH1750"));}
    dht1.begin();
    dht2.begin();
    elServo.attach(23);
}

void loop(){
    delay(30000);//30s
    float lux = lightMeter.readLightLevel();
    float humedad1 = dht1.readHumidity();
    float temperatura1 = dht1.readTemperature();
    float humedad2 = dht2.readHumidity();
    float temperatura2 = dht2.readTemperature();
    //Aqui iria la lectura de CO2 si tuviera una sola direccion    

    if (lux < 0){Serial.println("Error al leer los datos del sensor BH1750");}
    else{
        if (lux < 400){digitalWrite(ledPin, HIGH);}
        else{digitalWrite(ledPin, LOW);}
    }
    if (isnan(humedad1) || isnan(temperatura1) || isnan(humedad2) || isnan(temperatura2)){
        Serial.println("Error al leer los sensores DHT11");
        return;
    }
    float humMedia = (humedad1 + humedad2) / 2.0;
    float tempMedia = (temperatura1 + temperatura2) / 2.0;
    int rndCO2 = random(400, 451);//Remplazamos Las lecturas del sensor de CO2 por una funcion con valores random
    int rndmCO2 = random(450, 501);
    int rdmCO2 = (rndCO2 + rndmCO2)/2;

    //Aqui se construye el JSON
    StaticJsonDocument<200> jsonDocument;
        jsonDocument["luxometer"] = lux;
        jsonDocument["temperature"] = tempMedia;
        jsonDocument["humidity"] = humMedia;
        jsonDocument["co2"] = rdmCO2;
        jsonDocument["container_id"] = 1;

    //Serializar el objeto JSON       serializeJsonPretty(jsonDocument, Serial); (tambien sirve)
    String jsonString;
    serializeJson(jsonDocument, jsonString);
    udp.beginPacket(serverIP, serverPort);
    udp.print(jsonString);
    udp.endPacket();
    Serial.println(jsonString);


    if (humMedia <= rInf && !countServo){
        elServo.write(180);  delay(200);  elServo.write(90);
        countServo = true;//Los textos se imprimen nada mas para ver por donde esta pasando lo del servo
    }else if (humMedia <= rInf && countServo){//Serial.println("Aspersor encendido, humedad inferior");
    }else if (humMedia > rInf && humMedia <= rSup){//Serial.println("Humedad estable");
    }else if (humMedia > rSup && countServo){//Serial.println("Humedad máxima superada, aspersor en modo de espera:");
        elServo.write(180);  delay(200);  elServo.write(90);  //Serial.println("Aspersor se apaga.");
        elServo.write(180);  delay(200);  elServo.write(90);
        countServo = false;
    }else if (humMedia > rSup && !countServo){//Serial.println("Humedad superior, Aspersor apagado");
    }else{//Serial.println("Hay un problema");
    }Serial.println("");
}
