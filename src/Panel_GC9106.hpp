#pragma once

#include <lgfx/v1/panel/Panel_LCD.hpp>

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  struct Panel_GC9106 : public Panel_LCD
  {
    Panel_GC9106(void)
    {
    }

  protected:

    static constexpr uint8_t CMD_TELON   = 0x35;

    static constexpr uint8_t CMD_SCANLSET= 0x44;

    static constexpr uint8_t CMD_FRMCTR1 = 0xB1;
    static constexpr uint8_t CMD_FRMCTR3 = 0xB3;
    static constexpr uint8_t CMD_INVCTR  = 0xB4;

    static constexpr uint8_t CMD_VREG1CTL= 0xE6;
    static constexpr uint8_t CMD_VREG2CTL= 0xE7;
    static constexpr uint8_t CMD_INTERRE1= 0xFE;
    static constexpr uint8_t CMD_INTERRE2= 0xEF;

    static constexpr uint8_t CMD_GAMMA1  = 0xF0;
    static constexpr uint8_t CMD_GAMMA2  = 0xF1;

    const uint8_t* getInitCommands(uint8_t listno) const override {
      static constexpr uint8_t Bcmd[] = {                  // Initialization commands for 7735B screens
        CMD_INTERRE1, 0,
        CMD_INTERRE1, 0,
        CMD_INTERRE1, 0,
        CMD_INTERRE2, 0,
        CMD_FRMCTR3,  1      ,
          0x03,
        CMD_MADCTL,   1      ,
          0xD8,
        CMD_COLMOD,   1      ,
          0x05,
        0xB6,         1      ,
          0x11,
        0xAC,         1      ,
          0x0B,
        CMD_INVCTR,   1      ,
          0x21,
        CMD_FRMCTR1,  1      ,
          0xC0,
        CMD_VREG1CTL, 2      ,
          0x50,
          0x43,
        CMD_VREG2CTL, 2      ,
          0x56,
          0x43,
        CMD_GAMMA1,  14      ,
          0x1F, 0x41, 0x1B, 0x55,
          0x36, 0x3D, 0x3E, 0x00,
          0x16, 0x08, 0x09, 0x15,
          0x14, 0x0F,
        CMD_GAMMA2,  14,
          0x1F, 0x41, 0x1B, 0x55,
          0x36, 0x3D, 0x3E, 0x00,
          0x16, 0x08, 0x09, 0x15,
          0x14, 0x0F,
        CMD_INTERRE1, 0,
        0xFF, 0,
        CMD_TELON,    1      ,
          0x00,
        CMD_SCANLSET, 1      ,
          0x00,
        CMD_SLPOUT ,   CMD_INIT_DELAY, 255,  // Out of sleep mode, no args, w/delay
        CMD_DISPON ,   CMD_INIT_DELAY,  // Main screen turn on, no args, w/delay
          255 ,                   //     255 = 500 ms delay
        0xFF, 0xFF
      };

      switch (listno) {
      case 0: return Bcmd;
      default: return nullptr;
      }
    }
  };

//----------------------------------------------------------------------------
 }
}
