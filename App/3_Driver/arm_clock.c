//********************************************************************************
//arm_clock.c
//********************************************************************************
#include "stm32f4xx.h"
#include "arm_clock.h"
#include "discovery-kit.h"
#include "arm_flash.h"

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
//high speed external clock signal
    RCC_HSI,
//high speed internal clock signal
    RCC_HSE,
//external clock instead of  external crystal/ceramic resonator used
    RCC_HSEBYP,

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
//8 MHz crystal resonator, SYSCK = 168 MHz
//for 20 MHz crystal resonator М=20 is used
//for USB: PLL48CK = 48 MHz

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
//the number of wait states (LATENCY) must be correctly programmed in the Flash according to HCLK
//and the supply voltage of the device before starting phase locked loop (PLL)
    FLASH_Init();
//enable clock security system HSE. If the HSE clock happens to fail, the CSS generates an interrupt,
//which causes the automatic generation of an NMI
    RCC_HSEClockDetectorCmd(ENABLE_CMD);
//    RCC_ClockSourceCmd(RCC_HSI, DISABLE_CMD);
//configure PLL
    RCC_PLLConfig();
//start PLL
    RCC_ClockSourceCmd(RCC_PLL, ENABLE_CMD);
//select PLL as system clock
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
//start HSI 16 MHz
                RCC->CR |= RCC_CR_HSION;
//waiting for HSI ready flag is set
                while(((RCC->CR & RCC_CR_HSIRDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_HSIRDY) == RESET) {
                    RCCStatus |= RCC_STA_HSI_READY_ERR;

                } else {
                    RCCStatus &= ~RCC_STA_HSI_READY_ERR;
//reset HSI calibration
                    RCC->CR &= ~RCC_CR_HSITRIM;
//set the default HSI calibration
                    RCC->CR |= (uint32_t)0x80;
                }
            } else {
//stop HSI
                RCC->CR &= ~RCC_CR_HSION;
//waiting for HSI ready flag is cleared
                while(RCC->CR & RCC_CR_HSIRDY);
            }
            break;
        }
        case RCC_HSE: {
            if(cmd == ENABLE_CMD) {
// start HSE
                RCC->CR |= RCC_CR_HSEON;
//waiting for HSE ready flag is set
                while(((RCC->CR & RCC_CR_HSERDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_HSERDY) == RESET) {
                    RCCStatus |= RCC_STA_HSE_READY_ERR;
                } else {
                    RCCStatus &= ~RCC_STA_HSE_READY_ERR;
                }
            } else {
//stop HSI
                RCC->CR &= ~RCC_CR_HSEON;
//waiting for HSE ready flag is cleared
                while(RCC->CR & RCC_CR_HSERDY);
            }
            break;
        }
        case RCC_PLL: {
            if(cmd == ENABLE_CMD) {
//start PLL
                RCC->CR |= RCC_CR_PLLON;
//waiting for PLL ready flag is set
                while(((RCC->CR & RCC_CR_PLLRDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_PLLRDY) == RESET) {
                    RCCStatus |= RCC_STA_PLL_READY_ERR;
                } else {
                    RCCStatus &= ~RCC_STA_PLL_READY_ERR;
                }
            } else {
//stop PLL
                RCC->CR &= ~RCC_CR_PLLON;
//waiting for PLL ready flag is cleared
                while(RCC->CR & RCC_CR_PLLRDY);
            }
            break;
        }
        case RCC_HSEBYP: {
            if(cmd == ENABLE_CMD) {
//enable HSE with external clock
                RCC->CR |= RCC_CR_HSEBYP;
            } else {
//disable HSE with external clock
                RCC->CR &= ~RCC_CR_HSEBYP;
            }
            break;
        }
    }
}

static void RCC_ConfigReset(void)
{
//сlear clock configuration register
    RCC->CFGR = (uint32_t)0x00;
//waiting for none of the system clock sources will not be used
    while(RCC->CFGR & RCC_CFGR_SWS);
}

static void RCC_HSEClockDetectorCmd(PeriphCmd_enum cmd)
{
    if(cmd == ENABLE_CMD) {
//enable CSS
        RCC->CR |= RCC_CR_CSSON;
    } else {
//disable CSS
        RCC->CR &= ~RCC_CR_CSSON;
    }

}

static void RCC_PLLConfig(void)
{
    RCC->PLLCFGR = (uint32_t)0x00;
//configurе for maximum frequency: SYSCK=f*N/(M*P)=8*336/(8*2)=168MHz
//PLL48CK=f*N/(M*Q)=48MHZ
    RCC->PLLCFGR |= (RCC_PLL_COEFF_M_8 | RCC_PLL_COEFF_N_336 | RCC_PLL_COEFF_P_2 | RCC_PLL_COEFF_Q_7);
//select HSE as PLL source
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
}

static void RCC_SysClockSwitchCmd(RCC_ClockSources_enum source)
{
    RCC->CFGR &= ~RCC_CFGR_SW;
    switch(source) {
        case RCC_HSI: {
//select system clock source
            RCC->CFGR |= RCC_CFGR_SW_HSI;
//make sure the source is selected
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
