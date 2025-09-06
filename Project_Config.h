/* 
 * File:   Project_Config.h
 * Author: ybustillo
 *
 * Created on 3 de abril de 2024, 8:04
 */

#ifndef PROJECT_CONFIG_H
#define	PROJECT_CONFIG_H

#ifdef __COMPILA_PRUEBAS

#define __COMPILA_HEATER
//#define __COMPILA_SENSOR_UWAVE_LUZ
#define __COMPILA_10_CELDAS
#define __COMPILA_TEMP_DERRATING
#define __COMPILA_EXTERNAL_DAC
#define __COMPILA_90_MINUTOS
//#define __COMPILA_RED_LED_RESET_TEST
#else

#endif


#ifdef __COMPILA_F_2K70_K29_PS
#define __COMPILA_HEATER
#define __COMPILA_90_MINUTOS
#define __COMPILA_SENSOR_UWAVE_LUZ
#define __COMPILA_20_POR_CIENTO
#define __COMPILA_10_CELDAS
#define __COMPILA_TEMP_DERRATING
#define __COMPILA_2K70
#define __COMPILA_RED_LED_RESET_TEST
#endif 

#ifdef __COMPILA_F_2K70_K29
#define __COMPILA_HEATER
#define __COMPILA_90_MINUTOS
#define __COMPILA_SENSOR_UWAVE_LUZ
#define __COMPILA_20_POR_CIENTO
#define __COMPILA_10_CELDAS
#define __COMPILA_TEMP_DERRATING
#define __COMPILA_2K70
#define __COMPILA_RED_LED_RESET_TEST
#endif 

#ifdef __COMPILA_D_2K70_K29
#define __COMPILA_HEATER
#define __COMPILA_90_MINUTOS
#define __COMPILA_SENSOR_UWAVE_LUZ
#define __COMPILA_20_POR_CIENTO
#define __COMPILA_10_CELDAS
#define __COMPILA_TEMP_DERRATING
#define __COMPILA_2K70
#define __COMPILA_RED_LED_RESET_TEST
#endif 

#ifdef __COMPILA_K_1K00_K109
#define __COMPILA_HEATER
#define __COMPILA_90_MINUTOS
#define __COMPILA_SENSOR_UWAVE_LUZ
#define __COMPILA_10_CELDAS
#define __COMPILA_TEMP_DERRATING
#define __COMPILA_1K00
#define __COMPILA_RED_LED_RESET_TEST
#endif 

#ifdef __COMPILA_K_2K70_K29
#define __COMPILA_HEATER
#define __COMPILA_90_MINUTOS
#define __COMPILA_SENSOR_UWAVE_LUZ
#define __COMPILA_20_POR_CIENTO
#define __COMPILA_10_CELDAS
#define __COMPILA_TEMP_DERRATING
#define __COMPILA_2K70
#define __COMPILA_RED_LED_RESET_TEST
#endif 

#ifdef __COMPILA_D_2K70_Z26
#define __COMPILA_HEATER
#define __COMPILA_60_MINUTOS
#define __COMPILA_SENSOR_UWAVE_LUZ
#define __COMPILA_20_POR_CIENTO
#define __COMPILA_10_CELDAS
#define __COMPILA_TEMP_DERRATING
#define __COMPILA_2K70
#define __COMPILA_RED_LED_RESET_TEST
#endif 

/*----------------------------AUTONOMIA BATERIA-----------------------------------------*/
#if defined __COMPILA_90_MINUTOS
//esperando 90min
#define LIMITE_AUTONOMIA            94
//Esperando 75 min:
#define AUTONOMIA_MINIMA1           75
//Esperando 85min:
#define AUTONOMIA_MINIMA2           85
//Total Discharge Time
#define TIEMPO_DESCARGA_TOTAL       53
//Descarga parcial
#define DESCARGA_PARCIAL_1          70
#define DESCARGA_PARCIAL_2          80
#endif

#if defined __COMPILA_60_MINUTOS
//esperando 90min
#define LIMITE_AUTONOMIA            64
//Esperando 75 min:
#define AUTONOMIA_MINIMA1           50
//Esperando 85min:
#define AUTONOMIA_MINIMA2           55
//Descarga parcial
#define DESCARGA_PARCIAL_1          45
#define DESCARGA_PARCIAL_2          55
#endif

/*--------------------------------TENSIONES BATERIA------------------------------------------*/

#if defined __COMPILA_10_CELDAS
#define V_LIM_BAT_DESCARGADA        10500
#define V_LIM_BAT_DESCARGADA_B      10000                           //Por debajo de esta tension se considera descargado
#define V_LIM_BAT_DESCARGADA_A      11000                           //Por debajo de esta tension se considera descargado
#define V_MAXIMA_BAT                13800                           //V maxima Bateria a Baja temperatura
#define W_DESCARGA_BAT              3.9         //W     20%err        //Potencia de descarga de las baterias a lo bajo   Valor esperado 3.9W

#endif

#if defined __COMPILA_9_CELDAS
#define V_LIM_BAT_DESCARGADA_A      10000                           //Por debajo de esta tension se considera descargado
#define V_LIM_BAT_DESCARGADA        9600
#define V_LIM_BAT_DESCARGADA_B      9200                            //Por debajo de esta tension se considera descargado
#define V_MAXIMA_BAT                12500                           //V maxima Bateria a Baja temperatura
#define W_DESCARGA_BAT              3.9         //W     20%err        //Potencia de descarga de las baterias a lo bajo   Valor esperado 3.9W
#endif

/*---------------------------------POTENCIA LEDs EN EMERGENCIA-------------------------------------------*/
#if defined __COMPILA_20_POR_CIENTO
#define I_LEDS_EMERGENCIA           I_LED_FP*0.2     //mA           //corriente de los LEDs a baja potencia a lo bajo
#else
#define I_LEDS_EMERGENCIA           110              //mA           //corriente de los LEDs a baja potencia a lo bajo
#endif

#if defined __COMPILA_TEMP_DERRATING
#define TEMP_DERR                   79                              //Temperatura a la que empieza el derrating
#define TEMP_NO_DERR                48                              //Temperatura a la que no hay derrating
#define TEMP_TEST_HTR               40                              //Temperatura para el test de t_HTR
#define TEMP_TRABAJO                30                              //Temperatura habitual de trabajo
#endif

#if defined __COMPILA_HEATER
#define TEMP_HTR_BAJA               0                               //Temperatura para activar HTR
#define TEMP_HTR_ALTA               40                              //Temperatura para apagar HTR
#define TEMP_HTR_TRABAJO            25
#endif

#if defined __COMPILA_SENSOR_UWAVE_LUZ
#define V_DETECCION_UWAVE           4.5                             //tension a partir de la cual el sensor uWave se considera encendido
#define V_DETECCION_LUMINOSIDAD     0.35                            //Tension a partir de la cual la el sensor de luz esta captando luz
#endif

/*CONSTANTES GENERALES*/
#define I_CARGA_BAT             60         //mA    10%err        //corriente de carga de baterias a lo bajo  Valor esperado 37mA
#define I_LED_FP                110          //mA                  //corriente de los LEDs a max potencia a lo bajo
#define V_POWERSOURCE           24000        //mV
#define V_DC_BUS                24000 
#endif	/* PROJECT_CONFIG_H */
