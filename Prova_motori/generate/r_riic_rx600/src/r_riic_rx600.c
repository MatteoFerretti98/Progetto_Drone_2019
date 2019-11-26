/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name    : r_riic_rx600.c
* Version      : 1.00  
* Device(s)    : Renesas RX600 family
* Description  : RIIC driver common master/slave initialization and utility 
*              : functions.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 25.10.2011  1.00    First Release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/* Used for xchg() intrinsic */
#include <machine.h>

/* Access to peripherals */
#include "platform.h"
/* RIIC driver */
#include "r_riic_rx600.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define INTERRUPTS_EN   1
#define INTERRUPTS_DIS  0

/******************************************************************************
Exported global variables and functions
*******************************************************************************/
/* Used to prevent having duplicate code for each channel. This only works
   if the channels are identical (just at different locations in memory). This
   is easy to tell by looking in iodefine.h and seeing if the same structure
   was used for all channels. */
/* Initialize the array of pointers for up to 4 channels. */   
#if defined(MCU_RX62N)
volatile struct st_riic __evenaccess * g_riic_channels[] =
{
    #if defined(PACKAGE_LFBGA176) || defined(PACKAGE_LQFP144) || defined(PACKAGE_TFLGA85)
        &RIIC0, (volatile __evenaccess struct st_riic *)&RIIC1
    #elif defined(PACKAGE_LQFP100) 
        &RIIC0
    #endif
};
    
#elif defined(MCU_RX630) || defined(MCU_RX63N)
volatile struct st_riic0 __evenaccess * g_riic_channels[] =
{    
    #if defined(PACKAGE_LQFP176) || defined(PACKAGE_LQFP144)
        &RIIC0,
        (volatile __evenaccess struct st_riic0 *)&RIIC1, /* These look enough like st_riic0 */
        (volatile __evenaccess struct st_riic0 *)&RIIC2,
        (volatile __evenaccess struct st_riic0 *)&RIIC3    
    #elif defined(PACKAGE_LQFP100) || defined(PACKAGE_LQFP80)
        &RIIC0,(volatile __evenaccess struct st_riic0 *)&RIIC2
    #endif
};    

#endif 

/* Define global RIIC mode and state variables and inititialize. */
volatile i2c_mode_t  g_riic_mode[RIIC_NUM_CHANNELS] = {RIIC_IDLE_MODE};
volatile i2c_state_t g_riic_state[RIIC_NUM_CHANNELS] = {UNKNOWN_STATE};

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/* Holds the process currently using the RIIC peripheral */
static bool g_riic_locked[RIIC_NUM_CHANNELS] = {false};

/* Used for atomic accesses */
static int32_t riic_semaphore = 0;

/* This variable determines whether the peripheral has already been 
   initialized. */
static bool riic_initialized[RIIC_NUM_CHANNELS] = {false};

static riic_ret_t riic_ports_init(uint8_t channel);
static riic_ret_t riic_interrupts_init(uint8_t channel, uint8_t enable);


/*******************************************************************************
* Function Name:	riic_ports_init
* Description  :    RIIC port setup for RX600 
* Arguments    : 	RIIC channel - 
*                       The channel to initialize ports for.
* Return Value : 	RIIC_OK - 
*                       RIIC ports initialized OK.
*                   RIIC_NO_CHANNEL - 
*                       Channel requested is not a valid RIIC channel.
*******************************************************************************/
static riic_ret_t riic_ports_init(uint8_t channel)
{
    /* Return value */
    riic_ret_t ret = RIIC_OK;  
      
#if defined(MCU_RX630) || defined(MCU_RX63N) 
    /*                                                  Package
                                        Port 176-pin 144-pin 100-pin 80-pin
    RIIC0 	SCL0[FM+]   (input/output) 	P12 	+ 	    +       +       + 	 
            SDA0[FM+]   (input/output) 	P13 	+ 	    +       +       +  
    RIIC1 	SCL1        (input/output) 	P21 	+ 	    +       -       - 
            SDA1        (input/output) 	P20 	+ 	    +       -       -  
    RIIC2 	SCL2-DS     (input/output) 	P16 	+ 	    +       +       + 
            SDA2-DS     (input/output) 	P17 	+ 	    +       +       + 	 
    RIIC3 	SCL3        (input/output) 	PC0 	+ 	    +       -       - 	 
            SDA3        (input/output) 	PC1 	+ 	    +       -       -  
    */    
    
    /*  Enable writing to the port PFS register with PWPR. (PFS write protect register) */
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;      
#endif

    switch (channel)
    {      
       case 0:      /* All supported package types have channel 0. */
       #if defined(MCU_RX62N)  
            PORT1.DDR.BIT.B3 = 0;		/* SDA0 Set port direction to input */ 
            PORT1.DDR.BIT.B2 = 0;		/* SCL0 */ 

            #if defined(USE_PULLUP)
            PORT1.PCR.BIT.B3 = 1;       /* Enable port pin pull-up resistor. */
            PORT1.PCR.BIT.B2 = 1; 
            #endif 
            
            PORT1.ICR.BIT.B3 = 1;		/* SDA0 Enable the input buffer */
            PORT1.ICR.BIT.B2 = 1;		/* SCL0 */ 

        #elif defined(MCU_RX630) || defined(MCU_RX63N)        
            PORT1.PMR.BIT.B2 = 0;       /* Set pins as regular IO until pin function set up. */  
            PORT1.PMR.BIT.B3 = 0;

            PORT1.PDR.BIT.B2 = 0;       /* Clear PDR to 0 to select input mode. */
            PORT1.PDR.BIT.B3 = 0;
            
            #if defined(USE_PULLUP)            
            PORT1.PCR.BIT.B2 = 1;       /* Enable port pin pull-up resistor. */
            PORT1.PCR.BIT.B3 = 1;           
            #endif 
                
            MPC.P12PFS.BIT.PSEL = 0x0F;	/* Select P12 as SCL0, No IRQ. */
            MPC.P13PFS.BIT.PSEL = 0x0F;	/* Select P13 as SDA0, No IRQ. */

            PORT1.PMR.BIT.B2 = 1;       /* Set as peripheral function I/O. */
            PORT1.PMR.BIT.B3 = 1;        
        #endif
        break;
        
#if defined(PACKAGE_LQFP100) || defined(PACKAGE_LQFP80)
        #if defined(MCU_RX630)  
        case 1:
            PORT1.PMR.BIT.B6 = 0;       /* Set pins as regular IO until pin function set up. */  
            PORT1.PMR.BIT.B7 = 0;

            PORT1.PDR.BIT.B6 = 0;       /* Clear PDR to 0 to select input mode. */
            PORT1.PDR.BIT.B7 = 0;

            #if defined(USE_PULLUP)
            PORT1.PCR.BIT.B6 = 1;       /* Enable port pin pull-up resistor. */
            PORT1.PCR.BIT.B7 = 1; 
            #endif 
    
            MPC.P16PFS.BIT.PSEL = 0x0F;	/* Select P16 as SCL2, No IRQ. */
            MPC.P17PFS.BIT.PSEL = 0x0F;	/* Select P17 as SDA2, No IRQ. */

            PORT1.PMR.BIT.B6 = 1;       /* Set as peripheral function I/O. */
            PORT1.PMR.BIT.B7 = 1;               
        break;
        #endif        
        
#elif defined(PACKAGE_LQFP176) || defined(PACKAGE_LFBGA176) || defined(PACKAGE_LQFP144) || defined(PACKAGE_TFLGA85)         
        case 1:  
        #if defined(MCU_RX62N) 
            PORT2.DDR.BIT.B0 = 0;		/* SDA1 Set port direction to input */ 
            PORT2.DDR.BIT.B1 = 0;		/* SCL1 */ 
            
            #if defined(USE_PULLUP)
            PORT2.PCR.BIT.B0 = 1;       /* Enable port pin pull-up resistor. */
            PORT2.PCR.BIT.B1 = 1; 
            #endif 
            
            PORT2.ICR.BIT.B0 = 1;		/* SDA1 Enable the input buffer */
            PORT2.ICR.BIT.B1 = 1;		/* SCL1 */
                          
        #elif defined(MCU_RX630)        
            PORT2.PMR.BIT.B1 = 0;       /* Set pins as regular IO until pin function set up. */  
            PORT2.PMR.BIT.B0 = 0;

            PORT2.PDR.BIT.B1 = 0;       /* Clear PDR to 0 to select input mode. */
            PORT2.PDR.BIT.B0 = 0;
            
            #if defined(USE_PULLUP)
            PORT2.PCR.BIT.B1 = 1;       /* Enable port pin pull-up resistor. */
            PORT2.PCR.BIT.B0 = 1; 
            #endif  
    
            MPC.P21PFS.BIT.PSEL = 0x0F;	/* Select P21 as SCL1, No IRQ. */
            MPC.P20PFS.BIT.PSEL = 0x0F;	/* Select P20 as SDA1, No IRQ. */

            PORT2.PMR.BIT.B1 = 1;       /* Set as peripheral function I/O. */
            PORT2.PMR.BIT.B0 = 1;
        #endif                           
        break;
        
        #if defined(MCU_RX630) || defined(MCU_RX63N) 
        case 2:
            PORT1.PMR.BIT.B6 = 0;       /* Set pins as regular IO until pin function set up. */  
            PORT1.PMR.BIT.B7 = 0;

            PORT1.PDR.BIT.B6 = 0;       /* Clear PDR to 0 to select input mode. */
            PORT1.PDR.BIT.B7 = 0;
            
            #if defined(USE_PULLUP)
            PORT1.PCR.BIT.B6 = 1;       /* Enable port pin pull-up resistor. */
            PORT1.PCR.BIT.B7 = 1; 
            #endif 
    
            MPC.P16PFS.BIT.PSEL = 0x0F;	/* Select P16 as SCL2, No IRQ. */
            MPC.P17PFS.BIT.PSEL = 0x0F;	/* Select P17 as SDA2, No IRQ. */

            PORT1.PMR.BIT.B6 = 1;       /* Set as peripheral function I/O. */
            PORT1.PMR.BIT.B7 = 1;               
        break;  
               
        case 3:
            PORTC.PMR.BIT.B0 = 0;       /* Set pins as regular IO until pin function set up. */  
            PORTC.PMR.BIT.B1 = 0;

            PORTC.PDR.BIT.B0 = 0;       /* Clear PDR to 0 to select input mode. */
            PORTC.PDR.BIT.B1 = 0;
            
            #if defined(USE_PULLUP)
            PORTC.PCR.BIT.B0 = 1;       /* Enable port pin pull-up resistor. */
            PORTC.PCR.BIT.B1 = 1; 
            #endif 
    
            MPC.PC0PFS.BIT.PSEL = 0x0F;	/* Select P12 as SCL3, No IRQ. */
            MPC.PC1PFS.BIT.PSEL = 0x0F;	/* Select P13 as SDA3, No IRQ. */

            PORTC.PMR.BIT.B0 = 1;       /* Set as peripheral function I/O. */
            PORTC.PMR.BIT.B1 = 1;               
        break;
        #endif
#endif 
       
        default:
            ret = RIIC_NO_CHANNEL;  /* Channel requested is not a valid RIIC channel. */
        break;                                                    
    }
    
#if defined(MCU_RX630) || defined(MCU_RX63N)          
    /* Done with port function settings so lock the port function register */
    MPC.PWPR.BIT.PFSWE = 0; /* Disable writing to PFS */
    MPC.PWPR.BIT.B0WI = 1; 	/* Disable writing to PFSWE. */ 
#endif 
    return ret;
           
} /* End of function InitRiicPorts() */


/*******************************************************************************
* Function Name: R_RIIC_Init 
* Description  : Initializes the RIIC peripheral for master or slave mode
* Arguments    : *settings -
*                    Pointer to structure containing initialization parameters.
*                    Structure defined in r_riic_rx600.h
* Return Value : RIIC_OK -
*                    RIIC channel was initialized.
*                RIIC_BUSY_TMO -
*                    Channel is busy. Timeout occurred.
*                RIIC_LOCKED -
*                    RIIC channel was already locked for another operation.
*                RIIC_NO_CHANNEL -
*                    Channel requested is not a valid RIIC channel.
*******************************************************************************/
riic_ret_t R_RIIC_Init(riic_config_t * settings)
{ 
    /* Return value */
    riic_ret_t ret;
    
    uint8_t channel = settings->riic_channel;

    /* Check to see if this channel is already initialized */
    if (true == riic_initialized[channel])
    {
        /* This channel has already been initialized. */
        return RIIC_OK;
    }
    
    /* Try to lock this channel. */
    ret = riic_lock(channel); 

    /* Check result */
    if ( RIIC_OK != ret )
    {
        /* Either already locked or invalid channel. */
        return ret;
    }    

    /* All initialization that is channel specific and cannot be done through
       the g_riic_channels[] array should be placed here. Examples include 
       port pins, MSTP, and interrupts. */
       
    riic_ports_init(channel);       

#if defined(MCU_RX630) || defined(MCU_RX63N)    
    SYSTEM.PRCR.WORD = 0xA502;	/*b1: Enable writes to module stop registers */
#endif

    switch (channel)
    {
        case 0:
            MSTP(RIIC0) = 0;    /* Clear module stop state. */       
        break;
        
#if defined(PACKAGE_LQFP100) || defined(PACKAGE_LQFP80)  
        #if defined(MCU_RX630)      
        case 1: /* Channel here is an index, not physical channel number. So translate to physical channel. */
            MSTP(RIIC2) = 0;    /* Clear module stop state. */              
        break; 
        #endif
                
#elif defined(PACKAGE_LQFP176) || defined(PACKAGE_LFBGA176) || defined(PACKAGE_LQFP144) || defined(PACKAGE_TFLGA85)       
        case 1:
            MSTP(RIIC1) = 0;    /* Clear module stop state. */              
        break;
        
        #if defined(MCU_RX630) 
        case 2:
            MSTP(RIIC2) = 0;    /* Clear module stop state. */              
        break;        
        
        case 3:
            MSTP(RIIC3) = 0;    /* Clear module stop state. */             
        break;
        #endif
#endif 
       
        default:
            return RIIC_NO_CHANNEL;  /* Channel requested is not a valid RIIC channel. */
        break;                                                    
    }

#if defined(MCU_RX630) || defined(MCU_RX63N)      
    SYSTEM.PRCR.WORD = 0xA500;      /* Lock writes to module stop registers */
#endif
              
    /* I2C Bus Control Register 1 (ICCR1)      */       
    /*  b7   b6    b5  b4   b3   b2   b1   b0  */
    /* ICE IICRST CLO SOWP SCLO SDAO SCLI SDAI */
    /*	  |   |    |    |    |    |    |    |
          |   |    |    |    |    |    |    +-- SDA Bus Input Monitor.	 
          |   |    |    |    |    |    +------- SCL Bus Input Monitor.
          |   |    |    |    |    +------------ SDA Output Control.   
          |   |    |    |    +----------------- SCL Output Control.
          |   |    |    +---------------------- SCLO/SCLO/SDAO Write Protect.   
          |   |    +--------------------------- Extra SCL Clock Cycle Output.   
          |	  +-------------------------------- I2C Bus Interface Internal Reset.   
          +------------------------------------ I2C Bus Interface Enable.   */          
    /* To prevent unexpected communications, set up RIIC registers with
    ICE bit set to 0 (to disable the RIIC), and set the ICE bit to 1
    (to enable the transfer operation) after finishing all register settings. */
    (*g_riic_channels[channel]).ICCR1.BIT.ICE = 0;        /* RIIC function disabled */
      
    /* With ICE set to 0, reset all registers and internal states of the RIIC with
    IICRST = 1. */
    (*g_riic_channels[channel]).ICCR1.BIT.IICRST = 1;     /* RIIC internal reset */
    (*g_riic_channels[channel]).ICCR1.BIT.IICRST = 0;     /* Cancel RIIC internal reset */


    /* Slave Address Register Ly (SARLy) (y = 0 to 2)                         */
    /* SARL sets 7-bit slave address or lower eight bits of 10-bit address.   */
    /*  b7  b6  b5  b4  b3  b2  b1  b0 
       |-------SVA[6:0]-----------|SVA0|
                   |                 |
                   |                 +-- SVA0 10-Bit Address LSB
                   +-------------------- SVA[6:0] 7-Bit Address/10-Bit lower bits */
    (*g_riic_channels[channel]).SARL0.BYTE = settings->self_slave_addr_lo; 

    /* Slave Address Register Uy(SARUy) selects 7-bit address format or 10-bit 
    /* address format and sets the upper bits of a 10-bit slave address.       */
    /*   b7  b6  b5  b4  b3  b2    b1  b0
        | �         �         �         �          �  | SVA[1:0]| FS|
                 |              |       |
                 |              |       +-- 7-Bit/10-Bit Address Format: 0: 7-bit 
                 |              +---------- SVA[1:0] 10-Bit Address Upper Bits
                 +------------------------- Reserved. Always read/write 0.     */
    (*g_riic_channels[channel]).SARU0.BYTE = settings->self_slave_addr_hi;	
    

    /* I2C Bus Status Enable Register (ICSER) */
    /*   b7   b6  b5   b4  b3    b2    b1    b0
        HOAE  �  DIDE  �  GCAE SAR2E SAR1E SAR0E 
          |   |   |    |    |    |     |     |
          |   |   |    |    |    |     |     +-- Slave Address Register 0 Enable.	 
          |   |   |    |    |    |     +-------- Slave Address Register 1 Enable.
          |   |   |    |    |    +-------------- Slave Address Register 2 Enable.   
          |   |   |    |    +------------------- General Call Address Enable.
          |   |   |    +------------------------ Reserved: R/W value always 0.   
          |   |   +----------------------------- Device-ID Address Detection Enable.   
          |	  +--------------------------------- Reserved: R/W value always 0.   
          +------------------------------------- Host Address Enable.         */    
    (*g_riic_channels[channel]).ICSER.BYTE = 0x09; /* Set to default reset value for now */
    
    
    /* ICMR1.BIT.CKS = CKS[2:0] Internal Reference Clock (IRC) Selection  */
    (*g_riic_channels[channel]).ICMR1.BIT.CKS = PCLK_DIVISOR;  /* e.g.: PCLK/8 clock. 48/8 = 6 MHz. */ 
    
    /* I2C transfer rate and the SCL clock duty calculated as follows:    */
    /* Transfer rate = 1 / {[(ICBRH + 1) + (ICBRL + 1)] / IRC             */  
    /*                      + SCL line rising time  [tr]                  */  
    /*                      + SCL line falling time [tf]}                 */
    /* Duty cycle = {SCL line rising time  [tr] + (ICBRH + 1) / IRC}      */
    /*            : {SCL line falling time [tf] + (ICBRL + 1) / IRC}      */
    /* Note: tr and tf depend on bus capacitance and pullup resistors.    */

    /* Set bit rate registers for ~100 kbps, ~50% duty. */
    /* 100000 = 1 / (((ICBRH + 1) + (ICBRL + 1)) / IRC) + tr + tf)
       for: tr  = 1000nS  (Max I2C spec for 100kbps)
            tf  = 300nS   (Max I2C spec for 100kbps)
            IRC = 6000000 (6MHz)
       100000 = 1/(((ICBRH + 1) + (ICBRL + 1))/6000000) + .000001 + .0000003)
       50.2 = ICBRH + ICBRL
       51   =   23  +  28  round to whole numbers */           
    (*g_riic_channels[channel]).ICBRH.BIT.BRH = BITRATE_HIGH_PERIOD; /* Defined in r_riic_rx600_config.h */
    (*g_riic_channels[channel]).ICBRL.BIT.BRL = BITRATE_LOW_PERIOD;

 
    /* I2C Bus Mode Register 2 (ICMR2) */       
    /*   b7   b6  b5 b4   b3 b2   b1   b0
        DLCS  SDDL[2:0]   � TMOH TMOL TMOS 
          |   |   |   |   |   |   |    |
          |   |   |   |   |   |   |    +-- Timeout Detection Time Selection. 0: Long mode, 1: Short mode.	 
          |   |   |   |   |   |   +------- Timeout L Count Control. 0: Count disabled while SCLn line low.
          |   |   |   |   |   +----------- Timeout H Count Control  0: Count disabled while SCLn line high. 
          |   |   |   |   +--------------- Reserved: R/W value always 0.
          |   |   |   +------------------- b6-b4 SDDL[2:0] SDA Output Delay Counter   
          |   |   +-----------------------   
          |	  +---------------------------    
          +------------------------------- SDA Output Delay Clock Source */
    /* Value after reset: 0 0 0 0 0 1 1 0    */
    /* Timeout settings */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOS = 0;   /* Select long mode (16-bit count). */ 
    (*g_riic_channels[channel]).ICMR2.BIT.TMOL = 1;   /* Enable count while SCL line stuck low. */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOH = 1;   /* Enable count while SCL line stuck high. */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOL = 0;   /* disable count while SCL line stuck low. */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOH = 0;   /* disable count while SCL line stuck high. */
  
    
    /* Set the SDA delay counter. Depends on I2C bus spec and data rate          */
    /* For 100 kbps: tVD;DAT, tVD;ACK data valid time = 3450nS MAX.              */
    /* Adjust SDDL delay as required by bus electrical characteristics to remain */
    /* within spec for tVD;DAT, tVD;ACK.                                         */
    /* Internal Reference Clock(IRC) = 48/8 = 166nS/clock.                       */           
    (*g_riic_channels[channel]).ICMR2.BIT.SDDL = 0;   /* Delay for 0 clocks. */   
#if(0) /* Alternative use of SDA delay feature */
    (*g_riic_channels[channel]).ICMR2.BIT.SDDL = 7;   /* Delay for 7 clocks = 1.66*7 = 1166nS delay. */   
#endif                 
    (*g_riic_channels[channel]).ICMR2.BIT.DLCS = 0;   /* IRC/1 selected as clock of the SDA output delay counter */              

    /* I2C Bus Mode Register 3 (ICMR3) */
    /*  b7   b6    b5   b4     b3    b2   b1  b0   */
    /* SMBS WAIT RDRFS ACKWP ACKBT ACKBR  NF[1:0]  */
    /*   |    |    |     |     |     |    |    |
         |    |    |     |     |     |    |    +-- Bits [1:0] Noise Filter Stage Selection	 
         |    |    |     |     |     |    +------- 
         |    |    |     |     |     +------------ Receive Acknowledge.  0: 0 received as the acknowledge bit.
         |    |    |     |     |                                         1: 1 received as the acknowledge bit.
         |    |    |     |     +------------------ Transmit Acknowledge. 0: 0 sent as the acknowledge bit.
         |    |    |     |                                               1: 1 sent as the acknowledge bit.
         |    |    |     +------------------------ ACKBT Write Protect. 0: Write to ACKBT bit is disabled   
         |    |    +------------------------------ RDRF Flag Set Timing Selection  
         |	  +----------------------------------- WAIT.                 0: No WAIT, 1: WAIT
         +---------------------------------------- SMBus/I2C Bus Selection. 0: I2C bus, 1: SMBus. */   
    (*g_riic_channels[channel]).ICMR3.BIT.ACKWP = 1;  /* Disable ACKBT protect. */

#ifdef WAIT_OPTION_ON
    /* Slave holds SCL low while waiting to process a received byte. */
    /* Allows operation in a one byte at a time manner. */    
    (*g_riic_channels[channel]).ICMR3.BIT.WAIT = 1;   
#else
    /* Slave operates in double-buffer mode and continues to receive another byte */
    /* of data in ICDSR even if ICDRR is not read before new data is arriving. */
    /* However, when both ICDRR and ICDSR are full then the SCL line will be held low. */ 
    (*g_riic_channels[channel]).ICMR3.BIT.WAIT = 0;    
#endif

    /* I2C Bus Function Enable Register (ICFER) */
    /*  b7   b6   b5   b4   b3   b2   b1   b0   */
    /* FMPE SCLE NFE NACKE SALE NALE MALE TMOE  */
    /*   |    |   |    |    |    |    |    |
         |    |   |    |    |    |    |    +-- Timeout Function Enable	 
         |    |   |    |    |    |    +------- Master Arbitration Lost Detection Enable
         |    |   |    |    |    +------------ NACK Transmission Arbitration Lost Detection Enable 
         |    |   |    |    +----------------- Slave Arbitration Lost Detection Enable
         |    |   |    +---------------------- NACK Reception Transfer Suspension Enable   
         |    |   +--------------------------- Digital Noise Filter Circuit Enable  
         |	  +------------------------------- SCL Synchronous Circuit Enable   
         +------------------------------------ Fast-mode Plus Enable. 
      Note: FMPE only available on channel 0. All others write only 0.
       Value after reset: 0 1 1 1 0 0 1 0            */
    (*g_riic_channels[channel]).ICFER.BYTE = 0x72;        /* Use default reset value for now. */
    #ifdef USE_FASTMODE_PLUS
    (*g_riic_channels[channel]).ICFER.BIT.FMPE = 1;  /* fast mode+ enable */
    #endif

    /* I2C Bus Interrupt Enable Register (ICIER) */
    /*  b7   b6   b5   b4   b3   b2   b1   b0    */
    /* TIE  TEIE RIE NAKIE SPIE STIE ALIE TMOIE  */
    /*   |    |   |    |    |    |    |    |
         |    |   |    |    |    |    |    +-- Timeout Interrupt Enable.	 
         |    |   |    |    |    |    +------- Arbitration Lost Interrupt Enable.
         |    |   |    |    |    +------------ Start Condition Detection Interrupt Enable. 
         |    |   |    |    +----------------- Stop Condition Detection Interrupt Enable.
         |    |   |    +---------------------- NACK Reception Interrupt Enable.   
         |    |   +--------------------------- Receive Data Full Interrupt Enable.  
         |	  +------------------------------- Transmit End Interrupt Enable.   
         +------------------------------------ Transmit Data Empty Interrupt Enable. */     
    (*g_riic_channels[channel]).ICIER.BYTE = 0x00;        /* All interrupts disabled for now. */ 
    while ((*g_riic_channels[channel]).ICIER.BYTE)
    {
        /* Wait for pipelining to complete. */    
    }
      
    /* Set up ICU interrupt settings. */
    /* Enables all interrupts that will be used and clears any pending interrupts */
    /* Do this before enabling the RIIC for transfer operations */  
    if(RIIC_SLAVE_CONFIG == settings->configuration) 
    { /* Only slave is supporting interrupts in this implementation. */
        riic_interrupts_init(channel, INTERRUPTS_EN);
    } 
    else
    {
        riic_interrupts_init(channel, INTERRUPTS_DIS);        
    }                                                   
      
    /* RIIC enable. */
    (*g_riic_channels[channel]).ICCR1.BIT.ICE = 1;        /*RIIC transfer operation enabled. */

    /* I2C Bus Status Register 2 (ICSR2) */       
    /*   b7   b6   b5   b4    b3   b2   b1  b0
        TDRE TEND RDRF NACKF STOP START AL TMOF
          |    |    |    |     |    |    |   |
          |    |    |    |     |    |    |   +-- Timeout Detection Flag.   1: timeout.	 
          |    |    |    |     |    |    +------ Arbitration Lost Flag.    1: lost arbitration.
          |    |    |    |     |    +----------- Start Condition Detection Flag. 1: start detected.  
          |    |    |    |     +---------------- Stop Condition Detection Flag.  1: stop detected.
          |    |    |    +---------------------- NACK Detection Flag. 1: NACK detected.  
          |    |    +--------------------------- Receive Data Full Flag.   1: receive data in ICDRR.   
          |    +-------------------------------- Transmit End Flag.        1: Data has been transmitted.  
          +------------------------------------- Transmit Data Empty Flag. 1: no transmit data in ICDRT */     
    /* Clear Nack and Stop status. */
    (*g_riic_channels[channel]).ICSR2.BIT.NACKF = 0;
    (*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;
     
    /* Initialization complete. */
    riic_initialized[channel] = true;

    /* Give up lock */
    riic_unlock(channel);
    
    g_riic_mode[channel] = RIIC_IDLE_MODE;

    return RIIC_OK;          
} /* End of function R_RIIC_Init() */


/*******************************************************************************
* Function Name: R_RIIC_ReleaseChannel
* Description  : Release RIIC channel to allow it to be re-initialized if needed.
* Arguments    : channel -
*                    Which channel to use.
* Return Value : RIIC_OK -
*                    channel released.
*                RIIC_NO_CHANNEL -
*                    Invalid channel number input.
*******************************************************************************/
riic_ret_t R_RIIC_ReleaseChannel(uint8_t channel)
{
    /* First, check to make sure this is valid channel. */
    if (RIIC_NUM_CHANNELS <= channel)
    {
        /* Invalid channel number. */
        return RIIC_NO_CHANNEL;
    }
    riic_initialized[channel] = false;
    return RIIC_OK;    
} /* End of function R_RIIC_ReleaseChannel() */


/*******************************************************************************
* Function Name: R_RIIC_Reset
* Description  : This function reset RIIC. Then do return processing.
*                Return processing is 
*                Step1 - Rest RIIC (try to be SCL=High and SDA=Hgih)
*                        If SCL=Low though reset RIIC, other device out 
*                        put SCL=Low
*                Step2 - If SDA=Low, RIIC generates some clocks
*                Step3 - After SDA=High, RIIC generates stop condition.
* Arguments    : channel -
*                    Which RIIC channel to use
* Return Value : RIIC_LOCKED -
*                    RIIC channel was already locked for another operation.
*                RIIC_NO_CHANNEL -
*                    Channel requested is not a valid RIIC channel.
*******************************************************************************/
riic_ret_t R_RIIC_Reset(uint8_t channel)
{
	volatile uint16_t count;
    riic_ret_t result;

    /* Try to lock this channel. */
    result = riic_lock(channel); 

    /* Check result */
    if ( RIIC_OK != result )
    {
        /* Either already locked or invalid channel. */
        return result;
    }  

    /* First try to stop any active transfer. */
    if(1 == (*g_riic_channels[channel]).ICSR2.BIT.START )
    { 
		/* Generate stop condition */
		(*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;
		(*g_riic_channels[channel]).ICCR2.BIT.SP = 1;        
        /* Do a dummy read. */        
        (*g_riic_channels[channel]).ICDRR;
        
        /* Wait for stop condition detected. */
        count = 0x100;  /* Limit the time this condition is tested for. */
        while ((0 == (*g_riic_channels[channel]).ICSR2.BIT.STOP) && (0 < count))
        {
            count--;
        }          
                
    }

	/* ICE=1 and IICRST=1 clear some status flags. */
	/* This reset doesn't clear some settings, baud rate, slave address 
       and so on.*/
	/* Reset RIIC */
    (*g_riic_channels[channel]).ICCR1.BIT.IICRST = 1;			

	/* Time out setting */
	/* Disable time out detection */
    (*g_riic_channels[channel]).ICFER.BIT.TMOE = 0;			
	/* Disable SCL=Low time out */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOL = 1;			
	/* Enable SCL=High time out */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOH = 1;			
	/* Long mode (16bits counter), PCLK=48MHz,		*/
    (*g_riic_channels[channel]).ICMR2.BIT.TMOS = 0;			
    /* IIC phi=48MHz, Long mode is about 1.364 ms	*/										

	/* Clear reset */
    (*g_riic_channels[channel]).ICCR1.BIT.IICRST = 0;			

	for(count = 0; count < 100; count++)
    {
        /* Delay. */
        /* If RIIC output SDA = Low, SDA output is released	    */
        /* by IICRST = 1. But IIC bus is pulled up therefore 	*/
        /* the SDA rising is not immediate.					    */
    }

	/* Enable time out detection */
    (*g_riic_channels[channel]).ICFER.BIT.TMOE = 1;			

	/* After reset RIIC, this flow generates Stop condition.			*/
	/* But MST bit has to be set when RIIC generates Stop condition.	*/
	/* Here, disable protect, then set TRS and MST bits.				*/
	/* If setting only MST, RIIC generates some clocks because RDRF=0.  */
	/* When MST=1, TRS=0 and RDRF=0, RIIC starts Master Receive.		*/
	(*g_riic_channels[channel]).ICMR1.BIT.MTWP = 1;
	(*g_riic_channels[channel]).ICCR2.BIT.TRS = 1;
	(*g_riic_channels[channel]).ICCR2.BIT.MST = 1;

	/* If other devices output SDA=Low, RIIC generates some clocks.	 	*/
	/* Usually, devices release SDA=Low when they recieve some clocks.  */
	/* Check SDA level */
    if((*g_riic_channels[channel]).ICCR1.BIT.SDAI == 0)		
	{
		/* Generate 10 clocks until SDA=High */
        for(count=0; count<10; count++)
        {		
			if((*g_riic_channels[channel]).ICCR1.BIT.SDAI == 0)
			{
				/* Generate a clock */
                (*g_riic_channels[channel]).ICCR1.BIT.CLO = 1;				
                
				/* Wait to complete the clock */
                while((*g_riic_channels[channel]).ICCR1.BIT.CLO != 0)		
				{
					/* When other device output SCL = Low,	*/
                    if((*g_riic_channels[channel]).ICSR2.BIT.TMOF !=0)		
					{	
                        /* Time out occurred. */							
						(*g_riic_channels[channel]).ICSR2.BIT.TMOF =0;
						break;
					}
				}
			}
            else
			{
				/* When SDA=High, stop generating clocks */
                break;					
			}

			/* Call error function if SDA = Low after RIIC generates some clocks */
			if((count == 9)&&((*g_riic_channels[channel]).ICCR1.BIT.SDAI == 0))
			{
             /*   while(1);*/
			}
		}
	}

	/* When Bus is free, RIIC does't generate Stop condition */
    if((*g_riic_channels[channel]).ICCR2.BIT.BBSY == 0)		
	{
		/* Reset RIIC */
        (*g_riic_channels[channel]).ICCR1.BIT.IICRST = 1;		
		/* Clear reset */
        (*g_riic_channels[channel]).ICCR1.BIT.IICRST = 0;		
	}else
	{
		/* Generate stop condition */
		(*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;
		(*g_riic_channels[channel]).ICCR2.BIT.SP = 1; 
        
        /* Do a dummy read. (See Master Reception flowchart.) */        
        (*g_riic_channels[channel]).ICDRR;                         
	}

	/* Enable MST/TRS Write Protect */
	(*g_riic_channels[channel]).ICMR1.BIT.MTWP = 0;

	/* Set time out */
	/* Disable time out detection */
    (*g_riic_channels[channel]).ICFER.BIT.TMOE = 0;			
	/* Disable SCL=Low time out */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOL = 0;			
	/* Enable SCL=High time out */
    (*g_riic_channels[channel]).ICMR2.BIT.TMOH = 1;			
	/* Long mode (16bits counter), PCLK=48MHz,		*/
    (*g_riic_channels[channel]).ICMR2.BIT.TMOS = 0;			
    /* IIC phi=48MHz. Long mode is about 1.365 ms	*/
	
    /* Enable time out detection */
    (*g_riic_channels[channel]).ICFER.BIT.TMOE = 1;
    
    /* Give up lock */
    riic_unlock(channel);

    if((*g_riic_channels[channel]).ICCR2.BIT.BBSY == 1)
    {
        /* Still busy. soft-reset failed. Resort to complete reset. */
        return RIIC_RESET_ERR; /* Channel will need to be re-initialized now. */            
    }
    
    return RIIC_OK;        	   
} /* End of function R_RIIC_Reset() */


/*******************************************************************************
* Function Name : riic_interrupts_init
* Description	: Sets up the ICR registers and the interrupt enable bits in the RIIC
* Arguments 	: RIIC channel, enable 
* Return Value  : RIIC_OK - success.
*                 RIIC_NO_CHANNEL - Channel requested is not a valid RIIC channel.
* Note: this code is using locally re-defined macros. See  macro section above.
*******************************************************************************/   
static riic_ret_t riic_interrupts_init(uint8_t channel, uint8_t enable)
{      
    /* Return value */
    riic_ret_t ret = RIIC_OK;  
    
    switch (channel)
    {
        case 0:     
            /* Clear any pending interrupts stored in ICU. */
            X_IR(RIIC0, TXI0) = 0;
            X_IR(RIIC0,RXI0) = 0;
            
            /* Set interrupt priorities in ICU */            
            X_IPR(RIIC0, RXI0) = RIIC_INT_PRIO;
            X_IPR(RIIC0, TXI0) = RIIC_INT_PRIO;
            X_IPR(RIIC0, TEI0) = RIIC_INT_PRIO;     
            X_IPR(RIIC0, EEI0) = RIIC_INT_PRIO; 

            /* Enable interrupts in ICU */   
            X_IEN(RIIC0, RXI0) = enable;
            X_IEN(RIIC0, TXI0) = enable;
            X_IEN(RIIC0, TEI0) = enable;    
            X_IEN(RIIC0, EEI0) = enable;             
        break;

    #if defined(MCU_RX62N)        
        #if defined(PACKAGE_LFBGA176) || defined(PACKAGE_LQFP144)                    
        case 1:
            /* Clear any pending interrupts stored in ICU. */
            X_IR(RIIC1, TXI1) = 0;
            X_IR(RIIC1,RXI1) = 0;                     
            
            /* Set interrupt priorities in ICU */            
            X_IPR(RIIC1, RXI1) = RIIC_INT_PRIO;
            X_IPR(RIIC1, TXI1) = RIIC_INT_PRIO;
            X_IPR(RIIC1, TEI1) = RIIC_INT_PRIO;     
            X_IPR(RIIC1, EEI1) = RIIC_INT_PRIO; 

            /* Enable interrupts in ICU */   
            X_IEN(RIIC1, RXI1) = enable;
            X_IEN(RIIC1, TXI1) = enable;
            X_IEN(RIIC1, TEI1) = enable;    
            X_IEN(RIIC1, EEI1) = enable;  
        break;
        #endif      
        
    #elif defined(MCU_RX630) || defined(MCU_RX63N)           
        #if defined(PACKAGE_LQFP100) || defined(PACKAGE_LQFP80)     
        case 1:   
        #elif defined(PACKAGE_LQFP176) || defined(PACKAGE_LQFP144) 
        case 2:
        #endif
            /* Clear any pending interrupts stored in ICU. */
            X_IR(RIIC2, TXI2) = 0;
            X_IR(RIIC2,RXI2) = 0;                      
            
            /* Set interrupt priorities in ICU */            
            X_IPR(RIIC2, RXI2) = RIIC_INT_PRIO;
            X_IPR(RIIC2, TXI2) = RIIC_INT_PRIO;
            X_IPR(RIIC2, TEI2) = RIIC_INT_PRIO;     
            X_IPR(RIIC2, EEI2) = RIIC_INT_PRIO; 

            /* Enable interrupts in ICU */   
            X_IEN(RIIC2, RXI2) = enable;
            X_IEN(RIIC2, TXI2) = enable;
            X_IEN(RIIC2, TEI2) = enable;    
            X_IEN(RIIC2, EEI2) = enable;  
        break;
        
        #if defined(PACKAGE_LQFP176) || defined(PACKAGE_LQFP144)
        case 1:
            /* Clear any pending interrupts stored in ICU. */
            X_IR(RIIC1, TXI1) = 0;
            X_IR(RIIC1,RXI1) = 0;                     
            
            /* Set interrupt priorities in ICU */            
            X_IPR(RIIC1, RXI1) = RIIC_INT_PRIO;
            X_IPR(RIIC1, TXI1) = RIIC_INT_PRIO;
            X_IPR(RIIC1, TEI1) = RIIC_INT_PRIO;     
            X_IPR(RIIC1, EEI1) = RIIC_INT_PRIO; 

            /* Enable interrupts in ICU */   
            X_IEN(RIIC1, RXI1) = enable;
            X_IEN(RIIC1, TXI1) = enable;
            X_IEN(RIIC1, TEI1) = enable;    
            X_IEN(RIIC1, EEI1) = enable;  
        break;
                
        case 3:
            /* Clear any pending interrupts stored in ICU. */
            X_IR(RIIC3, TXI3) = 0;
            X_IR(RIIC3,RXI3) = 0;                      
            
            /* Set interrupt priorities in ICU */            
            X_IPR(RIIC3, RXI3) = RIIC_INT_PRIO;
            X_IPR(RIIC3, TXI3) = RIIC_INT_PRIO;
            X_IPR(RIIC3, TEI3) = RIIC_INT_PRIO;     
            X_IPR(RIIC3, EEI3) = RIIC_INT_PRIO; 

            /* Enable interrupts in ICU */   
            X_IEN(RIIC3, RXI3) = enable;
            X_IEN(RIIC3, TXI3) = enable;
            X_IEN(RIIC3, TEI3) = enable;    
            X_IEN(RIIC3, EEI3) = enable;  
        break; 
        #endif
    #endif
    
        default:
            ret = RIIC_NO_CHANNEL;  /* Channel requested is not a valid RIIC channel. */
        break;                                                    
    }
    
    /* Enable these interrupts in the RIIC peripheral as required by user application. */
    /* I2C Bus Interrupt Enable Register (ICIER) */
    /*  b7   b6   b5   b4   b3   b2   b1   b0    */
    /* TIE  TEIE RIE NAKIE SPIE STIE ALIE TMOIE  */
    /*   |    |   |    |    |    |    |    |
         |    |   |    |    |    |    |    +-- Timeout Interrupt Enable.	 
         |    |   |    |    |    |    +------- Arbitration Lost Interrupt Enable.
         |    |   |    |    |    +------------ Start Condition Detection Interrupt Enable. 
         |    |   |    |    +----------------- Stop Condition Detection Interrupt Enable.
         |    |   |    +---------------------- NACK Reception Interrupt Enable.   
         |    |   +--------------------------- Receive Data Full Interrupt Enable.  
         |	  +------------------------------- Transmit End Interrupt Enable.   
         +------------------------------------ Transmit Data Empty Interrupt Enable. */   

    if (enable)
    {
        (*g_riic_channels[channel]).ICIER.BIT.TIE  = 1;       
        (*g_riic_channels[channel]).ICIER.BIT.RIE  = 1;
        (*g_riic_channels[channel]).ICIER.BIT.NAKIE  = 1;     
        (*g_riic_channels[channel]).ICIER.BIT.SPIE  = 1;
        (*g_riic_channels[channel]).ICIER.BIT.STIE  = 1;  
        (*g_riic_channels[channel]).ICIER.BIT.TEIE = 1;        
    #if (0) /* Not currently supported in this driver */
        (*g_riic_channels[channel]).ICIER.BIT.ALIE  = 1;
        (*g_riic_channels[channel]).ICIER.BIT.TMOIE  = 1;          
    #endif  
    }

    return ret;    
} /* End of function riic_interrupts_init() */


/*******************************************************************************
* Function Name: riic_lock
* Description  : Get lock on RIIC access.
* Arguments    : channel -
*                    Which channel to use.
* Return Value : RIIC_OK -
*                    Lock acquired.
*                RIIC_NO_CHANNEL -
*                    Invalid channel number input.
*                RIIC_LOCKED - 
*                    Channel already locked.
*******************************************************************************/
riic_ret_t riic_lock(uint8_t channel)
{
    /* Return variable */
    riic_ret_t ret = RIIC_OK;
    /* Variable used in trying to grab semaphore. Using the xchg instruction 
       makes this atomic */
    int32_t semaphore = 1;

    /* First, check to make sure this is valid channel. */
    if (RIIC_NUM_CHANNELS <= channel)
    {
        /* Invalid channel number. */
        return RIIC_NO_CHANNEL;
    }

    /* Try to grab semaphore to change state */
    xchg(&semaphore, &riic_semaphore);
    
    /* Check to see if semaphore was successfully taken */
    if( semaphore == 0 )
    {
        /* Only give lock if no program has the lock already */
        if ( false == g_riic_locked[channel] )
        {
            /* Lock has been obtained */
            g_riic_locked[channel] = true;
        }
        else
        {
            /* Lock has already been taken */
            ret = RIIC_LOCKED;
        }

        /* Release semaphore */
        riic_semaphore = 0;
    }
    else
    {
        /* Semaphore was not taken. Another task is in this function
           and has taken the semaphore already. Try again later. */
        ret = RIIC_LOCKED;
    }

    return ret;
} /*  End of function riic_lock() */


/*******************************************************************************
* Function Name: riic_unlock
* Description  : Release RIIC lock so another task can use it
* Arguments    : channel -
*                    Which channel to use.
* Return Value : RIIC_OK -
*                    Lock relinquished.
*******************************************************************************/
riic_ret_t riic_unlock(uint8_t channel)
{
    /* Lock given back successfully */
    g_riic_locked[channel] = false;

    return RIIC_OK;
} /*  End of function riic_unlock() */

   
/*******************************************************************************
end  r_riic_rx600.c
*******************************************************************************/
