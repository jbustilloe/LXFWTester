/* 
 * File:   main.h
 * Author: ybustillo
 *
 * Created on March 26, 2024, 11:57 AM
 */

#ifndef MAIN_H
#define	MAIN_H

unsigned int i = 0;

#if defined __COMPILA_HEATER
    int16 VHTR = -1;                                //Tension del Calentador
    int1 gHTRPWR_OK = 0;
    int8 HTR_Pwr = 0;                            //Power Of the Heater
    int1 gHTR_OFF = 0;                              //Heater is OFF
    int1 gHTR_ON = 0;                               //Heater is ON 
    int16 Read_V_HTR();                             //Reading, filtering and conversion of V_HTR 
        /*Heater Testing*/
    void HTRTesting();                              //Test del control del Calentador
        /*Error in HTR power*/
   // void HTRPowErr();                               //Test de error del bus de potencia del Calentador
    int8 HTR_Power();                              //Calculo de la potencia del Calentador
#endif

    
#if defined __COMPILA_COMUNICACION
    /*Communications Test*/
    void CommTest1();                               //Test del PowerBus de comunicacion 1
    void CommTest2();                               //Test del PowerBus de comunicacion 2
#endif 
   
    
#if defined __COMPILA_SENSOR_UWAVE
    /*Motion And Light Sensor*/
    void MotionLightSensor();                       //Test de los sensores de Luz y uWave
#endif    

    
#if defined __COMPILA_TEMP_DERRATING
    int1 gTestTempDerrRealizado = 0;
    int16 Temp[2][2];                               //Matriz para almacenar las medidas de corriente a diferentes temperaturas.
#endif
    
#define Baud1 9600                                  //Baudios de Transmision
/*Test Selection*/
char gTestNumber;                                    //numero de Test elegido (segun protocolo)
int1 gTestElegido = 0;                              //flag indicadora de que el test se ha elegido 
int1 gTestFinalizado = 0;                           //flag indication test finished

/*ESTADO DE LA LUMINARIA*/

int1 gBAT_DISCHARGED = 0;                           //Bateria Descargada
int1 gBAT_FULLCHARGE = 0;                           //Bateria FullCharge
int1 gLEDs_FULLPWR = 0;                             //LEDs encendidos a maxima potencia
int1 gLEDs_20PerPWR = 0;                            //LEDs encendidos al 20%
int1 gLEDs_OFF = 0;                                 //LEDs apagados
int1 gDischargePowerOK = 0;                         //Potencia de Descarga Correcta
int1 gCharge_OK = 0;                                //Carga correcta
int1 gVbatOK = 0;
int1 GreenLED = 0;                                  //LED Verde Encendido
int1 RedLED = 0;                                    //LED Rojo Encendido
int16 ILD = -1;                                     //Intensidad en los LEDs
int16 I_carg = -1;                                  //Intensidad de carga de las Baterias
int16 I_desc = -1;                                  //Intendsidad de descarga de las Baterias
int16 VLD = -1;                                     //Tension De los LEDs
int16 VBat = -1;                                    //Tesnsion De las Baterias
int16 gVPS = -1;                                     //Tension de la Fuente de Tension
int16 gVDCBus = -1;                                  //Tension del Bus de Corriente Continua
double Bat_Pwr = -1;                                //Discharge Power Of the Battery
                    
int1 gTestDeepDischargeRealizado = 0;               //flag indicadora de que se ha llevado a cabo el test de deep discharge
int16 GrafCargaBat[120] = {0};                            //Vector almacenamiento datos de tension en la carga
int16 GrafDescargaBat[120] = {0};                         //Vector almacenamiento datos de tension en la descarga

//Test Flags
int1 gRampUp_OK = 0;                                //1 if Rampup Detected
int1 gRampDown_OK = 0;                              //1 If RampDown Detected
int1 gRedFlick_OK = 0;                              //1 if Red LED is flicking  
int8 gGreenFlick_OK = 0;                            //1 if green led s flicking 

int1 gIntermitentChargeOK;                          //Flag indicating intermittent charge is going OK
int8 TimepoDescarga1 = 0;  //min                    //Time the battery takes to run out (Exp = 90min)
int8 TiempoDescarga2 = 0;  //min                    //Time the Battery takes to run out (Exp = 50min)
 
int16 V_BAT_24h[110]={0};                           //Almacenamiento de la tension de bateria en carga de bateria durante 24H
int1 gTestContChargeRealizado = 0;                  //flag indicador de que se ha realizado el test de carga continua, carga alterna
/* ERROR CODES VECTOR*/
unsigned int8 ErrorCodes[30] = {0};                 //Vector to hold all the error codes (must be transmitted to PC)
int8 ErrorCont = 0;                                 //Counter for the number of errors recieved
/* Test Flags*/
int8 gTest_OK = 0;                                  //1 if test just finished was succesfull
int8 gTest_ERROR = 0;                               //1 if test just finished unsuccesfull

#if defined __COMPILA_SENSOR_UWAVE_LUZ
    void Activar_uWave(int1 presencia);                  //0 si no hay presencia, 1 si hay presencia, 2 en reposo
#endif

    
/*-------------------------------------------GENERIC USE FUNCTIONS----------------------------------------*/
void Turn_OFF();
void InitializeMicro(void);                         //Inicializacion del Microcontrolador
void Reset_Tester();                                //resetear tester a estado de reposo
void UART_SEND();                                   //Envio de datos en UART
void Get_Estado_LUM(int1 Carga_Desc);               //Obtener estado de tensiones y corrientes
void TestChoose();                                  //Selection of the test to perform 
int16 Read_V_PS();                                  //Power Sourve Voltage reading
int16 Read_V_DCBus();                               //Reading, filtering and conversion of V_DCBus
int16 Read_I_LED_DERR();                            //Reading, filtering and conversion of LED current while derrating mode
int16 Read_I_LED();                                 //Reading, filtering and conversion of I_LEDS 
int16 Read_I_BatDesc();                             //Reading, filtering and conversion of I_BatDesc  
int16 Read_I_BatCarg();                             //Reading, filtering and conversion of I_BatCarg
int16 Read_V_Bat();                                 //Reading, filtering and conversion of V_Bat 
int16 Read_V_LD();                                  //Reading, filtering and conversion of V_LD 
void Set_T_amb(int16 Temperatura);                  //Estableces una temperatura ambiente
void Set_T_HTR(float Temperatura);                  //Establecer temperatura de las baterias

void LEDFlickCheck(int1 LEDChoose);                 // 0 for Red, 1 for Green
float BatteryDischargePower();                      // Calculo de la Potencia de descarga de las baterias
void delay_min(int8 minutos);                       // Delay de minutos
void IntermitentCharge();                           // Comprobacion de la carga intermitente de las baterias
void Descarga_Bat();                                // Funcion para descargar las baterias


/*----------------------------------------------Test Functions----------------------------------------------*/
void TensionPSyDCBus();
/*Power Up Tests*/
void PwrUpTest1();                                  //Powe Up test 1                                    
void PwrUpTest2();                                  //Powe Up test 2  
void PwrUpTest3();                                  //Powe Up test 3  
void PwrUpTest4();                                  //Powe Up test 4  
void PwrUpTest5();                                  //Powe Up test 5  
   /*Test Push Button*/
void TstPshBtn1();                                  //Test Push Button 1
void TstPshBtn2();                                  //Test Push Button 2
void TstPshBtn3();                                  //Test Push Button 3
void TstPshBtn4();                                  //Test Push Button 4
void TstPshBtn5();                                  //Test Push Button 5
   /*Battery disconection with test push button*/
void BtryDiscTstPshBtn1();                          //Battery Disconnection using test Push Button 1 
void BtryDiscTstPshBtn2();                          //Battery Disconnection using test Push Button 2
void BtryDiscTstPshBtn3();                          //Battery Disconnection using test Push Button 3
void BtryDiscTstPshBtn4();                          //Battery Disconnection using test Push Button 4
void BtryDiscTstPshBtn5();                          //Battery Disconnection using test Push Button 5
void BtryDiscTstPshBtn6();                          //Battery Disconnection using test Push Button 6
   /*Continous charge -> intemitent charge -> Autonomy*/
void ContChargeInterChargeAutonomy();               
   /*Deep Discharge Battery: Continous charge -> intemitent charge -> Autonomy*/
void DeepChargeBattery();
   /*Autonomy auto-test*/
void AutonomyAutoTest();
   /*Red LED testing using test button*/
void RedLEDResetWithPshBtn();
/*Battery Disconection using power switch*/
void BattDiscPwrSwitch1();
void BattDiscPwrSwitch2();
void BattDiscPwrSwitch3();
   /*Temperature Derating*/
void TemperatureDerating();

#endif	/* MAIN_H */

