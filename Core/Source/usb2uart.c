#include "usb2uart.h"
extern UART_HandleTypeDef huart1;
// 添加回调函数指针
static uint32_t dma_tx_length = 0;
// DMA发送缓冲区和状态
static uint8_t uart3_tx_buffer[256];
 volatile uint8_t uart3_dma_tx_busy = 0;
static uint8_t uart3_rx_buffer[UART_DMA_BUF_LEN];
static uint8_t uart3_rx_buffer1[UART_DMA_BUF_LEN];
static volatile uint16_t uart3_rx_write_pos = 0;
extern chry_ringbuffer_t g_uartrx; // 假设这个环形缓冲区已定义

/* implment by user */
void chry_dap_usb2uart_uart_send_bydma(uint8_t *data, uint16_t len)
{
    if (uart3_dma_tx_busy) {
        chry_dap_usb2uart_uart_send_complete(0);
        return;
    }

    DAL_UART_Transmit(&huart1, data, len, 1000U);

    uint8_t ret = DAL_UART_Transmit_DMA(&huart1, data, len);

    // printf("DAL_UART_Transmit_DMA start %d\r\n", ret);
    uart3_dma_tx_busy = 1;
    dma_tx_length = len;
}
/* implment by user */
void chry_dap_usb2uart_uart_config_callback(struct cdc_line_coding *line_coding)
{
    huart1.Instance             = USART1;
    huart1.Init.BaudRate        = line_coding->dwDTERate;
    huart1.Init.WordLength      = line_coding->bDataBits;
    huart1.Init.StopBits        = line_coding->bCharFormat;
    huart1.Init.Parity          = line_coding->bParityType;
    huart1.Init.Mode            = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling    = UART_OVERSAMPLING_16;
    
    if (DAL_UART_Init(&huart1) != DAL_OK)
    {
        Error_Handler();
    }
    // 启动接收到空闲
    DAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)uart3_rx_buffer, sizeof(uart3_rx_buffer));
}

/**
 * @brief  Rx Transfer completed callbacks
 *
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module
 *
 * @retval None
 */
void DAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        chry_ringbuffer_write(&g_uartrx, &uart3_rx_buffer, sizeof(uart3_rx_buffer));
        DAL_UART_Receive_DMA(&huart1, uart3_rx_buffer, sizeof(uart3_rx_buffer));
    }
}

void DAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1) {
        DAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)uart3_rx_buffer, sizeof(uart3_rx_buffer));
        chry_ringbuffer_write(&g_uartrx, &uart3_rx_buffer, Size);
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
    // 清除忙标志
    if (huart->Instance == USART1) {
        // printf("%s\n", __func__);
        uart3_dma_tx_busy = 0;
        chry_dap_usb2uart_uart_send_complete(dma_tx_length);
    }
    // printf("%s\n", __func__);
}

/**
 * @brief  UART error callbacks
 *
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module
 *
 * @retval None
 */
void DAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
}
