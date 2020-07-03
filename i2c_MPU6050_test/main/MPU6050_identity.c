#include <stdio.h>
#include "driver/i2c.h"
#include "sdkconfig.h"


#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

/*MPU6050 register addresses */

#define MPU6050_REG_POWER               0x6B
#define MPU6050_REG_ACCEL_CONFIG        0x1C
#define MPU6050_REG_GYRO_CONFIG         0x1B

/*These are the addresses of mpu6050 from which you will fetch accelerometer x,y,z high and low values */
#define MPU6050_REG_ACC_X_HIGH          0x3B
#define MPU6050_REG_ACC_X_LOW           0x3C
#define MPU6050_REG_ACC_Y_HIGH          0x3D
#define MPU6050_REG_ACC_Y_LOW           0x3E
#define MPU6050_REG_ACC_Z_HIGH          0x3F
#define MPU6050_REG_ACC_Z_LOW           0x40

/*These are the addresses of mpu6050 from which you will fetch gyro x,y,z high and low values */

#define MPU6050_REG_GYRO_X_HIGH          0x43
#define MPU6050_REG_GYRO_X_LOW           0x44
#define MPU6050_REG_GYRO_Y_HIGH          0x45
#define MPU6050_REG_GYRO_Y_LOW           0x46
#define MPU6050_REG_GYRO_Z_HIGH          0x47
#define MPU6050_REG_GYRO_Z_LOW           0x48

/*MPU6050 address and who am i register*/

#define MPU6050_SLAVE_ADDR               0x68
#define who_am_i                         0x75

static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(I2C_NUM_0, &conf);
    return i2c_driver_install(I2C_NUM_0, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t i2c_master_sensor_test(uint8_t *data_h)
{
        int ret;
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, MPU6050_SLAVE_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, who_am_i, ACK_CHECK_EN);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        vTaskDelay(30 / portTICK_RATE_MS);
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, MPU6050_SLAVE_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
        i2c_master_read_byte(cmd, data_h, NACK_VAL);
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        return ret;
}

static void i2c_task_identity(void *arg)
{
    uint8_t sensor_data;
    int ret;
    while (1)
    {
        ret = i2c_master_sensor_test(&sensor_data);
        if (ret == ESP_ERR_TIMEOUT) {
            printf("\n I2C Timeout");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK: MASTER READ SENSOR( MPU6050 )\n");
            printf("*******************\n");
            printf("data_h: %02x\n", sensor_data);
        } else {
            printf("\n No ack, sensor not connected...skip...");
        }
    }
}

void app_main(void)
{
    int ret;
    ESP_ERROR_CHECK(i2c_master_init());
    xTaskCreate(i2c_task_identity, "i2c_test_task read who_am_i register", 1024 * 2, NULL, 10, NULL);
}