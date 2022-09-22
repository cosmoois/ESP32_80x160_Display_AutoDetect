#include "Display_Control.hpp"

// ディスプレイ接続指定
// DOIT ESP32 DEVKITにおける、代表的なディスプレイとの直結配線を示す
// 接続形態により調整してください（未使用ポートは-1とする）
#define DISP_GND 4
#define DISP_VCC 16
#define DISP_SCL 17
#define DISP_SDA 5
#define DISP_RES 18
#define DISP_DC  19
#define DISP_CS  21
#define DISP_BLK 3

LGFX_Device display;

void view_update(void);

void setup(void) {
  Serial.begin(115200);

  display = display_init(DISP_SCL, DISP_SDA, DISP_RES, DISP_DC, DISP_CS, DISP_BLK, DISP_VCC, DISP_GND);
  display.init();

  view_update();
}

void loop() {
  delay(2000);

  // 輝度確認用の明滅
  for (size_t i = 0x100; i > 0; i /= 2)
  {
    display.setBrightness(i - 1);
    delay(100);
  }

  view_update();

  for (size_t i = 1; i < 0x100; i *= 2)
  {
    display.setBrightness(i - 1);
    delay(100);
  }
}

void view_update(void) {
  static uint8_t rot = 0;
  int32_t view_width;
  int32_t view_height;

  display.setRotation(rot);
  view_width = display.width();
  view_height = display.height();
  rot = (rot + 1) % 4;  // 次回回転

  // カラーバー表示
  int32_t bar_width = view_width / 8;
  display.fillRect(bar_width * 0, 0, bar_width * 1, view_height, TFT_DARKGRAY);
  display.fillRect(bar_width * 1, 0, bar_width * 2, view_height, TFT_LIGHTGRAY);
  display.fillRect(bar_width * 2, 0, bar_width * 3, view_height, TFT_YELLOW);
  display.fillRect(bar_width * 3, 0, bar_width * 4, view_height, TFT_CYAN);
  display.fillRect(bar_width * 4, 0, bar_width * 5, view_height, TFT_GREEN);
  display.fillRect(bar_width * 5, 0, bar_width * 6, view_height, TFT_MAGENTA);
  display.fillRect(bar_width * 6, 0, bar_width * 7, view_height, TFT_RED);
  display.fillRect(bar_width * 7, 0, bar_width * 8, view_height, TFT_BLUE);
  display.drawRect(0, 0, view_width, view_height, TFT_WHITE);

  display.setFont(&Font4);

  uint32_t displayid;
  char drivername[10];
  char idbuf[8];

  displayid = display_get_info(drivername);
  sprintf(idbuf, "%06X", displayid);

  display.setCursor(5, 5);
  display.println(drivername);
  display.setCursor(5, 35);
  display.println(idbuf);
}
