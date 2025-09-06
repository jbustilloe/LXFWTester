/*******************************************************************************************************************************************************************************************************
 *	
 *	@File		PwrBusComm.h
 *	@Project	FW_LX_Tester
 *	@Brief		Header file with PowerBus communication functions configuration.
 *	@Version	1.4.0
 *	@Author		Andres Fernandez. IED Greenpower S.L.
 *																																											(C) COPYRIGHT IEDGREENPOWER
 ******************************************************************************************************************************************************************************************************/
//#ifndef __PWRBUSCOMM_H
//#define	__PWRBUSCOMM_H
//#include "project_config.h"
//
//
///* Variables */
////@ToDo #03 - Use struct for gather all information.
//struct LuminaryInfoStr{
//    int8 SOH;           // Porcentaje del nivel de capacidad de la batería
//    int8 SOC;           // Porcentaje de carga en carga continua
//    int8 BatTest;       // Numero de minutos en el último ciclo de descarga
//    int16 TempTest;     // Temperatura media del último ciclo de descarga en deciKelvin
//    int8 Status;
//    int16 Tamb;
//    int16 Tbat;
//    int8 DeviceType;
//    int8 SerialNum;
//    int8 ModelNum;
//} LuminaryInfo = {0,0,0,2731,0,0,0,0,0,0};
//
//int8 gCyclingAutonomy[5] = {0,0,0,0,0};							// AF - Añadido para GE Offshore proto.
//int16 gTemperaturasCiclado[5] = {2731,2731,2731,2731,2731};								// AF - Añadido para GE Offshore proto.
//
//int8 gTemporaryDeviceADR = 0;
//int8 gDeviceADR=0;
//int8 gDeviceDetectionON=0;
//int8 gFlagContinueFlipFlop = 0;
//
//int8 gRxBuffer[15];
//
//int8 gTxBytes=3;
//int8 gTxBuffer[25];
//
//int8 gCntRxByteNum=0;
//
//int8 gRxLinkSequence=0;
//
//int8 gRxDataON=0;
//int8 gTxDataON = 0;
//
//int32 gTimeoutAdrAssignation=0;
//int16 gAleatoryDelay = 0;																// Number of delay cycles to answer to PWRB_VALUE_ADR_ASIGNATION message.
//int8  gFlagAleatoryTxDelay = 0;															// Flag to activate delay to answer to PWRB_VALUE_ADR_ASIGNATION
//int8  gFlagAnswerToDetection = 0;														// Identify if a message sent was the answer to detection request.
//int8  gAdrAssignationError=0;
//int8  gTimeoutFlipFlop=0;
//
//int8 gPBSensorsState=0;
//
///* Function prototypes */
//void  PwrBusManager(void);
//void  PwrBusRxMsgManager(void);
//void  AnalizarPwrBusTxSensors(void);
//void  GestionComandosVerificacion(void);
//int8  TempDeciKelvinToGradOffset40(int16 tempDeciKelvin);
//int8  ComputeSerialCrc(int8* buffer, int8 length);
//int8  ComputeEslBusCrc(int8* buffer, int8 length);
//int16 ByteDecode8b10b (int16 byteToDecode);
//int16 ByteCode8b10b (int8 byteToCode);
//
///* Comunication protocol definitions */
//#ifndef __PWRBUS_MESSAGE_MASKS_DISABLE
//    // PWRBUS address masks
//    #define PWRB_BROADCAST                          0xFF								// All connected devices will receive the instruction.
//    #define PWRB_SWITCH_ADR                         0x00								// Master address.
//	#define MY_ADDRESS_RESET_VALUE					0xFE								// Device Address Reset Value.
//
//    // Communication protocol constants
//    #define PWRB_START_SEQUENCE                     0b10111111							// PwrBus Transmit Start Sequence.
//    #define PWRB_NUM_OF_SILENCE_BITS                6									// Number of consecutive zeros to consider clear bus for tx.    
//	#define PWRB_ADR_ASSIGNATION_TIME_OUT_CYCLES    180000           				    // Address assignment response timeout cycles. 100Hz (120sg) after which waiting is stopped and the output relay is opened.
//	#define PWRB_FLIP_FLOP_TIMEOU_500MS		        50              				    // Cycles waiting to receive network signal on communication bus for I/O relay detection at 100Hz.
//	#define PWRB_MAX_RX_MESSAGE_BYTE_LENGTH			6
//    
//    // PWRBus messages masks
//	#define PWRB_INST_DETECT_INIT                   0x00                                // Initializes system structure detection and configuration.
//	#define PWRB_INST_DETECT_CANCEL                 0x01                                // Cancels system structure detection and configuration.
//	#define PWRB_INST_LED_ON                        0x02                                // Turns light off.
//	#define PWRB_INST_LED_OFF                       0x03                                // Turns light on.
//	#define PWRB_INST_BLINK_ON                      0x04                                // Turns light blink on.
//	#define PWRB_INST_BLINK_OFF                     0x05                                // Turns light blink off.
//	#define PWRB_INST_TEST_ON                       0x06                                // Starts full discharge autonomy test.
//	#define PWRB_INST_TEST_OFF                      0x07                                // Cancels full discharge autonomy test.
//	#define PWRB_INST_CYCLING_ON                    0x08                                // Starts battery cycling.
//	#define PWRB_INST_CYCLING_OFF                   0x09                                // Cancels battery cycling.
//	#define PWRB_INST_SMALL_BLINK_ON                0x0A                                // Turns light blink (80%-100%) on.
//
//	#define PWRB_INST_ADR_ASIGNATION                0x0B                                // After intialization, asigns a direction to an element.
//	#define PWRB_INST_ADR_ASIG_ANW_A                0x0C                                // Answer to direction asignation, when comm input is connector A
//	#define PWRB_INST_ADR_ASIG_ANW_B                0x0D                                // Answer to direction asignation, when comm input is connector B
//	#define PWRB_INST_CLOSE_RELAY                   0x0E                                // Close output relay.
//
//	#define PWRB_INST_ADDR_OK                       0x0D                                // Asigned address is OK, confirmation to slave.
//	#define PWRB_INST_ADR_ACK                        0x0E                                // Acknowledge answer from slave. Control remains in master.
//
//	#define PWRB_INST_READ_P_SOH                    0x20                                // State of Health (0% - 100%), 8 bits.
//	#define PWRB_INST_READ_P_SOC                    0x21                                // State of Charge (0% - 100%), 8 bits.
//	#define PWRB_INST_READ_P_TEST_BAT               0x22                                // Battery autonomy test information.
//	#define PWRB_INST_READ_P_TEMP_TEST              0x23                                // Battery autonomy test medium temperature (celsius with offset +40).
//	#define PWRB_INST_READ_P_STATUS_1               0x24                                // Hardware status 1.
//	#define PWRB_INST_READ_P_STATUS_2               0x25                                // Hardware status 2.
//	#define PWRB_INST_READ_P_BAT_TEMP               0x26                                // Battery temperature (celsius with offset +40).
//	#define PWRB_INST_READ_P_INT_TEMP               0x27                                // Internal temperature (celsius with offset +40).
//	#define PWRB_INST_READ_P_TIME_CYCLE_1           0x28                                // Read battery cycle time 1
//	#define PWRB_INST_READ_P_TIME_CYCLE_2           0x29                                // Read battery cycle time 2
//	#define PWRB_INST_READ_P_TIME_CYCLE_3           0x2A                                // Read battery cycle time 3
//	#define PWRB_INST_READ_P_TIME_CYCLE_4           0x2B                                // Read battery cycle time 4
//	#define PWRB_INST_READ_P_TIME_CYCLE_5           0x2C                                // Read battery cycle time 5
//	#define PWRB_INST_READ_P_TEMP_CYCLE_1           0x2D                                // Read battery cycle temperature 1
//	#define PWRB_INST_READ_P_TEMP_CYCLE_2           0x2E                                // Read battery cycle temperature 2
//	#define PWRB_INST_READ_P_TEMP_CYCLE_3           0x2F                                // Read battery cycle temperature 3
//	#define PWRB_INST_READ_P_TEMP_CYCLE_4           0x30                                // Read battery cycle temperature 4
//	#define PWRB_INST_READ_P_TEMP_CYCLE_5           0x31                                // Read battery cycle temperature 5
//	#define PWRB_INST_READ_P_SERIAL_H               0x32                                // Serial number high byte.
//	#define PWRB_INST_READ_P_SERIAL_L               0x33                                // Serial number low byte.
//	#define PWRB_INST_READ_P_MODEL_12               0x34                                // First and second digit from model type number.
//	#define PWRB_INST_READ_P_MODEL_34               0x35                                // Third and fourth digit from model type number.
//	#define PWRB_INST_READ_P_MODEL_56               0x36                                // Fith and sixth digit from model type number.
//	#define PWRB_INST_READ_P_MODEL_78               0x37                                // Seventh and eight digit from model type number.
//	#define PWRB_INST_READ_P_MODEL_9A               0x38                                // Ninth and tenth digit from model type number.
//	#define PWRB_INST_READ_P_ELEMENT_TYPE           0x39                                // Element type description (LX-REM, LX-SOM, LV-Cloud)
//	#define PWRB_INST_READ_P_SERV_LUM_POWER         0x3A                                // Nominal service luminous power divided by 100 (i.e. 27 = 2700 lm)
//	#define PWRB_INST_READ_P_EMERG_LUM_POWER        0x3B                                // Emergency luminous power as a % of nominal lumen output.
//	#define PWRB_INST_READ_P_EMERG_AUTNONOMY        0x3C                                // Nominal emergency autonomy in minutes.
//	#define PWRB_INST_READ_INFO                     0x3D                                // Read element constant information (SN, Model, Type).
//	#define PWRB_INST_READ_DATA                     0x3E                                // Read element variable data (SOH, SOC, Temps, Status, Sensors, Alarms...)
//	#define PWRB_COM_RX_ERROR                       0x3F                                // PWRBus communication error.
//
//	#define PWRB_VALUE_P_SOH                        0x41                                // Answer to Read State of Health
//	#define PWRB_VALUE_P_SOC                        0x42                                // Answer to Read State of Charge
//	#define PWRB_VALUE_P_TEST_BAT                   0x43                                // Answer to Read Battery autonomy test information
//	#define PWRB_VALUE_P_TEMP_TEST                  0x44                                // Answer to Read autonomy test medium temperature
//	#define PWRB_VALUE_P_STATUS_1                   0x45                                // Answer to Read hardware status 1 parameter
//	#define PWRB_VALUE_P_STATUS_2                   0x46                                // Answer to Read hardware status 2 parameter
//	#define PWRB_VALUE_P_BAT_TEMP                   0x47                                // Answer to Read battery temperature
//	#define PWRB_VALUE_P_INT_TEMP                   0x48                                // Answer to Read internal temperature
//	#define PWRB_VALUE_P_TIME_CYCLE_1               0x49                                // Answer to Read batt cycle time 1
//	#define PWRB_VALUE_P_TIME_CYCLE_2               0x4A                                // Answer to Read batt cycle time 2
//	#define PWRB_VALUE_P_TIME_CYCLE_3               0x4B                                // Answer to Read batt cycle time 3
//	#define PWRB_VALUE_P_TIME_CYCLE_4               0x4C                                // Answer to Read batt cycle time 4
//	#define PWRB_VALUE_P_TIME_CYCLE_5               0x4D                                // Answer to Read batt cycle time 5
//	#define PWRB_VALUE_P_TEMP_CYCLE_1               0x4E                                // Answer to Read batt cycle temperature 1
//	#define PWRB_VALUE_P_TEMP_CYCLE_2               0x4F                                // Answer to Read batt cycle temperature 2
//	#define PWRB_VALUE_P_TEMP_CYCLE_3               0x50                                // Answer to Read batt cycle temperature 3
//	#define PWRB_VALUE_P_TEMP_CYCLE_4               0x51                                // Answer to Read batt cycle temperature 4
//	#define PWRB_VALUE_P_TEMP_CYCLE_5               0x52                                // Answer to Read batt cycle temperature 5
//	#define PWRB_VALUE_P_SERIAL_H                   0x53                                // Answer to Read serial number high byte
//	#define PWRB_VALUE_P_SERIAL_L                   0x54                                // Answer to Read serial number low byte
//	#define PWRB_VALUE_P_MODEL_12                   0x55                                // Answer to Read First and Second model type number
//	#define PWRB_VALUE_P_MODEL_34                   0x56                                // Answer to Read Third and Fourth model type number
//	#define PWRB_VALUE_P_MODEL_56                   0x57                                // Answer to Read Fith and Sixth model type number
//	#define PWRB_VALUE_P_MODEL_78                   0x58                                // Answer to Read Seventh and Eighth model type number
//	#define PWRB_VALUE_P_MODEL_9A                   0x59                                // Answer to Read Ninth and Tenth model type number
//	#define PWRB_VALUE_ELEMENT_TYPE                 0x5A                                // Answer to Read element type information
//	#define PWRB_VALUE_P_SERV_LUM_POWER             0x5B                                // Answer to Read element service luminous power information
//	#define PWRB_VALUE_P_EMERG_LUM_POWER            0x5C                                // Answer to Read element emergency luminous power information
//	#define PWRB_VALUE_ELEMENT_EMERG_AUTNONOMY      0x5D                                // Answer to Read element emergency autonomy information
//	#define PWRB_VALUE_INFO_P                       0x5E                                // Answer to Read_Info
//	#define PWRB_VALUE_DATA_P                       0x5F                                // Answer to Read_Data
//
//   // Length of messages
//    #define PWRB_INST_LENGTH						3									// Instructions like message lenght, including CRC
//    #define PWRB_INST_CLOSE_RELAY_LENGTH			4									// Instruction for closing relay message lenght, including CRC
//    #define PWRB_INST_ADR_OK_LENGTH					6									// Instruction for address confirmation lenght, including CRC
//    #define PWRB_VALUE_LENGHT						5									// Read value like message lenght, including CRC
//    #define PWRB_INFO_LENGTH						12									// Answer to read info message lenght, including CRC
//    #define PWRB_DATA_LENGTH						21									// Answer to read data message lenght, including CRC
//
//    //@ToDo #04 - Verify this codes PWRBus receive message error codes (PWRBusReceiveMessage_R0.pdf)
//    #define PWRBRM_NO_RX_SYNC                       0x00								// No Rx Sync.
//    #define PWRBRM_NEW_PWRBUS_MESSAGE               0x01								// New message.
//    #define PWRBRM_PWRBUS_CRC_ERR                   0x02								// CRC error.
//    #define PWRBRM_DECODING_ERR                     0x03								// 8b10b decoding error.
//    #define PWRBRM_RXBYTE_ERR                       0x04								// RxByte error.
//#endif
//
///* Element parameter's list */
//#ifndef __PWRBUS_PARAM_LIST
//	#define PARAM_ELEMENT_TYPE                      0x00                                // Type of element: 0 - Switch, 1 - LX_REM, 2 - LX_SOM, 3 - LV_CLOUD
//	#define PARAM_SERV_LUM_POWER                    0x01                                // Nominal service luminous power divided by 100 (i.e. 27 = 2700 lm)
//	#define PARAM_EMERG_LUM_POWER                   0x02                                // Emergency luminous power as a % of nominal lumen output.
//	#define PARAM_EMERG_AUTNONOMY                   0x03                                // Nominal emergency autonomy in minutes.
//	#define PARAM_SOH                               0x04                                // State of Health (0% - 100%), 8 bits.
//	#define PARAM_SOC                               0x05                                // State of Charge (0% - 100%), 8 bits.
//	#define PARAM_TEST_BATT                         0x06                                // Battery autonomy test information.
//	#define PARAM_TEMP_TEST                         0x07                                // Battery autonomy test medium temperature (celsius with offset +40).
//	#define PARAM_STATUS_1                          0x08                                // Hardware status register 1
//	#define PARAM_STATUS_2                          0x09                                // Hardware status register 2
//	#define PARAM_INT_TEMP                          0x0A                                // Internal temperature (celsius with offset +40).
//	#define PARAM_CYCLE_TIME_1                      0x0B                                // Autonomy minutes of battery cycling 1
//	#define PARAM_CYCLE_TEMP_1                      0x0C                                // Mean temperature during battery cycling 1
//	#define PARAM_CYCLE_TIME_2                      0x0D                                // Autonomy minutes of battery cycling 2
//	#define PARAM_CYCLE_TEMP_2                      0x0E                                // Mean temperature during battery cycling 2
//	#define PARAM_CYCLE_TIME_3                      0x0F                                // Autonomy minutes of battery cycling 3
//	#define PARAM_CYCLE_TEMP_3                      0x10                                // Mean temperature during battery cycling 3
//	#define PARAM_CYCLE_TIME_4                      0x11                                // Autonomy minutes of battery cycling 4
//	#define PARAM_CYCLE_TEMP_4                      0x12                                // Mean temperature during battery cycling 4
//	#define PARAM_CYCLE_TIME_5                      0x13                                // Autonomy minutes of battery cycling 5
//	#define PARAM_CYCLE_TEMP_5                      0x14                                // Mean temperature during battery cycling 5
//	#define PARAM_SERIAL_H                          0x15                                // Serial number high byte.
//	#define PARAM_SERIAL_L                          0x16                                // Serial number low byte
//	#define PARAM_MODEL_12                          0x17                                // First and second digit from model type number.
//	#define PARAM_MODEL_34                          0x18                                // Third and fourth digit from model type number.
//	#define PARAM_MODEL_56                          0x19                                // Fith and sixth digit from model type number.
//	#define PARAM_MODEL_78                          0x1A                                // Seventh and eight digit from model type number.
//	#define PARAM_MODEL_9A                          0x1B                                // Ninth and tenth digit from model type number.
//	#define PARAM_LX_LUM_TOTAL                      0x1C                                // Total number of parameters.
//	// PARAM_STATUS_1 bit mask
//	#define BIT_BATT_TEST                           7                                   // 0 = Test not running, 1 = Test running
//	#define BIT_CHARG_ERR                           6                                   // 0 = Charger OK, 1 = Charger Error
//	#define BIT_BATT_ERR                            5                                   // 0 = Battery Error, 1 = Battery OK
//	#define BIT_FA_ERR                              4                                   // 0 = P.S. Error, 1 = P.S. Ok
//	#define BIT_HTRD_ERR                            3                                   // 0 = Battery heater driver Error, 1 = Battery heater driver OK
//	#define BIT_HTRB_ERR                            2                                   // 0 = Battery heater board error, 1 = Battery heater board OK
//	#define BIT_LEDS_ERR                            1                                   // 0 = LED driver error, 1 = LED driver Ok
//	#define BIT_LEDS_ON                             0                                   // 0 = LEDs Off, 1 = LEDs On
//	#define ELEMENT_ERROR_MASK						0b01111110
//	// PARAM_STATUS_2 bit mask
//	#define BIT_BATT_CHANGE                         7                                   // 0 = Battery OK, 1 = Battery change
//	#define BIT_BATT_CYCLING                        6                                   // 0 = Cycling not running, 1 = Cycling running
//	#define BIT_BATT_CYCLING_NUM                    3                                   //  = Bits 3-5 = Number of cycling (1-5),  = 
//	#define BIT_BATT_CYCLING_DISCH                  2                                   // 0 = Not discharging, 1 = Discharging
// #endif
//
///* Constants stored in ROM for 8b10b and CRC */
//const int8 THREE_BIT_TO_FOUR_BIT[8] = {
////  0, 1, 2, 3, 4, 5, 6, 7
//    4, 9, 5, 3, 2,10, 6, 1};									// 3b to 4b constant conversion table.
//const int8 FOUR_BIT_TO_THREE_BIT[11] ={
////  0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 10
// 0xFF,0x07,0x04,0x03,0x00,0x02,0x06,0xFF,0xFF,0x01,0x05};									// 4b to 3b constant conversion table.
//const int8 FIVE_BIT_TO_SIX_BIT[32] = {
////   0, 1, 2, 3, 4, 5, 6, 7
//    39,29,45,49,53,41,25,56,
////   8, 9,10,11,12,13,14,15
//    57,37,21,52,13,44,28,23,
////  16,17,18,19,20,21,22,23
//    27,35,19,50,11,42,26,58,
////  24,25,26,27,28,29,33,31
//    51,38,22,54,14,46,30,43};										// 5b to 6b constant conversion table.
//const int8 SIX_BIT_TO_FIVE_BIT[48] = {
////  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
// 0x14,0xFF,0x0C,0x1C,0xFF,0xFF,0xFF,0xFF,0x12,0xFF,
//// 10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
// 0x0A,0x1A,0x0F,0xFF,0x06,0x16,0x10,0x0E,0x01,0x1E,
//// 20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
// 0xFF,0xFF,0xFF,0xFF,0x11,0xFF,0x09,0x19,0x00,0xFF,
//// 30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
// 0x05,0x15,0x1F,0x0D,0x02,0x1D,0xFF,0xFF,0x03,0x13,
//// 40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
// 0x18,0x0B,0x04,0x1B,0xFF,0x07,0x08,0x17};									// 6b to 5b constant conversion table.
//
//#ifndef __COMPILE_CRC_LUT_0X25															// To prevent recursive declaration of crc look up table.
//	#define __COMPILE_CRC_LUT_0X25
//	const int8 CRC_LUT_0X25[256] = {
//    0x00, 0x25, 0x4a, 0x6f, 0x94, 0xb1, 0xde, 0xfb, 0x0d, 0x28, 0x47, 0x62, 0x99, 
//    0xbc, 0xd3, 0xf6, 0x1a, 0x3f, 0x50, 0x75, 0x8e, 0xab, 0xc4, 0xe1, 0x17, 0x32, 
//    0x5d, 0x78, 0x83, 0xa6, 0xc9, 0xec, 0x34, 0x11, 0x7e, 0x5b, 0xa0, 0x85, 0xea, 
//    0xcf, 0x39, 0x1c, 0x73, 0x56, 0xad, 0x88, 0xe7, 0xc2, 0x2e, 0x0b, 0x64, 0x41, 
//    0xba, 0x9f, 0xf0, 0xd5, 0x23, 0x06, 0x69, 0x4c, 0xb7, 0x92, 0xfd, 0xd8, 0x68, 
//    0x4d, 0x22, 0x07, 0xfc, 0xd9, 0xb6, 0x93, 0x65, 0x40, 0x2f, 0x0a, 0xf1, 0xd4, 
//    0xbb, 0x9e, 0x72, 0x57, 0x38, 0x1d, 0xe6, 0xc3, 0xac, 0x89, 0x7f, 0x5a, 0x35, 
//    0x10, 0xeb, 0xce, 0xa1, 0x84, 0x5c, 0x79, 0x16, 0x33, 0xc8, 0xed, 0x82, 0xa7, 
//    0x51, 0x74, 0x1b, 0x3e, 0xc5, 0xe0, 0x8f, 0xaa, 0x46, 0x63, 0x0c, 0x29, 0xd2, 
//    0xf7, 0x98, 0xbd, 0x4b, 0x6e, 0x01, 0x24, 0xdf, 0xfa, 0x95, 0xb0, 0xd0, 0xf5, 
//    0x9a, 0xbf, 0x44, 0x61, 0x0e, 0x2b, 0xdd, 0xf8, 0x97, 0xb2, 0x49, 0x6c, 0x03, 
//    0x26, 0xca, 0xef, 0x80, 0xa5, 0x5e, 0x7b, 0x14, 0x31, 0xc7, 0xe2, 0x8d, 0xa8, 
//    0x53, 0x76, 0x19, 0x3c, 0xe4, 0xc1, 0xae, 0x8b, 0x70, 0x55, 0x3a, 0x1f, 0xe9, 
//    0xcc, 0xa3, 0x86, 0x7d, 0x58, 0x37, 0x12, 0xfe, 0xdb, 0xb4, 0x91, 0x6a, 0x4f, 
//    0x20, 0x05, 0xf3, 0xd6, 0xb9, 0x9c, 0x67, 0x42, 0x2d, 0x08, 0xb8, 0x9d, 0xf2, 
//    0xd7, 0x2c, 0x09, 0x66, 0x43, 0xb5, 0x90, 0xff, 0xda, 0x21, 0x04, 0x6b, 0x4e, 
//    0xa2, 0x87, 0xe8, 0xcd, 0x36, 0x13, 0x7c, 0x59, 0xaf, 0x8a, 0xe5, 0xc0, 0x3b, 
//    0x1e, 0x71, 0x54, 0x8c, 0xa9, 0xc6, 0xe3, 0x18, 0x3d, 0x52, 0x77, 0x81, 0xa4, 
//    0xcb, 0xee, 0x15, 0x30, 0x5f, 0x7a, 0x96, 0xb3, 0xdc, 0xf9, 0x02, 0x27, 0x48, 
//    0x6d, 0x9b, 0xbe, 0xd1, 0xf4, 0x0f, 0x2a, 0x45, 0x60};										// PwrBus Look up table for CRC calculation.
//#endif
//
//#endif //__PWRBUSCOMM_H
///*----------------------------------------------------------------------------------- (C) COPYRIGHT IEDGREENPOWER ------------------------------------------------------------------------------------*/
