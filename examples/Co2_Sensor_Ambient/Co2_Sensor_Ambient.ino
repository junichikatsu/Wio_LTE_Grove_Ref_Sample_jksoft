#include <WioLTEforArduino.h>
#include <WioLTEClient.h>
#include <Co2Sensor.h>
#include "Ambient.h"

#define APN               "soracom.io"
#define USERNAME          "sora"
#define PASSWORD          "sora"
#define INTERVAL          (30 * 1000)

Co2Sensor co2;

WioLTE Wio;
WioLTEClient WioClient(&Wio);

unsigned int channelId = 3112;
const char* writeKey = "543ea1f57ee3884d";
const char* userKey = " e8a7321c1519a69a4b";
Ambient ambient;

bool connectLTE() {

  SerialUSB.println("");
  SerialUSB.println("--- Connect LTE ---------------------------------------------------");

  SerialUSB.println("### Power supply ON.");
  Wio.PowerSupplyLTE(true);
  delay(500);

  SerialUSB.println("### Turn on or reset.");
  if (!Wio.TurnOnOrReset()) {
    SerialUSB.println("### ERROR! ###");
    return false;
  }

  SerialUSB.println("### Connecting to \""APN"\".");
  if (!Wio.Activate(APN, USERNAME, PASSWORD)) {
    SerialUSB.println("### ERROR! ###");
    return false;
  }

  SerialUSB.println("### Connect completed.");

  return true;
}

void setup() {
  // 初期化
  Wio.Init();
  // Groveの電源をON
  Wio.PowerSupplyGrove(true);
  // Co2センサとの通信準備
  co2.begin(&Serial);
  // Co2センサ キャリブレーション
  co2.calibration();
  // LTEに接続
  connectLTE();
  // Ambientへの接続準備
  ambient.begin(channelId, writeKey, &WioClient);  
}

void loop() {
  unsigned long stime = millis();

  // Co2センサーからデータ取得
  if(co2.readData()){
    // Co2センサが初期化完了してない場合はAmbientに送信しない
    if(!co2.getInitEndStatus())  return;
    
    // 取得したデータをシリアルモニタに表示
    SerialUSB.print("Temperature: ");
    SerialUSB.print(co2.getTemperature());
    SerialUSB.print("  CO2: ");
    SerialUSB.print(co2.getCo2());
    SerialUSB.println("");

    // Ambientに送信
    ambient.set(1, co2.getTemperature());
    ambient.set(2, co2.getCo2());
    ambient.send();
  }
  else
  {
    // センサから値を読めない場合はすぐに次のループへ
    return;
  }

  // インターバル時間に合うように残り時間をディレイする
  unsigned long elapse = millis() - stime;
  if (elapse < INTERVAL) delay(INTERVAL - elapse);
}
