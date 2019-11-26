#include "I2C.h"
#include "r_riic_rx600.h"
#include "r_riic_rx600_types.h"

/*******************************************************************************
* Function name: riic_master_init
* Description  : Prepare an RIIC channel for master mode communications used in the demo
* Arguments    : none
* Return value : riic_ret_t -
*                   RIIC result code
*******************************************************************************/
riic_ret_t riic_master_init(void)
{
    /* Initialize a data structure used by the R_RIIC_Init() API function. */
    /* See r_riic_rx600.h for definitions of this structure. */
    riic_config_t   riic_master_config = {RIIC_CHANNEL, RIIC_MASTER_CONFIG,
                                          0,
                                          0,
                                          0,
                                          0,
                                          MASTER_IIC_ADDRESS_LO,
                                          MASTER_IIC_ADDRESS_HI};

    /* Initialize the RIIC channel for communication with the accelerometer. */
    return R_RIIC_Init(&riic_master_config);
}
