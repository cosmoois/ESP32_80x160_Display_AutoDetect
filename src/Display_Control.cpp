// ST7789_readreg_diag.ino 参考
#include "Display_Control.hpp"

int8_t gnd_pin;
int8_t vcc_pin;
int8_t sck_pin;
int8_t sda_pin;
int8_t res_pin;
int8_t dc_pin;
int8_t cs_pin;

uint32_t display_id;
char display_drivername[10];

void write9(uint8_t val, uint8_t dc_lvl)
{
    pinMode(sda_pin, OUTPUT);
    digitalWrite(dc_pin, dc_lvl);
    for (int i = 0; i < 8; i++) {   //send command
        digitalWrite(sda_pin, (val & 0x80) != 0);
        delay(1);
        digitalWrite(sck_pin, HIGH);
        delay(1);
        digitalWrite(sck_pin, LOW);
        val <<= 1;
    }
}

uint32_t read8(uint8_t bits, uint8_t dummy)
{
    uint32_t ret = 0;
    int8_t SDAPIN = sda_pin;
    pinMode(SDAPIN, INPUT_PULLUP);
    digitalWrite(dc_pin, HIGH);
    for (int i = 0; i < dummy; i++) {  //any dummy clocks
        digitalWrite(sck_pin, HIGH);
        delay(1);
        digitalWrite(sck_pin, LOW);
        delay(1);
    }
    for (int i = 0; i < bits; i++) {  // read results
        ret <<= 1;
        delay(1);
        if (digitalRead(SDAPIN)) ret |= 1;;
        digitalWrite(sck_pin, HIGH);
        delay(1);
        digitalWrite(sck_pin, LOW);
    }
    return ret;
}

uint32_t readwrite8(uint8_t cmd, uint8_t bits, uint8_t dummy)
{
    if (cs_pin != -1) {
        digitalWrite(cs_pin, LOW);
    }
    write9(cmd, 0);
    uint32_t ret = read8(bits, dummy);
    if (cs_pin != -1) {
        digitalWrite(cs_pin, HIGH);
    }
    return ret;
}

void writeblock(uint8_t cmd, uint8_t *block, int8_t N)
{
    uint8_t val = cmd;
    if (cs_pin != -1) {
        digitalWrite(cs_pin, LOW);
    }
    write9(cmd, 0);
    while (N-- > 0) write9(*block++, 1);
    if (cs_pin != -1) {
        digitalWrite(cs_pin, HIGH);
    }
}

void display_hw_reset(void)
{
    digitalWrite(res_pin, HIGH);
    digitalWrite(res_pin, LOW);   //Hardware Reset
    delay(10);
    digitalWrite(res_pin, HIGH);
    delay(10);
}

void display_sw_reset(void)
{
    writeblock(0x01, NULL, 0);   //software reset
    delay(10);
}

uint32_t display_read_id(int8_t sck, int8_t sda, int8_t res, int8_t dc, int8_t cs, int8_t vcc, int8_t gnd) {
    gnd_pin = gnd;
    vcc_pin = vcc;
    sck_pin = sck;
    sda_pin = sda;
    res_pin = res;
    dc_pin = dc;
    cs_pin = cs;

    if (gnd != -1) {
        pinMode(gnd, OUTPUT);
        gpio_set_drive_capability((gpio_num_t)gnd, GPIO_DRIVE_CAP_3);
        digitalWrite(gnd, LOW);
    }
    if (vcc != -1) {
        pinMode(vcc, OUTPUT);
        gpio_set_drive_capability((gpio_num_t)vcc, GPIO_DRIVE_CAP_3);
        digitalWrite(vcc, HIGH);
    }

    if (cs != -1) {
        digitalWrite(cs, HIGH);
        pinMode(cs, OUTPUT);
    }

    pinMode(sck, OUTPUT);
    pinMode(sda, OUTPUT);
    pinMode(dc, OUTPUT);
    pinMode(res, OUTPUT);

    display_hw_reset();
    delay(10);
    // display_sw_reset();
    // delay(10);

    uint32_t ID;
    ID = readwrite8(0x04, 24, 1);
    ID = readwrite8(0x04, 24, 1);  //ST7789 needs twice

    return ID;
}

LGFX_Device display_init(int8_t sck, int8_t sda, int8_t res, int8_t dc, int8_t cs, int8_t blk, int8_t vcc, int8_t gnd) {
  LGFX_Device display;
  char display_idbuf[8];
  display_id = display_read_id(sck, sda, res, dc, cs, vcc, gnd);

  sprintf(display_idbuf, "%06X", display_id);
  Serial.print("panel_id = 0x");
  Serial.println(display_idbuf);

  display = displayid_init(display_id, blk);

  return display;
}

uint32_t display_get_info(char *drivername) {
  strcpy(drivername, display_drivername);
  return display_id;
}

LGFX_Device displayid_init(uint32_t id, int blk) {
  LGFX_Device display;

  static LGFX_ESP32_ST7735S_SPI display_st7735s(80, 160, sck_pin, sda_pin, res_pin, dc_pin, cs_pin, blk, vcc_pin, gnd_pin);
  static LGFX_ESP32_GC9106_SPI display_gc9106(80, 160, sck_pin, sda_pin, res_pin, dc_pin, cs_pin, blk, vcc_pin, gnd_pin);

  // ST7735 (未確認)
  if ((id & 0xFF0000) == 0x5C0000) {  // https://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
    display = display_st7735s;  // 設定はST7735Sと同じでよいか？
    strcpy(display_drivername, "ST7735");
  }
  else  // ST7735S
  if ((id & 0xFF0000) == 0x7C0000) {  // https://www.bolymin.com.tw/uploadfiles/862/IC-Controller-Datasheet/st7735s_v1.1.pdf p.109
    display = display_st7735s;
    strcpy(display_drivername, "ST7735S");
  }
  else  // GC9106
  if ((id & 0xFFFFFF) == 0x009106) {  // https://www.phoenixdisplay.com/wp-content/uploads/2019/07/GC9106_DS_V1.0_20170308.pdf
    display = display_gc9106;
    strcpy(display_drivername, "GC9106");
  }
  else
  {
    Serial.println("controllable display ID not found.");
    while(1);
  }

  return display;
}