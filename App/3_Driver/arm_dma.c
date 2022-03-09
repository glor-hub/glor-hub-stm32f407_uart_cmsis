//********************************************************************************
//arm_dma.c
//********************************************************************************
#include "stm32f4xx.h"
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "arm_dma.h"
#include "arm_clock.h"

//********************************************************************************
//Macros
//********************************************************************************

//********************************************************************************
//Enums
//********************************************************************************

//********************************************************************************
//Typedefs
//********************************************************************************

typedef struct ARM_DMA_Stream_t {
    uint8_t                 active_flag;
    ARM_DMA_SignalEvent_t   cb_event;
    IRQn_Type               irq;
    DMA_Stream_TypeDef      *p_reg;
    eARM_DMA_ChanNumbers    chan_num;
} ARM_DMA_Stream_t;

typedef struct {
    uint8_t                 init_counter;
    ePeriphTypes            dma_name;
    DMA_TypeDef            *p_reg;
    ARM_DMA_Stream_t        Stream[NUM_ARM_DMA_STREAM];
} ARM_DMA_Cfg_t;

//********************************************************************************
//Variables
//********************************************************************************

static ARM_DMA_Cfg_t ARM_DMA1_Config = {0};
static ARM_DMA_Cfg_t ARM_DMA2_Config = {0};

//********************************************************************************
//Prototypes
//********************************************************************************

static bool ARM_DMA_InitCfg(ARM_DMA_Cfg_t *dma);
__inline static bool ARM_DMA_SetStreamActiveFlag(ARM_DMA_Cfg_t *dma,
        eARM_DMA_StreamNumbers stream_num);
__inline static void ARM_DMA_ClearStreamActiveFlag(ARM_DMA_Cfg_t *dma,
        eARM_DMA_StreamNumbers stream_num);
static ARM_DMA_Cfg_t *ARM_DMA_GetConfigStruct(ePeriphTypes dma_name);
static void DMA_IRQHandler(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num);
//================================================================================
//Public
//================================================================================

bool ARM_DMA_Initialize(ePeriphTypes dma_name)
{
    ARM_DMA_Cfg_t *dma = ARM_DMA_GetConfigStruct(dma_name);
    dma->init_counter++;
    if(dma->init_counter > 1) {
        return PASSED;
    } else {
        ARM_RCC_Periph_ClockCmd(DMA_1, ENABLE_CMD);
// Reset DMA register
        ARM_RCC_Periph_ResetCmd(DMA_1, ENABLE_CMD);
// Release reset signal from DMA
        ARM_RCC_Periph_ResetCmd(DMA_1, DISABLE_CMD);
        for(uint8_t num = 0; num < NUM_ARM_DMA_STREAM; num++) {
            memset((uint8_t *)(&(dma->Stream[num])), 0, sizeof(ARM_DMA_Stream_t));
        }
        dma->dma_name = dma_name;
        ARM_DMA_InitCfg(dma);
        return PASSED;
    }
}

bool ARM_DMA_Uninitialize(ePeriphTypes dma_name)
{
    ARM_DMA_Cfg_t *dma = ARM_DMA_GetConfigStruct(dma_name);
    if(dma->init_counter == 0) {
        return FAILED;
    }
    dma->init_counter--;
    if(dma->init_counter > 0) {
        return PASSED;

    } else {
// dma->init_counter == 0
// Disable and Clear DMA1 IRQ
        for(uint8_t num = 0; num < NUM_ARM_DMA_STREAM; num++) {
            NVIC_DisableIRQ(dma->Stream[num].irq);
            NVIC_ClearPendingIRQ(dma->Stream[num].irq);
            memset((uint8_t *)(&(dma->Stream[num])), 0, sizeof(ARM_DMA_Stream_t));
        }
// Reset DMA register
        ARM_RCC_Periph_ResetCmd(dma_name, ENABLE_CMD);
// Release reset signal from DMA
        ARM_RCC_Periph_ResetCmd(dma_name, DISABLE_CMD);
        ARM_RCC_Periph_ClockCmd(dma_name, DISABLE_CMD);
    }
    return PASSED;
}

bool ARM_DMA_StreamEnable(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num)
{
    ARM_DMA_Cfg_t *dma = ARM_DMA_GetConfigStruct(dma_name);
    if(ARM_DMA_SetStreamActiveFlag(dma, stream_num) == FAILED) {
        return FAILED;
    }
    dma->Stream[stream_num].p_reg->CR |= DMA_SxCR_EN;
    while((dma->Stream[stream_num].p_reg->CR & DMA_SxCR_EN) == 0);
    return PASSED;
}

void ARM_DMA_StreamDisable(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num)
{
    ARM_DMA_Cfg_t *dma = ARM_DMA_GetConfigStruct(dma_name);
    dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_EN;
    while((dma->Stream[stream_num].p_reg->CR & DMA_SxCR_EN) == 1);
    ARM_DMA_ClearStreamActiveFlag(dma, stream_num);
}



bool ARM_DMA_StreamConfig(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num,
                          eARM_DMA_ChanNumbers chan_num, uint8_t priority,
                          eARM_DMA_Modes mode, uint32_t periph_addr,
                          uint32_t mem0_addr, uint32_t mem1_addr,
                          uint32_t periph_size, uint32_t mem_size,
                          uint32_t periph_incr_ena, uint32_t mem_incr_ena,
                          uint32_t data_size, uint32_t dir, uint32_t fifo_thresh,
                          uint32_t fifo_mem_trans_type, uint32_t fifo_periph_trans_type,
                          ARM_DMA_SignalEvent_t  cb_event)
{
    ARM_DMA_Cfg_t *dma = ARM_DMA_GetConfigStruct(dma_name);
    if(dma->init_counter == 0) {
        return FAILED;
    }
    dma->Stream[stream_num].cb_event = cb_event;
// Disable the stream
    {
        ARM_DMA_StreamDisable(dma_name, stream_num);
    }

// Select the channel of stream
    dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_CHSEL_Msk;
    dma->Stream[stream_num].p_reg->CR |= (((uint32_t)chan_num  << DMA_SxCR_CHSEL_Pos) & DMA_SxCR_CHSEL_Msk);

// Set the stream priority
    dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_PL_Msk;
    dma->Stream[stream_num].p_reg->CR |= ((priority << DMA_SxCR_PL_Pos) & DMA_SxCR_PL_Msk);

// Set the peripherial address
    dma->Stream[stream_num].p_reg->PAR = (periph_addr & DMA_SxPAR_PA_Msk);

// Set the base memory address
    dma->Stream[stream_num].p_reg->M0AR = (mem0_addr & DMA_SxM0AR_M0A_Msk);

// Set the transfer direction
    dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_DIR_Msk;
    dma->Stream[stream_num].p_reg->CR |= (((uint32_t)dir << DMA_SxCR_DIR_Pos) & DMA_SxCR_DIR_Msk);

// Set the total number of data items
    dma->Stream[stream_num].p_reg->NDTR = (data_size & DMA_SxNDT_Msk);

// Set mode
    if(mode == ARM_DMA_DIRECT_MODE) {

// Set direct mode
// Direct mode can be used only for Peripheral-to-memory and  Memory-to-peripheral
        if((dir == ARM_DMA_PERIPHERAL_TO_MEMORY_DIR) || (dir == ARM_DMA_MEMORY_TO_PERIPHERAL_DIR)) {
            dma->Stream[stream_num].p_reg->FCR &= ~DMA_SxFCR_DMDIS;
        } else {
            return FAILED;
        }
    } else {

// Set FIFO mode
        dma->Stream[stream_num].p_reg->FCR |= DMA_SxFCR_DMDIS;
// Clear double buffer mode bit
// Clear circular mode bit
        dma->Stream[stream_num].p_reg->CR &= ~(DMA_SxCR_DBM | DMA_SxCR_CIRC);
// Set FIFO threshold
        dma->Stream[stream_num].p_reg->CR &= ~DMA_SxFCR_FTH_Msk;
        dma->Stream[stream_num].p_reg->CR |= (((uint32_t)fifo_thresh <<
                                               DMA_SxFCR_FTH_Pos) & DMA_SxFCR_FTH_Msk);
// Configure transaction type (single or burst)in FIFO mode
        if(fifo_mem_trans_type == ARM_DMA_FIFO_SINGLE_TRANSACTION) {
            dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_MBURST_Msk;
        } else {
            dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_MBURST_Msk;
            dma->Stream[stream_num].p_reg->CR |= (((uint32_t)fifo_mem_trans_type <<
                                                   DMA_SxCR_MBURST_Pos) & DMA_SxCR_MBURST_Msk);
        }
        if(fifo_periph_trans_type == ARM_DMA_FIFO_SINGLE_TRANSACTION) {
            dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_PBURST_Msk;
        } else {
            dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_PBURST_Msk;
            dma->Stream[stream_num].p_reg->CR |= (((uint32_t)fifo_periph_trans_type <<
                                                   DMA_SxCR_PBURST_Pos) & DMA_SxCR_PBURST_Msk);
        }
        switch(mode) {
            case ARM_DMA_FIFO_REGULAR_MODE: {
                break;
            }
            case ARM_DMA_FIFO_DOUBLE_BUFFER_MODE: {
// Set double buffer mode
                dma->Stream[stream_num].p_reg->CR |= DMA_SxCR_DBM;
// Set the second memory address
                dma->Stream[stream_num].p_reg->M1AR = (mem1_addr & DMA_SxM1AR_M1A_Msk);
                break;
            }
//Circular mode can be used only for Peripheral-to-memory and  Memory-to-peripheral
            case ARM_DMA_FIFO_CIRCULAR_MODE: {
// Set circular mode
                dma->Stream[stream_num].p_reg->CR |= DMA_SxCR_CIRC;
                break;
            }
            default: {
                break;
            }
        }
    }

// Set memory data size (MSIZE)
// In direct mode, MSIZE is forced by hardware to the same value as PSIZE
    dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_MSIZE_Msk;
    dma->Stream[stream_num].p_reg->CR |= (((uint32_t)mem_size <<
                                           DMA_SxCR_MSIZE_Pos) & DMA_SxCR_MSIZE_Msk);

// Set peripheral data size (PSIZE)
    dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_PSIZE_Msk;
    dma->Stream[stream_num].p_reg->CR |= (((uint32_t)periph_size <<
                                           DMA_SxCR_PSIZE_Pos) & DMA_SxCR_PSIZE_Msk);

// Set memory increment
    if(mem_incr_ena == ARM_DMA_MINC_ENABLE) {
        dma->Stream[stream_num].p_reg->CR |= DMA_SxCR_MINC;
    } else {
        dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_MINC;
    }

// Set peripheral increment
    if(periph_incr_ena == ARM_DMA_PINC_ENABLE) {
        dma->Stream[stream_num].p_reg->CR |= DMA_SxCR_PINC;
    } else {
        dma->Stream[stream_num].p_reg->CR &= ~DMA_SxCR_PINC;
    }

// Activate the stream
    if(ARM_DMA_StreamEnable(dma_name, stream_num) == FAILED) {
        return FAILED;
    }
    return PASSED;
}

void DMA1_Stream0_IRQHandler(void)
{
    DMA_IRQHandler(DMA_1, ARM_DMA_STREAM_0);
}
//================================================================================
//Private
//================================================================================

static bool ARM_DMA_InitCfg(ARM_DMA_Cfg_t *dma)
{
    switch(dma->dma_name) {
        case DMA_1: {
            dma->p_reg = DMA1;
            dma->Stream[0].irq = DMA1_Stream0_IRQn;
            dma->Stream[1].irq = DMA1_Stream1_IRQn;
            dma->Stream[2].irq = DMA1_Stream2_IRQn;
            dma->Stream[3].irq = DMA1_Stream3_IRQn;
            dma->Stream[4].irq = DMA1_Stream4_IRQn;
            dma->Stream[5].irq = DMA1_Stream5_IRQn;
            dma->Stream[6].irq = DMA1_Stream6_IRQn;
            dma->Stream[7].irq = DMA1_Stream7_IRQn;
            dma->Stream[0].p_reg = DMA1_Stream0;
            dma->Stream[1].p_reg = DMA1_Stream1;
            dma->Stream[2].p_reg = DMA1_Stream2;
            dma->Stream[3].p_reg = DMA1_Stream3;
            dma->Stream[4].p_reg = DMA1_Stream4;
            dma->Stream[5].p_reg = DMA1_Stream5;
            dma->Stream[6].p_reg = DMA1_Stream6;
            dma->Stream[7].p_reg = DMA1_Stream7;
            break;
        }
        case DMA_2: {
            dma->p_reg = DMA2;
            dma->Stream[0].irq = DMA2_Stream0_IRQn;
            dma->Stream[1].irq = DMA2_Stream1_IRQn;
            dma->Stream[2].irq = DMA2_Stream2_IRQn;
            dma->Stream[3].irq = DMA2_Stream3_IRQn;
            dma->Stream[4].irq = DMA2_Stream4_IRQn;
            dma->Stream[5].irq = DMA2_Stream5_IRQn;
            dma->Stream[6].irq = DMA2_Stream6_IRQn;
            dma->Stream[7].irq = DMA2_Stream7_IRQn;
            dma->Stream[0].p_reg = DMA2_Stream0;
            dma->Stream[1].p_reg = DMA2_Stream1;
            dma->Stream[2].p_reg = DMA2_Stream2;
            dma->Stream[3].p_reg = DMA2_Stream3;
            dma->Stream[4].p_reg = DMA2_Stream4;
            dma->Stream[5].p_reg = DMA2_Stream5;
            dma->Stream[6].p_reg = DMA2_Stream6;
            dma->Stream[7].p_reg = DMA2_Stream7;
            break;
        }
        default: {
            return FAILED;
        }
    }
    return PASSED;
}

__inline static bool ARM_DMA_SetStreamActiveFlag(ARM_DMA_Cfg_t *dma,
        eARM_DMA_StreamNumbers stream_num)
{
    if(dma->Stream[stream_num].active_flag) {
        return FAILED;
    }
    dma->Stream[stream_num].active_flag = 1U;
    return PASSED;
}

__inline static void ARM_DMA_ClearStreamActiveFlag(ARM_DMA_Cfg_t *dma,
        eARM_DMA_StreamNumbers stream_num)
{
    dma->Stream[stream_num].active_flag = 0U;
}

static ARM_DMA_Cfg_t *ARM_DMA_GetConfigStruct(ePeriphTypes dma_name)
{
    ARM_DMA_Cfg_t *p_struct;
    switch(dma_name) {
        case DMA_1: {
            p_struct = &ARM_DMA1_Config;
            break;
        }
        case DMA_2: {
            p_struct = &ARM_DMA2_Config;
            break;
        }
    }
    return p_struct;
}

static void DMA_IRQHandler(ePeriphTypes dma_name, eARM_DMA_StreamNumbers stream_num)
{
    uint32_t event = 0;
    uint32_t flag, clear_flag_reg;
    ARM_DMA_Cfg_t *dma = ARM_DMA_GetConfigStruct(dma_name);
    if(stream_num < 4) {
        flag = dma->p_reg->LISR;
        clear_flag_reg = dma->p_reg->LIFCR;
    } else {
        flag = dma->p_reg->HISR;
        clear_flag_reg = dma->p_reg->HIFCR;
    }
    switch(stream_num) {
        case ARM_DMA_STREAM_0:
        case ARM_DMA_STREAM_4: {
// Stream transfer complete
            if(flag & DMA_LISR_TCIF0) {
                event |= ARM_DMA_EVENT_TRANSFER_COMPLETE;
// Clear Stream transfer complete flag
                clear_flag_reg |= DMA_LIFCR_CTCIF0;
            }
// Stream transfer error
            if(flag & DMA_LISR_TEIF0) {
                event |= ARM_DMA_EVENT_TRANSFER_ERROR;
// Clear Stream transfer error flag
                clear_flag_reg |= DMA_LIFCR_CTEIF0;
            }
// Stream direct mode error
            if(flag & DMA_LISR_DMEIF0) {
                event |= ARM_DMA_EVENT_DIRECT_MODE_ERROR;
// Clear Stream direct mode error flag
                clear_flag_reg |= DMA_LIFCR_CDMEIF0;
            }
// Stream FIFO error
            if(flag & DMA_LISR_FEIF0) {
                event |= ARM_DMA_EVENT_FIFO_ERROR;
// Clear Stream FIFO error flag
                clear_flag_reg |= DMA_LIFCR_CFEIF0;
            }
            break;
        }
        case ARM_DMA_STREAM_1:
        case ARM_DMA_STREAM_5: {
            if(flag & DMA_LISR_TCIF1) {
                event |= ARM_DMA_EVENT_TRANSFER_COMPLETE;
                clear_flag_reg |= DMA_LIFCR_CTCIF1;
            }
            if(flag & DMA_LISR_TEIF1) {
                event |= ARM_DMA_EVENT_TRANSFER_ERROR;
                clear_flag_reg |= DMA_LIFCR_CTEIF1;
            }
            if(flag & DMA_LISR_DMEIF1) {
                event |= ARM_DMA_EVENT_DIRECT_MODE_ERROR;
                clear_flag_reg |= DMA_LIFCR_CDMEIF1;
            }
            if(flag & DMA_LISR_FEIF1) {
                event |= ARM_DMA_EVENT_FIFO_ERROR;
                clear_flag_reg |= DMA_LIFCR_CFEIF1;
            }
            break;
        }
        case ARM_DMA_STREAM_2:
        case ARM_DMA_STREAM_6: {
            if(flag & DMA_LISR_TCIF2) {
                event |= ARM_DMA_EVENT_TRANSFER_COMPLETE;
                clear_flag_reg |= DMA_LIFCR_CTCIF2;
            }
            if(flag & DMA_LISR_TEIF2) {
                event |= ARM_DMA_EVENT_TRANSFER_ERROR;
                clear_flag_reg |= DMA_LIFCR_CTEIF2;
            }
            if(flag & DMA_LISR_DMEIF2) {
                event |= ARM_DMA_EVENT_DIRECT_MODE_ERROR;
                clear_flag_reg |= DMA_LIFCR_CDMEIF2;
            }
            if(flag & DMA_LISR_FEIF2) {
                event |= ARM_DMA_EVENT_FIFO_ERROR;
                clear_flag_reg |= DMA_LIFCR_CFEIF2;
            }
            break;
        }
        case ARM_DMA_STREAM_3:
        case ARM_DMA_STREAM_7: {
            if(flag & DMA_LISR_TCIF3) {
                event |= ARM_DMA_EVENT_TRANSFER_COMPLETE;
                clear_flag_reg |= DMA_LIFCR_CTCIF3;
            }
            if(flag & DMA_LISR_TEIF3) {
                event |= ARM_DMA_EVENT_TRANSFER_ERROR;
                clear_flag_reg |= DMA_LIFCR_CTEIF3;
            }
            if(flag & DMA_LISR_DMEIF3) {
                event |= ARM_DMA_EVENT_DIRECT_MODE_ERROR;
                clear_flag_reg |= DMA_LIFCR_CDMEIF3;
            }
            if(flag & DMA_LISR_FEIF3) {
                event |= ARM_DMA_EVENT_FIFO_ERROR;
                clear_flag_reg |= DMA_LIFCR_CFEIF3;
            }
            break;
        }
    }
    ARM_DMA_StreamDisable(dma_name, stream_num);
    if(event != 0U) {
        dma->Stream[stream_num].cb_event(event);
    }
}
