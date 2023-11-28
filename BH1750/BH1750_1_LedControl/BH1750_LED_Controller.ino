#include <Wire.h>
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
