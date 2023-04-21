#ifndef _SHARKJ1939_H_
#define _SHARKJ1939_H_

// PGN_KNOWN_ENTRY struct is used for associating PGNs with corresponding callback functions.
typedef struct
{
  uint32_t pgn;
  const char *pgnAcronym; // the memory for the string is managed outside of the class
  void (*pgnCallback)(uint8_t, uint8_t *);
} PGN_KNOWN_ENTRY;

class SharkJ1939
{
public:
  SharkJ1939(MCP2515 &mcp2515);

  bool bindPGNKnownTable(PGN_KNOWN_ENTRY *pgnKnownTable) { pgnKnownTable_ = pgnKnownTable; }

  void dumpMessage(const struct can_frame &j1939Msg); // Dump received CAN messages with J1939 format to Serial

  void mirrorMessage(struct can_frame &j1939Msg, uint8_t newSA); // message mirroring with a new source address.

  void processUserInput(void); // Processes user input to pause or resume processing of J1939 messages

  static uint32_t calculateSDMDataCRC(const uint8_t dlc, const uint8_t *data); // Computes a 32-bit CRC of the SDM data in the J1939 message.

private:
  MCP2515 &mcp2515Ref_; // Reference to MCP2515 object

  PGN_KNOWN_ENTRY *pgnKnownTable_ = NULL;

  bool pauseFlag_ = false; // Flag to pause processing
  uint32_t ringBuffer_[8]; // Ring buffer to store J1939 29-bit CAN IDs (for future use).
  uint8_t ringHead_ = 0;
};

#endif
