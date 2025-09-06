/* 
 * File:   Tests.h
 * Author: ybustillo
 *
 * Created on 26 de marzo de 2024, 14:15
 */


#include <main.h>
#ifndef TESTS_H
#define	TESTS_H

void TensionPSyDCBus(){
   gVPS =  Read_V_PS();
   gVPS = gVPS/1000;
   gVDCBus = Read_V_DCBus();
   gVDCBus= gVDCBus/1000;
}

   /*-----------------------------------------------------------------------Power Up Tests---------------------------------------------------------------------*/
/** 
  * @brief      Test de encendido 1.
  * @param      none.
  * @retval      none.
  * @details   none.
  */
void PwrUpTest1()
{
   /*variables locales*/
   int1 Test_OK = 0;
   int1 Error = 0;
   unsigned int8 ErrorCode = 0;
   
   //estado inicial         Sin Tension, Bateria conectada y Switch OFF
   output_high(POWER_DISCONNECTION);        
   output_low(BAT_DISC);                    
   output_high(SWITCH_CONTROL);
   //Accion 1: Encendido de la Luminaria
   delay_ms(100);
   output_low(POWER_DISCONNECTION);
   delay_ms(1000);
   //Comprobacion 1
   Get_Estado_LUM(1);                                                       //Funcion que obtiene todos los datos de tensiones y intensidades (Cargando)                                             
   delay_ms(100);
   if (gGreenFlick_OK == 1 && gLEDs_OFF == 1 && gCharge_OK == 1){           // Si se cumplen todas las condiciones el primer paso esta bien
     Test_OK = 1;
     //Accion 2
     delay_ms(100);
     output_low(SWITCH_CONTROL);
     delay_ms(1000);
     //Comprobacion 2
     Get_Estado_LUM(1);                                 
     delay_ms(100);
     if (gGreenFlick_OK == 1 && gCharge_OK == 1 && gLEDs_FULLPWR == 1){
         Test_OK = 1;
         //Accion 3
         delay_ms(100);
         output_high(SWITCH_CONTROL);
         delay_ms(1000); 
         //Comprobacion 3
         Get_Estado_LUM(1);
         delay_ms(100);
         if (gGreenFlick_OK == 1 && gLEDs_OFF == 1 && gCharge_OK == 1){
            Test_OK = 1;
            gRampDown_OK = 0;
            Error = 0;
         } else {
          ErrorCode = 13;
          Error = 1;
          Test_OK = 0;}
     }else{
        ErrorCode = 12;
        Error = 1;
        Test_OK = 0;}
   }else { 
   ErrorCode = 11;
   Error = 1;
   Test_OK = 0;}
   
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}


/** 
  * @brief      Test de encendido 2.
  * @param      none.
  * @retval      none.
  * @details   none.
  */
void PwrUpTest2()
{
    /*variables locales*/
    int1 Test_OK = 0;
    int1 Error = 0;
    unsigned int8 ErrorCode = 0;
   
    //estado inicial
    output_high(POWER_DISCONNECTION);
    output_low(SWITCH_CONTROL);
    output_low(BAT_DISC);
     delay_ms(1000); 
     //Accion 1
     output_low(POWER_DISCONNECTION); 
     delay_ms(1000);
     //Comprobacion 1
     Get_Estado_LUM(1);
     delay_ms(100);
     if (gGreenFlick_OK == 1 && gCharge_OK == 1 && gLEDs_FULLPWR == 1){
         Test_OK = 1;
         //Accion 2
         delay_ms(100);
         output_high(SWITCH_CONTROL);
         delay_ms(1000);
         //Comprobacion 2
         Get_Estado_LUM(1); 
         delay_ms(300);
         if (gGreenFlick_OK == 1 && gLEDs_OFF == 1 && gCharge_OK == 1){
            Test_OK = 1;
            gRampDown_OK = 0;
            Error = 0;
         } else {
          ErrorCode = 22;
          Error = 1;
          Test_OK = 0;}
     }else{
        ErrorCode = 21;
        Error = 1;
        Test_OK = 0;}
   
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}


/** 
  * @brief      Test de encendido 3.
  * @param      none.
  * @retval     none.
  * @details    none.
  */
void PwrUpTest3()
{
  /*variables locales*/
   int1 Test_OK = 0;
   int1 Error = 0;
   unsigned int8 ErrorCode = 0;
   
   //estado inicial
   output_high(POWER_DISCONNECTION);
   output_high(BAT_DISC);
   output_high(SWITCH_CONTROL);
   //Accion 1
   delay_ms(1000);
   output_low(POWER_DISCONNECTION);
   delay_ms(1000);
   //Comprobacion 1
   Get_Estado_LUM(0);   
   delay_ms(100);
   //Comprobacion 1
   if (GreenLED == 0 && gLEDs_OFF == 1 && gRedFlick_OK == 1){           // Si se cumplen todas las condiciones el primer paso esta bien
     delay_ms(100);
     output_low(SWITCH_CONTROL);
     delay_ms(1000); 
     Get_Estado_LUM(0);
     delay_ms(100);
     //Comprobacion 2
     if (GreenLED == 0 && gLEDs_FULLPWR == 1 && gRedFlick_OK == 1){
         delay_ms(100);
         output_high(SWITCH_CONTROL);
         delay_ms(1000);
         Get_Estado_LUM(0);
         delay_ms(100);
         //Comprobacion 3
         if (GreenLED == 0 && gLEDs_OFF && gRedFlick_OK == 1){
            Test_OK = 1;
            delay_ms(100);
            output_low(BAT_DISC);
            delay_ms(1000);
            Get_Estado_LUM(1); 
            delay_ms(100);
            //Comprobacion 4
            if (gLEDs_OFF == 1 && RedLED == 0 && gGreenFlick_OK == 1 && gCharge_OK == 1){
               Test_OK = 1;
               Error = 0;
            }else{
               ErrorCode = 34;
               Error = 1;
               Test_OK = 0;  
            }
         } else {
          ErrorCode = 33;
          Error = 1;
          Test_OK = 0;}
     }else{
        ErrorCode = 32;
        Error = 1;
        Test_OK = 0;}
   }else { 
   ErrorCode = 31;
   Error = 1;
   Test_OK = 0;}
   
  gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}


/** 
  * @brief      Test de encendido 4.
  * @param      none.
  * @retval     none.
  * @details    none.
  */
void PwrUpTest4()
{
   /*Variables Locales*/
   int1 Test_OK = 0;
   int1 Error = 0;
   unsigned int8 ErrorCode = 0;
  //Estado inicial
  output_high(POWER_DISCONNECTION);
  output_high(BAT_DISC);
  output_high(SWITCH_CONTROL);
  
  delay_ms(100);
  output_low(POWER_DISCONNECTION);
  output_low(SWITCH_CONTROL);
  //Encendido y apagado arbitrario del Switch
  delay_ms(100);
  output_high(SWITCH_CONTROL);      // Encendido y apagado aleatorio del switch  (Se puede SUSTITUIR con una variabe aleatoria)
  delay_ms(150);
  output_low(SWITCH_CONTROL);
  delay_ms(180);
  output_high(SWITCH_CONTROL);
  delay_ms(100);
  output_low(SWITCH_CONTROL);
  delay_ms(100);
  output_high(SWITCH_CONTROL);    
  delay_ms(150);
  output_low(SWITCH_CONTROL);
  delay_ms(180);
  output_high(SWITCH_CONTROL);
  delay_ms(10);
  output_low(SWITCH_CONTROL);
  delay_ms (2000);
  Get_Estado_LUM(0);
  //Comprobacion 1
  if ( GreenLED == 0 && gRedFlick_OK == 1 && gLEDs_FULLPWR == 1){
     Test_OK = 1;
     gRedFlick_OK = 0;
     Error = 0;
  }else{
     ErrorCode = 41;
     Error = 1;
     Test_OK = 0;  
   }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                             //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.

   }
}


/** 
  * @brief      Test de encendido 5.
  * @param      none.
  * @retval     none.
  * @details    none.
  */
void PwrUpTest5()
{
   /*Variables Locales*/
   int1 Test_OK = 0;
   int1 Error = 0;
   unsigned int8 ErrorCode = 0;
  //Estado inicial
  output_high(POWER_DISCONNECTION);
  output_low(BAT_DISC);
  output_high(SWITCH_CONTROL);
  
  delay_ms(100);
  output_low(POWER_DISCONNECTION);
  output_low(SWITCH_CONTROL);
  //Encendido y apagado arbitrario del Switch
  delay_ms(100);
  output_high(SWITCH_CONTROL);      // Encendido y apagado aleatorio del switch  (Se puede SUSTITUIR con una variabe aleatoria)
  delay_ms(150);
  output_low(SWITCH_CONTROL);
  delay_ms(180);
  output_high(SWITCH_CONTROL);
  delay_ms(100);
  output_low(SWITCH_CONTROL);
  delay_ms(100);
  output_high(SWITCH_CONTROL);      // Encendido y apagado aleatorio del switch  (Se puede SUSTITUIR con una variabe aleatoria)
  delay_ms(150);
  output_low(SWITCH_CONTROL);
  delay_ms(180);
  output_high(SWITCH_CONTROL);
  delay_ms(10);
  output_low(SWITCH_CONTROL);
  delay_ms (1000);
  
  Get_Estado_LUM(0);
  delay_ms(100);
  //Comprobacion 1
  if ( RedLED == 0 && gGreenFlick_OK == 1 && gLEDs_FULLPWR == 1){
     Test_OK = 1;
     gGreenFlick_OK = 0;
     Error = 0;
  }else{
     ErrorCode = 51;
     Error = 1;
     Test_OK = 0;  
   }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}
   /*-----------------------------------------------------------------------Test Push Button---------------------------------------------------------------------------*/
   

/** 
  * @brief      Test push button 1.
  * @param      none.
  * @retval      none.
  * @details   Comprobacion del boton.
  */
void TstPshBtn1()
{
/*Variables Locales*/ 
   int1 Test_OK = 0;
   int1 Error = 0;
   unsigned int8 ErrorCode = 0;
  /*Funcionalidad*/
  output_low(POWER_DISCONNECTION);
  output_low(BAT_DISC);
  output_high(SWITCH_CONTROL);
  delay_ms(2000);
  output_low(TEST_BUTTON);
  delay_ms(500);
  
  Get_Estado_LUM(2);
  delay_ms(50);
  if(RedLED == 0 && GreenLED == 0 && gDischargePowerOK == 1 && gLEDs_20PerPWR == 1){
      Test_OK = 1;
      delay_ms(10);
      ILD = Read_I_LED();
      output_high(TEST_BUTTON);
      delay_ms(1000);
      Get_Estado_LUM(1);
      delay_ms(100);
      if (gLEDs_OFF == 1 &&  gGreenFlick_OK == 1 && RedLED == 0 && gCharge_OK == 1 ){
         Test_OK = 1;
         Error = 0;
         delay_ms(10);
      }else{
         ErrorCode = 62;
         Error = 1;
         Test_OK = 0;
      }
  }else{
     ErrorCode = 61;
     Error = 1;
     Test_OK = 0;
  }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
  
}


/** 
  * @brief      Test push button 2.
  * @param      none.
  * @retval      none.
  * @details   Comprobacion del boton.
  */
void TstPshBtn2()
{
/*Variables Locales*/
   int1 Test_OK = 0;
   int1 Error = 0;
   unsigned int8 ErrorCode = 0;
  /*Funcionalidad*/
  output_low(BAT_DISC);
  output_low(POWER_DISCONNECTION);
  output_low(SWITCH_CONTROL);
  delay_ms(3000);
  output_low(TEST_BUTTON);
  delay_ms(2000);
  Get_Estado_LUM(2);
  delay_ms(100);
  if (gDischargePowerOK == 1 && gLEDs_20PerPWR == 1 && GreenLED == 0 && RedLED == 0){
      Test_OK = 1;
      delay_ms(10);
      ILD = Read_I_LED();
      output_high(TEST_BUTTON);
      delay_ms(3000);
      Get_Estado_LUM(1);
      delay_ms(100);
      if ( gGreenFlick_OK == 1 && RedLED == 0 && gCharge_OK == 1 && gLEDs_FULLPWR == 1 ){
         Test_OK = 1;
         gRampDown_OK = 0;
         gRampUp_OK = 0;
         Error = 0;
         delay_ms(10);
      }else{
         ErrorCode = 72;
         Error = 1;
         Test_OK = 0;
      }
  }else{
     ErrorCode = 71;
     Error = 1;
     Test_OK = 0;
  }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}


/** 
  * @brief      Test push button 3.
  * @param      none.
  * @retval      none.
  * @details   Comprobacion del boton.
  */
void TstPshBtn3()
{
/*Variables Locales*/
   int1 Test_OK = 0;
   int1 Error = 0;
   unsigned int8 ErrorCode = 0;   
  /*Funcionalidad*/
  output_low(POWER_DISCONNECTION);
  output_high(BAT_DISC);
  output_high(SWITCH_CONTROL);
  delay_ms(2000);
  output_low(TEST_BUTTON);
  delay_ms(1000);
  Get_Estado_LUM(0);
  delay_ms(100);
  if (gLEDs_OFF == 1 && RedLED == 1){
   Test_OK = 1;
   delay_ms(10);
   output_high(TEST_BUTTON);
   delay_ms(1000);
   Get_Estado_LUM(0);
   delay_ms(100);
   if (gLEDs_OFF == 1 && gRedFlick_OK == 1){
      Test_OK = 1;
      gRedFlick_OK = 0;
      delay_ms(10);
      ILD = Read_I_LED;
      output_low(SWITCH_CONTROL);
      delay_ms(1000);
      Get_Estado_LUM(0);
      delay_ms(100);
      if (gLEDs_FULLPWR == 1 && GreenLED == 0 && gRedFlick_OK == 1){
         Test_OK = 1;
         gRedFlick_OK = 0;
         delay_ms(10);
         output_low(TEST_BUTTON);
         delay_ms(1000);
         Get_Estado_LUM(0);
         delay_ms(100);
         if (gLEDs_OFF == 1 && GreenLED == 0 && RedLED == 1){
            Test_OK = 1;
            gRampUp_OK = 0;
            delay_ms(100);
            output_high(TEST_BUTTON);
            delay_ms(1000);
            Get_Estado_LUM(0);
            delay_ms(100);
            if (gLEDs_FULLPWR == 1 && GreenLED == 0 && gRedFlick_OK == 1){
                Test_OK = 1;
                gRedFlick_OK = 0;
                gRampUp_OK = 0;
                Error = 0;
            }else{
                ErrorCode = 85;
                Error = 1;
                Test_OK = 0;
            }
         }else{
            ErrorCode = 84;
            Error = 1;
            Test_OK = 0;
         }
      }else{
         ErrorCode = 83;
         Error = 1;
         Test_OK = 0;
      }
   }else{
     ErrorCode = 82;
     Error = 1;
     Test_OK = 0;
   } 
  }else{
     ErrorCode = 81;
     Error = 1;
     Test_OK = 0;
  }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}


/** 
  * @brief      Test push button 4.
  * @param      none.
  * @retval      none.
  * @details   Comprobacion del boton.
  */
void TstPshBtn4()
{
    /*Variables Locales*/
    int1 Test_OK = 0;
    int1 Error = 0;
    unsigned int8 ErrorCode = 0;
    /*Funcionalidad*/
//    Descarga_Bat();                     //Nos aseguramos de que la bateria esta casi descargada. si no lo esta se descarga
    output_low(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_high(SWITCH_CONTROL);
    delay_ms(100);
    while(Read_V_Bat()<10800){
        delay_ms(10);
    }
    output_low(TEST_BUTTON);
    delay_ms(100);
    Get_Estado_LUM(2);
    delay_ms(100);
    if (gLEDs_20PerPWR==1 && gDischargePowerOK==1 && GreenLED == 0 && RedLED == 0){
        Test_OK = 1;
        delay_ms(10);
        while(Read_I_LED() > 10){
            delay_ms(100);
        }
        delay_ms(10);
        Get_Estado_LUM(0);
        delay_ms(100);
        if(gLEDs_OFF == 1 && VBat <=V_LIM_BAT_DESCARGADA_A && VBat >=V_LIM_BAT_DESCARGADA_B && RedLED == 1){
           Test_OK = 1;
           delay_ms(10);
           output_high(TEST_BUTTON);
           delay_ms(1000);
           Get_Estado_LUM(1);
           delay_ms(100);
           if(gGreenFlick_OK == 1 && gLEDs_OFF == 1 && RedLED == 0 && gCharge_OK){
               Test_OK = 1;
               Error = 0;
               ErrorCode = 0;
           }else{
               Test_OK = 0;
               Error = 1;
               ErrorCode = 93;
           }
        }else{
            Test_OK = 0;
            Error = 1;
            ErrorCode = 92;
        }
    }else{
       Test_OK = 0;
       Error = 1;
       ErrorCode = 91; 
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }   
}


/** 
  * @brief      Test push button 5.
  * @param      none.
  * @retval      none.
  * @details   Comprobacion del boton.
  */
void TstPshBtn5()
{
    /*Variables Locales*/
    int1 Test_OK = 0;
    int1 Error = 0;
    unsigned int8 ErrorCode = 0;
    /*Funcionalidad*/
 //   Descarga_Bat();                     //Nos aseguramos de que la bateria esta casi descargada. si no lo esta se descarga
    output_low(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_low(SWITCH_CONTROL);
    delay_ms(1000);
    while(Read_V_Bat()<10800){
        delay_ms(10);
    }
    output_low(TEST_BUTTON);
    delay_ms(1000);
    Get_Estado_LUM(2);
    delay_ms(10);
    if (gLEDs_20PerPWR==1 && gDischargePowerOK==1 && GreenLED == 0 && RedLED == 0 ){
        Test_OK = 1;
        delay_ms(10);
        while(Read_I_LED()>10){
            delay_ms(100);
        }
        delay_ms(100);
        Get_Estado_LUM(0);
        delay_ms(100);
        if(gLEDs_OFF == 1 && VBat <= V_LIM_BAT_DESCARGADA_A && VBat >= V_LIM_BAT_DESCARGADA_B && RedLED == 1){
           Test_OK = 1;
           delay_ms(10);
           output_high(TEST_BUTTON);
           delay_ms(3000);
           Get_Estado_LUM(1);
           delay_ms(100);
           if(gGreenFlick_OK == 1 && gCharge_OK && RedLED == 0 && gLEDs_FULLPWR == 1){
               Test_OK = 1;
               Error = 0;
               ErrorCode = 0;
           }else{
               Test_OK = 0;
               Error = 1;
               ErrorCode = 103;
           }
        }else{
            Test_OK = 0;
            Error = 1;
            ErrorCode = 102;
        }
    }else{
       Test_OK = 0;
       Error = 1;
       ErrorCode = 101; 
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }  
}

   /*----------------------------------------------------------------Battery disconection with test push button---------------------------------------------------------------*/
   
/** 
  * @brief      Battery disconnect using test push button 1.
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void BtryDiscTstPshBtn1()
{
   /*Variables Locales*/
   int1 Error = 0;
   int1 Test_OK = 0;
   unsigned int8 ErrorCode = 000;
   /*Funcionalidad*/
   output_low(POWER_DISCONNECTION);
   output_low(BAT_DISC);
   output_low(SWITCH_CONTROL);
   while(Read_V_Bat() < 12000){         //para que el test uncione correctamente la bateria debe estar en un minimo de carga
       delay_ms(100);
   }
   delay_ms(1000);
   output_low(TEST_BUTTON);             // El boton tiene logica inversa. BTN --> ON
   delay_ms(11000);
   Get_Estado_LUM(0);
   delay_ms(100);
   if ( gLEDs_20PerPWR == 1 && GreenLED == 0 && RedLED == 0 ){
      Test_OK = 1;
      Error = 0;
   }else{
     ErrorCode = 111;
     Error = 1;
     Test_OK = 0;
   }
   output_high(TEST_BUTTON);
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}


/** 
  * @brief      Battery disconnect using test push button 2.
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void BtryDiscTstPshBtn2()
{
 /*Variables Locales*/
   int1 Error = 0;
   int1 Test_OK = 0;
   unsigned int8 ErrorCode = 000;
   /*Funcionalidad*/
   output_low(POWER_DISCONNECTION);
   output_low(BAT_DISC);
   output_high(SWITCH_CONTROL);
   while(Read_V_Bat() < 12000){         //para que el test uncione correctamente la bateria debe estar en un minimo de carga
       delay_ms(100);
   }
   delay_ms(1000);
   output_low(TEST_BUTTON);
   delay_ms(11000);
   Get_Estado_LUM(0);
   delay_ms(100);
   if ( gLEDs_20PerPWR == 1 && GreenLED == 0 && RedLED == 0 ){
      Test_OK = 1;
      Error = 0;
   }else{
     ErrorCode = 121;
     Error = 1;
     Test_OK = 0;
   }
   output_high(TEST_BUTTON);
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}


/** 
  * @brief      Battery disconnect using test push button 3.
  * @param      none.
  * @retval      none.
  * @details   none.
  */
void BtryDiscTstPshBtn3()
{
    /*Variables Locales*/   
    int1 Error = 0;
    int1 Test_OK = 0;
    unsigned int8 ErrorCode = 000;
    int8 i;
    while(Read_V_Bat() < 12000){         //para que el test uncione correctamente la bateria debe estar en un minimo de carga
       delay_ms(100);
    }
    /*Funcionalidad*/
    output_high(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_high(SWITCH_CONTROL);
    delay_ms(1000);
    for (i=0;i<13;i++){                     //Intermitent activation of test Push button for more than 10s
        output_low(TEST_BUTTON);
        delay_ms(500);
        output_high(TEST_BUTTON); 
        delay_ms(500);
    }
    output_low(TEST_BUTTON);
    delay_ms(100);
    Get_Estado_LUM(0);
    delay_ms(100);
    if ( gLEDs_20PerPWR == 0 ){                       //LEDs permanecen Encendidos
        Test_OK = 1;
        Error = 0;
    } else{
        ErrorCode = 131;
        Error = 1;
        Test_OK = 0;        
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   } 
}


/** 
  * @brief      Battery disconnect using test push button 4.
  * @param      none.
  * @retval      none.
  * @details   none.
  */
void BtryDiscTstPshBtn4()
{
    /*Variables Locales*/   
    int1 Error = 0;
    int1 Test_OK = 0;
    unsigned int8 ErrorCode = 000;
    int8 i;
    while(Read_V_Bat() < 12000){         //para que el test uncione correctamente la bateria debe estar en un minimo de carga
       delay_ms(100);
    }
    /*Funcionalidad*/
    output_low(POWER_DISCONNECTION);
    delay_ms(100);
    output_high(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_low(SWITCH_CONTROL);
    delay_ms(1000);
    for (i = 0; i <=4 ; i++){
        delay_ms(10);
        output_low(TEST_BUTTON);
        delay_ms(2000);
        output_high(TEST_BUTTON);
        delay_ms(1000);
        Get_Estado_LUM(0);
        delay_ms(100);
        if ( gLEDs_20PerPWR == 1 ){
            Test_OK = 1;
            Error = 0;
        } else{
            ErrorCode = 131+i;
            Error = 1;
            Test_OK = 0;        
        }   
        delay_ms(1000);
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   } 
}


/** 
  * @brief      Battery disconnect using test push button 5.
  * @param      none.
  * @retval      none.
  * @details   none.
  */
void BtryDiscTstPshBtn5()
{
    /*Variables Locales*/   
    int1 Error = 0;
    int1 Test_OK = 0;
    unsigned int8 ErrorCode = 000;
    int8 contador = 0;
    /*Funcionalidad*/
    output_low(POWER_DISCONNECTION);
    delay_ms(1000);
    output_high(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_low(SWITCH_CONTROL);
    delay_ms(2000);
    output_low(TEST_BUTTON);
    While(Read_I_LED()>10){
        delay_ms(1000);
        contador++;   
    }
    delay_ms(10);
    output_high(TEST_BUTTON);
    if(contador >= 3){
        Test_OK = 1;
        Error = 0;
        contador = 0;
    }else{
        Test_OK = 0;
        Error = 1;
        Errorcode = 151;
    }
    
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   } 
}


/** 
  * @brief      Battery disconnect using test push button 6.
  * @param      none.
  * @retval      none.
  * @details   none.
  */
void BtryDiscTstPshBtn6()
{
    /*Variables Locales*/
        int1 Error = 0;
        int1 Test_OK = 0;
        unsigned int8 ErrorCode = 000;
        int8 contador = 0;
    /*Funcionalidad*/

    output_low(POWER_DISCONNECTION);
    delay_ms(1000);
    output_high(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_low(SWITCH_CONTROL);
    delay_ms(2000);
    output_low(TEST_BUTTON);
    While(Read_I_LED()>10){
        delay_ms(1000);
        contador++;   
    }
    delay_ms(10);
    output_high(TEST_BUTTON);
    output_low(POWER_DISCONNECTION);
    delay_ms(1000);
    output_high(POWER_DISCONNECTION);
    delay_ms(1000);
    output_low(TEST_BUTTON);
    While(Read_I_LED()>10){
        delay_ms(950);
        contador++;  
    }
    output_high(TEST_BUTTON);
        if(contador >= 3){
        Test_OK = 1;
        Error = 0;
        contador = 0;
    }else{
        Test_OK = 0;
        Error = 1;
        Errorcode = 161;
    }
    
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   } 
    
}

   /*--------------------------------------------------------------------Battery Disconection using power switch----------------------------------------------------------*/
   
/** 
  * @brief      battery disconnection using power switch 1.
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void BattDiscPwrSwitch1()
{
    /*Variables Locales*/
    int1 Error = 0;
    int1 Test_OK = 0;
    unsigned int8 ErrorCode = 000;
    int8 i;
    /*funcionalidad*/
     while(Read_V_Bat() < 12000){         //para que el test uncione correctamente la bateria debe estar en un minimo de carga
       delay_ms(100);
    }
    output_low(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_high(SWITCH_CONTROL);
    delay_ms(1000);
    output_high(POWER_DISCONNECTION);
    delay_ms(500);
    Get_Estado_LUM(2);
    delay_ms(100);
    if (gDischargePowerOK == 1 && gLEDs_FULLPWR == 1 && GreenLED == 0 && RedLED == 0){
       Test_OK = 1;
       delay_min(6);
       output_low(POWER_DISCONNECTION);
       delay_ms(1000);
       Get_Estado_LUM(2);
       delay_ms(100);
       if(gGreenFlick_OK == 1 && RedLED == 0 && gLEDs_OFF == 1 && Bat_Pwr == 0){
           Test_OK = 1;
           delay_ms(100);
           for(i=0 ; i<=4 ; i++){
               delay_ms(10);
               output_low(SWITCH_CONTROL);
               delay_ms(500);
               output_high(SWITCH_CONTROL);
               delay_ms(500);
           }
           delay_ms(1000);
           output_high(POWER_DISCONNECTION);
           delay_ms(5000);
           //Lecturas
           Get_Estado_LUM(2);
           delay_ms(100);
           if (gDischargePowerOK == 1 && gLEDs_20PerPWR == 1 && GreenLED == 0 && RedLED == 0){
               Test_OK = 1;
               Error = 0;
           }else{
              Test_OK = 0;
              Error = 1;
              ErrorCode = 213; 
           }
       }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 212;
       }  
    }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 211;
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
    
}


/** 
  * @brief      battery disconnection using power switch 2.
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void BattDiscPwrSwitch2()
{
    /*Variables Locales*/
    int1 Error = 0;
    int1 Test_OK = 0;
    unsigned int8 ErrorCode = 000;
    int8 i;
    /*funcionalidad*/
    while(Read_V_Bat() < 12000){         //para que el test uncione correctamente la bateria debe estar en un minimo de carga
       delay_ms(100);
    }
    output_low(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_high(SWITCH_CONTROL);
    delay_ms(1000);
    output_high(POWER_DISCONNECTION);
    delay_ms(100);
    Get_Estado_LUM(2);
    delay_ms(100);
    if (gDischargePowerOK == 1 && gLEDs_FULLPWR && GreenLED == 0 && RedLED == 0){
       Test_OK = 1;
       Bat_Pwr = 0;
       delay_ms(1000);
       output_low(POWER_DISCONNECTION);
       delay_ms(1000);
       Get_Estado_LUM(2);
       delay_ms(100);
       if(Bat_Pwr == 0 && gLEDs_OFF == 1 && gGreenFlick_OK == 1 && RedLED == 0){
           Test_OK = 1;
           delay_ms(100);
           for(i=0 ; i<5 ; i++){
               delay_ms(10);
               output_low(SWITCH_CONTROL);
               delay_ms(500);
               output_high(SWITCH_CONTROL);
               delay_ms(500);
           }
           delay_ms(1000);
           output_high(POWER_DISCONNECTION);
           delay_ms(5000);
           //Lecturas
           Get_Estado_LUM(0);
           delay_ms(100);
           if (Bat_Pwr == 0 && gLEDs_OFF == 1 && GreenLED == 0 && RedLED == 0){
               Test_OK = 1;
               Error = 0;
           }else{
              Test_OK = 0;
              Error = 1;
              ErrorCode = 223; 
           }
       }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 222;
       }  
    }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 221;
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
   
}


/** 
  * @brief      battery disconnection using power switch 3.
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void BattDiscPwrSwitch3()
{
    /*Variables Locales*/
    int1 Error = 0;
    int1 Test_OK = 0;
    unsigned int8 ErrorCode = 000;
    int8 i;
    /*funcionalidad*/
    while(Read_V_Bat() < 12000){         //para que el test uncione correctamente la bateria debe estar en un minimo de carga
       delay_ms(100);
    }
    output_low(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_high(SWITCH_CONTROL);
    delay_ms(1000);
    output_high(POWER_DISCONNECTION);
    delay_ms(100);
    Get_Estado_LUM(2);
    delay_ms(100);
    if (gDischargePowerOK == 1 && gLEDs_20PErPWR == 1 && GreenLED == 0 && RedLED == 0){
       Test_OK = 1;
       delay_ms(3000);
       output_low(POWER_DISCONNECTION);
       //Lecturas
       delay_ms(2000);
       Get_Estado_LUM(2);
       delay_ms(100);
       if(Bat_Pwr == 0 && gLEDs_OFF == 1 && gGreenFlick_OK == 1 && RedLED == 0){
           Test_OK = 1;
           delay_ms(21000);
           for(i=0 ; i<5 ; i++){
               delay_ms(10);
               output_low(SWITCH_CONTROL);
               delay_ms(500);
               output_high(SWITCH_CONTROL);
               delay_ms(500);
           }
           output_high(POWER_DISCONNECTION);
           delay_ms(5000);
           //Lecturas
           Get_Estado_LUM(2);
           delay_ms(100);
           if (gDischargePowerOK == 1 && gLEDs_20PerPWR == 1 && GreenLED == 0 && RedLED == 0){
               Test_OK = 1;
               Error = 0;
           }else{
              Test_OK = 0;
              Error = 1;
              ErrorCode = 233; 
           }
       }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 232;
       }  
    }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 231;
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}

   /*--------------------------------------------------------------Continous charge -> intemitent charge -> Autonomy ---------------------------------------------------------*/
   
/**
  * @brief      Continous charge -> intermitent charge -> Autonomy
  * @param      none.
  * @retval      none.
  * @details   none
  */
void ContChargeInterChargeAutonomy()
{
    /*Variables Locales*/
    unsigned int8 i;
    int1 Test_OK;
    int1 Error;
    unsigned int8 ErrorCode = 0;
    int8 Cuenta = 0;
 
    /*Funcionalidad*/
    output_low(POWER_DISCONNECTION);        //Condiciones Iniciales
    output_low(BAT_DISC);
    output_high(SWITCH_CONTROL);
    delay_ms(30000);
    //Lecturas
    Get_Estado_LUM(1);
    delay_ms(100);
    if(gLEDs_OFF == 1 && gCharge_OK == 1 && gGreenFlick_OK== 1 && RedLED == 0){
        Test_OK = 1;
        delay_ms(10);
        for (i=0;i<110 ; i++){                      //Esperamos 18h y tomamos datos mientras tanto
            V_BAT_24h[i] = read_V_Bat();            //Es necesario que cargue al menos 18h
            delay_min(10);
        }
        //Lecturas
        IntermitentCharge();
        Get_Estado_LUM(0);
        delay_ms(100);
        if(gIntermitentChargeOK == 1 && GreenLED == 1 && RedLED == 0 && gLEDs_OFF == 1){
            Test_OK = 1;
            delay_ms(10);
            Reset_Tester();
            delay_ms(100);
            output_high(POWER_DISCONNECTION);
            delay_ms(2000);
            //Lecturas
            Get_Estado_LUM(2);
            delay_ms(100);
            if(gLEDs_20PerPWR == 1 && gDischargePowerOK == 1 && GreenLED == 0 && RedLED == 0){
                Test_OK = 1;
                delay_ms(100);
                while(Bat_Pwr!= 0){
                    Cuenta++;
                    Delay_min(1);
                    Bat_Pwr=BatteryDischargePower();
                }
                TimepoDescarga1 = Cuenta-1;
                Cuenta = 0;
                Get_Estado_LUM(0);
                delay_ms(100);
                if(TimepoDescarga1>=LIMITE_AUTONOMIA*0.95 && gLEDs_OFF == 1){
                    Test_OK = 1;
                    delay_ms(100);
                    output_low(POWER_DISCONNECTION);
                    delay_min(5);
                    output_high(POWER_DISCONNECTION);
                    delay_ms(100);
                  //Lecturas
                    Get_Estado_LUM(2);
                    delay_ms(100);
                    if(gLEDs_20PerPWR == 1 && gDischargePowerOK == 1 && GreenLED == 0 && RedLED == 0){
                        Test_OK = 1;
                        delay_ms(100);
                        while(Bat_Pwr!= 0){
                            Cuenta++;
                            Delay_min(1);
                            Bat_Pwr = BatteryDischargePower();
                        }
                        TiempoDescarga2 = Cuenta-1;
                        Cuenta = 0;
                        Get_Estado_LUM(0);
                        delay_ms(100);
                        if(gLEDs_OFF == 1 && TiempoDescarga2>=TIEMPO_DESCARGA_TOTAL*0.95 && TiempoDescarga2<= TIEMPO_DESCARGA_TOTAL*0.95 && gVbatOK == 1){
                            Test_OK = 1;
                            Error = 0;
                        }else{
                            Test_OK = 0;
                            Error = 1;
                            ErrorCode = 176;
                        }
                    }else{
                        Test_OK = 0;
                        Error = 1;
                        ErrorCode = 175;
                    }
                }else{
                    Test_OK = 0;
                    Error = 1;
                    ErrorCode = 174;
                }
            }else{
                Test_OK = 0;
                Error = 1;
                ErrorCode = 173;
            }
        }else{
            Test_OK = 0;
            Error = 1;
            ErrorCode = 172;
        }
    }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 171;
    }
    /*-----------------------------------------ZONA BORRABLE---------------------------------------------*/
    /*Test_OK = 1;
    Error = 0;
    V_BAT_24h[0]=10500;
    for(i=0;i<109;i++){
        V_BAT_24h[i+1]=V_BAT_24h[i]+10;
    }*/
    /*------------------------------------------------------------------------------------------------------*/
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
   gTestContChargeRealizado = 1;
}

   /*-----------------------------------------------------Deep Discharge Battery: Continous charge -> intemitent charge -> Autonomy--------------------------------------------*/

/** 
  * @brief      Deep Discharge Battery: Continous charge -> intemitent charge -> Autonomy.
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void DeepChargeBattery()        //Funcionalidad comprobada con tiempos menores desconectando la bateria
{
    /*Variables Locales*/
    int1 Test_OK =0;
    int1 Error = 0;
    unsigned int8 ErrorCode = 0;
    int8 Tiempo_min = 0;
    
    /*Fucionalidad*/
    Descarga_Bat();                     //Nos aseguramos de que la bateria esta descargada. si no lo esta se descarga
    output_low(POWER_DISCONNECTION);
    output_low(SWITCH_CONTROL);
    output_low(BAT_DISC);
    delay_ms(2000);
    Get_Estado_LUM(1);
    delay_ms(100);
    if(gCharge_OK == 1 && gLEDs_FULLPWR == 1 && RedLED == 0 && gGreenFlick_OK == 1){
        Test_OK = 1;
        delay_ms(10);
        for(i=0;i<73;i++){      //Tomamos una muestra cada 15 min durante 18h
            GrafCargaBat[i] = Read_V_Bat();
            delay_min(15);
        }
        output_high(POWER_DISCONNECTION);
        delay_ms(3000);
        Get_Estado_LUM(2);
        delay_ms(100);
        if(gLEDs_20PerPWR == 1 && GreenLED == 0 && RedLED == 0 && gDischargePowerOK == 1){
            Test_OK = 1;
            i = 0;
            delay_ms(10);
            while(Read_I_LED() > 10){
                GrafDescargaBat[i] = Read_V_Bat();
                delay_min(1);
                i++;
            }
            Tiempo_min = i;
            Get_Estado_LUM(0);
            delay_ms(100);
            if(gLEDs_OFF == 1 && Tiempo_min >= LIMITE_AUTONOMIA*0.95){
                Test_OK = 1;
                Error = 0;
            }else{
                Test_OK = 0;
                Error = 1;
                ErrorCode = 183;
            }
        }else{
           Test_OK = 0;
           Error = 1;
           ErrorCode = 182; 
        }     
    }else{
       Test_OK = 0;
       Error = 1;
       ErrorCode = 181; 
    }
    
    /*----------------------------------CODIGO DE PRUEBAS-----------------------------------------------*/
//    GrafCargaBat[0]=10500;
//    for(i=1;i<73;i++){
//        GrafCargaBat[i] = GrafCargaBat[i-1]+10*i;
//    }
//    i=0;
//    GrafDescargaBat[0] = GrafCargaBat[72];
//   for(i=1;i<120;i++){
//        GrafDescargaBat[i] = GrafDescargaBat[i-1]-100*i;
//    }
//    Test_OK = 1;
//    Error = 0;
  /*-----------------------------------------------------------------------------------------------------*/
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
   gTestDeepDischargeRealizado = 1;
}

   /*-------------------------------------------------------------------------Autonomy auto-test------------------------------------------------------------------------------*/

/** 
  * @brief      Autonomy Auto-test
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void AutonomyAutoTest()
{
    /*Variables Locales*/
    int1 Test_OK = 0;
    int1 Error = 0;
    unsigned int8 ErrorCode = 0;
    
    /*Funcionalidad*/
    //Estado Iicial
    output_low(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_low(SWITCH_CONTROL);
    
    delay_min(180);         //Carga de 3h
    delay_min(5);
    Get_Estado_LUM(0);
    delay_ms(100);
    if(gGreenFlick_OK == 1 && gLEDs_FULLPWR && RedLED == 0){
        Test_OK = 1;
        delay_ms(100);
        output_high(POWER_DISCONNECTION);
        delay_ms(500);
        Get_Estado_LUM(0);
        delay_ms(100);
        if(GreenLED == 0 && RedLED == 0 && gLEDs_20PerPWR == 1){
            Test_OK = 1;
            delay_min(5);
            output_high(BAT_DISC);
            delay_ms(500);
            Get_Estado_LUM(0);
            while(ILD>10){
                delay_ms(100);
                ILD = Read_I_LED();
            }
            delay_ms(100);
            output_low(POWER_DISCONNECTION);
            output_low(BAT_DISC);
            delay_ms(100);
            Get_Estado_LUM(0);
            delay_ms(100);
            if(RedLED == 0 && gGreenFlick_OK == 1 && gLEDs_FULLPWR == 1){
                    Test_OK = 1;
                    for(i = 0; i<73;i++){          //Charge for 18h
                        delay_min(15);
                    }
                    Get_Estado_LUM(0);
                    delay_ms(100);
                    if(GreenLED == 1 && RedLED == 0){//Tras 18h de carga LED verde encendido
                        Test_OK = 1;
                        delay_ms(100);
                        output_high(POWER_DISCONNECTION);
                        Delay_ms(100);
                        Get_Estado_LUM(0);
                        delay_ms(100); 
                        if(GreenLED == 0 && RedLED == 0 && gLEDs_20PerPWR == 1){
                            delay_min(DESCARGA_PARCIAL_1); //Depende del tipo de luminaria
                            output_high(BAT_DISC);
                            delay_ms(1000);
                            output_low(BAT_DISC);
                            output_low(POWER_DISCONNECTION);
                            delay_ms(1000);
                            Get_Estado_LUM(0);
                            delay_ms(100);
                            if(gGreenFlick_OK == 1 && RedLED == 0 && gLEDs_FULLPWR == 1){
                                Test_OK = 1;
                                delay_ms(10);
                                for(i = 0; i<73;i++){          //Esperar 18h
                                    delay_min(15);
                                }
                                Get_Estado_LUM(0);
                                delay_ms(100);
                                if(GreenLED == 1 && RedLED == 0){
                                    Test_OK = 1;
                                    delay_ms(10);
                                    output_high(POWER_DISCONNECTION);
                                    delay_ms(2000);
                                    Get_Estado_LUM(0);
                                    delay_ms(100);
                                    if(gLEDs_20PerPWR == 1 && GreenLED == 0 && RedLED == 0){
                                        Test_OK = 1;
                                        delay_ms(10);
                                        delay_min(DESCARGA_PARCIAL_2);
                                        output_high(BAT_DISC);
                                        delay_ms(100);
                                        output_low(POWER_DISCONNECTION);
                                        output_low(BAT_DISC);
                                        delay_ms(10);
                                        Get_Estado_LUM(0);
                                        delay_ms(100);
                                        if(gLEDs_FULLPWR == 1 && gGreenFlick_OK == 1 && RedLED == 0)
                                        {
                                            Test_OK = 1;
                                            Error = 0;
                                            ErrorCode = 0;
                                        }else{
                                            Test_OK = 0;
                                            Error = 1;
                                            ErrorCode = 199;
                                        }
                                    }else{
                                       Test_OK = 0;
                                       Error = 1;
                                       ErrorCode = 198; 
                                    }
                                }else{
                                   Test_OK = 0;
                                   Error = 1;
                                   ErrorCode = 197; 
                                }   
                            }else{
                                Test_OK = 0;
                                Error = 1;
                                ErrorCode = 196;
                            }
                        }else{
                            Test_OK = 0;
                            Error = 1;
                            ErrorCode = 195;
                        }
                    }else{
                        Test_OK = 0;
                        Error = 1;
                        ErrorCode = 194;
                    }
                }else{
                    Test_OK = 0;
                    Error = 1;
                    ErrorCode = 193;
                }
        }else{
            Test_OK = 0;
            Error = 1;
            ErrorCode = 192;
        }
    }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 191;
    }
    /*--------------------------------------------CODIGO DE PRUEBAS------------------------------------------------------------------------------------------------------------------------*/
//   Test_OK = 1;
//   Error = 0;
//    
    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
}

   /*------------------------------------------------------------------Red LED testing using test button----------------------------------------------------------------------*/
#if defined __COMPILA_RED_LED_RESET_TEST
/** 
  * @brief      Red LED testing using test puch button.
  * @param      none.
  * @retval      none.
  * @details    none.
  */
void RedLEDResetWithPshBtn()
{
    /*Variables Locales*/
    int1 Test_OK = 0;
    int1 Error = 0;
    unsigned int8 ErrorCode = 000;
    /*Functionality*/
    output_low(POWER_DISCONNECTION);
    output_low(SWITCH_CONTROL);
    output_low(BAT_DISC);
    delay_ms(5000);
    Get_Estado_LUM(0);
    delay_ms(100);
    if(RedLED == 0/* && gGreenFlick_OK == 1*/){
        Test_OK = 1;
        delay_ms(10);
        output_high(POWER_DISCONNECTION);
        delay_ms(1000);
        Get_Estado_LUM(0);
        delay_ms(100);
        if(RedLED == 0 && GreenLED == 0 && glEDs_20PerPWR == 1 ){
            Test_OK = 1;
            delay_min(5);
            output_high(BAT_DISC);
            delay_ms(500);
            Get_Estado_LUM(0);
            delay_ms(100);
            if(gLEDs_OFF == 1){
                delay_ms(10);
                output_low(POWER_DISCONNECTION);
                output_low(BAT_DISC);
                delay_ms(500);
                Get_Estado_LUM(0);
                delay_ms(100);
                if(RedLED == 0 && gGreenFlick_OK == 1 && gLEDs_FULLPWR == 1){
                    Test_OK = 1;
                    delay_ms(100);
                    for(i=0;i<=4;i++){
                        output_low(TEST_BUTTON);
                        delay_ms(100);
                        output_high(TEST_BUTTON);
                        delay_ms(2500);
                    }
                    Get_Estado_LUM(0);
                    delay_ms(100);
                    if(RedLED == 0 && gGreenFlick_OK == 1 && gLEDs_FULLPWR == 1){
                        Test_OK = 1;
                        i=0;
                        delay_ms(100);
                        for(i=0;i<=5;i++){
                            output_low(TEST_BUTTON);
                            delay_ms(500);
                            output_high(TEST_BUTTON);
                            delay_ms(500);
                        }
                        delay_ms(100);
                        Get_Estado_LUM(0);
                        delay_ms(100);
                        if(gGreenFlick_OK == 1 && gRedFlick_OK==1){
                            Test_OK = 1;
                            delay_ms(10000);
                            Get_estado_LUM(0);
                            delay_ms(100);
                            if(gGreenFlick_OK==1 && RedLED == 0){
                                Test_OK = 1;
                                i=0;
                                delay_ms(10);
                                for(i=0;i<=4;i++){
                                    output_low(TEST_BUTTON);
                                    delay_ms(200);
                                    output_high(TEST_BUTTON);
                                    delay_ms(500);
                                }
                                Get_Estado_LUM(0);
                                delay_ms(100);
                                if(gGreenFlick_OK == 1 && gRedFlick_OK==1){
                                    Test_OK = 1;
                                    output_high(TEST_BUTTON);
                                    delay_ms(2000);
                                    output_low(TEST_BUTTON);
                                    Get_Estado_LUM(0);
                                    delay_ms(100);
                                    if(gRedFlick_OK==1 && GreenLED == 1){
                                        Test_OK = 1;
                                        delay_ms(8000);
                                        Get_Estado_LUM(0);
                                        delay_ms(100);
                                        if(gGreenFlick_OK == 1 && RedLED == 0){
                                            Test_OK = 1;
                                            delay_ms(10);
                                            for(i=0;i<5;i++){
                                                output_low(TEST_BUTTON);
                                                delay_ms(100);
                                                output_high(TEST_BUTTON);
                                                delay_ms(1000);
                                            }
                                            delay_ms(10);
                                            Get_Estado_LUM(0);
                                            delay_ms(100);
                                            if(gGreenFlick_OK == 1 && gRedFlick_OK == 1){
                                                Test_OK = 1;
                                                delay_ms(100);
                                                output_high(TEST_BUTTON);
                                                delay_ms(3500);
                                                output_low(TEST_BUTTON);
                                                Get_Estado_LUM(0);
                                                delay_ms(100);
                                                if(RedLED==0 && gGreenFlick_OK == 1){
                                                    Test_OK = 1;
                                                    Error = 0;
                                                }else{
                                                    Test_OK = 0;
                                                    Error = 1;
                                                    ErrorCode = 209;
                                                }
                                            }else{
                                                Test_OK = 0;
                                                Error = 1;
                                                ErrorCode = 208;
                                            }
                                        }else{
                                            Test_OK = 0;
                                            Error = 1;
                                            ErrorCode = 206;
                                        }
                                    }else{
                                       Test_OK = 0;
                                       Error = 1;
                                       ErrorCode = 207; 
                                    }
                                }else{
                                   Test_OK = 0;
                                   Error = 1;
                                   ErrorCode = 205; 
                                }
                            }else{
                               Test_OK = 0;
                               Error = 1;
                               ErrorCode = 206; 
                            }
                        }else{
                          Test_OK = 0;
                          Error = 1;
                          ErrorCode = 205;  
                        }
                    }else{
                        Test_OK = 0;
                        Error = 1;
                        ErrorCode = 204;
                    }
                }else{
                    Test_OK = 0;
                    Error = 1;
                    ErrorCode = 203;
                }  
            }
        }else{
            Test_OK = 0;
            Error = 1;
            ErrorCode = 202;
        }
    }else{
        Test_OK = 0;
        Error = 1;
        ErrorCode = 201;
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   } 
}
#endif
   /*-----------------------------------------------------------------------Temperature Derating------------------------------------------------------------------------------*/
   
/** 
  * @brief     Temperature derrating
  * @param      none.
  * @retval      none.
  * @details   none
  */
void TemperatureDerating()
{
    /*Variables Locales*/
    int8 Error = 0;
    int8 Test_OK = 0;
    unsigned int8 ErrorCode = 000;
    int16 ILD_anterior = 0;
    /*Funcionalidad*/
    output_low(POWER_DISCONNECTION);
    output_low(SWITCH_CONTROL);
    output_low(BAT_DISC);
    Set_T_Amb(TEMP_NO_DERR);
    delay_ms(2000);
    Get_Estado_LUM(1);
    delay_ms(100);
    ILD_Anterior = ILD;
    if(gLEDs_FULLPWR == 1 && gGreenFlick_OK == 1 && RedLED == 0 && gCharge_OK == 1){
        Test_OK = 1;
        Temp[0][0] = TEMP_NO_DERR;
        Temp[0][1] = ILD;
        delay_ms(10);
        Set_T_Amb(TEMP_DERR);
        delay_ms(3000);
        Get_Estado_LUM(1);
        ILD = Read_I_LED_DERR();
        if(ILD<ILD_Anterior && gGreenFlick_OK == 1 && gCharge_OK && RedLED == 0){
            Test_OK = 1;
            Temp[1][0] = TEMP_DERR;
            Temp[1][1] = ILD;
        }else{
            Error = 1;
            Test_OK = 0;
            ErrorCode = 242;
        }
    }else{
        Error = 1;
        Test_OK = 0;
        ErrorCode = 241;
    }
   Set_T_Amb(TEMP_TRABAJO);
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
   gTestTempDerrRealizado = 1;
}

#if defined __COMPILA_HEATER
   /*-------------------------------------------------------------------------Heater Testing------------------------------------------------------------------------------------*/
/** 
  * @brief      Heater testing
  * @param      none.
  * @retval      none.
  * @details   WDT, IO_pins, Timer 2, Timer 4, ADC, Prueba vida batt.
  */
void HTRTesting()
{
/*Variables Globales*/
    int8 Test_OK = 0;
    int8 Error = 0;
    unsigned int8 ErrorCode = 0;
    /*Funcionalidad*/
    output_low(POWER_DISCONNECTION);
    output_low(SWITCH_CONTROL);
    output_low(BAT_DISC);
//    Set_T_Amb(TEMP_TRABAJO);
    delay_ms(1000);
    Get_Estado_LUM(1);
    delay_ms(100);
    if(gLEDs_FULLPWR == 1 && gCharge_OK == 1 && RedLED == 0 && gGreenFlick_OK == 1 && gHTR_OFF == 1){
        Test_OK = 1;
        delay_ms(5000);
        Set_T_HTR(TEMP_HTR_BAJA);
        delay_ms(1000);
        Get_Estado_LUM(1);
        delay_ms(100);
        if(gLEDs_FULLPWR == 1 && gHTR_OFF == 1 && gGreenFlick_OK == 1 && gCharge_OK && RedLED == 0){
            Test_OK = 1;
            delay_ms(100);
            output_high(SWITCH_CONTROL);
            delay_ms(3000);
            Get_Estado_LUM(1);
            delay_ms(100);
            if(gLEDs_OFF == 1 && gHTR_ON == 1 && gHTRPWR_OK == 1 && gCharge_OK == 1 && RedLED == 0 && gGreenFlick_OK == 1){
                Test_OK = 1;
                delay_ms(100);
                Set_T_HTR(TEMP_HTR_ALTA);
                delay_ms(1000);
                Get_Estado_LUM(1);
                delay_ms(100);
                if(gLEDs_OFF == 1 && gHTR_OFF == 1 && gGreenFlick_OK == 1 && gCharge_OK == 1 && RedLED == 0){
                    Test_OK = 1;
                    delay_ms(10);
                    Set_T_HTR(TEMP_HTR_BAJA);
                    delay_ms(1000);
                    Get_Estado_LUM(1);
                    delay_ms(100);
                    if(gLEDs_OFF == 1 && gHTR_ON == 1 && gHTRPWR_OK == 1 && gCharge_OK == 1 && RedLED == 0 && gGreenFlick_OK == 1){
                        Test_OK = 1;
                        delay_ms(10);
                        output_low(SWITCH_CONTROL);
                        delay_ms(1000);
                        Get_Estado_LUM(1);
                        delay_ms(100);
                        if(gLEDs_FULLPWR == 1 && gHTR_OFF == 1 && gGreenFlick_OK == 1 && gCharge_OK == 1 && RedLED == 0){
                            Test_OK = 1;
                            delay_ms(10);
                            output_high(SWITCH_CONTROL);
                            delay_ms(1000);
                            Get_Estado_LUM(1);
                            delay_ms(100);
                            if(gLEDs_OFF == 1 && gHTR_ON == 1 && gHTRPWR_OK == 1 && gCharge_OK == 1 && RedLED == 0 && gGreenFlick_OK == 1){
                                Test_OK = 1;
                                delay_ms(10);
                                Set_T_HTR(TEMP_HTR_ALTA);
                                delay_ms(1000);
                                Get_Estado_LUM(1);
                                delay_ms(100);
                                if(gLEDs_OFF == 1 && gHTR_OFF == 1 && gGreenFlick_OK == 1 && gCharge_OK == 1 && RedLED == 0){
                                    Test_OK = 1;
                                    delay_ms(10);
                                    Set_T_HTR(TEMP_HTR_BAJA);
                                    Set_T_Amb(TEMP_TEST_HTR);
                                    delay_ms(1000);
                                    Get_Estado_LUM(1);
                                    delay_ms(100);
                                    if(gLEDs_OFF == 1 && gHTR_OFF == 1 && gGreenFlick_OK == 1 && gCharge_OK == 1 && RedLED == 0){
                                        Test_OK = 1;
                                        delay_ms(10);
                                        Set_T_HTR(TEMP_HTR_BAJA);
                                        Set_T_Amb(TEMP_TRABAJO);
                                        delay_ms(1000);
                                        Get_Estado_LUM(1);
                                        delay_ms(100);
                                        if(gLEDs_OFF == 1 && gHTR_ON == 1 && gHTRPWR_OK == 1 && gCharge_OK == 1 && RedLED == 0 && gGreenFlick_OK == 1){
                                            Test_OK = 1;
                                            Error = 0;
                                        }else{
                                            Error = 1;
                                            Test_OK = 0;
                                            ErrorCode = 250;  
                                        }
                                    }else{
                                        Error = 1;
                                        Test_OK = 0;
                                        ErrorCode = 259;  
                                    }
                                }else{
                                    Error = 1;
                                    Test_OK = 0;
                                    ErrorCode = 258; 
                                }
                            }else{
                                Error = 1;
                                Test_OK = 0;
                                ErrorCode = 257;  
                            }
                        }else{
                            Error = 1;
                            Test_OK = 0;
                            ErrorCode = 256; 
                        }
                    }else{
                        Error = 1;
                        Test_OK = 0;
                        ErrorCode = 255;  
                    }
                }else{
                    Error = 1;
                    Test_OK = 0;
                    ErrorCode = 254; 
                }
            }else{
                Error = 1;
                Test_OK = 0;
                ErrorCode = 253; 
            }   
        }else{
            Error = 1;
            Test_OK = 0;
            ErrorCode = 252;    
        }
    }else{
        Error = 1;
        Test_OK = 0;
        ErrorCode = 251;  
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
   Set_T_HTR(TEMP_HTR_TRABAJO);
}
#endif


#if defined __COMPILA_SENSOR_UWAVE_LUZ
   /*------------------------------------------------------------------Motion And Light Sensor----------------------------------------------------------------------------------*/
/** 
  * @brief      Inicializacion de los perifericos del microcontrolador.
  * @param      none.
  * @retval      none.
  * @details   WDT, IO_pins, Timer 2, Timer 4, ADC, Prueba vida batt.
  */
void MotionLightSensor()
{
    /*Variables Locales*/
    int8 Test_OK = 0;
    int8 Error = 0;
    unsigned int8 ErrorCode = 0;
    
    /*Funcionalidad*/
    output_high(LIGHT_SENSOR);
    Activar_uWave(2);
    output_high(POWER_DISCONNECTION);
    output_low(BAT_DISC);
    output_low(SWITCH_CONTROL);
//    delay_min(5);
    delay_ms(100);
    output_low(POWER_DISCONNECTION);
    delay_ms(1000);
    Get_Estado_LUM(0);
    delay_ms(100);
    if(gLEDs_FULLPWR == 1 && gGreenFlick_OK == 1 && RedLED == 0){
        Test_OK = 1;
        delay_ms(100);
        output_high(LIGHT_SENSOR);
        Activar_uWave(0);
        delay_min(5);
        Get_Estado_LUM(0);
        delay_ms(100);
        if(gLEDs_OFF == 1 && gGreenFlick_OK == 1 && RedLED == 0){
            Test_OK = 1;
            delay_ms(10);
            Activar_uWave(1);
            output_high(LIGHT_SENSOR);              //detecta presencia, pero hay luz
            delay_ms(100);
            Get_Estado_LUM(0);
            delay_ms(100);
            if(gLEDs_OFF == 1 && gGreenFlick_OK == 1 && RedLED == 1){
                Test_OK = 1;
                Activar_uWave(0);
                delay_ms(10);
                output_low(LIGHT_SENSOR);               //no detecta luz, y detecta presencia
                delay_ms(10);
                Activar_uWave(1);
                delay_ms(1000);
                Get_Estado_LUM(0);
                delay_ms(100);
                if(gLEDs_FULLPWR == 1 && gGreenFlick_OK == 1 && RedLED == 0){
                    Test_OK = 1;
                    Activar_uWave(0);
                    delay_ms(100);
                    output_high(LIGHT_SENSOR);
                    delay_min(5);
                    Get_Estado_LUM(0);
                    delay_ms(100);
                    if(gLEDs_OFF == 1 && gGreenFlick_OK == 1 && RedLED == 0){
                        Test_OK = 1;
                        Error = 0;
                    }else{
                        Error = 1;
                        Test_OK = 0;
                        ErrorCode = 285;  
                    }
                }else{
                  Error = 1;
                  Test_OK = 0;
                  ErrorCode = 284;   
                }
            }else{
               Error = 1;
               Test_OK = 0;
               ErrorCode = 283;  
            }
        }else{
           Error = 1;
           Test_OK = 0;
           ErrorCode = 282;  
        }
    }else{
       Error = 1;
       Test_OK = 0;
       ErrorCode = 281;  
    }
   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
   if (ErrorCode != 0){
   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
   }
   Activar_uWave(2);
   output_high(LIGHT_SENSOR);
}

#endif

#if defined __COMPILA_HEATER
   /*----------------------------------------------------------------------Error in HTR power-------------------------------------------------------------------------------------*/
/** 
  * @brief      Inicializacion de los perifericos del microcontrolador.
  * @param      none.
  * @retval      none.
  * @details   WDT, IO_pins, Timer 2, Timer 4, ADC, Prueba vida batt.
  */
//void HTRPowErr()
//{
///*Variables Locales*/
//    int8 Test_OK = 0;
//    int8 Error = 0;
//    unsigned int8 ErrorCode = 0;
//    
//    /*Funcionalidad*/
//    output_low(POWER_DISCONNECTION);
//    output_low(BAT_DISC);
//    output_high(SWITCH_CONTROL);
//    delay_ms(100);
//    Set_T_HTR(5);
//    delay_ms(10000);
//    Get_Estado_LUM(1);
//    if(gLEDs_OFF==1 && gHTR_ON == 1 && gGreenFlick_OK == 1 && RedLED == 0 && gCharge_OK == 1 && gHTRPWR_OK==1){
//        Test_OK = 1;
//        delay_ms(10);
//        output_high( Q12_ERROR_ENABLE);
//        delay_ms(1000);
//        Get_Estado_LUM(0);
//        delay_ms(100);
//        if(gLEDs_OFF == 1 && gHTR_OFF == 1 && VPS == 0 && RedLED == 1 && GreenLED == 0){
//            Test_OK = 1;
//            delay_ms(10);
//            output_low( Q12_ERROR_ENABLE);
//            output_high(POWER_DISCONNECTION);
//            delay_ms(1000);
//            Get_Estado_LUM(0);
//            if(gLEDs_OFF == 1 && RedLED == 1 && GreenLED == 0 ){
//                Test_OK = 1;
//                delay_ms(10);
//                output_low(POWER_DISCONNECTION);
//                delay_ms(1000);
//                Get_Estado_LUM(0);
//                delay_ms(100);
//                if(gLEDs_OFF == 1 && RedLED == 0 && gGreenFlick_OK == 1){
//                    Test_OK = 1;
//                    Error = 0;
//                }else{
//                    Error = 1;
//                    Test_OK = 0;
//                    ErrorCode = 274;
//                }
//            }else{
//                Error = 1;
//                Test_OK = 0;
//                ErrorCode = 273;
//            }
//        }else{
//            Error = 1;
//            Test_OK = 0;
//            ErrorCode = 272;
//        }
//    }else{
//        Error = 1;
//        Test_OK = 0;
//        ErrorCode = 271;
//    }
//   gTest_OK += Test_OK;                      //Almacenamos el estado del test en la variable global
//   gTest_ERROR += Error;                     //Almacenamos el estado del test en la variabke global
//   if (ErrorCode != 0){
//   ErrorCodes[ErrorCont] = ErrorCode;        //Almacenamos el codigo del error en el vector de errores
//   ErrorCont++;                              //Sumamos 1 al contador de errores para que el siguiente error se coloque en el siguiente hueco.
//   }
//}

#endif

#endif	/* TESTS_H */

