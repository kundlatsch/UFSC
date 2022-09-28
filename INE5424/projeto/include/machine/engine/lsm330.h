// EPOS ST LSM330 Inertial Measurement Unit Mediator Declarations

#ifndef __lsm330_h
#define __lsm330_h

#include <machine/i2c.h>
#include <machine/machine.h>

__BEGIN_SYS

// ST Microelectronics Inertial Measurement Unit (IMU) LSM330
class LSM330
{
public:
    enum {
        // Slave addresses
        I2C_ACC_ADDR        = 0x1d, // The 7-bit base slave address for the linear acceleration sensor
        I2C_GYRO_ADDR       = 0x6b, // The 7-bit base slave address for the angular rate sensor (0x6B)

        // Accelerometer registers
        CTRL_REG5_A         = 0x20, // Register to select power mode for the acc
        CTRL_REG7_A         = 0x25, // Register to select general configuration for the acc
        FIFO_CTRL_REG_A     = 0x2e, // Control register for FIFO

        // Configurations for the accelerometer (To do: include all possible commands)
        POWER_MODE_100      = 0x67, // Enable X, Y and Z axis, power on mode and output data rate 100 Hz
        POWER_MODE_1600     = 0x97, // Enable X, Y and Z axis, power on mode and output data rate 1600 Hz
        FIFO_ENABLE         = 0x40, // Enable FIFO setting the FIFO_EN bit on register CTRL_REG7_A
        STREAM_MODE         = 0x40, // Enable Strean Mode for FIFO on register FIFO_CTRL_REG_A

        // Gyroscope registers
        CTRL_REG1_G         = 0x20, // Register to select power mode for the gyro
        CTRL_REG5_G         = 0x24, // Register to enable FIFO
        FIFO_CTRL_REG_G     = 0x2e, // Control register for FIFO

        // Configurations for the gyroscope
        POWER_MODE_95       = 0x0f, // Output data rate of 95 Hz and Bandwidth of  12.5Hz
        POWER_MODE_760      = 0xff, // Output data rate of 760 Hz and Bandwidth of 100 Hz

        // Data registers
        REG_OUT_X_L         = 0x28, // OUT_X_L_A
        REG_OUT_X_H         = 0x29, // OUT_X_H_A
        REG_OUT_Y_L         = 0x2a, // OUT_Y_L_A
        REG_OUT_Y_H         = 0x2b, // OUT_Y_H_A
        REG_OUT_Z_L         = 0x2c, // OUT_Z_L_A
        REG_OUT_Z_H         = 0x2d, // OUT_Z_H_A
    };

public:
    LSM330(I2C * i2c): _i2c(i2c) {}

    void accelerometer_config() {
        // Configure accelerometer to start streaming
        char config[2];

        // Configure power mode
        config[0] = CTRL_REG5_A;
        config[1] = POWER_MODE_100;
        _i2c->write(I2C_ACC_ADDR, config, 2, true);
        Machine::delay(50);

        // Read CTRL_REG5_A register
        _i2c->put(I2C_ACC_ADDR, CTRL_REG5_A);
        Machine::delay(50);

        // Enable FIFO
        config[0] = CTRL_REG7_A;
        config[1] = FIFO_ENABLE;
        _i2c->write(I2C_ACC_ADDR, config, 2, true);
        Machine::delay(50);

        // Read CTRL_REG7_A register
        _i2c->put(I2C_ACC_ADDR, CTRL_REG7_A);
        Machine::delay(50);

        // Enable FIFO Stream Mode
        config[0] = FIFO_CTRL_REG_A;
        config[1] = STREAM_MODE;
        _i2c->write(I2C_ACC_ADDR, config, 2, true);
        Machine::delay(50);

        // Read FIFO_CTRL_REG_A register
        _i2c->put(I2C_ACC_ADDR, FIFO_CTRL_REG_A);
        Machine::delay(50);
    }

    float accelerometer_x() { // in g
        // LSBX axis
        char x_lsb[2];
        _i2c->put(I2C_ACC_ADDR, REG_OUT_X_L);
        Machine::delay(5);

        _i2c->get(I2C_ACC_ADDR, x_lsb);               // i2c->get(I2C_ACC_ADDR, x_lsb, 2);
        Machine::delay(5);

        if(x_lsb == 0)
            return 0;

        // MSBX axis
        char x_msb[2];
        _i2c->put(I2C_ACC_ADDR, REG_OUT_X_H);
        Machine::delay(5);

        _i2c->get(I2C_ACC_ADDR, x_msb);              // _i2c->get(I2C_ACC_ADDR, x_msb, 2);
        Machine::delay(5);

        if(x_msb == 0)
            return 0;

        // Convert the two's complement value
        int ax = ((int)x_msb[0] << 8) | (int)x_lsb[0];
        if(ax > 32767)
            ax-= 65536;

        // Data conversion:
        // - Acceleration range +-2g
        // - Linear acceleration sensitivity 0.061 mg/digit
        return (float)ax * 0.000061;
    }

    float accelerometer_y() { // in g
        // LSBY axis
        char y_lsb[2];
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Y_L);
        Machine::delay(5);
        _i2c->get(I2C_ACC_ADDR, y_lsb);             // _i2c->get(I2C_ACC_ADDR, y_lsb, 2);
        Machine::delay(5);
        if(y_lsb == 0)
            return 0;

        // MSBY axis
        char y_msb[2];
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Y_H);
        Machine::delay(5);
        _i2c->get(I2C_ACC_ADDR, y_msb);             // _i2c->get(I2C_ACC_ADDR, y_msb, 2);
        Machine::delay(5);
        if(y_msb == 0)
            return 0;

        // Convert the two's complement value
        int ay = ((int)y_msb[0] << 8) | (int)y_lsb[0];
        if(ay > 32767)
            ay -= 65536;

        // Data conversion:
        // - Acceleration range +-2g
        // - Linear acceleration sensitivity 0.061 mg/digit
        return (float)ay * 0.000061;
    }

    float accelerometer_z() { // in g
        // LSBZ axis
        char z_lsb[2];
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Z_L);
        Machine::delay(5);
        _i2c->get(I2C_ACC_ADDR, z_lsb);            // _i2c->get(I2C_ACC_ADDR, z_lsb, 2);
        Machine::delay(5);
        if(z_lsb == 0)
            return 0;

        // MSBZ axis
        char z_msb[2];
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Z_H);
        Machine::delay(5);
        _i2c->get(I2C_ACC_ADDR, z_msb);            // _i2c->get(I2C_ACC_ADDR, z_msb, 2);
        Machine::delay(5);
        if(z_msb == 0)
            return 0;

        // Convert the two's complement value
        int az = ((int)z_msb[0] << 8) | (int)z_lsb[0];
        if(az > 32767)
            az -= 65536;

        // Data conversion:
        // - Acceleration range +-2g
        // - Linear acceleration sensitivity 0.061 mg/digit
        return (float)az * 0.000061;
    }

    void gyroscope_config() {
        // Configure gyroscope to start streaming
        char config[2];

        // Configure power mode
        config[0] = CTRL_REG1_G;
        config[1] = POWER_MODE_95;
        _i2c->write(I2C_GYRO_ADDR, config, 2, true);
        Machine::delay(50);

        // Read CTRL_REG5_A register
        _i2c->put(I2C_GYRO_ADDR,CTRL_REG1_G);
        Machine::delay(50);

        // Enable FIFO
        config[0] = CTRL_REG5_G;
        config[1] = FIFO_ENABLE;
        _i2c->write(I2C_GYRO_ADDR, config, 2, true);
        Machine::delay(50);

        // Read CTRL_REG7_A register
        _i2c->put(I2C_GYRO_ADDR, CTRL_REG5_G);
        Machine::delay(50);

        // Enable FIFO Stream Mode
        config[0] = FIFO_CTRL_REG_G;
        config[1] = STREAM_MODE;
        _i2c->write(I2C_GYRO_ADDR, config, 2, true);
        Machine::delay(50);

        // Read FIFO_CTRL_REG_A register
        _i2c->put(I2C_GYRO_ADDR,  FIFO_CTRL_REG_G);
        Machine::delay(50);
    }

    float gyroscope_x() { // in degrees per second (dps)
        // LSB X axis
        char x_lsb[2];
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_X_L);
        Machine::delay(5);
        _i2c->get(I2C_GYRO_ADDR, x_lsb);
        Machine::delay(5);
        if(x_lsb == 0)
            return 0;

        // MSB X axis
        char x_msb[2];
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_X_H);
        Machine::delay(5);
        _i2c->get(I2C_GYRO_ADDR, x_msb);
        Machine::delay(5);
        if(x_msb == 0)
            return 0;

        // Convert the two's complement value
        int gx = ((int)x_msb[0] << 8) | (int)x_lsb[0];
        if (gx > 32767)
            gx -= 65536;

        // Data conversion:
        // - Angular rate range +-250dps
        // - Sensitivity 0.00875 dps/digit
        return (float)gx * 0.00875;
    }

    float gyroscope_y() { // in degrees per second (dps)
        // LSB Y axis
        char y_lsb[2];
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Y_L);
        Machine::delay(5);
        _i2c->get(I2C_GYRO_ADDR, y_lsb);
        Machine::delay(5);
        if(y_lsb == 0)
            return 0;

        // MSB Y axis
        char y_msb[2];
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Y_H);
        Machine::delay(5);
        _i2c->get(I2C_GYRO_ADDR, y_msb);
        Machine::delay(5);
        if(y_msb == 0)
            return 0;

        // Convert the two's complement value
        int gy = ((int)y_msb[0] << 8) | (int)y_lsb[0];
        if (gy > 32767)
            gy -= 65536;

        // Data conversion:
        // - Angular rate range +-250dps
        // - Sensitivity 0.00875 dps/digit
        return (float)gy * 0.00875;
    }

    float gyroscope_z() { // in degrees per second (dps)
        // LSB Z axis
        char z_lsb[2];
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Z_L);
        Machine::delay(5);
        _i2c->get(I2C_GYRO_ADDR, z_lsb);
        Machine::delay(5);
        if(z_lsb == 0)
            return 0;

        // MSB Z axis
        char z_msb[2];
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Z_H);
        Machine::delay(5);
        _i2c->get(I2C_GYRO_ADDR, z_msb);
        Machine::delay(5);
        if(z_msb == 0)
            return 0;

        // Convert the two's complement value
        int gz = ((int)z_msb[0] << 8) | (int)z_lsb[0];
        if (gz > 32767)
            gz -= 65536;

        // Data conversion:
        // - Angular rate range +-250dps
        // - Sensitivity 0.00875 dps/digit
        return (float)gz * 0.00875;
    }

protected:
    I2C * _i2c;
};

__END_SYS

#endif
