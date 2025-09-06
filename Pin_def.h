/********************************************************************************************************************************************************************
 *   
 *   @File         Pin_def.h
 *   @Project      FW_LXFWTester
 *   @Brief         Archivo que contiene la definición de pines   i/o
 *   @Version      1.0.0
 *   @Author         Yago Bustillo. IED Greenpower S.L.
 *                                                                                                   (C) COPYRIGHT IED Greenpower S.L.
 *******************************************************************************************************************************************************************/


#ifndef __PIN_DEF_H
#define __PIN_DEF_H

/*UART Pins*/
#define UARTRX     PIN_A5                   
#define UARTTX     PIN_C0

/* Port define --------------------------------------------------------------------------------------------------------------------------------------------------- */
// Definición de pines digitales e/s
#define BAT_DISC                PIN_C7      // Battery disconnection                            Pin #1 del micro.
#define SWITCH_CONTROL          PIN_D5      // Control of the switch                            Pin #3 del micro.
#define POWER_DISCONNECTION     PIN_D6      // Desconexion de la tension de LX                  Pin #4 del micro.                         
#define TEST_BUTTON             PIN_E0      // Push button activation                           Pin #25 del micro.
#define GREEN_LED               PIN_E1      // Green LED                                        Pin #26 del micro.
#define RED_LED                 PIN_E2      // Red LED                                          Pin #27 del micro.
#define PWR_BUS_RX              PIN_A1      // Power Bus reception                              Pin #20 del micro.         
#define PWR_BUS_Tx              PIN_A4      // Power Bus transmission                           Pin #21 del micro.

#if defined __COMPILA_HEATER
    #define Q12_ERROR_ENABLE        PIN_B5      // Q12 transistor error simulation                  Pin #15 del micro.
#endif

#if defined __COMPILA_COMUNICACION
    #define CONSTANT_POWER          PIN_A0      // Power reading for comparison                     Pin #19 del micro.
    #define COM_TRANSMIT_SWITCH     PIN_D4      // Transmision channel change                       Pin #2 del micro.
    #define COM_RECIEVE_SWITCH      PIN_D7      // Reception channel change                         Pin #5 del micro.
#endif


// Definición de canales analógico      
   //ADC
   #define CANAL_V_PS                  8          // canal de lectura del tension de la PS                       Pin #8 del micro.
   #define CANAL_V_DC_BUS              9          // canal de lectura de la tension del DLBus                    Pin #9 del micro.
   #define CANAL_V_ILED                19         // canal de lectura de la V para calc la I de los LEDs         Pin #37 del micro.
   #define CANAL_V_BAT                 21         // canal de lectura de la tension de la bateria                Pin #43 del micro.
   #define CANAL_V_LEDS                24         // canal de lectura de la tension de los LEDs                  Pin #38 del micro.
   #define CANAL_V_DESCARGA_BAT        25         // canal de lectura de la V para calc la I de desc de Bat      Pin #39 del micro.
   #define CANAL_V_CARGA_BAT           26         // canal de lectura de la V para calc la I de carg de Bat      Pin #40 del micro.
  
#if defined __COMPILA_SENSOR_UWAVE_LUZ
    #define LIGHT_SENSOR            PIN_A6      // Light sensor simulator                           Pin #31 del micro.
#endif

#if defined __COMPILA_HEATER
    #define CANAL_V_HEATER              27    
    //#define T_HEATER              PIN_C2          // Canal de escritura de la temp del HTR                       Pin #36 del micro.
#endif

#if defined __COMPILA_EXTERNAL_DAC
#define DAC_CS              PIN_C2              

#endif

#endif // __PIN_DEF_H
/* ------------------------------------------------------------------ (C) COPYRIGHT IED Greenpower S.L. ---------------------------------------------------------- */