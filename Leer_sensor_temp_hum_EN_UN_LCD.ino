#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#define I2C_ADDR    0x27
#include <math.h> 
LiquidCrystal_I2C             lcd(I2C_ADDR,2, 1, 0, 4, 5, 6, 7);


#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(10,11);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth
 
dht DHT;

#define DHT11_PIN 8
 float  puntoRocio ;


  // metodos para imprimir la temperatura y la humedad punto de rocio
    void enviar_humedad(){
     BT.print(DHT.humidity);
  
  }
  void enviar_temperatura(){
     BT.print(DHT.temperature);
  
  }


  void enviar_puntodeRocio(){
    BT.print(puntoRocio);
    
    }



      void apagar_LCD(){
      lcd.setBacklight(LOW);
    
    }
        void encender_LCD(){
     lcd.setBacklight(HIGH);
     
    
    }

    
void setup()
   {


 lcd.setBacklight(HIGH);
 lcd.setBacklightPin(3,POSITIVE); 
 
  lcd.begin (16,2);    // Inicializar el display con 16 caraceres 2 lineas 
  BT.begin(9600);       // Inicializamos el puerto serie BT que hemos creado
  Serial.begin(9600);   // Inicializamos  el puerto serie  
     
   }

   
void loop() {    

       



  
    float chk = DHT.read11(DHT11_PIN);
    
       lcd.home ();// Sitúa el cursor en el primer carácter de la primera fila, sin borrar el display.
       lcd.print("Humedad  ");
       lcd.print(DHT.humidity);
       lcd.setCursor ( 0, 1 );        // Posiciona el cursor en una ubicación específica del display. Recibe dos argumentos. 
                                      //El primero se refiere al carácter de la fila y el segundo a la fila. 
       lcd.print("Temp     ");        //posiciona el primer caracter en 0 y la segunda fila 1.
       lcd.print(DHT.temperature);
    
      delay(6000); 
    
      //formula calcula el punto de rocio
        
        float R = pow((DHT.humidity/ 100), 0.125)*(112 - (-(0.9 * DHT.temperature))) - (-(0.1 * DHT.temperature)) - 112;

      puntoRocio = (round(R*110))/100; 

      // fin de formula punto rocio
      //imprimimos por pantalla LCD
      lcd.clear();
      lcd.print("Temp Rocio ");
      lcd.print(puntoRocio);
      lcd.setCursor ( 0, 1 );
       delay(4000);
      lcd.clear();
      
     // control para el puerto serie

              if( DHT.temperature>0)    // Si hay datos de temperatura envia a monitor serial
      {
     Serial.available();
     //Serial.write(BT.read());
    // Serial.print(DHT.humidity);
    // Serial.print(DHT.temperature);
      Serial.print(puntoRocio);
      Serial.print("\n");
     
     }
   if(DHT.temperature>0)  // Si hay dato de temperatura envia a BT
       {
               enviar_temperatura();
               enviar_humedad();
               enviar_puntodeRocio();
     }

  
     // apagar y encender el Display del LCS desde el BT
  if (BT.available()) { 
 char GetBT = (char)BT.read();
  if(GetBT=='a'){
           Serial.write("apagando");
            apagar_LCD();
           
            }
 if(GetBT=='e'){
           Serial.write("ENCENDIDO  ");
            encender_LCD();
            }
 }

// fin de apagar y encender display de sde BT


 }


      
