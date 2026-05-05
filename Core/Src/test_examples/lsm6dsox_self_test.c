/*
 ******************************************************************************
 * @file    _self_test.c
 * @author  Sensors Software Solution Team
 * @brief   This file implements the self test procedure.
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



/* Private macro -------------------------------------------------------------*/


/* Self test limits. */
#define MIN_ST_LIMIT_mg 50.0f
#define MAX_ST_LIMIT_mg 1700.0f
#define MIN_ST_LIMIT_mdps 150000.0f
#define MAX_ST_LIMIT_mdps 700000.0f

/* Self test results. */
#define ST_PASS 1U
#define ST_FAIL 0U

/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/* Main Example --------------------------------------------------------------*/
void lsm6dsox_self_test(void) {
    uint8_t tx_buffer[1000];
    stmdev_ctx_t dev_ctx;
    float val_st_off[3];
    int16_t data_raw[3];
    float val_st_on[3];
    float test_val[3];
    uint8_t st_result;
    uint8_t whoamI;
    uint8_t drdy;
    uint8_t rst;
    uint8_t i;
    uint8_t j;
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
    /*
     * Accelerometer Self Test
     */
    /* Set Output Data Rate */
    lsm6dsox_xl_data_rate_set(&dev_ctx, LSM6DSOX_XL_ODR_52Hz);
    /* Set full scale */
    lsm6dsox_xl_full_scale_set(&dev_ctx, LSM6DSOX_4g);
    /* Wait stable output */
    platform_delay(100);

    /* Check if new value available */
    do {
        lsm6dsox_xl_flag_data_ready_get(&dev_ctx, &drdy);
    } while (!drdy);

    /* Read dummy data and discard it */
    lsm6dsox_acceleration_raw_get(&dev_ctx, data_raw);
    /* Read 5 sample and get the average vale for each axis */
    memset(val_st_off, 0x00, 3 * sizeof(float));

    for (i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
            lsm6dsox_xl_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dsox_acceleration_raw_get(&dev_ctx, data_raw);

        for (j = 0; j < 3; j++) {
            val_st_off[j] += lsm6dsox_from_fs4_to_mg(data_raw[j]);
        }
    }

    /* Calculate the mg average values */
    for (i = 0; i < 3; i++) {
        val_st_off[i] /= 5.0f;
    }

    /* Enable Self Test positive (or negative) */
    lsm6dsox_xl_self_test_set(&dev_ctx, LSM6DSOX_XL_ST_NEGATIVE);
    // lsm6dsox_xl_self_test_set(&dev_ctx, LSM6DSOX_XL_ST_POSITIVE);
    /* Wait stable output */
    platform_delay(100);

    /* Check if new value available */
    do {
        lsm6dsox_xl_flag_data_ready_get(&dev_ctx, &drdy);
    } while (!drdy);

    /* Read dummy data and discard it */
    lsm6dsox_acceleration_raw_get(&dev_ctx, data_raw);
    /* Read 5 sample and get the average vale for each axis */
    memset(val_st_on, 0x00, 3 * sizeof(float));

    for (i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
            lsm6dsox_xl_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dsox_acceleration_raw_get(&dev_ctx, data_raw);

        for (j = 0; j < 3; j++) {
            val_st_on[j] += lsm6dsox_from_fs4_to_mg(data_raw[j]);
        }
    }

    /* Calculate the mg average values */
    for (i = 0; i < 3; i++) {
        val_st_on[i] /= 5.0f;
    }

    /* Calculate the mg values for self test */
    for (i = 0; i < 3; i++) {
        test_val[i] = fabsf((val_st_on[i] - val_st_off[i]));
    }

    /* Check self test limit */
    st_result = ST_PASS;

    for (i = 0; i < 3; i++) {
        if ((MIN_ST_LIMIT_mg > test_val[i]) || (test_val[i] > MAX_ST_LIMIT_mg)) {
            st_result = ST_FAIL;
        }
    }

    /* Disable Self Test */
    lsm6dsox_xl_self_test_set(&dev_ctx, LSM6DSOX_XL_ST_DISABLE);
    /* Disable sensor. */
    lsm6dsox_xl_data_rate_set(&dev_ctx, LSM6DSOX_XL_ODR_OFF);
    /*
     * Gyroscope Self Test
     */
    /* Set Output Data Rate */
    lsm6dsox_gy_data_rate_set(&dev_ctx, LSM6DSOX_GY_ODR_208Hz);
    /* Set full scale */
    lsm6dsox_gy_full_scale_set(&dev_ctx, LSM6DSOX_2000dps);
    /* Wait stable output */
    platform_delay(100);

    /* Check if new value available */
    do {
        lsm6dsox_gy_flag_data_ready_get(&dev_ctx, &drdy);
    } while (!drdy);

    /* Read dummy data and discard it */
    lsm6dsox_angular_rate_raw_get(&dev_ctx, data_raw);
    /* Read 5 sample and get the average vale for each axis */
    memset(val_st_off, 0x00, 3 * sizeof(float));

    for (i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
            lsm6dsox_gy_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dsox_angular_rate_raw_get(&dev_ctx, data_raw);

        for (j = 0; j < 3; j++) {
            val_st_off[j] += lsm6dsox_from_fs2000_to_mdps(data_raw[j]);
        }
    }

    /* Calculate the mg average values */
    for (i = 0; i < 3; i++) {
        val_st_off[i] /= 5.0f;
    }

    /* Enable Self Test positive (or negative) */
    lsm6dsox_gy_self_test_set(&dev_ctx, LSM6DSOX_GY_ST_POSITIVE);
    // lsm6dsox_gy_self_test_set(&dev_ctx, LIS2DH12_GY_ST_NEGATIVE);
    /* Wait stable output */
    platform_delay(100);
    /* Read 5 sample and get the average vale for each axis */
    memset(val_st_on, 0x00, 3 * sizeof(float));

    for (i = 0; i < 5; i++) {
        /* Check if new value available */
        do {
            lsm6dsox_gy_flag_data_ready_get(&dev_ctx, &drdy);
        } while (!drdy);

        /* Read data and accumulate the mg value */
        lsm6dsox_angular_rate_raw_get(&dev_ctx, data_raw);

        for (j = 0; j < 3; j++) {
            val_st_on[j] += lsm6dsox_from_fs2000_to_mdps(data_raw[j]);
        }
    }

    /* Calculate the mg average values */
    for (i = 0; i < 3; i++) {
        val_st_on[i] /= 5.0f;
    }

    /* Calculate the mg values for self test */
    for (i = 0; i < 3; i++) {
        test_val[i] = fabsf((val_st_on[i] - val_st_off[i]));
    }

    /* Check self test limit */
    for (i = 0; i < 3; i++) {
        if ((MIN_ST_LIMIT_mdps > test_val[i]) || (test_val[i] > MAX_ST_LIMIT_mdps)) {
            st_result = ST_FAIL;
        }
    }

    /* Disable Self Test */
    lsm6dsox_gy_self_test_set(&dev_ctx, LSM6DSOX_GY_ST_DISABLE);
    /* Disable sensor. */
    lsm6dsox_gy_data_rate_set(&dev_ctx, LSM6DSOX_GY_ODR_OFF);

    if (st_result == ST_PASS) {
        snprintf((char *)tx_buffer, sizeof(tx_buffer), "Self Test - PASS\r\n");
    }

    else {
        snprintf((char *)tx_buffer, sizeof(tx_buffer), "Self Test - FAIL\r\n");
    }

    tx_com(tx_buffer, strlen((char const *)tx_buffer));
}


