#ifndef REG_FICR_H
#define REG_FICR_H

#include <stdint.h>

/** \brief FICR registers base address */
#define FICR_BASE_ADDR 0x10000000u

/** \name FICR DEVICEID register */
/**@{*/

/** \brief FICR DEVICEID0 register address offset */
#define FICR_DEVICEID0_OFF 0x060u
/** \brief FICR DEVICEID1 register address offset */
#define FICR_DEVICEID1_OFF 0x064u

/** \brief FICR DEVICEID0 register address */
#define FICR_DEVICEID0_ADDR (FICR_BASE_ADDR + FICR_DEVICEID0_OFF)
/** \brief FICR DEVICEID1 register address */
#define FICR_DEVICEID1_ADDR (FICR_BASE_ADDR + FICR_DEVICEID1_OFF)

/** \brief FICR DEVICEID0 register */
#define FICR_DEVICEID0 (*((volatile uint32_t*) FICR_DEVICEID0_ADDR))
/** \brief FICR DEVICEID1 register */
#define FICR_DEVICEID1 (*((volatile uint32_t*) FICR_DEVICEID1_ADDR))

/**@}*/

#endif /* REG_FICR_H */
