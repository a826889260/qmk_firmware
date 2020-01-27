#ifndef NUMBERSETC_BLE_H_
#define NUMBERSETC_BLE_H_

#include "quantum.h"

// This a shortcut to help you visually see your layout.
// The following is an example using the Planck MIT layout
// The first section contains all of the arguments
// The second converts the arguments into a two-dimensional array
#define LAYOUT( \
    k01, k02, k03, k04, \
    k05, k06, k07, k08, \
    k09, k10, k11, k12, \
    k13, k14, k15, k16, \
    k17, k18, k19, k20 \
) \
{ \
    { k04, k03, k02, k01 }, \
    { k08, k07, k06, k05 }, \
    { k12, k11, k10, k09 }, \
    { k16, k15, k14, k13 }, \
    { k20, k19, k18, k17 }, \
}

// These macros are used to initiate the bootloader
#define DFU_MAGIC_SERIAL_ONLY_RESET   0x4e
#define DFU_MAGIC_UF2_RESET           0x57
#define DFU_MAGIC_OTA_RESET           0xA8

#endif /* NUMBERSETC_BLE_H_ */
