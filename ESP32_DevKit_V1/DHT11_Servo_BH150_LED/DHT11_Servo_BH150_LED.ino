/*#include <Wire.h>
#include <BH1750.h>

BH1750 luxo(0x23); // Dirección de BH1750
const int ledPinVController = 5;    // GPIO

void setup(){
    Serial.begin(9600); // Por conveniencia
    Wire.begin();
    pinMode(ledPinVController, OUTPUT); 

    if (luxo.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)){
        Serial.println(F("Sensor BH1750 iniciado"));
    }else{Serial.println(F("Error al iniciar el sensor BH1750"));}
}

void loop(){
    delay(5000); // 0.5s
    float lux = luxo.readLightLevel();

    if (lux < 0){Serial.println("Error al leer los datos del sensor");
    }else{Serial.print("Nivel de luz: ");  Serial.print(lux);  Serial.println(" lx");
        if (lux < 400){digitalWrite(ledPinVController, HIGH);
        }else{digitalWrite(ledPinVController, LOW);}
    }
}
*/

#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <ESP32Servo.h>

BH1750 lightMeter(0x23); // Dirección de BH1750
const int ledPin = 5;    // GPIO
DHT dht1(15, DHT11);
DHT dht2(16, DHT11);
Servo elServo; // Objeto Servo

bool countServo = false; // Para identificar si el servo está apagado o encendido
int rInf = 70;
int rSup = 90;

void setup()
{
    Serial.begin(9600); // Por conveniencia
    Wire.begin();

    pinMode(ledPin, OUTPUT); // Configura el pin de salida para los LEDs

    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
    {
        Serial.println(F("Sensor BH1750 iniciado"));
    }
    else
    {
        Serial.println(F("Error al iniciar el sensor BH1750"));
    }

    dht1.begin();
    dht2.begin();
    elServo.attach(23); // gpio
}

void loop()
{
    // Código para BH1750
    delay(5000); // 0.5s
    float lux = lightMeter.readLightLevel();
    float humedad1 = dht1.readHumidity();
    float temperatura1 = dht1.readTemperature();
    float humedad2 = dht2.readHumidity();
    float temperatura2 = dht2.readTemperature();
    if (lux < 0)
    {
        Serial.println("Error al leer los datos del sensor BH1750");
    }
    else
    {
        Serial.print("Nivel de luz: ");
        Serial.print(lux);
        Serial.println(" lx");

        if (lux < 400)
        {
            digitalWrite(ledPin, HIGH);
        }
        else
        {
            digitalWrite(ledPin, LOW);
        }
    }
   

    if (isnan(humedad1) || isnan(temperatura1) || isnan(humedad2) || isnan(temperatura2))
    {
        Serial.println("Error al leer los sensores DHT11");
        return;
    }

    Serial.print("Sensor 1 - Humedad: ");
    Serial.print(humedad1);
    Serial.print("%\t");
    Serial.print("Temperatura: ");
    Serial.print(temperatura1);
    Serial.println("°C");
    Serial.print("Sensor 2 - Humedad: ");
    Serial.print(humedad2);
    Serial.print("%\t");
    Serial.print("Temperatura: ");
    Serial.print(temperatura2);
    Serial.println("°C");

    float humedadMedia = (humedad1 + humedad2) / 2.0;
    float temperaturaMedia = (temperatura1 + temperatura2) / 2.0;

    Serial.print("Humedad Media: ");
    Serial.print(humedadMedia);
    Serial.print("%\t");
    Serial.print("Temperatura Media: ");
    Serial.print(temperaturaMedia);
    Serial.println("°C");

    if (humedadMedia <= rInf && !countServo)
    {
        elServo.write(180);
        delay(200);
        elServo.write(90); // Primer Giro encendido
        countServo = true;
        Serial.println("Aspersor se enciende");
    }
    else if (humedadMedia <= rInf && countServo)
    {
        Serial.println("Aspersor encendido, humedad inferior");
    }
    else if (humedadMedia > rInf && humedadMedia <= rSup)
    {
        Serial.println("Humedad estable");
    }
    else if (humedadMedia > rSup && countServo)
    {
        Serial.println("Humedad máxima superada, aspersor en modo de espera:");
        elServo.write(180);
        delay(200);
        elServo.write(90); // Segundo Giro pausa
        delay(500);
        Serial.println("Aspersor se apaga.");
        elServo.write(180);
        delay(200);
        elServo.write(90); // Tercer Giro apagado
        countServo = false;
    }
    else if (humedadMedia > rSup && !countServo)
    {
        Serial.println("Humedad superior, Aspersor apagado");
    }
    else
    {
        Serial.println("Hay un problema");
    }
    Serial.println("");
}
