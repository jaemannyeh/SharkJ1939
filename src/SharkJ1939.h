#ifndef _SHARKJ1939_H_
#define _SHARKJ1939_H_

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

  bool bindPGNKnownTable(PGN_KNOWN_ENTRY* pgnKnownTable) { pgnKnownTable_ = pgnKnownTable; }

  void dumpMessage(struct can_frame &j1939Msg);

  void processUserInput(void);

private:
  MCP2515 &mcp2515Ref_; // Reference to MCP2515 object

  PGN_KNOWN_ENTRY* pgnKnownTable_ = NULL;
  PGN_KNOWN_ENTRY pgnBindTable_[4] = {{.pgn = 0}};

  bool pauseFlag_ = false; // Flag to pause processing
  uint8_t userInput_[2];
  uint32_t ringBuffer_[8];
  uint8_t ringHead_ = 0;
};

#endif
