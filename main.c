/* 
 * File:   main.c
 * Author: ybustillo
 *
 * Created on March 26, 2024, 11:57 AM
 */


#include <Project_Config.h>
#include <18F45Q43.h>
#include <Pin_def.h>
#device ADC = 10
 #use delay(clock=4000000)
#include <main.h>
#include <Tests.h>


/*Configuracion UART*/
#pin_select U5TX = UARTTX   // Asigna el pin B0 como TX de UART1
#pin_select U5RX = UARTRX  // Asigna el pin B1 como RX de UART1
#use rs232(BAUD=9600, UART5, ERRORS, STREAM=RS232 )
#pin_select SCK2OUT = PIN_B3
#pin_select SDO2 = PIN_B2

#use standard_io(A)                         //Pin Configuration
#use standard_io(B)
#use standard_io(C)
#use standard_io(D)
#use standard_io(E)


//#use spi(MASTER, DI = PIN_B4, DO = PIN_B2 , CLK = PIN_B3 )

#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H)
#define SPI_MODE_1  (SPI_L_TO_H)
#define SPI_MODE_2  (SPI_H_TO_L)
#define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H)


void main(){              //Main Functiom
    
    
    InitializeMicro();
    while(TRUE){
        /*CODIGO REAL*/
        if(kbhit()){
            
            gTestNumber = getc();
            gTestElegido = 1; 
            TestChoose();                       //segun el comando recibido de el ordenador de elige el test
            UART_SEND();    
        }
    }
}


void InitializeMicro(void)                                   
{
    //UART Setings
    setup_uart(Baud1);                                                 //baud number defined as global constant
    //ADC                                              
    setup_dac(DAC_VSS_VDD | DAC_OUTPUT_A2);                           //setup of DAC with output pin A2
    //SPI
    output_high(DAC_CS);
    setup_spi2(SPI_MASTER | SPI_MODE_0 | SPI_CLK_MFINTRC );
    //DAC
    setup_adc(ADC_CLOCK_DIV_32);                                      //setup ADC 
    setup_adc_ports(sAN8 |sAN9 |sAN19 |sAN20 |sAN21 |sAN24 |sAN25 |sAN26 |sAN27);    //Setup ADC port    output_high(TEST_BUTTON);                       //Activate push button to default high    
    //Estado de reposo
    Reset_Tester();
    //Interrupciones
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
#if defined __COMPILA_SENSOR_UWAVE_LUZ
    output_high(LIGHT_SENSOR);
    Activar_uWave(3);
#endif         
}

/** 
  * @brief      Comprobacion del estado de la LUM LX.
  * @param      none.
  * @retval      none.
  * @details     Comprobacion del estado de las baterias, FULL_CHARGE, DISCHARGE or other, LUM ON/OFF, LEDS ON/OFF, EMERGENCY
  */
Void Get_Estado_LUM(int8 Carga_Desc){                       //Carga_Desc 2 si esta descargando, 1 si esta cargando, 0 no importa
        I_carg = Read_I_BatCarg();                          //Intensidad de carga de las Baterias
        I_desc = Read_I_BatDesc();                          //Intendsidad de descarga de las Baterias
        GreenLED = input_state(GREEN_LED);                  //LED Verde Encendido
        RedLED = input_state(RED_LED);                      //LED Rojo Encendido
        VLD = Read_V_LD();                                  //Tension De los LEDs
        VBat = Read_V_Bat();                                //Tesnsion De las Baterias
        LEDFlickCheck(0);                                   //Comprobacion parpadeo LED rojo
        LEDFlickCheck(1);                                   //Comprobacion parpadeo LED verde
        ILD =Read_I_LED();                                  //Lectura Intensidad de los LEDs

#if defined __COMPILA_HEATER
        VHTR = Read_V_HTR(); 
        if(VHTR == 0){                                      //HTR apagado
            gHTR_OFF = 1;
            gHTR_ON = 0;
        }else{                                              //HTR encendido
            gHTR_OFF = 0;
            gHTR_ON = 1;
        }
        HTR_Pwr = HTR_Power();                              //calculo de la potencia consumida por el calentador
        if(HTR_Pwr<7){
            gHTRPWR_OK = 0;
        }else{
            gHTRPWR_OK = 1;
        }
#endif    
        //ESTADO DE CARGA
        if(VBat >= V_MAXIMA_BAT){                                             
            gBAT_FULLCHARGE = 1;
            gBAT_DISCHARGED = 0;
        }else if(VBat <= V_LIM_BAT_DESCARGADA){
            gBAT_FULLCHARGE = 0;
            gBAT_DISCHARGED = 1;
        }else{
            gBAT_FULLCHARGE = 0;
            gBAT_DISCHARGED = 0;
        }
        if(VBat >= V_MAXIMA_BAT*0.9 && VBat<=V_MAXIMA_BAT*1.1){
            gVbatOK = 1;                                    //Tension de la bateria correcta
        }else{gVbatOK =0;}
        if(Carga_Desc == 2){                                //Esta Descargando
           Bat_Pwr = BatteryDischargePower();
           if(Bat_Pwr <= W_DESCARGA_BAT*1.2 && Bat_Pwr >= W_DESCARGA_BAT*0.8){
               gDischargePowerOK = 1;
           }else{ gDischargePowerOK=0;
           }
        }else if(Carga_Desc == 1){                          //Esta Cargando
            if(I_carg < (I_CARGA_BAT*1.1) && I_carg > (I_CARGA_BAT*0.9)){
                gCharge_OK = 1;
            }else{gCharge_OK = 0;}
        }
        if(ILD < 30){
            gLEDs_FULLPWR = 0;
            gLEDs_20PerPWR = 0;
            gLEDs_OFF = 1;

        }
#if defined __COMPILA_20_POR_CIENTO
        else if(ILD <= I_LEDS_EMERGENCIA*1.1 && ILD >= I_LEDS_EMERGENCIA*0.9){
            gLEDs_FULLPWR = 0;
            gLEDs_20PerPWR = 1;
            gLEDs_OFF = 0;
        }else if(ILD <= I_LED_FP*1.1 && ILD >= I_LED_FP*0.9){
            gLEDs_FULLPWR = 1;
            gLEDs_20PerPWR = 0;
            gLEDs_OFF = 0;
        }
#else
        else if((ILD <= I_LED_FP*1.1 && ILD >= I_LED_FP*0.9)){
            gLEDs_FULLPWR = 1;
            gLEDs_20PerPWR = 1;
            gLEDs_OFF = 0;
        }
#endif
        else{
            gLEDs_FULLPWR = 0;
            gLEDs_20PerPWR = 0;
            gLEDs_OFF = 0;
        }
}


/** 
  * @brief      Reset tester to default mode
  * @param      none.
  * @retval      none.
  * @details     
  */
void Reset_Tester(){
    output_high(PWR_BUS_Tx);    
    output_high(TEST_BUTTON);
    output_low(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_high(SWITCH_CONTROL);
    Set_T_amb(TEMP_TRABAJO);
#if defined __COMPILA_HEATER
    Set_T_HTR(TEMP_HTR_TRABAJO);
#endif
}


/** 
  * @brief          Envio de los datos de error.
  * @param          none.
  * @retval         none.
  * @details        Envio de los datos de error y de realizacion de los tests via UART
  */
void UART_SEND(){
    int j, k;
    //ENVIO DE NUMERO DE ERRORES Y TEST COMPLETADOS SATISFACTORIAMENTE
    putc(gVDCBUS);
    putc(gVPS);
    putc(gTest_OK);
    putc(gTest_ERROR);
    for(i=0;i<30;i++){
            putc(ErrorCodes[i]);
            ErrorCodes[i] = 0;
    }
    gTest_OK = 0;
    gTest_ERROR = 0;
    ErrorCont = 0;
    if(gTestTempDerrRealizado){
        putc(Temp[0][0]);
        putc(Temp[0][1]);
        putc(Temp[1][0]);
        putc(Temp[1][1]);
        Temp[0][0] = 0;
        Temp[0][1] = 0;
        Temp[1][0] = 0;
        Temp[1][1] = 0;
    }
    int8 entero1, entero2;
    if(gTestContChargeRealizado){
        for(j=0;j<110;j++){
            entero1 = (V_BAT_24h[j] >> 8) & 0xFF; // Parte alta
            entero2 = V_BAT_24h[j] & 0xFF; // Parte baja 
            putc(entero1);
            putc(entero2);
            entero1 = 0;
            entero2 = 0;
            V_BAT_24h[j] = 0;
        }
    }
    if(gTestDeepDischargeRealizado){
        for(k=0;k<73;k++){
            entero1 = (GrafCargaBat[k] >> 8) & 0xFF; // Parte alta
            entero2 = GrafCargaBat[k] & 0xFF; // Parte baja 
            putc(entero1);
            putc(entero2);
            entero1 = 0;
            entero2 = 0;
            GrafCargaBat[k] = 0;
        }
        k=0;
        for(k=0;k<120;k++){
            entero1 = (GrafDescargaBat[k] >> 8) & 0xFF; // Parte alta
            entero2 = GrafDescargaBat[k] & 0xFF; // Parte baja 
            putc(entero1);
            putc(entero2);
            entero1 = 0;
            entero2 = 0;
            GrafDescargaBat[k] = 0;
        }
    }
}


/** 
  * @brief      Gestion de la eleccion del Test.
  * @param      none.
  * @retval      none.
  * @details     Segun la entrada del UART.
  */
void TestChoose()                                             // Eleccion de el test a realizar.
{
 if (gTestElegido==1){
     TensionPSyDCBus();
       switch(gTestNumber) {
        case 1:
            PwrUpTest1();
            Reset_Tester();
            break;
        case 2:
            PwrUpTest2();
            Reset_Tester();
            break;
        case 3:
            PwrUpTest3();
            Reset_Tester();
            break;
        case 4:
            PwrUpTest4();
            Reset_Tester();
            break;
        case 5:
            PwrUpTest5();
            Reset_Tester();
            break;
        case 6:
            TstPshBtn1();
            Reset_Tester();
            break;
        case 7:
            TstPshBtn2();
            Reset_Tester();
            break;
        case 8:
            TstPshBtn3();
            Reset_Tester();
            break;
        case 9:
            TstPshBtn4();
            Reset_Tester();
            break;
        case 10:
            TstPshBtn5();
            Reset_Tester();
            break;
        case 11:
            BtryDiscTstPshBtn1();
            Reset_Tester();
            break;
        case 12:
            BtryDiscTstPshBtn2();
            Reset_Tester();
            break;
        case 13:
            BtryDiscTstPshBtn3();
            Reset_Tester();
            break;
        case 14:
            BtryDiscTstPshBtn4();
            Reset_Tester();
            break;
        case 15:
            BtryDiscTstPshBtn5();
            Reset_Tester();
            break;
        case 16:
            BtryDiscTstPshBtn6();
            Reset_Tester();
            break;
        case 17:
            ContChargeInterChargeAutonomy();
            Reset_Tester();
            break;
        case 18:
            DeepChargeBattery();
            Reset_Tester();
            break;
        case 19:
            AutonomyAutoTest();
            Reset_Tester();
            break;
#if defined __COMPILA_RED_LED_RESET_TEST          
        case 20:
            RedLEDResetWithPshBtn();
            Reset_Tester();
            break;
#endif
        case 21:
            BattDiscPwrSwitch1();
            Reset_Tester();
            break;
        case 22:
            BattDiscPwrSwitch2();
            Reset_Tester();
            break;
        case 23:
            BattDiscPwrSwitch3();
            Reset_Tester();
            break;
        case 24:
            TemperatureDerating();
            Reset_Tester();
            break;
#if defined __COMPILA_HEATER
        case 25:
            HTRTesting();
            Reset_Tester();
            break;
#endif
#if defined __COMPILA_SENSOR_UWAVE_LUZ
        case 26:
            MotionLightSensor();
            Reset_Tester();
            break;
#endif
        case 32:
              Reset_Tester();
              delay_ms(1000);
              PwrUpTest1();
                
              Reset_Tester();
              delay_ms(1000);    
              PwrUpTest2(); // Test 2
         
              Reset_Tester();
              delay_ms(1000);
              PwrUpTest3(); // Test 3
         
              Reset_Tester();
              delay_ms(1000);
              PwrUpTest4(); // Test 4
         
              Reset_Tester();
              delay_ms(1000);
              PwrUpTest5(); // Test 5
         
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn1(); // Test 6
         
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn2(); // Test 7
         
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn3(); // Test 8
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn1(); // Test 11
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn2(); // Test 12
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn3(); // Test 13
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn4(); // Test 14
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn5(); // Test 15
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn6(); // Test 16
         
              Reset_Tester();
              delay_ms(1000);
              ContChargeInterChargeAutonomy(); // Test 17
       
              Reset_Tester();
              delay_ms(1000);
              DeepChargeBattery(); // Test 18
         //Estos tests necesitan que la bateria este descargada entes de ejecutarse 
             Reset_Tester();
              delay_ms(1000);
              TstPshBtn4(); // Test 9
         
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn5(); // Test 10
              
              Reset_Tester();
              delay_ms(1000);
              AutonomyAutoTest(); // Test 19
              

#if defined __COMPILA_RED_LED_RESET_TEST
              Reset_Tester();                       //No implementado en la luminaria
              delay_ms(1000);
              RedLEDResetWithPshBtn(); // Test 20
#endif
              Reset_Tester();
              delay_ms(1000);
              BattDiscPwrSwitch3(); // Test 23
              
              //Se desconecta durante un rato el micro:
              output_high(POWER_DISCONNECTION);
              output_high(BAT_DISC);
              delay_min(1);   
              
              Reset_Tester();
              delay_ms(1000);
              BattDiscPwrSwitch1(); // Test 22
              
              //Se desconecta durante un rato el micro:
              output_high(POWER_DISCONNECTION);
              output_high(BAT_DISC);
              delay_min(1); 
              
              Reset_Tester();
              delay_ms(1000);
              BattDiscPwrSwitch2(); // Test 21

              Reset_Tester();
              delay_ms(1000);
              TemperatureDerating(); // Test 24
              
#if defined __COMPILA_HEATER
              Reset_Tester();
              delay_ms(1000);
              HTRTesting(); // Test 25
#endif
#if defined __COMPILA_SENSOR_UWAVE_LUZ
              
              Reset_Tester();
              delay_ms(1000);
              MotionLightSensor(); // Test 26
#endif
            break;
        case 27:
             Reset_Tester();
              delay_ms(1000);
              PwrUpTest1();
                
              Reset_Tester();
              delay_ms(1000);    
              PwrUpTest2(); // Test 2
         
              Reset_Tester();
              delay_ms(1000);
              PwrUpTest3(); // Test 3
         
              Reset_Tester();
              delay_ms(1000);
              PwrUpTest4(); // Test 4
         
              Reset_Tester();
              delay_ms(1000);
              PwrUpTest5(); // Test 5
            break;
        case 28:
             Reset_Tester();
              delay_ms(1000);
              TstPshBtn1(); // Test 6
         
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn2(); // Test 7
         
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn3(); // Test 8
         /*
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn4(); // Test 9
         
              Reset_Tester();
              delay_ms(1000);
              TstPshBtn5(); // Test 10*/
            break;
        case 29:
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn1(); // Test 11
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn2(); // Test 12
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn3(); // Test 13
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn4(); // Test 14
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn5(); // Test 15
         
              Reset_Tester();
              delay_ms(1000);
              BtryDiscTstPshBtn6(); // Test 16
            break;
        case 30:
              Reset_Tester();
              delay_ms(1000);
              BattDiscPwrSwitch3(); // Test 23 
              
              Reset_Tester();
              delay_ms(1000);
              BattDiscPwrSwitch1(); // Test 22
              
              Reset_Tester();
              delay_ms(1000);
              BattDiscPwrSwitch2(); // Test 21
            break;
        case 31:
              Reset_Tester();
              delay_ms(1000);
              ContChargeInterChargeAutonomy(); // Test 17
         
              Reset_Tester();
              delay_ms(1000);
              DeepChargeBattery(); // Test 18
         
              Reset_Tester();
              delay_ms(1000);
              AutonomyAutoTest(); // Test 19
            break;
        default:
               ErrorCodes[ErrorCont] = 400;        //Almacenamos el codigo del error en el vector de errores
               ErrorCont++; 
            break;
    }
     gTestElegido = 0;
    gTestFinalizado = 1;
   }

}


//Lectura y conversion V_PS      V_PSd = V_PS * 18/118 * 1024/5 -> V_PS(mV) = V_PSd * 1000/31.2 = V_PS * 32
int16 Read_V_PS() {
    int16 V_PS = 0;
    set_adc_channel(CANAL_V_PS);                     
    delay_us(10);
    V_PS = read_adc();
    V_PS += read_adc();
    V_PS = V_PS <<4;                     //equivale a *16   
    return V_PS;                         //es equivalente a multiplicar por 16, es decir a multiplicar por 32 y dividir entre 2
}//                                                 multiplicar por 32 y dividir entre 2


// Lectura y conversión V_DCBus           V_DCBusd = V_DCbus * 18/118 * 1024/5 -> V_DCBus(mV) = VDCBusd * 1000/31.2 = Vdig * 32
int16 Read_V_DCBus() {
    int16 V_DCBus = 0;
    set_adc_channel(CANAL_V_DC_BUS);
    delay_us(10);
    V_DCBus = read_adc();
    V_DCBus += read_adc();
    V_DCBus = V_DCBus <<4;            //Es equivalente a multiplicar por 16, es decir a multiplicar por 32 y dividir entre 2
    return V_DCBus;
}


//Lectura y conversion I_LEDS cuando estan en derrating
int16 Read_I_LED_DERR() {
    int16 Med;
    int16 Suma = 0;
    for(i=0;i<128;i++){
        delay_us(250); 
        Suma =Suma + Read_I_LED();
    }
    Med = Suma>>7;
    return Med;
}


//Lectura y conversion I_LEDS      V_ILEDd = V_ILED * 1024/5 -> V_ILED(mV) = V_ILEDd * 5/1024 -> I_LD = V_ILEDd * 5/1024 * 1/100 *1/0.075 -> I_LD (mA) = V_ILDd * 0.65
int16 Read_I_LED() {
    double V_ILD = 0;
    set_adc_channel(CANAL_V_ILED);
    delay_us(10);
    V_ILD = read_adc();
    V_ILD += read_adc();
    return V_ILD * 0.32;
}


//#if defined __COMPILA_HEATER
////Lectura y conversion I_HTR     
//int8 Read_I_HTR() {
//    int8 V_IHTR = 0;
//    V_IHTR = Read_V_HTR();
//    return V_IHTR/74;
//}
//#endif


//Lectura y conversion I_BatDesc     V_descd = V_Desc * 1024/5 -> V_BatDesc(mV) = V_Descd * 5/1024 -> I_BatDesc = V_Descd * 5/1024 * 1/100 *1/0.068 -> I_BatDesc (mA) = V_Descd * 0.71
int16 Read_I_BatDesc() {
    int16 V_Desc = 0;
    set_adc_channel(CANAL_V_DESCARGA_BAT);
    delay_us(10);
    V_Desc = read_adc();
    V_Desc += read_adc();
    V_Desc = V_Desc *0.36;          //equivale a *0.36
    return V_Desc;
}


//Lectura y conversion I_BatCarg      V_Cargd = V_Carg * 1024/5 -> V_Carg(mV) = V_cargd * 5/1024 -> I_BatCarg = V_Cargd * 5/1024 * 1/100  -> I_BatCarg (mA) = V_Cargd * 0.0488
int16 Read_I_BatCarg() {
    int16 V_Carg = 0;
    set_adc_channel(CANAL_V_CARGA_BAT);
    delay_us(10);
    V_Carg = read_adc();
    V_Carg += read_adc();
    V_carg = V_Carg * 0.052;
    return V_Carg;
}


//Lectura y conversion V_Bat      V_Batd = V_Bat * 18/118 * 1024/5 -> V_Bat(mV) = V_Batd * 1000/31.2 = V_PS * 32
int16 Read_V_Bat() {
    double V_Bat;
    set_adc_channel(CANAL_V_BAT);
    delay_us(10);
    V_Bat = read_adc();
    V_Bat += read_adc();
    V_Bat = V_Bat *16 ;
    return V_Bat;
}


//Lectura y conversion V_LD      V_PSd = V_PS * 18/158 * 1024/5 -> V_PS(mV) = V_PSd * 1000/31.2 = V_PS * 43
int16 Read_V_LD() {
    double V_LD = 0;
    set_adc_channel(CANAL_V_LEDS);
    delay_us(10);
    V_LD = read_adc();
    V_LD += read_adc();
    V_LD = V_LD * 21.43;
    return V_LD;
}


#if defined __COMPILA_HEATER
//Lectura y conversion V_HTR      V_PSd = V_PS * 18/118 * 1024/5 -> V_PS(mV) = V_PSd * 1000/31.2 = V_PS * 32
int16 Read_V_HTR() {
    int16 V_HTR = 0;
    set_adc_channel(CANAL_V_HEATER);
    delay_us(10);
    V_HTR = read_adc();
    V_HTR += read_adc();
    V_HTR = V_HTR *16;
    return V_HTR;
}
#endif


/** 
  * @brief      Temperatura Ambiente
  * @param      **
  * @retval     **
  * @details    Manipulacion de la entrada del sensor de temperatura de la LX
  */
void Set_T_Amb(int16 Temperatura){
        int16 T = 0;
        int16 Kelvin = Temperatura+273;
        T = (Kelvin*131)>>8;           //10(mV/K)*(256/5000) ya que es de 8 bits
        dac_write(T);
    }
    
    
#if defined __COMPILA_HEATER
/** 
  * @brief      Temperatura Calentador
  * @param      **
  * @retval     **
  * @details    Manipulacion de la entrada del sensor de temperatura del Calentador
  */
void Set_T_HTR(float Temperatura){
    int8 WRT1;
    int8 WRT2;
    int16 Kelvin = Temperatura + 273;       //Se pasa la temperatura a K, despues de multiplica por 10*(1024/5000)/2 = 1  
    Kelvin = (Kelvin*2.5);
    WRT1 = Kelvin >> 6;                     //Se divide entre 64 oara obtener los primeros 4 bits
    WRT1 = (0x10) | WRT1;                   //Se introduce la cadena 0001 0000 de la configuracion (A  0  x2  1)    (0x10)
    WRT2 = 0x3F & Kelvin;                   //Se separan los otros 6 bits
    WRT2 = WRT2 << 2;                       //Se introducen dos ceros al final
    delay_ms(1000);
    for(i=0;i<5;i++){
    output_low(DAC_CS);                     //Para transmitir ponemos el in CS a cero
    delay_ms(10);
    spi_write2(WRT1);                       //Se transmite la primera cadena
    spi_write2(WRT2);                       //Se transmite la segunda cadena
    delay_ms(10);
    output_high(DAC_CS);   
    delay_ms(100);
    }
    delay_ms(1000);
}
#endif


#if defined __COMPILA_SENSOR_UWAVE_LUZ
/** 
  * @brief      Sensor uWave
  * @param      **
  * @retval     **
  * @details    Manipulacion de la entrada del sensor de temperatura del Calentador
  */
void Activar_uWave(int1 presencia){               //0 for no presence, 1 for presence
    int8 WRT1;
    int8 WRT2;
    if(presencia == 0){
        WRT1 = 0x98;
        WRT2 = 0x00;
        
    }else if(presencia == 1){
        WRT1 = 0x90;
        WRT2 = 0x00;
    }else{
        WRT1 = 0x90;
        WRT2 = 0x00;
    }
    delay_ms(1000);
    for(i=0;i<5;i++){
    output_low(DAC_CS);                     //Para transmitir ponemos el in CS a cero
    delay_ms(10);
    spi_write2(WRT1);                       //Se transmite la primera cadena
    spi_write2(WRT2);                       //Se transmite la segunda cadena
    delay_ms(10);
    output_high(DAC_CS);   
    delay_ms(100);
    }
    delay_ms(1000);
}
#endif


  /** 
  * @brief      Comprobacion del parpadeo de los LEDs rojo y Verde
  * @param      0 for RED, 1 GREEN
  * @retval     Parpadea
  * @details    Comprobacion de si los LEDs verde y rojo estan parpadeando
  */
void LEDFlickCheck(int8 LEDChoose){                  // 0 for Red, 1 for Green
   /* Variables Locales*/
   int8 PreState;
   int8 CurrentState;
   int8 ContCambios = 0;
   int8 cont = 0;
   
   /*Funcionalidad*/ 
  if(LEDChoose == 0){                                   //Para el LED Rojo
            PreState = input_state(RED_LED);                            //Primer estado 
            delay_ms(100);
            CurrentState = input_state(RED_LED);                        // Segundo estado
            delay_ms(100);
            while (cont<30){
                PreState = CurrentState;
                CurrentState = input_state(RED_LED);
                if (CurrentState != PreState){ ContCambios++;}     //contamos la cantidad de veces que cambia
                cont++;
                delay_ms(100);
            }
            if (ContCambios >= 3){
      gRedFlick_OK = 1;                                             //Si ha cambiado mas de 3 veces devuelve 2
      }else{
      gRedFlick_OK = 0;}                                            //Si ha cambiado menos se considera que no parpadea
    }else if (LEDChoose == 1){
          PreState = input_state(GREEN_LED);
          delay_ms(10);
          CurrentState = input_state(GREEN_LED);
          delay_ms(100);
          while (cont<30){
             PreState = CurrentState;
             CurrentState = input_state(GREEN_LED);
             if (CurrentState != PreState){ 
                 ContCambios++;}
             cont++;
             delay_ms(100);
          }
          if (ContCambios >= 3){
      gGreenFlick_OK = 1;
      } else {
      gGreenFlick_OK = 0;}    
          }    
      }


  /** 
  * @brief      Battery Discharge Power
  * @param      **
  * @retval     BatDischargePow
  * @details    Calculo de la potecia de descarga de las baterias.
  */
  float BatteryDischargePower(){     //en W
  /*Variables Locales*/
  float Power;
  /*funcionalidad*/
  VBat = Read_V_Bat();
  I_desc = Read_I_BatDesc();
  Power = (VBat/1000) * I_desc;
  Power = Power/1000;
  return Power;
  }
  
  
#if defined __COMPILA_HEATER
  /** 
  * @brief      Heater Power
  * @param      **
  * @retval     BatDischargePow
  * @details    Calculo de la potecia del Calentador
  */
  int8 HTR_Power(){     //en W    
  /*Variables Locales*/
  int16 square;
  int16 Power;
  int16 V_Heater;
  int16 V_Heater_V;
  int16 resistencia = 74;
  /*funcionalidad*/
  V_Heater = Read_V_HTR();
  V_Heater_V = V_Heater*0.001f;
  square = V_Heater_V * V_Heater_V;
  Power = square/resistencia;
  return Power;
  }
#endif
  
  
  /** 
  * @brief      Delay min
  * @param      **
  * @retval     **
  * @details    **
  */
  void delay_min(int8 minutos){
      int16 j;
      int16 Segundos10;
      Segundos10 = minutos*6;
      for(j=0;j<=Segundos10-1 ; j++){
          delay_ms(10000);
      }
  }

  
  /** 
  * @brief      Intermitent Charge comprobation
  * @param      **
  * @retval     **
  * @details    **
  */
  void IntermitentCharge(){
      int8 OK = 0;
      for(int j=0;j<3;j++){
      while (Read_I_BatCarg() == 0){                      //nos aseguramos de que no sea 0
          delay_ms(100);           
      }
        delay_ms(20000);                             //Espera 20s
        if (Read_I_BatCarg() == 0){                         //No deberia estar cargando
            delay_ms(20000);     
            delay_ms(20000); 
            delay_min(2);//Espera 160s
            if (Read_I_BatCarg() > 0){                       //Deberia estar Cargando
                 OK ++;
            }   
       }
      }
        if(OK >1){
              gIntermitentChargeOK = 1;
          }else {
              gIntermitentChargeOK = 0;
          }
        delay_ms(100);
  }
 
  
  /** 
  * @brief      Descarga de las baterias en caso de que sea necesario
  * @param      **
  * @retval     **
  * @details    Se usa para algunos tests en los que se requiere que la bateria este descargada
  */
  void Descarga_Bat(){
    output_high(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_low(SWITCH_CONTROL);
    Get_Estado_LUM(0);
    while(Read_V_Bat() >= 11000){
        delay_min(1);
    }
  }