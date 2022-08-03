#pragma once

class LGFX_ESP32_GC9106_SPI : public lgfx::LGFX_Device
{
    lgfx::Panel_GC9106  _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;

  public:

    uint32_t _read_panel_id(int32_t pin_cs, uint32_t cmd = 0x04, uint8_t dummy_read_bit = 1) // 0x04 = RDDID command
    {
      _bus_instance.beginTransaction();
      pinMode(pin_cs, OUTPUT);
      digitalWrite(pin_cs, HIGH);
      _bus_instance.writeCommand(0, 8);
      _bus_instance.wait();
      digitalWrite(pin_cs, LOW);
      _bus_instance.writeCommand(cmd, 8);
      _bus_instance.beginRead(dummy_read_bit);
      uint32_t res = _bus_instance.readData(32);
      _bus_instance.endTransaction();
      digitalWrite(pin_cs, HIGH);

      ESP_LOGW(LIBRARY_NAME, "[Autodetect] read cmd:%02x = %08x", cmd, res);
      return res;
    }

    LGFX_ESP32_GC9106_SPI(int width, int height, int sclk, int mosi, int rst, int dc, int cs, int bl, int vcc = -1, int gnd = -1, int miso = -1, int busy = -1)
    {
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

      {
        auto cfg = _bus_instance.config();

        cfg.spi_host = VSPI_HOST;
        cfg.spi_mode = 0;
        cfg.freq_write = 20000000;
        cfg.freq_read  = 16000000;
        cfg.spi_3wire  = true;
        cfg.use_lock   = true;
        cfg.dma_channel = 1;
        cfg.pin_sclk = sclk;
        cfg.pin_mosi = mosi;
        cfg.pin_miso = miso;
        cfg.pin_dc   = dc;

        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto cfg = _panel_instance.config();

        cfg.pin_cs           = cs;
        cfg.pin_rst          = rst;
        cfg.pin_busy         = busy;

        cfg.memory_width     = 128;
        cfg.memory_height    = 160;
        cfg.panel_width      = width;
        cfg.panel_height     = height;
        cfg.offset_x         = 24;
        cfg.offset_y         = 0;
        cfg.offset_rotation  = 0;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits  = 1;
        cfg.readable         = true;
        cfg.invert           = false;
        cfg.rgb_order        = false;
        cfg.dlen_16bit       = false;
        cfg.bus_shared       = true;

        _panel_instance.config(cfg);
      }

      {
        auto cfg = _light_instance.config();

        cfg.pin_bl = bl;
        cfg.invert = false;
        cfg.freq   = 44100;
        cfg.pwm_channel = 7;

        _light_instance.config(cfg);
        _panel_instance.setLight(&_light_instance);
      }

      setPanel(&_panel_instance);
    }
};