/*!
 * \file adc128_fw.c
 * \brief ADC128 FIRWMARE SOURCE FILE
 * \author Felipe A. Gómez Olaya (OPEN SOURCE)
 * \date April 2025
 * \details This file contains the functions, variables and definitions for the adc128d818 module
 * \copyright 
 *
 * Copyright (C) 2025 Felipe Antonio Gomez Olaya
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * GitHub Repository: https://github.com/yourusername/your-repository
 */




#include "F28x_Project.h"
#include ".\Firmware\typedef_fw.h"
#include ".\Firmware\adc128_fw.h"
#include ".\Firmware\i2c_fw.h"

//
// Data Structures
//
volatile struct ADC128_REGS Adc128aRegs;

//
//  Functions
//

//
// write_pred_conf(): This function writes the pre-defined configuration of the ADC128D818 module under the user porpose
//
void write_pred_conf(void)
{         
    Adc128aRegs.ADC128CONF.all = ADC128_CONF_INIT;                  // Initialization of Configuration Register
    rw_adc128_config('W');
    Adc128aRegs.ADC128ADVCCONF.all = ADC128_ADV_CONF_PRED;          // Advance Conf. Register set with External VREF and Mode 1
    rw_adc128_advc_conf('W');
    Adc128aRegs.ADC128CONVRATE.all = ADC128_CONV_RATE_PRED;         // Conversion Rate Register set with Continuous Conversion Mode
    rw_adc128_conv_rate('W');
    Adc128aRegs.ADC128CONF.all = ADC128_CONF_PRED;                  // Configuration Register set with Enable startup and Clear Interrupt Output
    rw_adc128_config('W');
}

//
//  init_adc128(): This function initializes the adc128d818 module
//
void init_adc128(void)
{   
    write_pred_conf();
    rw_adc128_config('R');
    r_adc128_inst_stat();
    rw_adc128_int_msk('R');
    rw_adc128_conv_rate('R');
    rw_adc128_chnn_dis('R');
    rw_adc128_deepshw('R');
    rw_adc128_advc_conf('R');
    r_adc128_busy_stat();
    r_adc128_channels();
    rw_adc128_limits('R');
    r_adc128_manf_id();
    r_adc128_rev_id();
}

//
// rw_adc128_config(Uint8): This function initializes the adc128d818 module by setting the pre-defined configurations and reading all the ADC128D818 registers
//
void rw_adc128_config(Uint8 read_or_write)
{
    switch (read_or_write)
    {
        case 'R':
            Adc128aRegs.ADC128CONF.all      = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CONF_REG_ADD, 1);
            break;
        case 'W':
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CONF_REG_ADD, Adc128aRegs.ADC128CONF.all);
            break;
        default:
            break;
    
    }
}

//
// r_adc128_inst_stat(): This function can read the Interrupt Status Register of the ADC128D818 module
//
void r_adc128_inst_stat(void)
{
    Adc128aRegs.ADC128INTSTAT.all   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_INT_STAT_REG_ADD, 1);
}

//
// rw_adc128_int_msk(Uint8): This function can read/write the Interrupt Mask Register of the ADC128D818
//
void rw_adc128_int_msk(Uint8 read_or_write)
{
    switch (read_or_write)
    {
        case 'R':
            Adc128aRegs.ADC128INTMSK.all    = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_INT_MSK_REG_ADD, 1);
            break;
        case 'W':
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_INT_MSK_REG_ADD, Adc128aRegs.ADC128INTMSK.all);
            break;
        default:
            break;
    }  
}

//
// rw_adc128_conv_rate(Uint8): This function can read/write the Convertion Rate Register of the ADC128D818 module
//
void rw_adc128_conv_rate(Uint8 read_or_write)
{
    switch (read_or_write)
    {
        case 'R':
            Adc128aRegs.ADC128CONVRATE.all  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CONV_RATE_REG_ADD, 1);
            break;
        case 'W':
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CONV_RATE_REG_ADD, Adc128aRegs.ADC128CONVRATE.all);
            break;
        default:
            break;
    }  
}

//
// rw_adc128_chnn_dis(Uint8): This function can read/write the Channel Disable Register of the ADC128D818 module
//
void rw_adc128_chnn_dis(Uint8 read_or_write)
{
    switch (read_or_write)
    {
        case 'R':
            Adc128aRegs.ADC128CHNNDIS.all   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_DIS_REG_ADD, 1);
            break;
        case 'W':
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_DIS_REG_ADD, Adc128aRegs.ADC128CHNNDIS.all);
            break;
        default:
            break;
    }
}

//
// w_adc128_one_shot(): This function can write the One-Shot Register of the ADC128D818 module
//
void w_adc128_one_shot()
{
    write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_ONE_SHOT_REG_ADD, Adc128aRegs.ADC128ONESHOT.all);
}

//
// rw_adc128_deepshw(Uint8): This function can read/write the Deep ShutDown Register of the ADC128D818 module
//
void rw_adc128_deepshw(Uint8 read_or_write)
{
    switch (read_or_write)
    {
        case 'R':
            Adc128aRegs.ADC128DEEPSHDW.all  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_DEEP_SHDW_REG_ADD, 1);
            break;
        case 'W':
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_DEEP_SHDW_REG_ADD, Adc128aRegs.ADC128DEEPSHDW.all);
            break;
        default:
            break;
    }
}


//
// rw_adc128_advc_conf(Uint8): This function can read/write the Advanced Configuration Register of the ADC128D818 module
//
void rw_adc128_advc_conf(Uint8 read_or_write)
{
    switch (read_or_write)
    {
        case 'R':
            Adc128aRegs.ADC128ADVCCONF.all  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_ADVC_CONF_REG_ADD, 1);
            break;
        case 'W':
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_ADVC_CONF_REG_ADD, Adc128aRegs.ADC128ADVCCONF.all);
            break;
        default:
            break;
    }
}

//
// r_adc128_busy_stat(): This function can read the Busy Status Register of the ADC128D818 module
//
void r_adc128_busy_stat()
{
    Adc128aRegs.ADC128BUSYSTAT.all  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_BUSY_STAT_REG_ADD, 1);
}

//
// r_adc128_channels(): This function can read the Channel Redings Registers of the ADC128D818 module
//
void r_adc128_channels()
{
    Adc128aRegs.ADC128CHNNIN0.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN0_REG_ADD, 2);
    Adc128aRegs.ADC128CHNNIN1.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN1_REG_ADD, 2);
    Adc128aRegs.ADC128CHNNIN2.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN2_REG_ADD, 2);
    Adc128aRegs.ADC128CHNNIN3.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN3_REG_ADD, 2);
    Adc128aRegs.ADC128CHNNIN4.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN4_REG_ADD, 2);
    Adc128aRegs.ADC128CHNNIN5.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN5_REG_ADD, 2);
    Adc128aRegs.ADC128CHNNIN6.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN6_REG_ADD, 2);
    Adc128aRegs.ADC128CHNNIN7.all = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_CHNN_IN7_REG_ADD, 2);
}

//
// rw_adc128_limits(Uint8): This function can read/write the Limits Registers of the ADC128D818 module
//
void rw_adc128_limits(Uint8 read_or_write)
{
    switch (read_or_write)
    {
        case 'R':
            Adc128aRegs.ADC128LIMITIN0.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN0_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN0.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN0_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN1.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN1_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN1.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN1_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN2.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN2_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN2.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN2_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN3.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN3_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN3.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN3_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN4.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN4_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN4.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN4_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN5.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN5_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN5.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN5_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN6.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN6_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN6.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN6_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN7.level.HIGH  = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN7_REG_ADD, 1);
            Adc128aRegs.ADC128LIMITIN7.level.LOW   = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN7_REG_ADD, 1);
            break;
        case 'W':
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN0_REG_ADD, Adc128aRegs.ADC128LIMITIN0.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN0_REG_ADD, Adc128aRegs.ADC128LIMITIN0.level.LOW);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN1_REG_ADD, Adc128aRegs.ADC128LIMITIN1.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN1_REG_ADD, Adc128aRegs.ADC128LIMITIN1.level.LOW);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN2_REG_ADD, Adc128aRegs.ADC128LIMITIN2.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN2_REG_ADD, Adc128aRegs.ADC128LIMITIN2.level.LOW);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN3_REG_ADD, Adc128aRegs.ADC128LIMITIN3.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN3_REG_ADD, Adc128aRegs.ADC128LIMITIN3.level.LOW);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN4_REG_ADD, Adc128aRegs.ADC128LIMITIN4.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN4_REG_ADD, Adc128aRegs.ADC128LIMITIN4.level.LOW);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN5_REG_ADD, Adc128aRegs.ADC128LIMITIN5.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN5_REG_ADD, Adc128aRegs.ADC128LIMITIN5.level.LOW);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN6_REG_ADD, Adc128aRegs.ADC128LIMITIN6.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN6_REG_ADD, Adc128aRegs.ADC128LIMITIN6.level.LOW);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_HG_IN7_REG_ADD, Adc128aRegs.ADC128LIMITIN7.level.HIGH);
            write_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_LIMIT_LW_IN7_REG_ADD, Adc128aRegs.ADC128LIMITIN7.level.LOW);
            break;
        default:
            break;
    }
}

//
// r_adc128_manf_id(): This function can read the Manufacturer ID Register of the ADC128D818 module
//
void r_adc128_manf_id()
{
    Adc128aRegs.ADC128MANFID.all    = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_MANF_ID_REG_ADD, 1);
}

//
// r_adc128_rev_id(): his function can read the Revision ID Register of the ADC128D818 module
//
void r_adc128_rev_id()
{
    Adc128aRegs.ADC128REVID.all     = read_i2c_mod(ADC128_ADD_A1_A0_1D,ADC128_REV_ID_REG_ADD, 1);
}
