/**************************************************************************************************
 * Copyright (c) 2014 TTControl. All rights reserved. Confidential proprietary
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 **************************************************************************************************
 *
 * Name
 *    main.c
 *
 * Purpose
 *   Template for (safety) application development
 *
 * Revisions
 *
 *    15-Dec-2014  (MSI) Creation
 *   ��revision-date��
 *************************************************************************************************/

/**************************************************************************************************
 * Includes
 *************************************************************************************************/
#include "APDB.h"
#include "IO_Driver.h"
#include "IO_RTC.h"
#include "IO_DIO.h"
#include "IO_CAN.h"
#include "IO_ADC.h"
#include "IO_POWER.h"
#include "ptypes_apdb.h"
#include "ptypes_tms570.h"

#include "PID.h"

/**************************************************************************************************
 * Defines
 *************************************************************************************************/
/* modify to adjust application version */
#define MAJOR_NUMBER            1
#define MINOR_NUMBER            0
#define REVISION_NUMBER         0

/**************************************************************************************************
 * Static data
 *************************************************************************************************/

/**************************************************************************************************
 * Application Database,
 * needed for TTC-Downloader
 *************************************************************************************************/
#pragma SET_DATA_SECTION (".APDB_SEC")
volatile const BL_APDB Apdb_t =
{
    APDB_VERSION,           /* APDB version                                        */
    {0},                    /* Flash date (provided by TTC-Downloader)             */
                            /* Build date                                          */
    {((((RTS_TTC_FLASH_DATE_YEAR)   & 0x0FFF) <<  0) |
      (((RTS_TTC_FLASH_DATE_MONTH)  & 0x0F  ) << 12) |
      (((RTS_TTC_FLASH_DATE_DAY)    & 0x1F  ) << 16) |
      (((RTS_TTC_FLASH_DATE_HOUR)   & 0x1F  ) << 21) |
      (((RTS_TTC_FLASH_DATE_MINUTE) & 0x3F  ) << 26))},
    0,                      /* Node type                                           */
    0,                      /* CRC start address (provided by TTC-Downloader)      */
    0,                      /* code size (provided by TTC-Downloader)              */
    0,                      /* Legacy application CRC (provided by TTC-Downloader) */
    0,                      /* Application CRC (provided by TTC-Downloader)        */
    1,                      /* Node number                                         */
    0,                      /* CRC seed (provided by TTC-Downloader)               */
    0,                      /* Flags                                               */
    0,                      /* Hook 1                                              */
    0,                      /* Hook 2                                              */
    0,                      /* Hook 3                                              */
    APPL_START,             /* Main address, i.e., application entry point         */
    {0, 1},                 /* CAN download ID (standard format, ID 0x1)           */
    {0, 2},                 /* CAN upload ID (standard format, ID 0x2)             */
    0,                      /* Legacy header CRC (provided by TTC-Downloader)      */
                            /* Application version (major.minor.revision)          */
    ((((ubyte4)REVISION_NUMBER) <<  0) |
     (((ubyte4)   MINOR_NUMBER) << 16) |
     (((ubyte4)   MAJOR_NUMBER) << 24)),
    250,                    /* CAN baud rate in kbps                               */
    0,                      /* CAN channel                                         */
    0,                      /* Password (disable password protection)              */
    0,                      /* Magic seed                                          */
    { 10, 100,  30, 200},   /* Target IP address                                   */
    {255, 255,   0,   0},   /* Subnet mask                                         */
    {239,   0,   0,   1},   /* Multicast IP address                                */
    0,                      /* Debug key                                           */
    0,                      /* Automatic baud rate detection timeout               */
    0x00,                   /* Manufacturer ID                                     */
    0x00,                   /* Application ID                                      */
    {0},                    /* Reserved, must be set to zero                       */
    0                       /* Header CRC (provided by TTC-Downloader)             */
};

#pragma SET_DATA_SECTION()

/**************************************************************************************************
 * Static prototypes
 *************************************************************************************************/
static ubyte2 APPL_ErrorCb(ubyte1 diag_state,
                           ubyte1 watchdog_state,
                           DIAG_ERRORCODE * const error);

static void APPL_NotifyCb(ubyte1 diag_state,
                          ubyte1 watchdog_state,
                          DIAG_ERRORCODE * const error);

static void APPL_FPUhandler(bool division_by_zero,
                            bool input_denormal,
                            bool invalid_operation,
                            bool overflow,
                            bool underflow);

/**************************************************************************************************
 * Global Variables
 *************************************************************************************************/
{
ubyte4 AI_103; //датчик температуры НН
ubyte4 AI_127; //датчик температуры ВН
ubyte4 AI_104; //датчик давления РТС 1-го моста
ubyte4 AI_128; //датчик давления РТС 2-го моста
ubyte4 AI_105; //датчик температуры обмотки статора №1
ubyte4 AI_129; //датчик температуры обмотки статора №2
ubyte4 AI_106; //датчик температуры обмотки статора №3
ubyte4 AI_130; //датчик температуры обмотки статора №4
ubyte4 AI_107; //датчик температуры обмотки статора №5
ubyte4 AI_131; //датчик температуры обмотки статора №6
bool adc_00;
bool adc_01;
bool adc_02;
bool adc_03;
bool adc_04;
bool adc_05;
bool adc_06;
bool adc_07;
bool adc_08;
bool adc_09;
bool DI_108; //Редуктор полуприцепа
bool DI_132; //Датчик БМКД 1-го моста
bool DI_109; //Датчик БМКД 2-го моста
bool DI_133; //Датчик задней передачи
bool DI_110; //Датчик пониженной передачи
bool DI_134; //Чек контакт тэд
bool DI_111; //Чек контакт вентилятора
bool DI_135; //Чек контакт тока 1
bool DI_112; //Чек контакт тока 2
bool DI_136; //Чек контакт тока 3
bool DI_113; //Чек ФОРСАЖ
//bool DO_153 - DO_16;
//bool DO_177 - DO_17;
//bool DO_156 - DO_18;
//bool DO_180 - DO_19;
//bool DO_159 - DO_20;
ubyte2 handle1_w;
ubyte2 handle2_w;
ubyte2 handle_r_m;
ubyte2 handle_r_e;
ubyte2 handle_r_t;
ubyte2 handle3_w;
ubyte2 handle4_w;
ubyte1 kp;
ubyte1 ki;
ubyte1 kd;
ubyte2 Torque;
}

/**************************************************************************************************
 * Functions
 *************************************************************************************************/

void Initialization(){
    {//ADC SETTINGS
    io_error = IO_ADC_ChannelInit(IO_ADC_00,       /* measure 103 pin */
                                  IO_ADC_CURRENT,  /* absolute measurement type */
                                  IO_ADC_NO_RANGE,  /* range is fixed for this type */
                                  IO_ADC_NO_PULL,   /* pull resistor is fixed for this type */
                                  IO_PIN_NONE,      /* no sensor supply */
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_01,       
                                  IO_ADC_CURRENT,  
                                  IO_ADC_NO_RANGE,  
                                  IO_ADC_NO_PULL,   
                                  IO_PIN_NONE,      
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_02,      
                                  IO_ADC_CURRENT,  
                                  IO_ADC_NO_RANGE,  
                                  IO_ADC_NO_PULL,   
                                  IO_PIN_NONE,      
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_03,      
                                  IO_ADC_CURRENT,  
                                  IO_ADC_NO_RANGE,  
                                  IO_ADC_NO_PULL,  
                                  IO_PIN_NONE,     
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_04,      
                                  IO_ADC_ABSOLUTE,  
                                  IO_ADC_RANGE_10V,  
                                  IO_ADC_NO_PULL,   
                                  IO_PIN_NONE,      
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_05,      
                                  IO_ADC_ABSOLUTE,  
                                  IO_ADC_RANGE_10V,  
                                  IO_ADC_NO_PULL,  
                                  IO_PIN_NONE,     
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_06,      
                                  IO_ADC_ABSOLUTE,  
                                  IO_ADC_RANGE_10V,  
                                  IO_ADC_NO_PULL,   
                                  IO_PIN_NONE,      
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_07,      
                                  IO_ADC_ABSOLUTE,  
                                  IO_ADC_RANGE_10V,  
                                  IO_ADC_NO_PULL,  
                                  IO_PIN_NONE,     
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_08,      
                                  IO_ADC_ABSOLUTE,  
                                  IO_ADC_RANGE_10V,  
                                  IO_ADC_NO_PULL,   
                                  IO_PIN_NONE,      
                                  NULL);
    io_error = IO_ADC_ChannelInit(IO_ADC_09,      
                                  IO_ADC_ABSOLUTE,  
                                  IO_ADC_RANGE_10V,  
                                  IO_ADC_NO_PULL,  
                                  IO_PIN_NONE,     
                                  NULL);
    }
    {//DI SETTINGS
    io_error = IO_DI_Init(IO_DI_58, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_59, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_60, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_61, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_62, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_63, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_64, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_65, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_66, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_67, IO_DI_PU_10K, NULL);
    io_error = IO_DI_Init(IO_DI_68, IO_DI_PU_10K, NULL);
    }
    {//DO SETTINGS
    io_error = IO_DO_Init(IO_DO_16, FALSE, NULL);
    io_error = IO_DO_Init(IO_DO_17, FALSE, NULL);
    io_error = IO_DO_Init(IO_DO_18, FALSE, NULL);
    io_error = IO_DO_Init(IO_DO_19, FALSE, NULL);
    io_error = IO_DO_Init(IO_DO_20, FALSE, NULL);
    }
    {//CAN_1 SETTENGS
    io_error = IO_CAN_Init(IO_CAN_CHANNEL_1,        /* CAN channel */
                              IO_CAN_BIT_250_KB,       /* 250 kBit/s */
                              0,                       /* default */
                              0,                       /* default */
                              0,                       /* default */
                              0);                      /* default */

    io_error = IO_CAN_ConfigMsg(&handle1_w,
                                IO_CAN_CHANNEL_1, // channel 1
                                IO_CAN_MSG_WRITE, // transmit message buffer
                                IO_CAN_STD_FRAME, // standard ID
                                0,
                                0);
    }
    {//CAN_2 SETTENGS
    io_error = IO_CAN_Init(IO_CAN_CHANNEL_2,        /* CAN channel */
                              IO_CAN_BIT_250_KB,       /* 250 kBit/s */
                              0,                       /* default */
                              0,                       /* default */
                              0,                       /* default */
                              0);                      /* default */

    io_error = IO_CAN_ConfigMsg(&handle2_w,
                                IO_CAN_CHANNEL_2, // channel 2
                                IO_CAN_MSG_WRITE, // transmit message buffer
                                IO_CAN_STD_FRAME, // standard ID
                                0,
                                0);
    io_error = IO_CAN_ConfigMsg(&handle_r_m, // read inv_sensors_m message
                                IO_CAN_CHANNEL_2, // channel 2
                                IO_CAN_MSG_READ, // read message buffer
                                IO_CAN_STD_FRAME, // standard ID
                                0x0CFFE1C0,
                                0x1FFFFFFF); // check all
    io_error = IO_CAN_ConfigMsg(&handle_r_e, // read inv_sensors_e message
                                IO_CAN_CHANNEL_2, // channel 2
                                IO_CAN_MSG_READ, // read message buffer
                                IO_CAN_STD_FRAME, // standard ID
                                0x0CFFE2C0,
                                0x1FFFFFFF); // check all
    io_error = IO_CAN_ConfigMsg(&handle_r_t, // read inv_sensors_t message
                                IO_CAN_CHANNEL_2, // channel 2
                                IO_CAN_MSG_READ, // read message buffer
                                IO_CAN_STD_FRAME, // standard ID
                                0x0CFFE3C0,
                                0x1FFFFFFF); // check all
    }
    io_error = IO_POWER_Set(IO_INT_POWERSTAGE_ENABLE, IO_POWER_ON);
    io_error = IO_POWER_Set(IO_INT_SAFETY_SW_0, IO_POWER_ON);
}
void Sensors(){
    io_error = IO_ADC_Get(IO_ADC_00, &AI_103, &adc_00);
    io_error = IO_ADC_Get(IO_ADC_01, &AI_127, &adc_01);
    io_error = IO_ADC_Get(IO_ADC_02, &AI_104, &adc_02);
    io_error = IO_ADC_Get(IO_ADC_03, &AI_128, &adc_03);
    io_error = IO_ADC_Get(IO_ADC_04, &AI_105, &adc_04);
    io_error = IO_ADC_Get(IO_ADC_05, &AI_129, &adc_05);
    io_error = IO_ADC_Get(IO_ADC_06, &AI_106, &adc_06);
    io_error = IO_ADC_Get(IO_ADC_07, &AI_130, &adc_07);
    io_error = IO_ADC_Get(IO_ADC_08, &AI_107, &adc_08);
    io_error = IO_ADC_Get(IO_ADC_09, &AI_131, &adc_09);

    io_error = IO_DI_Get(IO_DI_58, &DI_108);
    io_error = IO_DI_Get(IO_DI_59, &DI_132);
    io_error = IO_DI_Get(IO_DI_60, &DI_109);
    io_error = IO_DI_Get(IO_DI_61, &DI_133);
    io_error = IO_DI_Get(IO_DI_62, &DI_110);
    io_error = IO_DI_Get(IO_DI_63, &DI_134);
    io_error = IO_DI_Get(IO_DI_64, &DI_111);
    io_error = IO_DI_Get(IO_DI_65, &DI_135);
    io_error = IO_DI_Get(IO_DI_66, &DI_112);
    io_error = IO_DI_Get(IO_DI_67, &DI_136);
    io_error = IO_DI_Get(IO_DI_68, &DI_113);
}
void CAN_CHANNEL_1(){
IO_CAN_DATA_FRAME can_frame = {0};
    {//MTU-SENSORS-1
    can_frame.id = 255;
    can_frame.id_format = IO_CAN_STD_FRAME;
    can_frame.length = 8;
    can_frame.data[0] = temp_n;
    can_frame.data[1] = temp_v;
    can_frame.data[2] = PTC_1;
    can_frame.data[3] = PTC_2;
    can_frame.data[4] = RP;
    can_frame.data[5] = BMKD_1;
    can_frame.data[6] = BMKD_2;
    can_frame.data[7] = RG;

    IO_CAN_WriteMsg(handle1_w,&can_frame);
    while (IO_CAN_MsgStatus(handle1_w) != IO_E_OK);
    }
    {//MTU-SENSORS-2
    can_frame.id = 254;
    can_frame.id_format = IO_CAN_STD_FRAME;
    can_frame.length = 8;
    can_frame.data[0] = LG;
    can_frame.data[1] = TED;
    can_frame.data[2] = FAN;
    can_frame.data[3] = K1;
    can_frame.data[4] = K2;
    can_frame.data[5] = K3;
    can_frame.data[6] = F;
    can_frame.data[7] = 0;
    IO_CAN_WriteMsg(handle1_w, &can_frame);
    while (IO_CAN_MsgStatus(handle1_w) != IO_E_OK);
    }
}
void CAN_CHANNEL_2(){
    IO_CAN_DATA_FRAME INV_SENSORS_M = {0};
    IO_CAN_DATA_FRAME INV_SENSORS_E = {0};
    IO_CAN_DATA_FRAME INV_SENSORS_T = {0};
    IO_CAN_DATA_FRAME MTU_SENSORS_M = {0};
    IO_CAN_DATA_FRAME MTU_SENSORS_E = {0};
    IO_CAN_DATA_FRAME MTU_SENSORS_T = {0};

    MTU_SENSORS_M.id = 0x1C0;
    MTU_SENSORS_M.id_format = IO_CAN_STD_FRAME;
    MTU_SENSORS_M.lenght = 4;
    MTU_SENSORS_M.id = 0x2C0;
    MTU_SENSORS_M.id_format = IO_CAN_STD_FRAME;
    MTU_SENSORS_M.lenght = 5;
    MTU_SENSORS_M.id = 0x3C0;
    MTU_SENSORS_M.id_format = IO_CAN_STD_FRAME;
    MTU_SENSORS_M.lenght = 8;


    if (IO_CAN_MsgStatus(handle_r_m) == IO_E_OK){
        IO_CAN_ReadMsg(handle_r_m, &INV_SENSORS_M);
        MTU_SENSORS_M = INV_SENSORS_M; //Maybe I will calrify this
        IO_CAN_WriteMsg(handle2_w, &MTU_SENSORS_M);
        while (IO_CAN_MsgStatus(handle2_w) != IO_E_OK);
    }
    if (IO_CAN_MsgStatus(handle_r_e) == IO_E_OK){
        IO_CAN_ReadMsg(handle_r_e, &INV_SENSORS_E);
        MTU_SENSORS_E = INV_SENSORS_E; //Maybe I will clarify this
        IO_CAN_WriteMsg(handle2_w, &MTU_SENSORS_E);
        while (IO_CAN_MsgStatus(handle2_w) != IO_E_OK);
    }
    if (IO_CAN_MsgStatus(handle_r_t) == IO_E_OK){
        IO_CAN_ReadMsg(handle_r_t, &INV_SENSORS_T);
        MTU_SENSORS_T = INV_SENSORS_T; //Maybe I will clarify this
        IO_CAN_WriteMsg(handle2_w, &MTU_SENSORS_T);
        while (IO_CAN_MsgStatus(handle2_w) != IO_E_OK);
    }
}
void CAN_CHANNEL_3(){}
void CAN_CHANNEL_4(){}

/* ============================================================================================= */
/*    Function Name:          main                                                               */
/* ============================================================================================= */

#pragma TASK( main );
void main(void)
{
    {
    volatile IO_ErrorType io_error = IO_E_OK;
    ubyte4 timestamp = 0;

    /* safety configuration */
    IO_DRIVER_SAFETY_CONF safety_conf;

    /* cycle period: 10 ms with 25% percent window size, no resets, 30 ms glitch filter, with error
     * and notification callback */
    safety_conf.command_period      = 10000;
    safety_conf.window_size         = SAFETY_CONF_WINDOW_SIZE_25_PERCENT;
    safety_conf.reset_behavior      = SAFETY_CONF_RESETS_DISABLED;
    safety_conf.glitch_filter_time  = 30;
    safety_conf.error_callback      = APPL_ErrorCb;
    safety_conf.notify_callback     = APPL_NotifyCb;

    /* initialize IO driver with safety configuration */
    io_error = IO_Driver_Init(&safety_conf);

    /* configure floating point exception handler */
    io_error = IO_Driver_SetFPUHandler(&APPL_FPUhandler);
    }

    /* place your custom driver initialization here */
    Initialization();
    /* start the RTC */
    IO_RTC_StartTime(&timestamp);

    while (1)
    {
    io_error = IO_Driver_TaskBegin();

    /* application begin */
    Sensors();

    if (DI_132&&DI_109&&DI_133&&DI_110==TRUE)//УПРАВЛЕНИЕ ТЭД
        {
            {//ЗАПУСКАЕМ ТЭД
            io_error = IO_DO_Set(IO_DO_16, TRUE); // Питание катушки 1 БСКР 149 pin
            io_error = IO_DO_Set(IO_DO_17, TRUE); // Питание катушки 2 БСКР 173 pin
            io_error = IO_DO_Set(IO_DO_18, TRUE); // Питание катушки 3 БСКР 152 pin
            io_error = IO_DO_Set(IO_DO_19, TRUE); // Питание реле ТЭД 176 pin
            io_error = IO_DO_Set(IO_DO_20, TRUE); // Питание реле вентилятора 155 pin
            }
            ubyte2 k = PID((ubute2)safety_conf.command_period, 0, 0, kp, ki, kd); // Коэффициент
            Torque = k*(286 + 514*DI_113); // ДОБАВИТЬ ОБНУЛЕНИЕ ПРИ ТОРМОЖЕНИИ!!!
        }
    else //ОТКЛЮЧАЕМ ТЭД
    {
            io_error = IO_DO_Set(IO_DO_16, FALSE);
            io_error = IO_DO_Set(IO_DO_17, FALSE);
            io_error = IO_DO_Set(IO_DO_18, FALSE);
            io_error = IO_DO_Set(IO_DO_19, FALSE);
            io_error = IO_DO_Set(IO_DO_20, FALSE);
    }

    CAN_CHANNEL_1();
    CAN_CHANNEL_2();
    CAN_CHANNEL_3();
    CAN_CHANNEL_4();

    /* application end */

    io_error = IO_Driver_TaskEnd();

    /* wait until cycle time has passed */
    while (IO_RTC_GetTimeUS(timestamp) < safety_conf.command_period) ;
        
    /* increase time stamp by cycle time */
    timestamp += safety_conf.command_period;
    }

} /* END OF main */

/* ============================================================================================= */
/*    Function Name:          APPL_ErrorCb                                                       */
/* ============================================================================================= */

static ubyte2 APPL_ErrorCb(ubyte1 diag_state,
                           ubyte1 watchdog_state,
                           DIAG_ERRORCODE * const error)
{
    /* decide, which reaction shall be taken */

    (void)diag_state;
    (void)watchdog_state;
    (void)error;

    return DIAG_ERR_SAFESTATE;
}

/* ============================================================================================= */
/*    Function Name:          APPL_NotifyCb                                                      */
/* ============================================================================================= */

static void APPL_NotifyCb(ubyte1 diag_state,
                          ubyte1 watchdog_state,
                          DIAG_ERRORCODE * const error)
{
    /* no decision can be done here */

    (void)diag_state;
    (void)watchdog_state;
    (void)error;
}

/* ============================================================================================= */
/*    Function Name:          APPL_FPUhandler                                                    */
/* ============================================================================================= */

static void APPL_FPUhandler(bool division_by_zero,
                            bool input_denormal,
                            bool invalid_operation,
                            bool overflow,
                            bool underflow)
{
    if (division_by_zero != FALSE)
    {
        /* Application specific handling */
    }

    if (input_denormal != FALSE)
    {
        /* Application specific handling */
    }

    if (invalid_operation != FALSE)
    {
        /* Application specific handling */
    }

    if (overflow != FALSE)
    {
        /* Application specific handling */
    }

    if (underflow != FALSE)
    {
        /* Application specific handling */
    }
}
