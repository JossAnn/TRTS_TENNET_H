#include <DHT.h>
#include <ESP32Servo.h> //Seria la libreria Servo si se usara Arduino, pero ESP32 solo soporta esta

#define DHTPIN1 15 // gpio
#define DHTPIN2 16 // gpio
#define DHTTYPE DHT11

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
Servo elServo; //Objeto servo

void setup(){
    Serial.begin(9600);
    dht1.begin();
    dht2.begin();
    elServo.attach(23); //gpio
}

void loop(){
    delay(5000); // 5s
    float humedad1 = dht1.readHumidity();
    float temperatura1 = dht1.readTemperature();
    float humedad2 = dht2.readHumidity();
    float temperatura2 = dht2.readTemperature();

    if (isnan(humedad1) || isnan(temperatura1) || isnan(humedad2) || isnan(temperatura2)){
        Serial.println("Error al leer los sensores DHT11");
        return;
    }

    Serial.print("Sensor 1 - Humedad: ");  Serial.print(humedad1);  Serial.print("%\t");  Serial.print("Temperatura: ");  Serial.print(temperatura1);  Serial.println("°C");
    Serial.print("Sensor 2 - Humedad: ");  Serial.print(humedad2);  Serial.print("%\t");  Serial.print("Temperatura: ");  Serial.print(temperatura2);  Serial.println("°C");

    float humedadMedia = (humedad1 + humedad2) / 2.0;
    float temperaturaMedia = (temperatura1 + temperatura2) / 2.0;

    Serial.print("Sensor 0 - Humedad Media: ");  Serial.print(humedadMedia);  Serial.print("%\t");  Serial.print("Temperatura Media: ");  Serial.print(temperaturaMedia);  Serial.println("°C");

    if (humedadMedia < 70){
        elServo.write(180);  delay(200);  elServo.write(90);
        //Pero gira siempre que sea <70 no una sola vez
    }
    Serial.println("");
}