#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23); //Direccion de BH1750

void setup(){
    delay(5000);//5s
    Serial.begin(9600);//Por convenienca
    Wire.begin();

    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)){
        Serial.println(F("Sensor BH1750 iniciado"));
    }else{
        Serial.println(F("Error al iniciar el sensor BH1750"));
    }
}

void loop(){
    float lux = lightMeter.readLightLevel();
    if (lux < 0){
        Serial.println("Error al leer los datos del sensor");
    }else{
        Serial.print("Nivel de luz: ");  Serial.print(lux);  Serial.println(" lx");
    }
}
