#include <SPI.h>
#include "mcp2515.h"
#include "SharkJ1939.h"

#define J1939_CAN_ID_MASK 0x1FFFFFFF
#define J1939_PGN_MASK 0x03FFFF00
#define J1939_PF_MASK 0x0000FF00
#define J1939_PS_MASK 0x000000FF
#define J1939_PDU1_RANGE(PF) (PF < 240)

// Helper function to print hexadecimal values with leading zeros
inline void printHexWithLeadingZero(uint8_t value)
{
  if (value < 0x10)
  {
    Serial.print('0');
  }
  Serial.print(value, HEX);
}

void SharkJ1939::dumpMessage(struct can_frame &j1939Msg)
{
  if (pauseFlag_)
    return;

  // Extract J1939 PGN, PF, and PS from CAN ID and print
  uint32_t j1939_29_bit_can_id = j1939Msg.can_id & J1939_CAN_ID_MASK;
  uint32_t j1939_18_bit_pgn = (j1939_29_bit_can_id & J1939_PGN_MASK) >> 8;
  uint8_t j1939_8_bit_pf = (j1939_18_bit_pgn & J1939_PF_MASK) >> 8;
  uint32_t pgn;

  if (j1939_29_bit_can_id < 0x10000000)
    Serial.print('0');                    // with a leading zero.
  Serial.print(j1939_29_bit_can_id, HEX); // print J1939 29 bit CAN ID

  ringHead_ &= 0x07; // Limit ringHead_ to a range of 0 to 7
  ringBuffer_[ringHead_++] = j1939_29_bit_can_id;

  Serial.print(' ');

  Serial.print(j1939Msg.can_dlc, HEX); // print DLC

  Serial.print(' ');

  for (int i = 0; i < j1939Msg.can_dlc; i++)
  { // print the data
    printHexWithLeadingZero(j1939Msg.data[i]);
    Serial.print(' ');
  }

  Serial.print(millis() + 1000000000); // 1000000000 is to print millis() with a fixed width.

  Serial.print(' ');

  if (J1939_PDU1_RANGE(j1939_8_bit_pf))
  {
    printHexWithLeadingZero(j1939_18_bit_pgn & J1939_PS_MASK); // PDU1 contains destination address.
    pgn = j1939_18_bit_pgn & 0x0003FF00;                       // remove destination address.
  }
  else
  {
    Serial.print(" --"); // PDU2 contains group extension.
    pgn = j1939_18_bit_pgn;
  }

  Serial.print(' ');

  int index;

  for (index = 0; index < 4; index++)
  {
    if (pgnKnownTable_[index].pgn != pgn)
      continue;
    
    Serial.print(pgnKnownTable_[index].pgnAcronym); // Print the acronym (PGN name) for the known PGN.
    if (pgnKnownTable_[index].pgnCallback != NULL)
    {
      pgnKnownTable_[index].pgnCallback(j1939Msg.data); // Call the callback function with the CAN data.
    }
    
    break;
  }

  if (index==4) {
    Serial.print(pgn); // Print the PGN number for unknown PGN.
  }

  Serial.println();
}

void SharkJ1939::processUserInput(void)
{
  if (Serial.read() == 0x0A)
    pauseFlag_ = !pauseFlag_;
}
