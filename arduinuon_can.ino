#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsgTx;    // tx variable
struct can_frame canMsgRx;    // rx variable
MCP2515 mcp2515(10);  //MCP CS Pin

const uint8_t totalIdCount = 13;
const uint8_t canTxBytes[8] = {0};
uint32_t canBaseId = 0x00;

const uint32_t targetCANIds[] = {
  0x18500140,
  0x18570140,
  0x18620140,
  0x18630140,
  0x18900140,
  0x18910140,
  0x18920140,
  0x18930140,
  0x18940140,
  0x18950140,
  0x18960140,
  0x18970140,
  0x18980140
};

uint8_t CAN_Tx(uint32_t id, uint8_t *pData) {
  canMsgTx.can_id  = id | CAN_EFF_FLAG;
  canMsgTx.can_dlc = 8;
  memcpy(canMsgTx.data, pData, 8);
  return mcp2515.sendMessage(&canMsgTx);
}

void CAN_Rx() {
  while (mcp2515.readMessage(&canMsgRx) == MCP2515::ERROR_OK)  {
    Serial.print(canMsgRx.can_id & 0x1FFFFFFFUL, HEX);
    Serial.print(" ");
    Serial.print(canMsgRx.can_dlc, HEX);
    for (byte i = 0; i < 8; i++)    {
      Serial.print(" ");
      Serial.print(canMsgRx.data[i], HEX);
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  for (uint8_t i = 0; i < totalIdCount; i++)  {
    canBaseId = targetCANIds[i];
    CAN_Tx(canBaseId, canTxBytes);
    delay(100);  // Allow time for responses
    CAN_Rx();
    delay(1000);
  }
}
