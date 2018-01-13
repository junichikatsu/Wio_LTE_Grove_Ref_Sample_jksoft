#include <WioLTEforArduino.h>

#define BUTTON_PIN  (WIOLTE_D20) // I2Cの場合はPB8、UARTの場合はPB7とする
#define INTERVAL    (100)

WioLTE Wio;                      // 追加

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  Wio.Init();　　　　　　　　　　// 追加
  Wio.PowerSupplyGrove(true);    // 追加
}

void loop()
{
  int buttonState = digitalRead(BUTTON_PIN);
  SerialUSB.print(buttonState ? '*' : '.');
  
  delay(INTERVAL);
}

