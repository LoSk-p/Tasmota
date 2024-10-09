/*
  xdrv_100_1_robonomics.ino.ino - Robonomics Network support for Tasmota
*/

#define USE_ROBONOMICS
#ifdef USE_ROBONOMICS
/*********************************************************************************************\
 * My IoT Device with command support
 *
 *
\*********************************************************************************************/

#warning **** Robonomics Driver is included... ****

#define XDRV_100 100

#include <Ed25519.h>
#include <address.h>
// #include <Robonomics.h>

uint8_t robonomicsPrivateKey[32];
uint8_t robonomicsPublicKey[32];
std::string robonomicsSs58Address;
// Robonomics robonomics;


/*********************************************************************************************\
 * My IoT Device Functions
\*********************************************************************************************/

// This variable will be set to true after initialization
bool initSuccess = false;

/* 
  Optional: if you need to pass any command for your device 
  Commands are issued in Console or Web Console
  Commands:
    Say_Hello  - Only prints some text. Can be made something more useful...
    SendMQTT   - Send a MQTT example message
    Help       - Prints a list of available commands  
*/

const char RobonomicsCommands[] PROGMEM = "|"  // No Prefix
  "Print_Address|SendDatalog";

void (* const RobonomicsCommand[])(void) PROGMEM = { &CmdPrint_Address, &CmdSend_Datalog };

void CmdPrint_Address(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("Robonomics address: %s"), robonomicsSs58Address.c_str());
  ResponseCmndDone();
}

void CmdSend_Datalog(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("Sending Datalog: %s"), robonomicsSs58Address.c_str());
  // robonomics.sendDatalogRecord("Hello");
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Tasmota Functions
\*********************************************************************************************/



void RobonomicsInit()
{
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Robonomics init..."));
  RobonomicsPublicKey public_key;
  Ed25519::generatePrivateKey(robonomicsPrivateKey);
  Ed25519::derivePublicKey(robonomicsPublicKey, robonomicsPrivateKey);
  memcpy(public_key.bytes, robonomicsPublicKey, PUBLIC_KEY_LENGTH);
  robonomicsSs58Address = AddressUtils::getAddrFromPublicKey(public_key);
  // SettingsUpdateText(SET_ROBONOMICS_PRIVATE_KEY, (const char*)robonomicsPrivateKey)
  initSuccess = true;
  // robonomics.setup();
}



void RobonomicsProcessing(void)
{

  /*
    Here goes My Project code.
    Usually this part is included into loop() function
  */

}






/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv100(uint32_t function)
{


  bool result = false;

  if (FUNC_INIT == function) {
    RobonomicsInit();
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("My project init is done..."));
  }
  else if (initSuccess) {

    switch (function) {
      // Select suitable interval for polling your function
        case FUNC_EVERY_SECOND:
//   case FUNC_EVERY_250_MSECOND:
//    case FUNC_EVERY_200_MSECOND:
//    case FUNC_EVERY_100_MSECOND:
        RobonomicsProcessing();
        break;

      // Command support
      case FUNC_COMMAND:
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Calling Robonomics Command..."));
        result = DecodeCommand(RobonomicsCommands, RobonomicsCommand);
        break;

    }

  }

  return result;
}

#endif  // USE_MY_PROJECT_CMD