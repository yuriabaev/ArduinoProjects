#include <Motor.h>


/*Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
 * Thanks to Neil Kolbans for his efoorts in adding the support to Arduino IDE
 * Turotial on: www.circuitdigest.com 
 */

#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino
#include <Motor.h>
#include <MillisConverter.h>
#include "StringSplitter.h"

Motor motor(2);
MillisConverter millisConverter;

BluetoothSerial ESP_BT; //Object for Bluetooth

int incoming;
int LED_BUILTIN = 2;

///
int WateringDuration = 3000;

String getCommand() {
  String command="";
  while(ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
//    char  c=ESP_BT.read();
//    command+=String(c);
 command=ESP_BT.readString();
  };
  return command;
}


void setWateringDuration(int time) {
    WateringDuration = time;
}
void doLedOn() {
    //digitalWrite(LED_BUILTIN, HIGH);
    motor.on();
    ESP_BT.println("LED turned ON");
}

void doCustomWater() {
    motor.on();
    ESP_BT.println("CUSTOM turned ON");
    //delay(settings.getWateringDuration());
    delay(WateringDuration);
    motor.off();
    ESP_BT.println("CUSTOM turned OFF");
}
 
void doLedOff() {
   //digitalWrite(LED_BUILTIN, LOW);
   motor.off();
   ESP_BT.println("LED turned OFF");
}

void setup() {
  Serial.begin(9600); //Start Serial monitor in 9600
  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
}

void loop() {

  
   String fullCommand;//=getCommand();
  if(ESP_BT.available()){
     fullCommand=ESP_BT.readString();
      Serial.print("fullCommand1:");Serial.println(fullCommand);
  }
  if(fullCommand.length() !=0){
      StringSplitter *splitter = new StringSplitter(fullCommand, ' ', 3);

      String command = splitter->getItemAtIndex(0);
      String arg = splitter->getItemAtIndex(1);
      
      Serial.print("splitter->getItemCount():");Serial.println(splitter->getItemCount());
       Serial.print("fullCommand:");Serial.println(fullCommand);
       Serial.print("command:");Serial.println(command);
       Serial.print("arg:");Serial.println(arg);

    Serial.print("Received:");Serial.println(command );
    if (command== "water") {
        doCustomWater();
    }else if (command== "set_watering_duration") {
        setWateringDuration(arg.toInt());
    }else if (command== "get_watering_duration") {
         ESP_BT.println(WateringDuration);
    }else if (command== "time") {
        ESP_BT.println(millisConverter.convert( millis()/1000));
    }else{
        ESP_BT.println("no such command");
    }    
  }
  delay(20);

}
