#include "usb2uart.h"

extern UART_HandleTypeDef huart1;

static uint32_t dma_tx_length = 0;
volatile uint8_t uart_dma_tx_busy = 0;

static volatile uint16_t last_rx_pos = 0;
static uint8_t uart_rx_buffer[UART_DMA_BUF_LEN];
extern chry_ringbuffer_t g_uartrx;

/* implment by user */
void chry_dap_usb2uart_uart_send_bydma(uint8_t *data, uint16_t len)
{
    if (uart_dma_tx_busy) {
        chry_dap_usb2uart_uart_send_complete(0);
        return;
    }

    DAL_UART_Transmit_DMA(&huart1, data, len);

    uart_dma_tx_busy = 1;
    dma_tx_length = len;
}

/* implment by user */
void chry_dap_usb2uart_uart_config_callback(struct cdc_line_coding *line_coding)
{
    huart1.Instance             = USART1;
    huart1.Init.BaudRate        = line_coding->dwDTERate;
    huart1.Init.WordLength      = UART_WORDLENGTH_8B;
    huart1.Init.StopBits        = UART_STOPBITS_1;
    huart1.Init.Parity          = UART_PARITY_NONE;
    huart1.Init.Mode            = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling    = UART_OVERSAMPLING_16;
    
    if (DAL_UART_Init(&huart1) != DAL_OK)
    {
        Error_Handler();
    }
    /* 启动 DMA 接收 */
    DAL_UART_Receive_DMA(&huart1, uart_rx_buffer, UART_DMA_BUF_LEN);
    
    /* 配置 DMA 为循环模式 - APM32 寄存器 */
    huart1.hdmarx->Instance->SCFG |= DMA_SCFGx_CIRCMEN;
    
    /* 启用 IDLE 中断 */
    __DAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    last_rx_pos = 0;
}

static inline void uart_rx_copy_data(uint16_t start, uint16_t end)
{
    if (end > start) {
        chry_ringbuffer_write(&g_uartrx, &uart_rx_buffer[start], end - start);
    } else if (end < start) {
        chry_ringbuffer_write(&g_uartrx, &uart_rx_buffer[start], UART_DMA_BUF_LEN - start);
        if (end > 0) {
            chry_ringbuffer_write(&g_uartrx, uart_rx_buffer, end);
        }
    }
}

void USART1_IRQHandler(void)
{
    if (__DAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
        __DAL_UART_CLEAR_IDLEFLAG(&huart1);
        
        uint16_t current_pos = UART_DMA_BUF_LEN - __DAL_DMA_GET_COUNTER(huart1.hdmarx);
        uint16_t old_pos = last_rx_pos;
        
        if (current_pos != old_pos) {
            uart_rx_copy_data(old_pos, current_pos);
            last_rx_pos = current_pos;
        }
        return;
    }
    
    DAL_UART_IRQHandler(&huart1);
}

void DAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        uint16_t half_pos = UART_DMA_BUF_LEN / 2;
        uint16_t old_pos = last_rx_pos;
        
        if (half_pos != old_pos) {
            uart_rx_copy_data(old_pos, half_pos);
            last_rx_pos = half_pos;
        }
    }
}

void DAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        uint16_t old_pos = last_rx_pos;
        
        if (UART_DMA_BUF_LEN != old_pos) {
            uart_rx_copy_data(old_pos, UART_DMA_BUF_LEN);
            last_rx_pos = 0;
        }
    }
}

/**
 * @brief  Tx Transfer completed callbacks
 *
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module
 *
 * @retval None
 */
void DAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        uart_dma_tx_busy = 0;
        chry_dap_usb2uart_uart_send_complete(dma_tx_length);
    }
}
