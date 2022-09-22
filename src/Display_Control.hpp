#define LGFX_USE_V1
#include "Panel_GC9106.hpp"
#include <LovyanGFX.hpp>

#include "LGFX_ESP32_ST7735S_SPI.hpp"
#include "LGFX_ESP32_GC9106_SPI.hpp"

uint32_t display_read_id(int8_t sck, int8_t sda, int8_t res, int8_t dc, int8_t cs, int8_t vcc = -1, int8_t gnd = -1);
LGFX_Device display_init(int8_t sck, int8_t sda, int8_t res, int8_t dc, int8_t cs, int8_t blk = -1, int8_t vcc = -1, int8_t gnd = -1);
LGFX_Device displayid_init(uint32_t id, int blk);
uint32_t display_get_info(char *drivername);
