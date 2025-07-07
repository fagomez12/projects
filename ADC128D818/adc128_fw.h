/*!
 * \file adc128_fw.h
 * \ingroup firmware
 * \brief ADC128 FIRWMARE HEADER FILE (OPEN SOURCE)
 * \author Felipe A. Gómez Olaya 
 * \date April 2025
 * \details This header contains the functions, variables and definitions for the adc128d818 module
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

#ifndef ADC_128_FW
#define ADC_128_FW


//
// Definitions
//

//
// Serial Bus Address table
//

#define ADC128_ADD_A1_A0_1D                                 (0x1D)          //!<  \brief  A1 LOW and A2 LOW  
#define ADC128_ADD_A1_A0_1E                                 (0x1E)          //!<  \brief  A1 LOW and A2 MID  
#define ADC128_ADD_A1_A0_1F                                 (0x1F)          //!<  \brief  A1 LOW and A2 HIGH 
#define ADC128_ADD_A1_A0_2D                                 (0x2D)          //!<  \brief  A1 MID and A2 LOW  
#define ADC128_ADD_A1_A0_2E                                 (0x2E)          //!<  \brief  A1 MID and A2 MID  
#define ADC128_ADD_A1_A0_2F                                 (0x2F)          //!<  \brief  A1 MID and A2 HIGH 
#define ADC128_ADD_A1_A0_35                                 (0x35)          //!<  \brief  A1 HIGH and A2 LOW 
#define ADC128_ADD_A1_A0_36                                 (0x36)          //!<  \brief  A1 HIGH and A2 MID 
#define ADC128_ADD_A1_A0_37                                 (0x37)          //!<  \brief  A1 HIGH and A2 HIGH


//
// ADC128D8128 Internal Registers
//

#define ADC128_CONF_REG_ADD                                 (0x00)          //!< \brief Configuration Register           
#define ADC128_INT_STAT_REG_ADD                             (0x01)          //!< \brief Interrupt Status Register        
#define ADC128_INT_MSK_REG_ADD                              (0x03)          //!< \brief Interrupt Mask Register          
#define ADC128_CONV_RATE_REG_ADD                            (0x07)          //!< \brief Conversion Rate Register         
#define ADC128_CHNN_DIS_REG_ADD                             (0x08)          //!< \brief Channel Disable Register         
#define ADC128_ONE_SHOT_REG_ADD                             (0x09)          //!< \brief One-Shot Register                
#define ADC128_DEEP_SHDW_REG_ADD                            (0x0A)          //!< \brief Deep Shutdown Register           
#define ADC128_ADVC_CONF_REG_ADD                            (0x0B)          //!< \brief Advanced Configuration Register    
#define ADC128_BUSY_STAT_REG_ADD                            (0x0C)          //!< \brief Busy Status Register               
#define ADC128_CHNN_IN0_REG_ADD                             (0x20)          //!< \brief Channel IN0 Reading Register     
#define ADC128_CHNN_IN1_REG_ADD                             (0x21)          //!< \brief Channel IN1 Reading Register       
#define ADC128_CHNN_IN2_REG_ADD                             (0x22)          //!< \brief Channel IN2 Reading Register       
#define ADC128_CHNN_IN3_REG_ADD                             (0x23)          //!< \brief Channel IN3 Reading Register       
#define ADC128_CHNN_IN4_REG_ADD                             (0x24)          //!< \brief Channel IN4 Reading Register       
#define ADC128_CHNN_IN5_REG_ADD                             (0x25)          //!< \brief Channel IN5 Reading Register       
#define ADC128_CHNN_IN6_REG_ADD                             (0x26)          //!< \brief Channel IN6 Reading Register       
#define ADC128_CHNN_IN7_REG_ADD                             (0x27)          //!< \brief Channel IN7 Reading Register           
#define ADC128_LIMIT_HG_IN0_REG_ADD                         (0x2A)          //!< \brief High Limit IN0 Register            
#define ADC128_LIMIT_LW_IN0_REG_ADD                         (0x2B)          //!< \brief Low Limit IN0 Register             
#define ADC128_LIMIT_HG_IN1_REG_ADD                         (0x2C)          //!< \brief High Limit IN1 Register            
#define ADC128_LIMIT_LW_IN1_REG_ADD                         (0x2D)          //!< \brief Low Limit IN1 Register             
#define ADC128_LIMIT_HG_IN2_REG_ADD                         (0x2E)          //!< \brief High Limit IN2 Register            
#define ADC128_LIMIT_LW_IN2_REG_ADD                         (0x2F)          //!< \brief Low Limit IN2 Register             
#define ADC128_LIMIT_HG_IN3_REG_ADD                         (0x30)          //!< \brief High Limit IN3 Register            
#define ADC128_LIMIT_LW_IN3_REG_ADD                         (0x31)          //!< \brief Low Limit IN3 Register             
#define ADC128_LIMIT_HG_IN4_REG_ADD                         (0x32)          //!< \brief High Limit IN4 Register            
#define ADC128_LIMIT_LW_IN4_REG_ADD                         (0x33)          //!< \brief Low Limit IN4 Register             
#define ADC128_LIMIT_HG_IN5_REG_ADD                         (0x34)          //!< \brief High Limit IN5 Register            
#define ADC128_LIMIT_LW_IN5_REG_ADD                         (0x35)          //!< \brief Low Limit IN5 Register             
#define ADC128_LIMIT_HG_IN6_REG_ADD                         (0x36)          //!< \brief High Limit IN6 Register            
#define ADC128_LIMIT_LW_IN6_REG_ADD                         (0x37)          //!< \brief Low Limit IN6 Register             
#define ADC128_LIMIT_HG_IN7_REG_ADD                         (0x38)          //!< \brief High Limit IN7 Register            
#define ADC128_LIMIT_LW_IN7_REG_ADD                         (0x39)          //!< \brief Low Limit IN7 Register             
#define ADC128_MANF_ID_REG_ADD                              (0x3E)          //!< \brief Manufacturer ID Register           
#define ADC128_REV_ID_REG_ADD                               (0x3F)          //!< \brief Reports the revision's ID              

//
// Structures
//
struct ADC128CONF_BITS {
    Uint8  START:1;         //!< R/W   Bit 0  Enable startup of monitoring operations
    Uint8  INT_ENABLE:1;    //!< R/W   Bit 1  Enable interrupt output pin
    Uint8  rsvd1:1;         //!< R/    Bit 2  Reserved 
    Uint8  INT_CLEAR:1;     //!< R/W   Bit 3  Clear the interrupt output pin
    Uint8  rsvd2:3;         //!< R/    Bit 4-6 Reserved
    Uint8  INITIAL:1;       //!< R/W   Bit 7 Restore default values
};
union ADC128CONF_REG{
    Uint8 all;
    struct ADC128CONF_BITS  bit;
};

struct ADC128INTSTAT_BITS{
    Uint8   IN0ERROR:1;     //!< R/   Bit 0  A High or Low limit has been exceeded - IN0
    Uint8   IN1ERROR:1;     //!< R/   Bit 1  A High or Low limit has been exceeded - IN1
    Uint8   IN2ERROR:1;     //!< R/   Bit 2  A High or Low limit has been exceeded - IN2
    Uint8   IN3ERROR:1;     //!< R/   Bit 3  A High or Low limit has been exceeded - IN3
    Uint8   IN4ERROR:1;     //!< R/   Bit 4  A High or Low limit has been exceeded - IN4
    Uint8   IN5ERROR:1;     //!< R/   Bit 5  A High or Low limit has been exceeded - IN5
    Uint8   IN6ERROR:1;     //!< R/   Bit 6  A High or Low limit has been exceeded - IN6
    Uint8   TEMPERROR:1;    //!< R/   Bit 7  A High or Low limit has been exceeded - IN7
};
union ADC128INTSTAT_REG{
    Uint8 all;
    struct ADC128INTSTAT_BITS bit;
};

struct ADC128INTMSK_BITS{
    Uint8   IN0MASK:1;      //!< R/W  Bit 0 Mask the corresponding interrupt status from propagating to the interrupt output pin
    Uint8   IN1MASK:1;      //!< R/W  Bit 1 Mask the corresponding interrupt status from propagating to the interrupt output pin
    Uint8   IN2MASK:1;      //!< R/W  Bit 2 Mask the corresponding interrupt status from propagating to the interrupt output pin
    Uint8   IN3MASK:1;      //!< R/W  Bit 3 Mask the corresponding interrupt status from propagating to the interrupt output pin
    Uint8   IN4MASK:1;      //!< R/W  Bit 4 Mask the corresponding interrupt status from propagating to the interrupt output pin
    Uint8   IN5MASK:1;      //!< R/W  Bit 5 Mask the corresponding interrupt status from propagating to the interrupt output pin
    Uint8   IN6MASK:1;      //!< R/W  Bit 6 Mask the corresponding interrupt status from propagating to the interrupt output pin
    Uint8   TEMPMASK:1;     //!< R/W  Bit 7 Mask the corresponding interrupt status from propagating to the interrupt output pin
};
union ADC128INTMSK_REG{
    Uint8 all;
    struct ADC128INTMSK_BITS bit;
};

struct ACD128CONVRATE_BITS{
    Uint8 CONVRATE:1;       //!> R/W  Bit 0 Controls the conversion rate
    Uint8 rsvd1:7;          //!> R/   Bit 1-7 reserved
};
union ACD128CONVRATE_REG{
    Uint8 all;
    struct ACD128CONVRATE_BITS bit;
};

struct ACD128CHNNDIS_BITS{
    Uint8   IN0DISABLE:1;   //!< R/W  Bit 0 Conversions are skipped and disabled 
    Uint8   IN1DISABLE:1;   //!< R/W  Bit 1 Conversions are skipped and disabled 
    Uint8   IN2DISABLE:1;   //!< R/W  Bit 2 Conversions are skipped and disabled 
    Uint8   IN3DISABLE:1;   //!< R/W  Bit 3 Conversions are skipped and disabled 
    Uint8   IN4DISABLE:1;   //!< R/W  Bit 4 Conversions are skipped and disabled 
    Uint8   IN5DISABLE:1;   //!< R/W  Bit 5 Conversions are skipped and disabled 
    Uint8   IN6DISABLE:1;   //!< R/W  Bit 6 Conversions are skipped and disabled 
    Uint8   IN7DISABLE:1;   //!< R/W  Bit 7 Conversions are skipped and disabled 
};
union ACD128CHNNDIS_REG{
    Uint8 all;
    struct ACD128CHNNDIS_BITS bit;
};

struct ADC128ONESHOT_BITS{
    Uint8 ONESHOT:1;        //!<  /W Initiate a single conversion and comparison cycle
    Uint8 rsvd1:7;          //!< R/  Reserved
};
union ADC128ONESHOT_REG{
    Uint8 all;
    struct ADC128ONESHOT_BITS bit;
};

struct ADC128DEEPSHDW_BITS{
    Uint8 DEEPSHDWEN:1;     //!< R/W When 'START' = 0 (address 00h, bit 0), setting this bit high will place the device in deep shutdown mode
    Uint8 rsvd1:7;          //!< R/ Reserved
};
union ADC128DEEPSHDW_REG{
    Uint8 all;
    struct ADC128DEEPSHDW_BITS bit;
};

struct ADC128ADVCCONF_BITS{
    Uint8 EXTREFEN:1;       //!< R/W  0: Selects the 2.56V internal VREF - 1: Selects the variable external VREF
    Uint8 MODESLT0:1;       //!< R/W  Mode Select [0]
    Uint8 MODESLT1:1;       //!< R/W  Mode Select [1]
    Uint8 rsvd1:5;          //!< R/   Reserved
};
union ADC128ADVCCONF_REG{
    Uint8 all;
    struct ADC128ADVCCONF_BITS bit;
};

struct ADC128BUSYSTAT_BITS
{
    Uint8 BUSY:1;           //!< R/  ADC128D818 is converting
    Uint8 NOTREADY:1;       //!< R/  Waiting for the power-up sequence to end
    Uint8 rsvd1:6;          //!< R/  Reserved
};
union ADC128BUSYSTAT_REG{
    Uint8 all;
    struct ADC128BUSYSTAT_BITS bit;
};


union ADC128CHNNIN0_REG{
    Uint16 all;
};
union ADC128CHNNIN1_REG{
    Uint16 all;
};
union ADC128CHNNIN2_REG{
    Uint16 all;
};
union ADC128CHNNIN3_REG{
    Uint16 all;
};
union ADC128CHNNIN4_REG{
    Uint16 all;
};
union ADC128CHNNIN5_REG{
    Uint16 all;
};
union ADC128CHNNIN6_REG{
    Uint16 all;
};
union ADC128CHNNIN7_REG{
    Uint16 all;
};



struct ADC128LIMITIN0_LEVEL{
    Uint8   LOW:8;               //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;              //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN0_REG{
    Uint16 all;
    struct ADC128LIMITIN0_LEVEL level;
};

struct ADC128LIMITIN1_LEVEL{
    Uint8   LOW:8;              //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;             //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN1_REG{
    Uint16 all;
    struct ADC128LIMITIN1_LEVEL level;
};

struct ADC128LIMITIN2_LEVEL{
    Uint8   LOW:8;               //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;              //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN2_REG{
    Uint16 all;
    struct ADC128LIMITIN2_LEVEL level;
};

struct ADC128LIMITIN3_LEVEL{
    Uint8   LOW:8;               //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;              //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN3_REG{
    Uint16 all;
    struct ADC128LIMITIN3_LEVEL level;
};

struct ADC128LIMITIN4_LEVEL{
    Uint8   LOW:8;               //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;              //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN4_REG{
    Uint16 all;
    struct ADC128LIMITIN4_LEVEL level;
};

struct ADC128LIMITIN5_LEVEL{
    Uint8   LOW:8;               //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;              //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN5_REG{
    Uint16 all;
    struct ADC128LIMITIN5_LEVEL level;
};

struct ADC128LIMITIN6_LEVEL{
    Uint8   LOW:8;               //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;              //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN6_REG{
    Uint16 all;
    struct ADC128LIMITIN6_LEVEL level;
};

struct ADC128LIMITIN7_LEVEL{
    Uint8   LOW:8;               //!< ADC128 LOW LIMIT
    Uint8   HIGH:8;              //!< ADC128 HIGH LIMIT
};
union ADC128LIMITIN7_REG{
    Uint16 all;
    struct ADC128LIMITIN7_LEVEL level;
};

union ADC128MANFID_REG{
    Uint8 all;
};

union ADC128REVID_REG{
    Uint8 all;
};

struct ADC128_REGS {
    union ADC128CONF_REG            ADC128CONF      ;   //!<  \brief  Configuration Register               
    union ADC128INTSTAT_REG         ADC128INTSTAT   ;   //!<  \brief  Interrupt Status Register            
    union ADC128INTMSK_REG          ADC128INTMSK    ;   //!<  \brief  Interrupt Mask Register              
    union ACD128CONVRATE_REG        ADC128CONVRATE  ;   //!<  \brief  Conversion Rate Register             
    union ACD128CHNNDIS_REG         ADC128CHNNDIS   ;   //!<  \brief  Channel Disable Register             
    union ADC128ONESHOT_REG         ADC128ONESHOT   ;   //!<  \brief  One-Shot Register                    
    union ADC128DEEPSHDW_REG        ADC128DEEPSHDW  ;   //!<  \brief  Deep Shutdown Register               
    union ADC128ADVCCONF_REG        ADC128ADVCCONF  ;   //!<  \brief  Advanced Configuration Register      
    union ADC128BUSYSTAT_REG        ADC128BUSYSTAT  ;   //!<  \brief  Busy Status Register                 
    union ADC128CHNNIN0_REG         ADC128CHNNIN0   ;   //!<  \brief  Channel IN0 Reading Register        
    union ADC128CHNNIN1_REG         ADC128CHNNIN1   ;   //!<  \brief  Channel IN1 Reading Register         
    union ADC128CHNNIN2_REG         ADC128CHNNIN2   ;   //!<  \brief  Channel IN2 Reading Register         
    union ADC128CHNNIN3_REG         ADC128CHNNIN3   ;   //!<  \brief  Channel IN3 Reading Register         
    union ADC128CHNNIN4_REG         ADC128CHNNIN4   ;   //!<  \brief  Channel IN4 Reading Register         
    union ADC128CHNNIN5_REG         ADC128CHNNIN5   ;   //!<  \brief  Channel IN5 Reading Register         
    union ADC128CHNNIN6_REG         ADC128CHNNIN6   ;   //!<  \brief  Channel IN6 Reading Register         
    union ADC128CHNNIN7_REG         ADC128CHNNIN7   ;   //!<  \brief  Channel IN7 Reading Register          
    union ADC128LIMITIN0_REG        ADC128LIMITIN0  ;   //!<  \brief  Low/High IN0 Limit Register          
    union ADC128LIMITIN1_REG        ADC128LIMITIN1  ;   //!<  \brief  Low/High IN1 Limit Register          
    union ADC128LIMITIN2_REG        ADC128LIMITIN2  ;   //!<  \brief  Low/High IN2 Limit Register          
    union ADC128LIMITIN3_REG        ADC128LIMITIN3  ;   //!<  \brief  Low/High IN3 Limit Register          
    union ADC128LIMITIN4_REG        ADC128LIMITIN4  ;   //!<  \brief  Low/High IN4 Limit Register          
    union ADC128LIMITIN5_REG        ADC128LIMITIN5  ;   //!<  \brief  Low/High IN5 Limit Register          
    union ADC128LIMITIN6_REG        ADC128LIMITIN6  ;   //!<  \brief  Low/High IN6 Limit Register          
    union ADC128LIMITIN7_REG        ADC128LIMITIN7  ;   //!<  \brief  Low/High IN7 Limit Register          
    union ADC128MANFID_REG          ADC128MANFID;       //!<  \brief  Manufacturer ID Register             
    union ADC128REVID_REG           ADC128REVID ;       //!<  \brief  Reports the revision's ID            
};

//---------------------------------------------------------------------------
// ADC External References & Function Declarations:
//
extern volatile struct ADC128_REGS Adc128aRegs;

//
//  Functions
//
extern void init_adc128();
extern void write_pred_conf();
extern void rw_adc128_config(Uint8 read_or_write);
extern void r_adc128_inst_stat();
extern void rw_adc128_int_msk(Uint8 read_or_write);
extern void rw_adc128_conv_rate(Uint8 read_or_write);
extern void rw_adc128_chnn_dis(Uint8 read_or_write);
extern void w_adc128_one_shot();
extern void rw_adc128_deepshw(Uint8 read_or_write);
extern void rw_adc128_advc_conf(Uint8 read_or_write);
extern void r_adc128_busy_stat();
extern void r_adc128_channels();
extern void rw_adc128_limits(Uint8 read_or_write);
extern void r_adc128_manf_id();
extern void r_adc128_rev_id();




#define ADC128_CONF_PRED        (0x01)
#define ADC128_CONF_INIT        (0x80)
#define ADC128_ADV_CONF_PRED    (0x03)
#define ADC128_CONV_RATE_PRED   (0x01)
#endif


// PLEASE DO NOT ALTER THE NEXT COMMENT LINES UNLESS NECESSARY CHANGES TO THE DOXYGEN DOCUMENTATION 

/*!
*
* \brief
* This function initializes the adc128d818 module by setting the pre-defined configurations and reading all the ADC128D818 registers
* \fn extern void init_adc128(void)
* \param None
* \return None
*/

/*!
* \brief
* This function writes the pre-defined configuration of the ADC128D818 module under the user porpose
* \fn extern void write_pred_conf(void)
* \return None
* \details 
* This function sets the user pre-defined configuration by setting: \n
* 1. Initialization of Configuration Register \n
* 2. Advance Conf. Register set with External VREF and Mode 1 \n
* 3. Conversion Rate Register set with Continuous Conversion Mode \n
* 4. Configuration Register set with Enable startup and Clear Interrupt Output \n
*/

/*!
* \brief
* This function can read/write the Configuration Register
* \fn extern void rw_adc128_config(Uint8)
* \param read_or_write that can be both: \n
* \b R: Type Uint8 (unsigned Char) means to read \n
* \b W: Type Uint8 (unsigned Char) means to write \n
* \return None
* \details 
* This function provides control and configuration enabling by: \n
* \b Reading  ('R'): Reads the Configuration Register and setting the read data in the \ref ADC128CONF_BITS data structure \n 
* \b Writing  ('W'): Writes data previously set in the \ref ADC128CONF_BITS data structure to the Configuration Register \n 
*/

/*!
* \brief
* This function can read the Interrupt Status Register of the ADC128D818 module
* \fn extern void r_adc128_inst_stat(void)
* \return None
* \details 
* This function provides status of each WATCHDOG limit or interrupt event by: \n
* Reading the Interrupt Status Register and setting the read data in the \ref ADC128INTSTAT_BITS data structure \n 
*/

/*!
* \brief
* This function can read/write the Interrupt Mask Register of the ADC128D818
* \fn extern void rw_adc128_int_msk(Uint8)
* \param read_or_write that can be both: \n
* \b R: Type Uint8 (unsigned Char) means to read \n
* \b W: Type Uint8 (unsigned Char) means to write \n
* \return None
* \details 
* This function masks the interrupt status from propagating to INT by: \n
* \b Reading  ('R'): Reads the Interrupt Mask Register and setting the read data in the \ref ADC128INTMSK_BITS data structure \n 
* \b Writing  ('W'): Writes data previously set in the \ref ADC128INTMSK_BITS data structure to the Interrupt Mask Register \n 
*/

/*!
* \brief
* This function can read/write the Convertion Rate Register of the ADC128D818 module
* \fn extern void rw_adc128_conv_rate(Uint8)
* \param R: Type Uint8 (unsigned Char) means to read
* \param W: Type Uint8 (unsigned Char) means to write
* \return None
* \details 
* This function controls the convertion rate by: \n
* \b Reading  ('R'): Reads the Convertion Rate Register and setting the read data in the \ref ACD128CONVRATE_BITS data structure \n 
* \b Writing  ('W'): Writes data previously set in the \ref ACD128CONVRATE_BITS data structure to the Convertion Rate Register \n 
*/

/*!
* \brief
* This function can read/write the Channel Disable Register of the ADC128D818 module
* \fn extern void rw_adc128_chnn_dis(Uint8)
* \param read_or_write that can be both: \n
* \b R: Type Uint8 (unsigned Char) means to read \n
* \b W: Type Uint8 (unsigned Char) means to write \n
* \return None
* \details 
* This function disables convertion for each voltage or temperature channel by: \n
* \b Reading  ('R'): Reads the Channel Disable Register and setting the read data in the \ref ACD128CHNNDIS_BITS data structure \n 
* \b Writing  ('W'): Writes data previously set in the \ref ACD128CHNNDIS_BITS data structure to the Channel Disable Register \n 
*/

/*!
* \brief
* This function can write the One-Shot Register of the ADC128D818 module
* \fn extern void w_adc128_one_shot(void)
* \return None
* \details 
* This function initiates a single conversion of all enabled channels by: \n
* Writing data previously set in the \ref ADC128ONESHOT_BITS data structure to the One-Shot Register \n 
*/

/*!
* \brief
* This function can read/write the Deep ShutDown Register of the ADC128D818 module
* \fn extern void rw_adc128_deepshw(Uint8)
* \param read_or_write that can be both: \n
* \b R: Type Uint8 (unsigned Char) means to read \n
* \b W: Type Uint8 (unsigned Char) means to write \n
* \return None
* \details 
* This function enables deep shutdown mode by: \n
* \b Reading  ('R'): Reads the Deep ShutDown Register and setting the read data in the \ref ADC128DEEPSHDW_BITS data structure \n 
* \b Writing  ('W'): Writes data previously set in the \ref ADC128DEEPSHDW_BITS data structure to the Deep ShutDown Register \n 
*/


/*!
* \brief
* This function can read/write the Advanced Configuration Register of the ADC128D818 module
* \fn extern void rw_adc128_advc_conf(Uint8)
* \param read_or_write that can be both: \n
* \b R: Type Uint8 (unsigned Char) means to read \n
* \b W: Type Uint8 (unsigned Char) means to write \n
* \return None
* \details 
* This function selects internal or external VREF and modes of operation by: \n
* \b Reading  ('R'): Reads the Advanced Configuration Register and setting the read data in the \ref ADC128ADVCCONF_BITS data structure \n 
* \b Writing  ('W'): Writes data previously set in the \ref ADC128ADVCCONF_BITS data structure to the Advanced Configuration Register \n 
*/

/*!
* \brief
* This function can read the Busy Status Register of the ADC128D818 module
* \fn extern void r_adc128_busy_stat(void)
* \return None
* \details 
* This function reflects ADC128D818 'Busy' and 'Not Ready' statuses by: \n
* Reading the Busy Status Register and setting the read data in the \ref ADC128BUSYSTAT_BITS data structure \n 
*/

/*!
* \brief
* This function can read the Channel Redings Registers of the ADC128D818 module
* \fn extern void r_adc128_channels(void)
* \return None
* \details 
* This function reports the channels (voltage and temperature) readings by: \n
* Reading the Channel Redings Registers and then save them in the \ref ADC128CHNNIN0_REG to \ref ADC128CHNNIN7_REG data structures respectively \n 
*/

/*!
* \brief
* This function can read/write the Limits Registers of the ADC128D818 module
* \fn extern void rw_adc128_limits(Uint8)
* \param read_or_write that can be both: \n
* \b R: Type Uint8 (unsigned Char) means to read \n
* \b W: Type Uint8 (unsigned Char) means to write \n
* \return None
* \details 
* This function set the limits for the voltage and temperature channels by: \n
* \b Reading  ('R'): Reads the Limits Registers and setting the read data in the \ref ADC128LIMITIN0_REG to \ref ADC128LIMITIN7_REG data structure \n 
* \b Writing  ('W'): Writes data previously set in the \ref ADC128LIMITIN0_REG to \ref ADC128LIMITIN7_REG data structure to the Limits Registers \n 
*/

/*!
* \brief
* This function can read the Manufacturer ID Register of the ADC128D818 module
* \fn extern void r_adc128_manf_id(void)
* \return None
* \details 
* This function reports the manufacturer's ID by: \n
* Reading the Manufacturer ID Register and setting the read data in the \ref ADC128MANFID_REG  data structures respectively \n 
*/

/*!
* \brief
* This function can read the Revision ID Register of the ADC128D818 module
* \fn extern void r_adc128_rev_id(void)
* \return None
* \details 
* This function reports the revision's ID by: \n
* Reading the Revision ID Register and setting the read data in the \ref ADC128REVID_REG  data structures respectively \n 
*/
