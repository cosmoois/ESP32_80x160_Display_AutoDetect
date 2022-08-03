#define LGFX_USE_V1
#include "Panel_GC9106.hpp"
#include <LovyanGFX.hpp>

#include "LGFX_ESP32_ST7735S_SPI.hpp"
#include "LGFX_ESP32_GC9106_SPI.hpp"

#define DISP_GND 4
#define DISP_VCC 16
#define DISP_SCL 17
#define DISP_SDA 5
#define DISP_RES 18
#define DISP_DC  19
#define DISP_CS  21
#define DISP_BLK 3

// 0.96inch 80x160
LGFX_ESP32_ST7735S_SPI display_st7735s(80, 160, DISP_SCL, DISP_SDA, DISP_RES, DISP_DC, DISP_CS, DISP_BLK, DISP_VCC, DISP_GND);
LGFX_ESP32_GC9106_SPI display_gc9106(80, 160, DISP_SCL, DISP_SDA, DISP_RES, DISP_DC, DISP_CS, DISP_BLK, DISP_VCC, DISP_GND);

LGFX_Device *gfx;

char idbuf[32];

void display_update(void);

void setup(void) {
  Serial.begin(115200);

  gfx = &display_gc9106;
  gfx->init();

  uint32_t id = display_gc9106._read_panel_id(DISP_CS);
  sprintf(idbuf, "%08X", id);
  Serial.print("panel_id = ");
  Serial.println(idbuf);

  // GC9106のIDが読めなかったらST7735Sに切り替える
  if ((id & 0x00FFFFFF) != 0x069100) {
    gfx = &display_st7735s;
    gfx->init();
  }

  display_update();
}

void loop() {
  delay(2000);

  // 輝度確認用の明滅
  for (size_t i = 0x100; i > 0; i /= 2)
  {
    gfx->setBrightness(i - 1);
    delay(100);
  }

  display_update();

  for (size_t i = 1; i < 0x100; i *= 2)
  {
    gfx->setBrightness(i - 1);
    delay(100);
  }
}

void display_update(void)
{
  static uint8_t rot = 0;
  int32_t view_width;
  int32_t view_height;

  gfx->setRotation(rot);
  view_width = gfx->width();
  view_height = gfx->height();
  rot = (rot + 1) % 4;  // 次回回転

  // カラーバー表示
  int32_t bar_width = view_width / 8;
  gfx->fillRect(bar_width * 0, 0, bar_width * 1, view_height, TFT_DARKGRAY);
  gfx->fillRect(bar_width * 1, 0, bar_width * 2, view_height, TFT_LIGHTGRAY);
  gfx->fillRect(bar_width * 2, 0, bar_width * 3, view_height, TFT_YELLOW);
  gfx->fillRect(bar_width * 3, 0, bar_width * 4, view_height, TFT_CYAN);
  gfx->fillRect(bar_width * 4, 0, bar_width * 5, view_height, TFT_GREEN);
  gfx->fillRect(bar_width * 5, 0, bar_width * 6, view_height, TFT_MAGENTA);
  gfx->fillRect(bar_width * 6, 0, bar_width * 7, view_height, TFT_RED);
  gfx->fillRect(bar_width * 7, 0, bar_width * 8, view_height, TFT_BLUE);
  gfx->drawRect(0, 0, view_width, view_height, TFT_WHITE);

  gfx->setFont(&Font4);

  if (gfx == &display_st7735s) {
    gfx->setCursor(5, 5);
    gfx->print("ST7735");
  } else {
    gfx->setCursor(5, 5);
    gfx->print("GC9106");
    gfx->setFont(&Font2);
    gfx->setCursor(5, 60);
    gfx->print(idbuf);
  }
}
