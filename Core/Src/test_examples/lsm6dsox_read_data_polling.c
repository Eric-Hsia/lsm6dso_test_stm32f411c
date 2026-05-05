/*
 ******************************************************************************
 * @file    read_data_polling.c
 * @author  Sensors Software Solution Team
 * @brief   This file shows how to get data from sensor.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "platform.h"

/* Private variables ---------------------------------------------------------*/

/* Main Example --------------------------------------------------------------*/
void lsm6dsox_read_data_polling(void) {

    uint8_t whoamI, rst;
    float_t temperature_degC;
    uint8_t tx_buffer[1000];
    int16_t data_raw_acceleration[3];
    int16_t data_raw_angular_rate[3];
    int16_t data_raw_temperature;
    float_t acceleration_mg[3];
    float_t angular_rate_mdps[3];

    stmdev_ctx_t dev_ctx;
    /* Initialize mems driver interface */
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.mdelay = platform_delay;
    dev_ctx.handle = &SENSOR_BUS;
    /* Init test platform */
    platform_init();
    /* Wait sensor boot time */
    platform_delay(BOOT_TIME);
    /* Check device ID */
    lsm6dsox_device_id_get(&dev_ctx, &whoamI);

    if (whoamI != LSM6DSOX_ID)
        while (1)
            ;

    /* Restore default configuration */
    lsm6dsox_reset_set(&dev_ctx, PROPERTY_ENABLE);

    do {
        lsm6dsox_reset_get(&dev_ctx, &rst);
    } while (rst);

    /* Disable I3C interface */
    lsm6dsox_i3c_disable_set(&dev_ctx, LSM6DSOX_I3C_DISABLE);
    /* Enable Block Data Update */
    lsm6dsox_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    /* Set Output Data Rate */
    lsm6dsox_xl_data_rate_set(&dev_ctx, LSM6DSOX_XL_ODR_12Hz5);
    lsm6dsox_gy_data_rate_set(&dev_ctx, LSM6DSOX_GY_ODR_12Hz5);
    /* Set full scale */
    lsm6dsox_xl_full_scale_set(&dev_ctx, LSM6DSOX_2g);
    lsm6dsox_gy_full_scale_set(&dev_ctx, LSM6DSOX_2000dps);
    /* Configure filtering chain(No aux interface)
     * Accelerometer - LPF1 + LPF2 path
     */
    lsm6dsox_xl_hp_path_on_out_set(&dev_ctx, LSM6DSOX_LP_ODR_DIV_100);
    lsm6dsox_xl_filter_lp2_set(&dev_ctx, PROPERTY_ENABLE);

    /* Read samples in polling mode (no int) */
    while (1) {
        uint8_t reg;
        /* Read output only if new xl value is available */
        lsm6dsox_xl_flag_data_ready_get(&dev_ctx, &reg);

        if (reg) {
            /* Read acceleration field data */
            memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
            lsm6dsox_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
            acceleration_mg[0] = lsm6dsox_from_fs2_to_mg(data_raw_acceleration[0]);
            acceleration_mg[1] = lsm6dsox_from_fs2_to_mg(data_raw_acceleration[1]);
            acceleration_mg[2] = lsm6dsox_from_fs2_to_mg(data_raw_acceleration[2]);
            snprintf((char *)tx_buffer, sizeof(tx_buffer),
                     "Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n", acceleration_mg[0],
                     acceleration_mg[1], acceleration_mg[2]);
            tx_com(tx_buffer, strlen((char const *)tx_buffer));
        }

        lsm6dsox_gy_flag_data_ready_get(&dev_ctx, &reg);

        if (reg) {
            /* Read angular rate field data */
            memset(data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
            lsm6dsox_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate);
            angular_rate_mdps[0] = lsm6dsox_from_fs2000_to_mdps(data_raw_angular_rate[0]);
            angular_rate_mdps[1] = lsm6dsox_from_fs2000_to_mdps(data_raw_angular_rate[1]);
            angular_rate_mdps[2] = lsm6dsox_from_fs2000_to_mdps(data_raw_angular_rate[2]);
            snprintf((char *)tx_buffer, sizeof(tx_buffer),
                     "Angular rate [mdps]:%4.2f\t%4.2f\t%4.2f\r\n", angular_rate_mdps[0],
                     angular_rate_mdps[1], angular_rate_mdps[2]);
            tx_com(tx_buffer, strlen((char const *)tx_buffer));
        }

        lsm6dsox_temp_flag_data_ready_get(&dev_ctx, &reg);

        if (reg) {
            /* Read temperature data */
            memset(&data_raw_temperature, 0x00, sizeof(int16_t));
            lsm6dsox_temperature_raw_get(&dev_ctx, &data_raw_temperature);
            temperature_degC = lsm6dsox_from_lsb_to_celsius(data_raw_temperature);
            snprintf((char *)tx_buffer, sizeof(tx_buffer), "Temperature [degC]:%6.2f\r\n",
                     temperature_degC);
            tx_com(tx_buffer, strlen((char const *)tx_buffer));
        }

        platform_delay(200);
    }
}
