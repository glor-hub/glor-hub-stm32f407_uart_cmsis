//********************************************************************************
//clock.c
//********************************************************************************
#include "stm32f4xx.h"
#include "clock.h"
#include "discovery-kit.h"
#include "flash.h"

//********************************************************************************
//Macros
//********************************************************************************
#define RCC_STARTUP_TIMOUT 2000
//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

typedef enum {
    RCC_HSI,
    RCC_HSE,
    RCC_HSEBYP, //внешний генератор вместо кварцевого резонатора
    RCC_PLL,
    NUM_CLOCK_SOURCES
} RCC_ClockSources_enum;

typedef enum {
    RCC_STA_READY = (uint32_t)0x00000000,
    RCC_STA_HSE_READY_ERR = (uint32_t)0x00000001,
    RCC_STA_HSI_READY_ERR = (uint32_t)0x00000002,
    RCC_STA_PLL_READY_ERR = (uint32_t)0x00000004,
    RCC_STA_PLL_CLOCK_SWITCH_ERR = (uint32_t)0x00000008
} RCC_Status_enum;

typedef enum {
//Используется кварц 8 MHz, SYSCK = 168 MHz
//Для кварца 20 MHz необходимо взять М=20
//Для USB PLL48CK = 48 MHz

    RCC_PLL_COEFF_M_8 = RCC_PLLCFGR_PLLM_3,
    RCC_PLL_COEFF_N_336 = (RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_6 |
                           RCC_PLLCFGR_PLLN_8),
    RCC_PLL_COEFF_P_2 = (uint32_t)0x00,
    RCC_PLL_COEFF_Q_7 = (RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_2)
} RCC_PLL_Param_enum;

//********************************************************************************
//Variables
//********************************************************************************
static uint32_t RCCStatus = (uint32_t)0x00;;
//********************************************************************************
//Prototypes
//********************************************************************************

static void RCC_ConfigReset(void);
static void RCC_ClockSourceCmd(RCC_ClockSources_enum source, PeriphCmd_enum cmd);
static void RCC_HSEClockDetectorCmd(PeriphCmd_enum cmd);
static void RCC_SysClockSwitchCmd(RCC_ClockSources_enum source);
static void RCC_PLLConfig(void);

//================================================================================
//Public
//================================================================================

uint32_t *RCC_GetStatus(void)
{
    return &RCCStatus;
}

void RCC_Reset(void)
{
    RCC_ClockSourceCmd(RCC_HSI, ENABLE_CMD);
    RCC_ConfigReset();
    RCC_ClockSourceCmd(RCC_PLL, DISABLE_CMD);
    RCC_ClockSourceCmd(RCC_HSE, DISABLE_CMD);
    RCC_HSEClockDetectorCmd(DISABLE_CMD);
    RCC_ClockSourceCmd(RCC_HSEBYP, DISABLE_CMD);
//Сбросим все флаги генерации сброса RCC
    RCC->CSR &= ~RCC_CSR_RMVF;
//Запретим все прерывания по RCC
    RCC->CIR = (uint32_t)0x00;
}

void RCC_SetSysClockTo168(void)
{
    RCC_ClockSourceCmd(RCC_HSE, ENABLE_CMD);
//Перед включением ФАБЧ (PLL) необходимо установить соответствующую латентность
//для FLASH
    FLASH_Init();
//Включаем детектор сигнала HSE. При отсутствии сигнала HSE будет сгенетировано немаскируемое
//прерывание NMI
    RCC_HSEClockDetectorCmd(ENABLE_CMD);
//    RCC_ClockSourceCmd(RCC_HSI, DISABLE_CMD);
//Конфигурируем PLL
    RCC_PLLConfig();
//Запускаем PLL
    RCC_ClockSourceCmd(RCC_PLL, ENABLE_CMD);
//Выбираем в качестве тактового сигнала сигнал с PLL
    RCC_SysClockSwitchCmd(RCC_PLL);
}

//================================================================================
//Private
//================================================================================
static void RCC_ClockSourceCmd(RCC_ClockSources_enum source, PeriphCmd_enum cmd)
{
    uint32_t counter;
    switch(source) {
        case RCC_HSI: {
            if(cmd == ENABLE_CMD) {
// Запускаем внутренний генератор HSI 16 МГц
                RCC->CR |= RCC_CR_HSION;
//Ждем появления бита готовности HSI
                while(((RCC->CR & RCC_CR_HSIRDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_HSIRDY) == RESET) {
                    RCCStatus |= RCC_STA_HSI_READY_ERR;

                } else {
                    RCCStatus &= ~RCC_STA_HSI_READY_ERR;
//Сбросим калибровку HSI
                    RCC->CR &= ~RCC_CR_HSITRIM;
//Установим значение калибровки по умолчанию
                    RCC->CR |= (uint32_t)0x80;
                }
            } else {
//Выключаем HSI
                RCC->CR &= ~RCC_CR_HSION;
//Ждем сброса бита готовности HSI
                while(RCC->CR & RCC_CR_HSIRDY);
            }
            break;
        }
        case RCC_HSE: {
            if(cmd == ENABLE_CMD) {
// Запускаем внешний генератор HSE
                RCC->CR |= RCC_CR_HSEON;
//Ждем появления бита готовности HSE
                while(((RCC->CR & RCC_CR_HSERDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_HSERDY) == RESET) {
                    RCCStatus |= RCC_STA_HSE_READY_ERR;
                } else {
                    RCCStatus &= ~RCC_STA_HSE_READY_ERR;
                }
            } else {
//Выключаем HSE
                RCC->CR &= ~RCC_CR_HSEON;
//Ждем сброса бита готовности HSE
                while(RCC->CR & RCC_CR_HSERDY);
            }
            break;
        }
        case RCC_PLL: {
            if(cmd == ENABLE_CMD) {
// Запускаем ФАБЧ PLL
                RCC->CR |= RCC_CR_PLLON;
//Ждем появления бита готовности PLL
                while(((RCC->CR & RCC_CR_PLLRDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_PLLRDY) == RESET) {
                    RCCStatus |= RCC_STA_PLL_READY_ERR;
                } else {
                    RCCStatus &= ~RCC_STA_PLL_READY_ERR;
                }
            } else {
//Выключаем PLL
                RCC->CR &= ~RCC_CR_PLLON;
//Ждем сброса бита готовности PLL
                while(RCC->CR & RCC_CR_PLLRDY);
            }
            break;
        }
        case RCC_HSEBYP: {
            if(cmd == ENABLE_CMD) {
//Установим бит, разрешающий использование внешнего генератора
                RCC->CR |= RCC_CR_HSEBYP;
            } else {
//Сбросим бит, разрешающий использование внешнего генератора
                RCC->CR &= ~RCC_CR_HSEBYP;
            }
            break;
        }
    }
}

static void RCC_ConfigReset(void)
{
//Очищаем регистр конфигурации
    RCC->CFGR = (uint32_t)0x00;
//Ждем очистку битов выбора источника системного тактирования
//Необходимо убедиться, что ни один из источников не используется
    while(RCC->CFGR & RCC_CFGR_SWS);
}

static void RCC_HSEClockDetectorCmd(PeriphCmd_enum cmd)
{
    if(cmd == ENABLE_CMD) {
//Включаем детектор сигнала HSE
        RCC->CR |= RCC_CR_CSSON;
    } else {
//Отключаем детектор сигнала HSE
        RCC->CR &= ~RCC_CR_CSSON;
    }

}

static void RCC_PLLConfig(void)
{
    RCC->PLLCFGR = (uint32_t)0x00;
//Настраиваем на максимальную частоту: SYSCK=f*N/(M*P)=8*336/(8*2)=168MHz
//PLL48CK=f*N/(M*Q)=48MHZ
    RCC->PLLCFGR |= (RCC_PLL_COEFF_M_8 | RCC_PLL_COEFF_N_336 | RCC_PLL_COEFF_P_2 | RCC_PLL_COEFF_Q_7);
//Источником сигнала для PLL выбираем HSE
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
}

static void RCC_SysClockSwitchCmd(RCC_ClockSources_enum source)
{
    RCC->CFGR &= ~RCC_CFGR_SW;
    switch(source) {
        case RCC_HSI: {
//Выбираем источник системной частоты
            RCC->CFGR |= RCC_CFGR_SW_HSI;
//Убеждаемся, что источник выбран
            while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
            break;
        }
        case RCC_HSE: {
            RCC->CFGR |= RCC_CFGR_SW_HSE;
            while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
            break;
        }
        case RCC_PLL: {
            RCC->CFGR |= RCC_CFGR_SW_PLL;
            while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
            break;
        }
    }
}

void NMI_Handler(void)
{
    RCCStatus |= RCC_STA_HSE_READY_ERR;
}
