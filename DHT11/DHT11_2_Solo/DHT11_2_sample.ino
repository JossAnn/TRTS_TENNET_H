  #include <DHT.h>

  #define DHTPIN1 15//gpio
  #define DHTPIN2 16//gpio
  #define DHTTYPE DHT11

  DHT dht1(DHTPIN1, DHTTYPE);
  DHT dht2(DHTPIN2, DHTTYPE);

  void setup() {
    Serial.begin(9600);
    dht1.begin();
    dht2.begin();
  }

  void loop() {
  delay(5000);
    float humedad1 = dht1.readHumidity();
    float temperatura1 = dht1.readTemperature();
    float humedad2 = dht2.readHumidity();
    float temperatura2 = dht2.readTemperature();
    if (isnan(humedad1) || isnan(temperatura1) || isnan(humedad2) || isnan(temperatura2)) {
      Serial.println("Error al leer los sensores DHT11");
      return;
    }
    Serial.print("Sensor 1 - Humedad: ");  Serial.print(humedad1);  Serial.print("%\t");  Serial.print("Temperatura: ");  Serial.print(temperatura1);Serial.println("°C");
    Serial.print("Sensor 2 - Humedad: ");  Serial.print(humedad2);  Serial.print("%\t");  Serial.print("Temperatura: ");  Serial.print(temperatura2);Serial.println("°C");
    Serial.println("")
  }