#include <WioLTEforArduino.h>

#define BUZZER_PIN      (WIOLTE_A4) // I2Cの場合はPB8、UARTの場合はPB7とする
#define BUZZER_ON_TIME  (100)
#define BUZZER_OFF_TIME (3000)

WioLTE Wio;                      // 追加

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  Wio.Init();　　　　　　　　　　// 追加
  Wio.PowerSupplyGrove(true);    // 追加
  delay(5000);
}

void loop()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(BUZZER_ON_TIME);

  digitalWrite(BUZZER_PIN, LOW);
  delay(BUZZER_OFF_TIME);
}

