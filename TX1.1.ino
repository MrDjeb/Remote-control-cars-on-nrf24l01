/*   Данный скетч модернизация скетча AlexGyver
 *    исходник тут https://github.com/AlexGyver/nRF24L01
*/

#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

RF24 radio(9, 10); 


byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte transmit_data[4];


void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК

  radio.begin(); //активировать модуль
  radio.setAutoAck(0);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 0);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}

void loop() {
  transmit_data[0] = map(analogRead(A0), 0, 1023, 0, 255); //считываем и обрезаем показания с джостика 1 оси X
  transmit_data[1] = map(analogRead(A1), 0, 1023, 0, 255); //считываем и обрезаем показания с джостика 1 оси Y
  transmit_data[2] = map(analogRead(A2), 0, 1023, 0, 255); //считываем и обрезаем показания с джостика 2 оси X
  transmit_data[3] = map(analogRead(A3), 0, 1023, 0, 255); //считываем и обрезаем показания с джостика 2 оси Y
  radio.write(&transmit_data, sizeof(transmit_data)); //отправляем получившийся массив


}
