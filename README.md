# SharkJ1939

SharkJ1939 is a communication library that allows sniffing and displaying of J1939 CAN messages using the MCP2515 driver.

## Requirements

To use SharkJ1939, you need to install the following Arduino library:

arduino-mcp2515 (https://github.com/autowp/arduino-mcp2515)

## How to use

You can refer to the examples folder for usage of the SharkJ1939 API. Example code on how to use the library can be found in the examples folder of the library's repository. Additionally, you can create your own PGN (Parameter Group Number) table as shown below:
```c
static PGN_KNOWN_ENTRY pgnKnownTable[] = {
    {256, "TC1", tc1_callback}, // PGN 256(=0x0100) Transmission Control 1 with 'tc1_callback()'.
    {3584, "SHM", NULL}, // PGN 3584(=0xE00) Safety Header Message with no callback function
    {0xBF00, "PCM15", NULL},
    {0xC000, "PCM16", NULL},
    {0xEE00, "AC", NULL},
    {0xEEC2, "EEC2", NULL},
    {0, NULL, NULL} // The last entry with 0 and NULL to indicates the end of the table.
};
```
Please note that the above table is just for illustration purposes and you can create your own PGN table based on your specific requirements.

Here are some sample J1939 messages displayed in the Serial Display:
```text
18EEFF05 8 10 00 40 00 00 05 02 10 1000055821 FF AC
0C0E0305 8 07 FA FC FE 6C 3B 60 1B 1000055869 03 SHM
0C010305 8 FF FF 7D FF FF 3F 3F FF 1000055871 03 TC1 N
```
Note 1: The above messages are just examples and may not represent real-world J1939 messages.  
Note 2: Press the Enter key to toggle between pause and resume.



