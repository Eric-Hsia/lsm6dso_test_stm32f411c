#include "platform.h"

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
 int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len) {
#if defined(NUCLEO_F401RE)
    HAL_I2C_Mem_Write(handle, LSM6DSOX_I2C_ADD_L, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t *)bufp, len,
                      1000);
#elif defined(STEVAL_MKI109V3)
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, &reg, 1, 1000);
    HAL_SPI_Transmit(handle, (uint8_t *)bufp, len, 1000);
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);
#elif defined(SPC584B_DIS)
    i2c_lld_write(handle, LSM6DSOX_I2C_ADD_L & 0xFE, reg, (uint8_t *)bufp, len);
#endif
    return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
 int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len) {
#if defined(NUCLEO_F401RE)
    HAL_I2C_Mem_Read(handle, LSM6DSOX_I2C_ADD_L, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
#elif defined(STEVAL_MKI109V3)
    reg |= 0x80;
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, &reg, 1, 1000);
    HAL_SPI_Receive(handle, bufp, len, 1000);
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);
#elif defined(SPC584B_DIS)
    i2c_lld_read(handle, LSM6DSOX_I2C_ADD_L & 0xFE, reg, bufp, len);
#endif
    return 0;
}

/*
 * @brief  platform specific outputs on terminal (platform dependent)
 *
 * @param  tx_buffer     buffer to transmit
 * @param  len           number of byte to send
 *
 */
 void tx_com(uint8_t *tx_buffer, uint16_t len) {
#if defined(NUCLEO_F401RE)
    HAL_UART_Transmit(&huart1, tx_buffer, len, 1000);
#elif defined(STEVAL_MKI109V3)
    CDC_Transmit_FS(tx_buffer, len);
#elif defined(SPC584B_DIS)
    sd_lld_write(&SD2, tx_buffer, len);
#endif
}

/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */
 void platform_delay(uint32_t ms) {
#if defined(NUCLEO_F401RE) | defined(STEVAL_MKI109V3)
    HAL_Delay(ms);
#elif defined(SPC584B_DIS)
    osalThreadDelayMilliseconds(ms);
#endif
}

/*
 * @brief  platform specific initialization (platform dependent)
 */
 void platform_init(void) {
#if defined(STEVAL_MKI109V3)
    TIM3->CCR1 = PWM_3V3;
    TIM3->CCR2 = PWM_3V3;
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_Delay(1000);
#endif
}