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
* File Name    : r_riic_rx600_master.c
* Version      : 1.00  
* Device(s)    : Renesas RX600 family
* Tool-Chain   : Renesas RX Standard Toolchain  
* H/W Platform : Generic RX600
* Description  : Polled mode RIIC driver functions for RIIC master 
*              : receive/transmit operations.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 25.10.2011 1.00    First Release    
*         : 23.11.2011 1.01       
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Fixed width integers */
#include <stdint.h>
/* Boolean defines */
#include <stdbool.h>
/* Used for xchg() intrinsic */
#include <machine.h>
/* Access to peripherals */
#include "platform.h"
/* RIIC driver */
#include "r_riic_rx600.h"

#include "r_riic_rx600_master.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* How many retries to attempt. 
   Adjust this if you want to alter how long to wait for status bits. */
#define RIIC_RETRY_START_COUNT	0xF0000

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static riic_ret_t nack_detected(uint8_t channel);
static riic_ret_t wait_for_status(uint8_t channel, riic_ret_t status);
static riic_ret_t riic_master_tx_byte(uint8_t channel, uint8_t tx_data);


/*******************************************************************************
Function definitions
*******************************************************************************/
/*******************************************************************************
* Function Name: R_RIIC_MasterTransmitHead
* Description  : Transmits header information on I2C bus to precede other write
*                or read operations. Starts out a transmission with a start 
*                condition but does not end with a stop. Leaves bus state ready
*                for continuing operations.		
* Parameters   : source_buf -
*                    Pointer to the buffer holding the data to be transmitted.         
*                nr_bytes -
*                    The number of bytes to transmit.
* Return Value : RIIC_OK -
*                    Operation completed successfully.
*                RIIC_BUSY_TMO -
*                    Channel is busy. Timeout occurred.
*                RIIC_TDRE_TMO -
*                    Timeout occured waiting for last transfer to finish.
*                RIIC_TEND_TMO -
*                    Timeout occurred waiting for data to finish transmitting.
*                RIIC_STOP_TMO - 
*                    Timeout waiting for detection of stop condition.
*                RIIC_LOCKED -
*                    RIIC channel was already locked for another operation.
*                RIIC_NO_CHANNEL -
*                    Channel requested is not a valid RIIC channel.
*                RIIC_NACK_ERR -
*                    Got a NACK when an ACK was expected.
*******************************************************************************/
riic_ret_t R_RIIC_MasterTransmitHead(uint8_t          channel,                                      
                                     uint8_t *        p_data_buff, 
                                     const uint32_t   num_bytes)
{
    uint32_t count; /* Local loop counter variable. */
    
    /* Return value */
    riic_ret_t ret = RIIC_OK;

    /* Try to lock this channel. */
    ret = riic_lock(channel); 

    /* Check result */
    if ( RIIC_OK != ret )
    {
        /* Either already locked or invalid channel. */
        return ret;
    }

    /* Wait for RIIC bus free. */
    if(RIIC_OK != wait_for_status(channel, RIIC_BUSY_TMO))
    {
        /* Give up lock */
        riic_unlock(channel);

        return RIIC_BUSY_TMO;            
    } 

    /* Generate start condition. */
    (*g_riic_channels[channel]).ICCR2.BIT.ST = 1;

    /* Check for NACK */
    ret = nack_detected(channel);

    if (RIIC_OK != ret)
    {
        /* Give up lock */
        riic_unlock(channel);

        return ret;
    }

    
    /* Transmit header. */
    for (count = 0; count < num_bytes; count++)
    {
        /* Send 1 byte at a time */
        ret = riic_master_tx_byte(channel, p_data_buff[count]);

        /* Check result. */
        if (RIIC_OK != ret)
        {
            /* Give up lock */
            riic_unlock(channel);

            return ret;
        }
    }

    /* Wait for complete end of transmission status. */
    ret = wait_for_status(channel, RIIC_TEND_TMO);
    if(RIIC_OK != ret)
    {
        ret |= RIIC_TEND_TMO;
        
        /* If NACK detected, it issues stop to complete the transmission. */   
        ret |= nack_detected(channel);

        /* Give up lock */
        riic_unlock(channel);             
        return ret;            
    }
    
    /* Check for NACK */
    ret = nack_detected(channel);

    if (RIIC_OK != ret)
    {
        /* Give up lock */
        riic_unlock(channel);
    }

    /* Since this is just sending the header, riic_unlock() will be called
       later from R_RIIC_MasterReceive() or R_RIIC_MasterTransmit(). */

    g_riic_mode[channel] = MASTER_TRANSMIT_MODE;

    return ret;   
} /* End of function R_RIIC_MasterTransmitHead() */


/*******************************************************************************
* Function Name: R_RIIC_MasterTransmit 
* Description  : Transmits data over RIIC. R_RIIC_MasterTransmitHead() should 
*                already have been called before this
* Arguments    : channel -
*                    Which RIIC channel to use
*                p_data_buff -
*                    Pointer to data buffer
*                num_bytes -
*                    Number of bytes to transmit
*					 If num_bytes = 0, then only the stop is processed.
* Return Value : RIIC_OK -
*                    Operation completed successfully.
*                RIIC_BUSY_TMO -
*                    Channel is busy. Timeout occurred.
*                RIIC_TDRE_TMO -
*                    Timeout occured waiting for last transfer to finish.
*                RIIC_TEND_TMO -
*                    Timeout occurred waiting for data to finish transmitting.
*                RIIC_STOP_TMO - 
*                    Timeout waiting for detection of stop condition.
*                RIIC_LOCKED -
*                    RIIC channel was already locked for another operation.
*                RIIC_NO_CHANNEL -
*                    Channel requested is not a valid RIIC channel.
*******************************************************************************/
riic_ret_t R_RIIC_MasterTransmit(uint8_t        channel,
                                 uint8_t *      p_data_buff, 
                                 const uint32_t num_bytes)
{
    uint32_t count; /* Local loop counter variable. */ 
    riic_ret_t ret = RIIC_OK;/* Return value */

    /* Test to see if this channel is already locked. It should be since
       R_RIIC_MasterTransmitHead() must be called before this function and
       it should already have locked the channel. If this returns that the lock
       was obtained then a function called this one out-of-turn. */
    if (RIIC_OK == riic_lock(channel))
    {
        /* This function was called out-of-turn. */
        /* Give up lock */
        riic_unlock(channel);

        return RIIC_LOCKED;
    }

	if(num_bytes > 0) /* Skip ahead and just do the Stop otherwise */
	{
		/* Transmit data. */
		for (count = 0; count < num_bytes; count++)
		{
			/* Send 1 byte at a time */
			ret = riic_master_tx_byte(channel, p_data_buff[count]);

			/* Check result. */
			if (RIIC_OK != ret)
			{
				/* Give up lock */
				riic_unlock(channel);

				return ret;
			}
		}

		/* Wait for transmission completed status. */
		if(RIIC_OK != wait_for_status(channel, RIIC_TEND_TMO))
		{
			/* Give up lock */
			riic_unlock(channel);

			return RIIC_TEND_TMO;
		}       

		/* Check for NACK */
		ret = nack_detected(channel);

		if (RIIC_OK != ret)
		{
			/* Give up lock */
			riic_unlock(channel);

			return ret;
		}

	}
    /* Request to issue a stop condition. */
    (*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;
    (*g_riic_channels[channel]).ICCR2.BIT.SP = 1;

    /* Wait for a detected stop condition. */
    ret = wait_for_status(channel, RIIC_STOP_TMO);
    
    if (RIIC_OK != ret)
    {
        /* Give up lock */
        riic_unlock(channel);

        return ret;
    }
          
    /* Clear status before exit. */
    (*g_riic_channels[channel]).ICSR2.BIT.NACKF = 0;     
    (*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;
    
    /* Give up lock */
    riic_unlock(channel);

    g_riic_mode[channel] = RIIC_IDLE_MODE;

    return RIIC_OK;
} /* End of function R_RIIC_MasterTransmit() */


/*******************************************************************************
* Function Name: R_RIIC_MasterReceive 
* Description  : Receive data over RIIC. 
*              : Supports entry on start or restart conditions.
*              : For entry on restart condition, R_RIIC_MasterTransmitHead() should 
*              : already have been called before this. 
* Arguments    : channel -
*                    Which RIIC channel to use
*                slave_addr -
*                    Slave address
*                p_data_buff -
*                    Pointer to data buffer
*                num_bytes -
*                    Number of bytes to read
* Return Value : RIIC_OK -
*                    Operation completed successfully.
*                RIIC_LOCKED -
*                    This task has not locked this RIIC channel yet.
*                RIIC_BUSY_TMO -
*                    Channel is busy. Timeout occurred.
*                RIIC_TDRE_TMO -
*                    Timeout occured waiting for last transfer to finish.
*                RIIC_TEND_TMO -
*                    Timeout occurred waiting for data to finish transmitting.
*                RIIC_STOP_TMO - 
*                    Timeout waiting for detection of stop condition.
*                RIIC_START_TMO -
*                    Timeout waiting for detection of start condition
*                RIIC_RDRF_TMO -
*                    Timeout waiting for receive data
*                RIIC_NACK_ERR - 
*                    NACK received.
*                RIIC_LOCKED -
*                    RIIC channel was already locked for another operation.
*                RIIC_NO_CHANNEL -
*                    Channel requested is not a valid RIIC channel.
*******************************************************************************/
riic_ret_t R_RIIC_MasterReceive(uint8_t         channel,
                                uint8_t         slave_addr,
                                uint8_t *       p_data_buff,
                                const uint32_t  num_bytes)
{
    /* Return value */
    riic_ret_t ret = RIIC_OK;
    /* Loop variable */
    uint32_t   count = 1; 

    if(RIIC_IDLE_MODE == g_riic_mode[channel])
    {            
        if(RIIC_OK != wait_for_status(channel, RIIC_BUSY_TMO))
        {
            return RIIC_BUSY_TMO;            
        } 
        
        (*g_riic_channels[channel]).ICSR2.BIT.START = 0;
            
        /* Issue start condition. */
        (*g_riic_channels[channel]).ICCR2.BIT.ST = 1;            
    }
    else if (MASTER_TRANSMIT_MODE == g_riic_mode[channel])        
    {
        /* Test to see if this channel is already locked. It should be since
           the mode is not IDLE. R_RIIC_MasterTransmitHead() must have been 
           called before this function and it should already have locked the 
           channel. If this returns that the lock was obtained then a function 
           called this one out-of-turn. */
        if ( RIIC_OK == riic_lock(channel))
        {
            /* This function was called out-of-turn. */
            /* Give up lock */
            riic_unlock(channel);

            return RIIC_LOCKED;
        }  
                
        /* Issue re-start condition. */
        (*g_riic_channels[channel]).ICSR2.BIT.START = 0;
        (*g_riic_channels[channel]).ICCR2.BIT.RS = 1;
    }
    else
    {
        riic_unlock(channel);            
        return RIIC_MODE_ERR; 
    }     


    if(RIIC_OK != wait_for_status(channel, RIIC_START_TMO))
    {
        /* Give up lock */
        riic_unlock(channel);

        return RIIC_START_TMO;            
    }

    /* Wait for TDRE status. */
    if(RIIC_OK != wait_for_status(channel, RIIC_TDRE_TMO))
    {
        /* Give up lock */
        riic_unlock(channel);

        return RIIC_TDRE_TMO;            
    }  
  
    /*** Send slave address + READ-bit (b7). ***/
    (*g_riic_channels[channel]).ICDRT = slave_addr | 0x01;

    /* Wait for receive data ready status. */
    if(RIIC_OK != wait_for_status(channel, RIIC_RDRF_TMO))
    {
        /* Give up lock */
        riic_unlock(channel);

        return RIIC_RDRF_TMO;            
    }      

    /* Check for NACK */
    ret = nack_detected(channel);

    if (RIIC_OK != ret)
    {
        /* Give up lock */
        riic_unlock(channel);

        return ret;
    }

    /* Dummy read ICDRR. Starts outputting clocks to perform real read. */
    (*g_riic_channels[channel]).ICDRR;
     
    while (count < (num_bytes))
    {
        /* Wait for receive data ready status. */
        if(RIIC_OK != wait_for_status(channel, RIIC_RDRF_TMO))
        {
            /* Give up lock */
            riic_unlock(channel);

            return RIIC_RDRF_TMO;            
        }

        /* Set the WAIT bit - (The period between ninth clock cycle and 
           first clock cycle is held low.) Low-hold is released by 
           reading ICDRR.*/
        if(count == (num_bytes - 2))
        {
            (*g_riic_channels[channel]).ICMR3.BIT.WAIT = 1;
        }
        /* Set bit to send NACK */ 
        else if (count == (num_bytes - 1))
        {            
            (*g_riic_channels[channel]).ICMR3.BIT.ACKBT = 1;                  
        }
        else
        {
            /* Do nothing. */
        }
        
        /* Copy data to application read buffer, and increment its pointer. */
        *p_data_buff = (*g_riic_channels[channel]).ICDRR;
        p_data_buff++;
        count++;
    }              

    /* If num_bytes is 1 then the while loop above will be skipped. The
       ACKBT bit still needs to be set. */
    if (num_bytes <= 1)
    {
        /* Set bit to send NACK */ 
        (*g_riic_channels[channel]).ICMR3.BIT.ACKBT = 1;
    }

    if(RIIC_OK != wait_for_status(channel, RIIC_RDRF_TMO)) 
    {
        /* Give up lock */
        riic_unlock(channel);

        return RIIC_RDRF_TMO;            
    }
      
    /* Issue Stop. */
    (*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;
    (*g_riic_channels[channel]).ICCR2.BIT.SP = 1;
    
    /* Read final byte. */       
    *p_data_buff = (*g_riic_channels[channel]).ICDRR;

    (*g_riic_channels[channel]).ICMR3.BIT.WAIT = 0;

    /* Wait for a detected stop condition. */
    if(RIIC_OK != wait_for_status(channel, RIIC_STOP_TMO))
    {
        /* Give up lock */
        riic_unlock(channel);
        ret = RIIC_STOP_TMO;            
    } 
 
    (*g_riic_channels[channel]).ICSR2.BIT.NACKF = 0;
    (*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;     

    /* Give up lock */
    riic_unlock(channel);

    g_riic_mode[channel] = RIIC_IDLE_MODE;

    return ret;
} /* End of function R_RIIC_MasterReceive() */


/*******************************************************************************
* Function Name: nack_detected 
* Description  : Check if a '0' was not issued in ack bit time slot. This is
*                called a "NACK".
* Arguments    : channel -
*                    Which RIIC channel to use
* Return Value : RIIC_OK -
*                    No NACK detected.
*                RIIC_BUSY_TMO -
*                    Channel is busy. Timeout occurred.
*                RIIC_STOP_TMO - 
*                    Timeout waiting for detection of stop condition.
*                RIIC_NACK_ERR -
*                    NACK detected.
*******************************************************************************/
static riic_ret_t nack_detected(uint8_t channel)
{
    volatile uint8_t tmp;    
     
    /* If NACK error, request a stop. */
    if (1 == (*g_riic_channels[channel]).ICSR2.BIT.NACKF)
    {
        /* NACK detected. */
        (*g_riic_channels[channel]).ICSR2.BIT.NACKF = 0;
        (*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;

        /* Issue stop. */
        (*g_riic_channels[channel]).ICCR2.BIT.SP = 1;

        /* Do a dummy read. (See Master Reception flowchart.) */
        tmp = (*g_riic_channels[channel]).ICDRR;

        /* Wait for a detected stop condition. */
        if(RIIC_OK != wait_for_status(channel, RIIC_STOP_TMO))
        {
            return RIIC_STOP_TMO;            
        }  

        if(RIIC_OK != wait_for_status(channel, RIIC_BUSY_TMO))
        {
            return RIIC_BUSY_TMO;            
        }
          
        (*g_riic_channels[channel]).ICSR2.BIT.STOP = 0;

        return RIIC_NACK_ERR;
    }    
    else
    {
        /* No NACK detected. */
        return RIIC_OK;
    }
} /* End of function nack_detected() */


/*******************************************************************************
* Function Name: wait_for_status 
* Description  : Implements a waiting mechanism for status flags. Has timeouts.
* Arguments    : channel -
*                    Which RIIC channel to use
*                status -
*                    Which status flag to wait on
* Return Value : RIIC_OK -
*                    No timeout.
*                Others -
*                    Timeout Occurred.
*******************************************************************************/
static riic_ret_t wait_for_status(uint8_t channel, riic_ret_t status)
{
    /* Counter variable */
    uint32_t retry_count;
    
    retry_count = RIIC_RETRY_START_COUNT; 
    
    /* Wait for status flag. */    
    switch (status)
    {
        /* Start Condition Detection Flag. */
        case RIIC_START_TMO:            
            while ((0 == (*g_riic_channels[channel]).ICSR2.BIT.START) && 
                   (0 < retry_count))
            {
                retry_count--;
            }        
        break; 
        
        /* Stop Condition Detection Flag */
        case RIIC_STOP_TMO:             
            while ((0 == (*g_riic_channels[channel]).ICSR2.BIT.STOP) && 
                   (0 < retry_count))
            {
                retry_count--;
            }        
        break;
        
        /* NACK Detection Flag. */
        case RIIC_NACK_ERR:            
        break;

        /* Receive Data Full Flag. */
        case RIIC_RDRF_TMO:             
            while ((0 == (*g_riic_channels[channel]).ICSR2.BIT.RDRF) && 
                   (0 < retry_count))
            {
            	retry_count--;
            }   
        break;
        
        /* Transmit End Flag */
        case RIIC_TEND_TMO:            
            while ((0 == (*g_riic_channels[channel]).ICSR2.BIT.TEND) && 
                   (0 < retry_count))        
            {
                retry_count--;
            }
            /* Clear the flag in the RIIC status register. */
            (*g_riic_channels[channel]).ICSR2.BIT.TEND = 0;                             
        break;
        
        /* Transmit Data Empty Flag */
        case RIIC_TDRE_TMO:    
            while ((0 == (*g_riic_channels[channel]).ICSR2.BIT.TDRE) && 
                   (0 < retry_count))
            {
                retry_count--;
            }
        break;
        
        /* Bus busy detection. */
        case RIIC_BUSY_TMO:
            while ((1 == (*g_riic_channels[channel]).ICCR2.BIT.BBSY) && 
                   (0 < retry_count))
            {
                retry_count--; 
            }
        break;     
    
        default:
        break;                    
    }

    /* Did timeout occur? */
    if (0 < retry_count)
    {
        /* Timeout did not occur. */
        status = RIIC_OK;
    }
    
    return status;
} /* End of function wait_for_status() */           


/*******************************************************************************
* Function Name: riic_tx_byte 
* Description  : Transmits one byte in master mode over RIIC channel
* Arguments    : channel -
*                    Which RIIC channel to use
*                tx_data -
*                    Byte to transmit
* Return Value : RIIC_OK -
*                    Byte transmitted successfully.
*                RIIC_TDRE_TMO -
*                    Timeout occured waiting for last transfer to finish.
*                RIIC_NACK_ERR -
*                    NACK detected.
*******************************************************************************/
static riic_ret_t riic_master_tx_byte(uint8_t channel, uint8_t tx_data)
{
    riic_ret_t result = RIIC_OK;
   
    /* Wait for TDRE status. */
    result = wait_for_status(channel, RIIC_TDRE_TMO);

    /* Proceed with write. */
    if (RIIC_OK == result)
    {
        /* Copying data to the TX register starts transmission. */
        (*g_riic_channels[channel]).ICDRT = (uint8_t)(tx_data); 

        /* Check for NACK. */
        result = nack_detected(channel);
    }
         
    return result;
} /* End of function riic_tx_byte() */

/*******************************************************************************
end r_riic_rx600_master.c
*******************************************************************************/


