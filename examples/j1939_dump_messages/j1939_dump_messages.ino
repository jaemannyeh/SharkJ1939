#include <SPI.h>
#include "mcp2515.h"
#include "SharkJ1939.h"

struct can_frame canMsg;
MCP2515 mcp2515(10);

static void tc1_callback(uint8_t *data)
{
  Serial.print(' ');

  uint8_t spn525 = data[2];
  if (spn525 == 0x7D)
    Serial.print('N');
  else if (spn525 == 0xDF)
    Serial.print('R');
  else
    Serial.print(spn525,HEX);
}

static PGN_KNOWN_ENTRY pgnKnownTable[8] = {
  { 0x100,   "TC1",      tc1_callback },
  { 0xBF00,  "PCM15",    NULL         },
  { 0xC000,  "PCM16",    NULL         },
  { 0xEE00,  "AC",       NULL         },
  { 0xEEC2,  "EEC2",     NULL         },
  { 0,        NULL,      NULL         },
};

SharkJ1939 sharkj1939(&mcp2515);

void setup()
{
  Serial.begin(115200);

  sharkj1939.bindPGNKnownTable(pgnKnownTable);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop()
{
  if (Serial.available())
  {
    sharkj1939.processUserInput();
  }

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    sharkj1939.dumpMessage(canMsg);
  }
}
