#include <DHT.h>

#define DHTPIN 15 //gpio
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup(){
    Serial.begin(9600);//Por conveniencia 9600
    dht.begin();
}

void loop(){
    delay(5000);

    float humedad = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (isnan(humedad) || isnan(temperatura)){
        Serial.println("Error al leer el sensor DHT11");
        return;
    }

    Serial.print("Humedad: ");  Serial.print(humedad);  Serial.print("%\t");  Serial.print("Temperatura: ");  Serial.print(temperatura);  Serial.println("°C");
    Serial.println("");
}