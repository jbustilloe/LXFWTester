/*******************************************************************************************************************************************************************************************************
 *	
 *	@File		PwrBusComm.c
 *	@Project	FW_LX_REMOTE
 *	@Brief		File with PowerBus communication functions.
 *	@Version	1.4.0
 *	@Author		Andres Fernandez.
 * 
 *  @Details	
 *		- During the positive cycle of the permanent phase signal, the light is switched on or off.
 *		- During the negative cycle of the permanent phase, communication between the switch and the luminaires takes place.
 *		- Detection of simultaneous transmissions. If two luminaires are transmitting at the same time, one of the transmissions is deferred.
 *																																											(C) COPYRIGHT IEDGREENPOWER
 ******************************************************************************************************************************************************************************************************/
//#include "PwrBusComm.h"
//
///* Global variable defined in this module */
//int8 gPostponeTx=0;  
//
///* 
// * @brief	Management of PowerBusCommunication phisical layer for Tx and Rx.
// * @param	none.
// * @retval	none.
// * @details	- During positive cycle of sine-wave of permanent power, LEDs ON/OFF command is sent.
// *			- During negative cycle of sine-wave of permanent power, PowerBus Communication is handled.
// *			- LX-Switch is the power bus line manager, LX Luminaires are slaves.
// *			- Collisions are detected thanks to CRC.
// */
//void PwrBusManager(void) {
//	// Macro to check constants.
//	
//	// Local variables.
//	static int8 txBit = 0;
//	static int8 lastTxBit = 0;
//	static int8 cntCycles = 0;
//	static int16 rxData = 0;
//	static int8 txStage = 0;
//	static int8 cntRxBitNum = 0;
//	static int8 cntTxBitNum = 0;
//	static int8 cntTxByteNum = 0;
//	static int16 cntBitsSilencio = 0;
//	static int8 flag6BitSilence = 0;
//	
//	static int8 txDelayCycleCounter = 0;
//	static int8 flagTransmitting = 0;
//	static int8 finalTxDelay = 0;
//	
//	
//	// Algorithm  
//	if(gPermanentPowerStatus != gPermanentPowerPreviousStatus){							// Flange of sinwave cycle change.
//		cntCycles = 0;
//		txBit = 0;
//	} else {
//		cntCycles ++;
//	}
//	
//	/* Negative cycle of the sine wave */
//	if(cntCycles == 1 && gPermanentPowerStatus == 0){									// Flange + No permanent = Negative Cycle.
//		// Delay to Tx transmision
//		if(gFlagAleatoryTxDelay){
//			txDelayCycleCounter ++;	
//			if(txDelayCycleCounter >= finalTxDelay) {
//				txDelayCycleCounter = 0;
//				gFlagAleatoryTxDelay = 0;
//				flag6BitSilence = 0;													// Start counting silence after delay.
//			}
//		} else {
//			txDelayCycleCounter = 0;
//			finalTxDelay = gAleatoryDelay & 0x7F;
//		}
//		
//		// @ToDo #05 - Think if it is worth implementing the detection of a colission by reading own transmitting message.
//		
//		// If the device is in the identification stage.
//		if(gDeviceDetectionON) {
//			gTimeoutAdrAssignation++;
//			if(gTimeoutAdrAssignation >= PWRB_ADR_ASSIGNATION_TIME_OUT_CYCLES) {
//				// Auto-detection time-out error.
//				gTimeoutAdrAssignation = 0;
//				gTimeoutFlipFlop = 0;
//				gAdrAssignationError = 1;
//				gDeviceDetectionON = 0;
//				output_low(COMM_CONN_A_DIS);												// Close relay to allow further communication.
//				output_low(COMM_CONN_B_DIS);												// Close relay to allow further communication.
//			} else if(gRxPwrBus) {
//				// End of Input-Output relay swap.
//				gTimeoutFlipFlop = 0;
//				// @ToDo #08 - Save in EEPROM gPwrBusRelay_A_B?
//			} else if(!gRxPwrBus && gFlagContinueFlipFlop) {
//				// Input-Output relay swap.
//				gTimeoutFlipFlop++;
//				if(gTimeoutFlipFlop >= PWRB_FLIP_FLOP_TIMEOU_500MS) {
//					gTimeoutFlipFlop = 0;
//					if(gFlagPwrBusRelay_A_B) {
//						gFlagPwrBusRelay_A_B = 0;
//					} else {
//						gFlagPwrBusRelay_A_B = 1;
//					}
//					if(!gFlagPwrBusRelay_A_B) {
//						output_high(COMM_CONN_B_DIS);
//						delay_ms(20);
//						output_low(COMM_CONN_A_DIS);
//					} else {
//						output_high(COMM_CONN_A_DIS);
//						delay_ms(20);
//						output_low(COMM_CONN_B_DIS);
//					}
//				}
//			}
//		}
//		
//		// Device is not transmitting data.
//		if(flagTransmitting == 0){
//			cntTxBitNum = 0;
//			if(flag6BitSilence && gRxPwrBus) {											// After a 6-bit silence, reading message starts.
//				gRxDataON = 1;
//				cntRxBitNum = 0;
//			}
//			if(!gRxPwrBus) {															// Counter of 6-bit silence.
//				cntBitsSilencio ++;
//				if(cntBitsSilencio > PWRB_NUM_OF_SILENCE_BITS) {
//					flag6BitSilence = 1;
//					cntBitsSilencio --;
//				}
//			} else {																	// If during silence count a bit is received, silence has been broken.
//				flag6BitSilence = 0;
//				cntBitsSilencio = 0;
//				if(gFlagAnswerToDetection) {											// If waiting to answer to detection request, if not a silence, discard sending message.
//					gDeviceADR = MY_ADDRESS_RESET_VALUE;								// Reset Device Address to default value.
//					gTxDataON = 0;														// Discard sending the message. New ADR will be asigned.
//					gFlagAnswerToDetection = 0;
//					gPostponeTx = 0;
//				}
//			}
//		}
//        
//		// Error in receiving message. More bytes have been received.
//		if(gCntRxByteNum > PWRB_MAX_RX_MESSAGE_BYTE_LENGTH) {
//			gRxDataON = 0; 
//			if(gPostponeTx == 0) {
//				gTxDataON = 0; 
//			}
//			gPostponeTx = 0;
//			txStage = 0; 
//			gCntRxByteNum = 0;
//		}
//		
//		// Message reception.
//		if(flag6BitSilence && gRxDataON == 2) {											// Analyze Rx message after reception and a silence of 6-bit.
//			if(gTxDataON == 0 || gPostponeTx == 1) {
//				PwrBusRxMsgManager();
//			} else {
//				gTxDataON = 0;
//				txStage = 0;
//			}
//			cntRxBitNum = 0;
//			gCntRxByteNum = 0;
//			gRxDataON = 0;
//			rxData = 0;
//			gPostponeTx = 0;
//		} else if(gRxDataON == 1) {														// Link frame.
//			rxData |= (int16)gRxPwrBus << cntRxBitNum;
//			cntRxBitNum++;
//			if(cntRxBitNum == 8) {
//				gRxLinkSequence = rxData;
//				gCntRxByteNum = 0;
//				gRxDataON = 2;
//				cntRxBitNum = 0;
//				rxData=0;
//			}
//		} else if(gRxDataON == 2) {														// Read message.
//			rxData |= (int16)gRxPwrBus << cntRxBitNum;
//			cntRxBitNum++;
//			if(cntRxBitNum == 10){
//				gRxBuffer[gCntRxByteNum] = (int8)ByteDecode8b10b(rxData);
//				cntRxBitNum = 0;
//				rxData = 0;
//				gCntRxByteNum++;
//			}
//		}
//	} else if(cntCycles == 1 && gPermanentPowerStatus == 1) {							// PhotoTriac is zero crossing, Tx line will be switched in the following SineWave cycle.
//		
//		/* Write PowerBus line in positive cycle of SineWave */
//		txBit = 0;
//		if(gFlagAleatoryTxDelay == 0){													// No aleatory delay is ON.
//			if(gTxDataON == 1  && gPostponeTx == 0) {
//				switch(txStage) {
//					case 0:																// Stage 0 -> Silence.
//						txBit = 0;
//						if(flag6BitSilence) {											// Wait for a silence.
//							txStage = 1;
//							case 1:														// Stage 1 -> Send link frame.
//								flagTransmitting = 1;
//								txBit = (PWRB_START_SEQUENCE >> cntTxBitNum) & 0x01;	// Calculate txBit according to link frame.
//								cntTxBitNum++;
//								if(cntTxBitNum == 8){									// When all link frame has been transmitted.
//									cntTxBitNum = 0;
//									cntTxByteNum = 0;
//									txStage = 2;										// Go to stage 2
//
//								}
//						}
//						break;
//					case 2:																// Stage 2 -> Message body.
//						txBit = (ByteCode8b10b(gTxBuffer[cntTxByteNum]) >> cntTxBitNum) & 0x01;
//						cntTxBitNum++;
//						if(cntTxBitNum == 10){
//							cntTxBitNum = 0;
//							cntTxByteNum++;
//							if(cntTxByteNum == gTxBytes) {
//								cntTxByteNum = 0;
//								txStage = 3;
//							}
//						}
//						break;
//					case 3:																// Satge 3 -> Send silence.
//						txBit = 0;
//						flagTransmitting = 0;
//						if(flag6BitSilence){											// End of transmission.
//							gTxDataON = 0;
//							txStage = 0;  
//						}
//						if(gFlagAnswerToDetection) {									// If message was the answer to detection request.
//							gFlagAnswerToDetection = 0;
//						}
//						break;
//				}
//			} else {
//				txStage = 0;
//			}
//			lastTxBit = txBit;
//		}
//	}
//	output_bit(PWR_BUS_TX,txBit);
//}
//
//
///* 
// * @brief	Manger for recevied PowerBus Message
// * @param	none.
// * @retval	none.
// * @details	none.
// */
//void PwrBusRxMsgManager(void){
//	// Macro to check constants.
//	
//	// Local variables.
//	int8 crc;
//	
//	// Algorithm
//	crc = ComputeSerialCrc(gRxBuffer,gCntRxByteNum - 1);
//    
//	if((gRxLinkSequence == PWRB_START_SEQUENCE) && (crc == gRxBuffer[gCntRxByteNum - 1])) {
//		if(gRxBuffer[0] == PWRB_BROADCAST) {
//			switch(gRxBuffer[1]) {			
//				case PWRB_INST_DETECT_INIT:
//					gDeviceADR = MY_ADDRESS_RESET_VALUE;								// Reset Device Address to default value.
//					gTemporaryDeviceADR = MY_ADDRESS_RESET_VALUE;
//					gDeviceDetectionON = 1;												// Set flag, detecion is active.
//					// Open previously (if known) downwards realy output.
//					if(!gFlagPwrBusRelay_A_B) {
//						output_high(COMM_CONN_B_DIS);
//						delay_ms(20);
//						output_low(COMM_CONN_A_DIS);
//					} else {
//						output_high(COMM_CONN_A_DIS);
//						delay_ms(20);
//						output_low(COMM_CONN_B_DIS);
//					}
//					gFlagContinueFlipFlop = 1;											// Start flip-flop.
//					gTimeoutFlipFlop = 0;												// Reset flip flop timer count.
//					gTimeoutAdrAssignation = 0;											// Reset Autodetection time-out counter.
//					break;
//				case PWRB_INST_DETECT_CANCEL:
//					gDeviceADR = MY_ADDRESS_RESET_VALUE;								// Reset Device Address to default value.
//					gTemporaryDeviceADR = MY_ADDRESS_RESET_VALUE;
//					output_low(COMM_CONN_A_DIS);											// Close relays.
//					output_low(COMM_CONN_B_DIS);
//					gDeviceDetectionON = 0;												// Device identification is finished.
//					gFlagContinueFlipFlop = 0;											// Stop flip-flop.
//					gTimeoutFlipFlop = 0;												// Reset flip flop timer count.
//					gTimeoutAdrAssignation = 0;											// Reset Autodetection time-out counter.
//					gFlagAnswerToDetection = 0;
//					gTxDataON = 0;
//					break;
//				case PWRB_INST_ADR_ASIGNATION:
//					if((gDeviceADR == MY_ADDRESS_RESET_VALUE) && gDeviceDetectionON) {	//If we do not have an assigned address or the address is our own
//						gTimeoutFlipFlop = 0;
//						gTimeoutAdrAssignation = 0; 
//						gAdrAssignationError = 0;
//						// Pseudo-aleatory time control.				
//						gFlagAleatoryTxDelay = 1;										// Trigger aletory time delay to answer.
//						// Load message in Tx Buffer
//						gTemporaryDeviceADR = gRxBuffer[2];
//						gTxBuffer[0] = PWRB_SWITCH_ADR;
//						gTxBuffer[1] = gTemporaryDeviceADR;
//						if(gFlagPwrBusRelay_A_B){
//							gTxBuffer[2] = PWRB_INST_ADR_ASIG_ANW_B;
//						} else {
//							gTxBuffer[2] = PWRB_INST_ADR_ASIG_ANW_A;
//						}
//						gTxBuffer[3] = (SERIAL_NUMBER >> 16) & 0xFF;
//						gTxBuffer[4] = (SERIAL_NUMBER >> 8) & 0xFF;
//						gTxBuffer[5] = SERIAL_NUMBER & 0xFF;
//						gTxBuffer[6] = ComputeSerialCrc(gTxBuffer, 6);		
//						gTxBytes = 7;
//						gTxDataON = 1;
//						gFlagAnswerToDetection = 1;
//						gFlagContinueFlipFlop = 0;										// Stop flip-flop.
//					}
//					break;
//				case PWRB_INST_LED_ON:
//					gFlagLightON = 1;
//					break;
//				case PWRB_INST_LED_OFF:
//					gFlagLightON = 0;
//					break;
//				case PWRB_INST_BLINK_ON:
//					gFlagLightBlinkON = 1;
//					break;
//				case PWRB_INST_BLINK_OFF:
//					gFlagLightBlinkON = 0;
//					break;
//				case PWRB_INST_SMALL_BLINK_ON:
//					gFlagLightSmallBlinkON = 1;
//					break;
//				case PWRB_INST_TEST_ON:
//					if(!gFlagBatteryTest){
//						if(gFlagContinousCharge){
//							gFlagLaunchBatteryTest = 1;
//							gFlagBatteryTest = 0;
//						}
//						else{
//							gFlagBatteryTest = 1;
//						}
//					}
//					break;
//				case PWRB_INST_TEST_OFF:
//					gFlagBatteryTest = 0;
//					gFlagLaunchBatteryTest = 0;
//					break;
//				case PWRB_INST_CYCLING_ON:
//					if(gFlagBatteryCycling == 0){
//						gFlagBatteryCycling=1;
//						for(int8 j=0; j<5; j++){
//							gCyclingAutonomy[j] = 0;
//						}
//					}
//					break;
//				case PWRB_INST_CYCLING_OFF:
//					gFlagBatteryCycling=0;
//					gCyclingStage = 0;
//					gCyclingDischCnt = 0;
//					break;           
//			}   
//		} else if((gRxBuffer[0] == gDeviceADR) && (gDeviceADR > 0)) {
//			switch(gRxBuffer[1]){
//				case PWRB_INST_CLOSE_RELAY:												// Close i/o relays
//					output_low(COMM_CONN_A_DIS);
//					output_low(COMM_CONN_B_DIS);
//					gDeviceDetectionON = 0;												// Device identification is finished.
//					break;
//				case PWRB_INST_LED_ON:
//					gFlagLightON = 1;
//					break;
//				case PWRB_INST_LED_OFF:
//					gFlagLightON = 0;
//					break;
//				case PWRB_INST_BLINK_ON:
//					gFlagLightBlinkON = 1;
//					break;
//				case PWRB_INST_BLINK_OFF:
//					gFlagLightBlinkON = 0;
//					break;
//				case PWRB_INST_SMALL_BLINK_ON:
//					gFlagLightSmallBlinkON = 1;
//					break;
//				case PWRB_INST_TEST_ON:
//					if(!gFlagBatteryTest){
//						if(gFlagContinousCharge){
//							gFlagLaunchBatteryTest = 1;
//							gFlagBatteryTest = 0;
//						}
//						else{
//							gFlagBatteryTest = 1;
//						}
//					}
//					break;
//				case PWRB_INST_TEST_OFF:
//					gFlagBatteryTest = 0;
//					gFlagLaunchBatteryTest = 0;
//					break;
//				case PWRB_INST_CYCLING_ON:
//					gFlagBatteryCycling = 1;
//					break;
//				case PWRB_INST_CYCLING_OFF:
//					gFlagBatteryCycling = 0;
//					gCyclingStage = 0;
//					gCyclingDischCnt = 0;
//					break;    
//				case PWRB_INST_READ_P_SOH:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_SOH;
//					gTxBuffer[3] = LuminaryInfo.SOH;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_SOC:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_SOC;
//					gTxBuffer[3] = LuminaryInfo.SOC;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_TEST_BAT:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TEST_BAT;
//					gTxBuffer[3] = LuminaryInfo.BatTest;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;		
//				case PWRB_INST_READ_P_TIME_CYCLE_1:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TIME_CYCLE_1;
//					gTxBuffer[3] = gCyclingAutonomy[0];
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TIME_CYCLE_2:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TIME_CYCLE_2;
//					gTxBuffer[3] = gCyclingAutonomy[1];
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TIME_CYCLE_3:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TIME_CYCLE_3;
//					gTxBuffer[3] = gCyclingAutonomy[2];
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TIME_CYCLE_4:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TIME_CYCLE_4;
//					gTxBuffer[3] = gCyclingAutonomy[3];
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TIME_CYCLE_5:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TIME_CYCLE_5;
//					gTxBuffer[3] = gCyclingAutonomy[4];
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TEMP_CYCLE_1:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TEMP_CYCLE_1;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[0]);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TEMP_CYCLE_2:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TEMP_CYCLE_2;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[1]);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TEMP_CYCLE_3:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TEMP_CYCLE_3;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[2]);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TEMP_CYCLE_4:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TEMP_CYCLE_4;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[3]);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TEMP_CYCLE_5:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TEMP_CYCLE_5;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[4]);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_INST_READ_P_TEMP_TEST:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_TEMP_TEST;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(LuminaryInfo.TempTest);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_STATUS_1:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_STATUS_1;
//					gTxBuffer[3] = (((gFlagBatteryTest|gFlagLaunchBatteryTest) & 0x01) << BIT_BATT_TEST) | 
//									((gFlagChargerError & 0x01) << BIT_CHARG_ERR) | 
//									((gFlagErrorBattNotConnected & 0x01) << BIT_BATT_ERR) | 
//									((gFlagPowerSupplyError & 0x01) << BIT_FA_ERR) |
//									((gFlagHeaterOpenCircuitError & 0x01) << BIT_HTRD_ERR) | 
//									((gFlagHeaterShortCircuitError & 0x01) << BIT_HTRB_ERR) | 
//									((0 & 0x01) << BIT_LEDS_ERR) |						// @ToDo #06 - Add LEDS_ERR functionality 									
//									((gLightSwitchLineStatus & 0x01) << BIT_LEDS_ON);	// @ToDo #09 - LEDs on is not only dependent on gLightSwitchLineStatus
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_STATUS_2:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_STATUS_2;
//					if(gFlagBatteryCycling){
//						// If in cycling mode, send also number of disch cycle.
//						gTxBuffer[3] = ((gFlagReplaceBattery & 0x01) << BIT_BATT_CHANGE) |
//									   ((gFlagBatteryCycling & 0x01) << BIT_BATT_CYCLING) | 
//									   (((gCyclingDischCnt + 1) & 0x07) << BIT_BATT_CYCLING_NUM) |
//									   (((gCyclingStage ^ 0x01) & 0x01) << BIT_BATT_CYCLING_DISCH);
//					} else {
//						// If not in cycling mode, do not send disch cycle.
//						gTxBuffer[3] = ((gFlagReplaceBattery&0x01)<<BIT_BATT_CHANGE) | 
//									   ((gFlagBatteryCycling&0x01)<<BIT_BATT_CYCLING);
//					}
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT - 1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;  
//				case PWRB_INST_READ_P_BAT_TEMP:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_BAT_TEMP;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(gBattTempFiltered);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_INT_TEMP:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_INT_TEMP;
//					gTxBuffer[3] = TempDeciKelvinToGradOffset40(gAmbTempFiltered);
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;       
//				case PWRB_INST_READ_P_SERIAL_H:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_SERIAL_H;
//					gTxBuffer[3] = (SERIAL_NUMBER>>8) & 0xFF;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_SERIAL_L:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_SERIAL_L;
//					gTxBuffer[3] = SERIAL_NUMBER & 0xFF;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_MODEL_12:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_MODEL_12;
//					gTxBuffer[3] = ((LUMINARY_MODEL[0] - '0')<<4) + (LUMINARY_MODEL[1] - '0');
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_MODEL_34:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_MODEL_34;
//					gTxBuffer[3] = ((LUMINARY_MODEL[2] - '0')<<4) + (LUMINARY_MODEL[3] - '0');
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_MODEL_56:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_MODEL_56;
//					gTxBuffer[3] = ((LUMINARY_MODEL[4] - '0')<<4) + (LUMINARY_MODEL[5] - '0');
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_MODEL_78:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_MODEL_78;
//					gTxBuffer[3] = ((LUMINARY_MODEL[6] - '0')<<4) + (LUMINARY_MODEL[7] - '0');
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_MODEL_9A:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_MODEL_9A;
//					gTxBuffer[3] = ((LUMINARY_MODEL[8] - '0')<<4) + (LUMINARY_MODEL[9] - '0');
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_P_ELEMENT_TYPE:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_ELEMENT_TYPE;
//					gTxBuffer[3] = LUMINAIRE_TYPE_NEX;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_VALUE_P_SERV_LUM_POWER:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_SERV_LUM_POWER;
//					gTxBuffer[3] = LUMINAIRE_SERV_LUM;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_VALUE_P_EMERG_LUM_POWER:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_SERV_LUM_POWER;
//					gTxBuffer[3] = LUMINAIRE_EMERG_LUM;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break;
//				case PWRB_VALUE_ELEMENT_EMERG_AUTNONOMY:
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_P_SERV_LUM_POWER;
//					gTxBuffer[3] = LUMINAIRE_EMERG_TIME;
//					gTxBuffer[4] = ComputeSerialCrc(gTxBuffer, PWRB_VALUE_LENGHT-1);
//					gTxBytes = PWRB_VALUE_LENGHT;
//					gTxDataON = 1;
//					break; 
//				case PWRB_INST_READ_INFO:                                                           // Read information instruction
//					gTxBuffer[0] = PWRB_SWITCH_ADR;
//					gTxBuffer[1] = gDeviceADR;
//					gTxBuffer[2] = PWRB_VALUE_INFO_P;
//					gTxBuffer[3] = (SERIAL_NUMBER>>8) & 0xFF;										// Serial number high part
//					gTxBuffer[4] = SERIAL_NUMBER & 0xFF;											// Serial number low part
//					gTxBuffer[5] = ((LUMINARY_MODEL[0] - '0')<<4) + (LUMINARY_MODEL[1] - '0');		// Model number digits 1 and 2
//					gTxBuffer[6] = ((LUMINARY_MODEL[2] - '0')<<4) + (LUMINARY_MODEL[3] - '0');		// Model number digits 3 and 4
//					gTxBuffer[7] = ((LUMINARY_MODEL[4] - '0')<<4) + (LUMINARY_MODEL[5] - '0');		// Model number digits 5 and 6
//					gTxBuffer[8] = ((LUMINARY_MODEL[6] - '0')<<4) + (LUMINARY_MODEL[7] - '0');		// Model number digits 7 and 8
//					gTxBuffer[9] = ((LUMINARY_MODEL[8] - '0')<<4) + (LUMINARY_MODEL[9] - '0');		// Model number digits 9 and 10
//					gTxBuffer[10] = LUMINAIRE_TYPE_NEX;												// Luminaire type
//					gTxBuffer[11] = LUMINAIRE_SERV_LUM;												// Luminarie service luminosity
//					gTxBuffer[12] = LUMINAIRE_EMERG_LUM;											// Luminaire emergency luminosity
//					gTxBuffer[13] = LUMINAIRE_EMERG_TIME;											// Luminarie emergency time
//					gTxBuffer[14] = ComputeSerialCrc(gTxBuffer, 14);
//					gTxBytes = 15;
//					gTxDataON = 1;
//					break;    
//				case PWRB_INST_READ_DATA:												// Read data instruction
//					gTxBuffer[0] = PWRB_SWITCH_ADR;																// LX Switch ID
//					gTxBuffer[1] = gDeviceADR;																	// LX Luminaire ID
//					gTxBuffer[2] = PWRB_VALUE_DATA_P;															// PwrBus instruction
//					gTxBuffer[3] = LuminaryInfo.SOH;															// State of health
//					gTxBuffer[4] = LuminaryInfo.SOC;															// State of charge
//					gTxBuffer[5] = LuminaryInfo.BatTest;														// Battery test time
//					gTxBuffer[6] = TempDeciKelvinToGradOffset40(LuminaryInfo.TempTest);							// Battery temperature during last battery test
//                    gTxBuffer[7] = (((gFlagBatteryTest|gFlagLaunchBatteryTest) & 0x01) << BIT_BATT_TEST) |		// Status 1
//									((gFlagChargerError&0x01) << BIT_CHARG_ERR) | 
//									((gFlagErrorBattNotConnected & 0x01) << BIT_BATT_ERR) | 
//									((gFlagPowerSupplyError & 0x01) << BIT_FA_ERR) |
//									((gFlagHeaterOpenCircuitError & 0x01) << BIT_HTRD_ERR) | 
//									((gFlagHeaterShortCircuitError & 0x01) << BIT_HTRB_ERR) | 
//									((0 & 0x01) << BIT_LEDS_ERR) |						// @ToDo #06 - Add LEDS_ERR functionality 									
//									((gLightSwitchLineStatus & 0x01) << BIT_LEDS_ON);	// @ToDo #09 - LEDs on is not only dependent on gLightSwitchLineStatus
//					if(gFlagBatteryCycling){
//						// If in cycling mode, send also number of disch cycle.
//						gTxBuffer[8] = ((gFlagReplaceBattery & 0x01) << BIT_BATT_CHANGE) |						// Status 2
//										((gFlagBatteryCycling & 0x01) << BIT_BATT_CYCLING) | 
//										(((gCyclingDischCnt + 1) & 0x07) << BIT_BATT_CYCLING_NUM);
//					} else {
//						// If not in cycling mode, do not send disch cycle.
//						gTxBuffer[8] = ((gFlagReplaceBattery & 0x01) << BIT_BATT_CHANGE) |						// Status 2
//										((gFlagBatteryCycling & 0x01) << BIT_BATT_CYCLING);
//					}					
//					gTxBuffer[9] = TempDeciKelvinToGradOffset40(gBattTempFiltered);
//					gTxBuffer[10] = TempDeciKelvinToGradOffset40(gAmbTempFiltered);					
//					gTxBuffer[11] = gCyclingAutonomy[0];
//					gTxBuffer[12] = gCyclingAutonomy[1];
//					gTxBuffer[13] = gCyclingAutonomy[2];
//					gTxBuffer[14] = gCyclingAutonomy[3];
//					gTxBuffer[15] = gCyclingAutonomy[4];
//					gTxBuffer[16] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[0]);
//					gTxBuffer[17] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[1]);
//					gTxBuffer[18] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[2]);
//					gTxBuffer[19] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[3]);
//					gTxBuffer[20] = TempDeciKelvinToGradOffset40(gTemperaturasCiclado[4]);
//					gTxBuffer[21] = ComputeSerialCrc(gTxBuffer, 21);
//					gTxBytes = 22;
//					gTxDataON = 1;
//					break; 
//			}
//		} else if((gRxBuffer[0] == gTemporaryDeviceADR) && (gTemporaryDeviceADR > 0)) {
//			if((gRxBuffer[1] == PWRB_INST_ADDR_OK) && (gRxBuffer[2] == ((SERIAL_NUMBER >> 16) & 0xFF)) && (gRxBuffer[3] == ((SERIAL_NUMBER >> 8) & 0xFF)) && (gRxBuffer[4] == (SERIAL_NUMBER & 0xFF))){
//				gDeviceADR = gTemporaryDeviceADR;
//				gTemporaryDeviceADR = MY_ADDRESS_RESET_VALUE;
//				gFlagLuminaireIdentification = 1;												// Turn ON the LEDs to warn about correct identification.
//				SaveDeviceID(gDeviceADR);
//				gTxBuffer[0] = PWRB_SWITCH_ADR;
//				gTxBuffer[1] = gDeviceADR;
//				gTxBuffer[2] = PWRB_INST_ADR_ACK;
//				gTxBuffer[3] = ((SERIAL_NUMBER >> 16) & 0xFF);
//				gTxBuffer[4] = ((SERIAL_NUMBER >> 8) & 0xFF);
//				gTxBuffer[5] = (SERIAL_NUMBER & 0xFF);
//				gTxBuffer[6] = ComputeSerialCrc(gTxBuffer, 6);
//				gTxBytes = 7;
//				gTxDataON = 1;
//			}
//		}
//	}
//}
//
//
///** 
//  * @brief		Transforma la temperatura de deciKelvin a Grados centígrados -40ºC
//  * @param		none.
//  * @retval		none.
//  * @details	none.
//  */
//int8 TempDeciKelvinToGradOffset40(int16 tempDeciKelvin){
//    int8 tempGrad;
//    
//    tempGrad = (tempDeciKelvin - 2331) / 10;
//    
//    return tempGrad;
//}
//
///*
// * @brief	8b10b Byte decoding.
// * @param	(byval) int16 byteToDecode: 10 bit value to be decoded to 8 bit.
// * @retval	(byval) int16 decodedValue: 8 bit value decoded from 10 bit value.
// * @details	- Converts 10b to 8b with reverse conversion tables stored in ROM.
// *			- Impossible values are translated to 0xFFFF (error). 
// */
//int16 ByteDecode8b10b (int16 byteToDecode){
//	// Local variables.
//	int16 decodedByte = 0xFFFF;													// Stores 16 bit decoded value. Init in 0xFFFF (error).
//	int8 fourMsb = 0;															// Stores 4 MSb of 10 bit coded valued.
//	int8 threeMsb = 0;															// Stores 3 MSb of 8 bit decoded valued. 
//	int8 sixLsb = 0;															// Stores 6 LSb of 10 bit coded valued.
//	int8 fiveLsb = 0;															// Stores 5 LSb of 8 bit coded valued.
//	
//	// Decoding algorithm.
//	fourMsb = byteToDecode >> 6;												// Extract 4 MSb of byte to decode.
//	sixLsb = byteToDecode & 0x3F;												// Extract 6 LSb of byte to decode.
//	
//	if((fourMsb > 10) || (fourMsb < 1)){										// If out of FOUR_BIT_TO_THREE_BIT table range.
//		decodedByte = 0xFFFF;
//		return (decodedByte);													// Return error message.
//	} else {																	// If no error in 4 MSb, continue with 6 LSb.
//		if((sixLsb  > 58) || (sixLsb < 11)){									// If out of SIX_BIT_TO_FIVE_BIT table range.		
//			decodedByte = 0xFFFF;
//			return (decodedByte);												// Return error message.
//		} else {
//			threeMsb = FOUR_BIT_TO_THREE_BIT[fourMsb];							// Extract values from the tables.
//			fiveLsb = SIX_BIT_TO_FIVE_BIT[sixLsb - 11];							// Remove offset, (11) is the minimum inverse value.
//		}								
//	}
//	if((threeMsb == 0xFF) || (fiveLsb == 0xFF)){								// If any of the two blocks has no equivalent.
//		decodedByte = 0xFFFF;													// Error message.
//	} else {
//		decodedByte = 0;														// Reset error coding.
//		decodedByte = (threeMsb << 5) + fiveLsb;								// Combine values to form 8 bit decoded value.
//	}
//	return (decodedByte);														// Return decoded value.
//}
//
//
///*
// * @brief	8b10b Byte codification.
// * @param	(byval) int8 byteToCode: 8 bit value to be coded to 10 bit.
// * @retval	(byval) int16 codedByte: 10 bit value decoded.
// * @details	- Converts 8b to 10b with conversion tables stored in ROM.
// */
//int16 ByteCode8b10b (int8 byteToCode){
//	// Local variables
//	int16 auxCodedValue;														// Stores 10 bit coded value.
//	int8 fourMsb = 0;															// Stores 4 MSb of the 10b coded value.
//	int8 sixLsb = 0;															// Stores 6 LSb of the 10b coded value.
//	
//	// Codification algorithm.
//	fourMsb = THREE_BIT_TO_FOUR_BIT[byteToCode >> 5];							// Code 3 MSb to 4b MSb
//	sixLsb = FIVE_BIT_TO_SIX_BIT[byteToCode & 0x1F];							// Code 5 LSb to 6b LSb
//	auxCodedValue = ((int16)fourMsb << 6) + sixLsb;								// Construc 10b coded value.
//	return (auxCodedValue);	
//}
//
//
///*
// * @brief	Calculates serial communication message CRC.
// * @param	(byval) int8* buffer: Pointer to message buffer array.
// *			(byval) int8  length: Number of message bytes.
// * @retval	(byval) int8     crc: Calculated CRC.
// * @details - Max. permissible length of message is 256 bytes.
// *			- Polynomial for CRC calculation is 0x25. Same as eSLBus crc in order
// *			  to share same look up table.
// *			- CRC algorithm is boosted with lookup table.
// *			- Reference doc: Understanding CRC.pdf
// */
//int8 ComputeSerialCrc(int8* buffer, int8 length) {
//	// Local variables
//	int8 crc = 0x00;
//	int8 aux = 0;
//	
//	// Algorithm
//	for (int8 pos = 0; pos < length; pos++) {
//		aux = buffer[pos] ^ crc;												
//		crc = CRC_LUT_0X25[aux];
//	}
//	return (crc);
//}
//
//
///*
// * @brief	Calculates eSLBus message CRC.
// * @param	(byval) int8* buffer: Pointer to message buffer array.
// *			(byval) int8  length: Number of message bytes.
// * @retval	(byval) int8     crc: Calculated CRC.
// * @details - Max. permissible length of message is 256 bytes.
// *			- Polynomial for CRC calculation is 0x25. Same as serial crc in order
// *			  to share same look up table.
// *			- CRC algorithm is boosted with lookup table.
// *			- Reference doc: Understanding CRC.pdf
// */
//int8 ComputeEslBusCrc(int8* buffer, int8 length) {
//	// Local variables
//	int8 crc = 0x00;
//	int8 aux = 0;
//
//	// Algorithm
//	for (int8 pos = 1; pos < length + 1; pos++) {								// Element 0 of eSLBus rx message is info and not for CRC calculation.Thus an offset of 1.
//		aux = buffer[pos] ^ crc;												
//		crc = CRC_LUT_0X25[aux];
//	}
//	return (crc);
//}
//
///*----------------------------------------------------------------------------------- (C) COPYRIGHT IEDGREENPOWER ------------------------------------------------------------------------------------*/
