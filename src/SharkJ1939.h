#ifndef _SHARKJ1939_H_
#define _SHARKJ1939_H_

// PGN_KNOWN_ENTRY struct is used for associating PGNs with corresponding callback functions.
typedef struct
{
  uint32_t pgn;
  const char *pgnAcronym; // the memory for the string is managed outside of the class
  void (*pgnCallback)(uint8_t *);
} PGN_KNOWN_ENTRY;

class SharkJ1939
{
public:
  SharkJ1939(MCP2515 &mcp2515) : mcp2515Ref_(mcp2515) {}

  bool bindPGNKnownTable(PGN_KNOWN_ENTRY *pgnKnownTable) { pgnKnownTable_ = pgnKnownTable; }

  void dumpMessage(struct can_frame &j1939Msg); // Dump received CAN messages with J1939 format to Serial

  void processUserInput(void); // Processes user input to pause or resume processing of J1939 messages

private:
  MCP2515 &mcp2515Ref_; // Reference to MCP2515 object

  PGN_KNOWN_ENTRY *pgnKnownTable_ = NULL;

  bool pauseFlag_ = false; // Flag to pause processing
  uint32_t ringBuffer_[8]; // Ring buffer to store J1939 29-bit CAN IDs (for future use).
  uint8_t ringHead_ = 0;
};

#endif
