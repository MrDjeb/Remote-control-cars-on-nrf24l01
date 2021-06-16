#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб
byte recieved_data[4];
void setup(){
  Serial.begin(9600); //открываем порт для связи с ПК

  pinMode(4, OUTPUT);
  
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(5, OUTPUT);
  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(6, OUTPUT);
  
  
  radio.begin(); //активировать модуль
  radio.setAutoAck(0);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  
  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
}

void loop() {
    byte pipeNo;                     
    while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
      radio.read( &recieved_data, sizeof(recieved_data) ); //читаем присланный массив
      digitalWrite(4, HIGH);

      int x1p = constrain(map(recieved_data[1], 128, 255, 0, 255), 0, 255); //обрезаем и переворачиваем значения
      int x1b = constrain(map(recieved_data[1], 128, 0, 0, 255), 0, 255); //обрезаем и переворачиваем значения
      int y2r = constrain(map(recieved_data[2], 122, 255, 100, 1), 0, 100); //обрезаем и переворачиваем значения
      int y2l = constrain(map(recieved_data[2], 122, 0, 100, 1), 0, 100); //обрезаем и переворачиваем значения
      
      int stl = constrain(map(recieved_data[2], 123, 255, 0, 255), 0, 255); //обрезаем и переворачиваем значения
      int str = constrain(map(recieved_data[2], 122, 0, 0, 255), 0, 255); //обрезаем и переворачиваем значения
      
      //Serial.print(x1p); Serial.print(" | "); Serial.print(x1b); Serial.print(" | "); Serial.print(y2r); Serial.print(" | "); Serial.print(y2l); Serial.println();
      int spr = constrain(x1p * y2r / 100, 0, 255); //скорость правого колеса
      int spl = constrain(x1p * y2l / 100, 0, 255); //скорость левого колеса
      int bspr = constrain(x1b * y2r / 100, 0, 255); //скорость правого колеса назад
      int bspl = constrain(x1b * y2l / 100, 0, 255); //скорость левого колеса назад
      Serial.print(stl); Serial.print(" | "); Serial.print(str); Serial.println();
      //Serial.print(recieved_data[2]); Serial.println();
      if (recieved_data[1]>138)
    { digitalWrite(A1, LOW);
      digitalWrite(A0, HIGH);
      analogWrite(5, spr);

      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      analogWrite(6, spl);
    } else if (recieved_data[1]<118)
    { digitalWrite(A1, !LOW);
      digitalWrite(A0, !HIGH);
      analogWrite(5, bspr);

      digitalWrite(7, !LOW);
      digitalWrite(8, !HIGH);
      analogWrite(6, bspl);
    }else if (118<recieved_data[1]<138)
    { digitalWrite(A1, LOW);
      digitalWrite(A0, HIGH);
      analogWrite(5, str);

      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      analogWrite(6, stl);
      }
  }
}
