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
    ARM_RCC_HSI,
//high speed internal clock signal
    ARM_RCC_HSE,
//external clock instead of  external crystal/ceramic resonator used
    ARM_RCC_HSEBYP,

    ARM_RCC_PLL,
    NUM_CLOCK_SOURCES
} ARM_RCC_ClockSources_enum;

typedef enum {
    ARM_RCC_STA_READY = (uint32_t)0x00000000,
    ARM_RCC_STA_HSE_READY_ERR = (uint32_t)0x00000001,
    ARM_RCC_STA_HSI_READY_ERR = (uint32_t)0x00000002,
    ARM_RCC_STA_PLL_READY_ERR = (uint32_t)0x00000004,
    ARM_RCC_STA_PLL_CLOCK_SWITCH_ERR = (uint32_t)0x00000008
} ARM_RCC_Status_enum;

typedef enum {
//8 MHz crystal resonator, SYSCK = 168 MHz
//for 20 MHz crystal resonator �=20 is used
//for USB: PLL48CK = 48 MHz

    ARM_RCC_PLL_COEFF_M_8 = RCC_PLLCFGR_PLLM_3,
    ARM_RCC_PLL_COEFF_N_336 = (RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_6 |
                               RCC_PLLCFGR_PLLN_8),
    ARM_RCC_PLL_COEFF_P_2 = (uint32_t)0x00,
    ARM_RCC_PLL_COEFF_Q_7 = (RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_2)
} ARM_RCC_PLL_Param_enum;

//********************************************************************************
//Variables
//********************************************************************************
static uint32_t ARM_RCCStatus = (uint32_t)0x00;;
//********************************************************************************
//Prototypes
//********************************************************************************

static void ARM_RCC_ConfigReset(void);
static void ARM_RCC_ClockSourceCmd(ARM_RCC_ClockSources_enum source, PeriphCmd_enum cmd);
static void ARM_RCC_HSEClockDetectorCmd(PeriphCmd_enum cmd);
static void ARM_RCC_SysClockSwitchCmd(ARM_RCC_ClockSources_enum source);
static void ARM_RCC_PLLConfig(void);
static void ARM_RCC_ClearResetFlags(void);
static void ARM_RCC_InteruptDisable(void);
static void ARM_RCC_ConfigMCO2(void);

//================================================================================
//Public
//================================================================================

uint32_t *ARM_RCC_GetStatus(void)
{
    return &ARM_RCCStatus;
}

void ARM_RCC_Reset(void)
{
    ARM_RCC_ClockSourceCmd(ARM_RCC_HSI, ENABLE_CMD);
    ARM_RCC_ConfigReset();
    ARM_RCC_ClockSourceCmd(ARM_RCC_PLL, DISABLE_CMD);
    ARM_RCC_ClockSourceCmd(ARM_RCC_HSE, DISABLE_CMD);
    ARM_RCC_HSEClockDetectorCmd(DISABLE_CMD);
    ARM_RCC_ClockSourceCmd(ARM_RCC_HSEBYP, DISABLE_CMD);
#ifdef HARDWARE_TESTING_MODE
//configure for output System clock divided by 5 (168/5=33,6 MHz) to MCO2 pin
    ARM_RCC_ConfigMCO2();
#endif//HARDWARE_TESTING_MODE
    ARM_RCC_ClearResetFlags();
    ARM_RCC_InteruptDisable();
}

void ARM_RCC_SetSysClockTo168(void)
{
    ARM_RCC_ClockSourceCmd(ARM_RCC_HSE, ENABLE_CMD);
//the number of wait states (LATENCY) must be correctly programmed in the Flash according to HCLK
//and the supply voltage of the device before starting phase locked loop (PLL)
    ARM_FLASH_Init();
//enable clock security system HSE. If the HSE clock happens to fail, the CSS generates an interrupt,
//which causes the automatic generation of an NMI
    ARM_RCC_HSEClockDetectorCmd(ENABLE_CMD);
//    ARM_RCC_ClockSourceCmd(ARM_RCC_HSI, DISABLE_CMD);
//configure PLL
    ARM_RCC_PLLConfig();
//start PLL
    ARM_RCC_ClockSourceCmd(ARM_RCC_PLL, ENABLE_CMD);
//select PLL as system clock
    ARM_RCC_SysClockSwitchCmd(ARM_RCC_PLL);
}

void NMI_Handler(void)
{
    ARM_RCCStatus |= ARM_RCC_STA_HSE_READY_ERR;
}

//================================================================================
//Private
//================================================================================

static void ARM_RCC_ClockSourceCmd(ARM_RCC_ClockSources_enum source, PeriphCmd_enum cmd)
{
    uint32_t counter;
    switch(source) {
        case ARM_RCC_HSI: {
            if(cmd == ENABLE_CMD) {
//start HSI 16 MHz
                RCC->CR |= RCC_CR_HSION;
//waiting for HSI ready flag is set
                while(((RCC->CR & RCC_CR_HSIRDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_HSIRDY) == RESET) {
                    ARM_RCCStatus |= ARM_RCC_STA_HSI_READY_ERR;

                } else {
                    ARM_RCCStatus &= ~ARM_RCC_STA_HSI_READY_ERR;
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
        case ARM_RCC_HSE: {
            if(cmd == ENABLE_CMD) {
// start HSE
                RCC->CR |= RCC_CR_HSEON;
//waiting for HSE ready flag is set
                while(((RCC->CR & RCC_CR_HSERDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_HSERDY) == RESET) {
                    ARM_RCCStatus |= ARM_RCC_STA_HSE_READY_ERR;
                } else {
                    ARM_RCCStatus &= ~ARM_RCC_STA_HSE_READY_ERR;
                }
            } else {
//stop HSI
                RCC->CR &= ~RCC_CR_HSEON;
//waiting for HSE ready flag is cleared
                while(RCC->CR & RCC_CR_HSERDY);
            }
            break;
        }
        case ARM_RCC_PLL: {
            if(cmd == ENABLE_CMD) {
//start PLL
                RCC->CR |= RCC_CR_PLLON;
//waiting for PLL ready flag is set
                while(((RCC->CR & RCC_CR_PLLRDY) == RESET) && (counter != RCC_STARTUP_TIMOUT)) {
                    counter++;
                }
                if((RCC->CR & RCC_CR_PLLRDY) == RESET) {
                    ARM_RCCStatus |= ARM_RCC_STA_PLL_READY_ERR;
                } else {
                    ARM_RCCStatus &= ~ARM_RCC_STA_PLL_READY_ERR;
                }
            } else {
//stop PLL
                RCC->CR &= ~RCC_CR_PLLON;
//waiting for PLL ready flag is cleared
                while(RCC->CR & RCC_CR_PLLRDY);
            }
            break;
        }
        case ARM_RCC_HSEBYP: {
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

static void ARM_RCC_ClearResetFlags(void)
{
    RCC->CSR &= ~RCC_CSR_RMVF;
}

static void ARM_RCC_InteruptDisable(void)
{
    RCC->CIR = (uint32_t)0x00;
}

static void ARM_RCC_ConfigReset(void)
{
//�lear clock configuration register
    RCC->CFGR = (uint32_t)0x00;
//waiting for none of the system clock sources will not be used
    while(RCC->CFGR & RCC_CFGR_SWS);
}

static void ARM_RCC_HSEClockDetectorCmd(PeriphCmd_enum cmd)
{
    if(cmd == ENABLE_CMD) {
//enable CSS
        RCC->CR |= RCC_CR_CSSON;
    } else {
//disable CSS
        RCC->CR &= ~RCC_CR_CSSON;
    }

}

static void ARM_RCC_PLLConfig(void)
{
    RCC->PLLCFGR = (uint32_t)0x00;
//configur� for maximum frequency: SYSCK=f*N/(M*P)=8*336/(8*2)=168MHz
//PLL48CK=f*N/(M*Q)=48MHZ
    RCC->PLLCFGR |= (ARM_RCC_PLL_COEFF_M_8 | ARM_RCC_PLL_COEFF_N_336 | ARM_RCC_PLL_COEFF_P_2 | ARM_RCC_PLL_COEFF_Q_7);
//select HSE as PLL source
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
}

static void ARM_RCC_SysClockSwitchCmd(ARM_RCC_ClockSources_enum source)
{
    RCC->CFGR &= ~RCC_CFGR_SW;
    switch(source) {
        case ARM_RCC_HSI: {
//select system clock source
            RCC->CFGR |= RCC_CFGR_SW_HSI;
//make sure the source is selected
            while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
            break;
        }
        case ARM_RCC_HSE: {
            RCC->CFGR |= RCC_CFGR_SW_HSE;
            while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
            break;
        }
        case ARM_RCC_PLL: {
            RCC->CFGR |= RCC_CFGR_SW_PLL;
            while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
            break;
        }
    }
}

static void ARM_RCC_ConfigMCO2(void)
{
//select prescaler 5
    RCC->CFGR |= RCC_CFGR_MCO2PRE_Msk;
//select SYSCLOCK (default value)
    RCC->CFGR &= ~RCC_CFGR_MCO2_Msk;

}
