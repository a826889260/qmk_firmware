#ifndef NUMBERSETC_BLE_H_
#define NUMBERSETC_BLE_H_

#include "quantum.h"

// This a shortcut to help you visually see your layout.
// The following is an example using the Planck MIT layout
// The first section contains all of the arguments
// The second converts the arguments into a two-dimensional array
#define LAYOUT_4X6( \
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

#define LAYOUT_10X4( \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, \
    k11, k12, k13, k14, k15, k16, k17, k18, k19, k20, \
    k21, k22, k23, k24, k25, k26, k27, k28, k29, k30, \
    k31, k32, k33, k34, k35, k36, k37, k38, k39, k40 \
) \
{ \
    { k35, k25, k15, k05 }, \
    { k34, k24, k14, k04 }, \
    { k33, k23, k13, k03 }, \
    { k32, k22, k12, k02 }, \
    { k31, k21, k11, k01 }, \
    { k40, k30, k20, k10 }, \
    { k39, k29, k19, k09 }, \
    { k38, k28, k18, k08 }, \
    { k37, k27, k17, k07 }, \
    { k36, k26, k16, k06 }, \
}

// These macros are used to initiate the bootloader
#define DFU_MAGIC_SERIAL_ONLY_RESET   0x4e
#define DFU_MAGIC_UF2_RESET           0x57
#define DFU_MAGIC_OTA_RESET           0xA8

// nRF52 specific keycodes
enum nrf_keycodes {
    AD_WO_L = SAFE_RANGE, /* Start advertising without whitelist  */
    BLE_DIS,              /* Disable BLE HID sending              */
    BLE_EN,               /* Enable BLE HID sending               */
    USB_DIS,              /* Disable USB HID sending              */
    USB_EN,               /* Enable USB HID sending               */
    DELBNDS,              /* Delete all bonding                   */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    DEL_IDC,
    ENT_DFU,              /* Start bootloader                     */
    ENT_SLP               /* Deep sleep mode                      */
};

void led_on(int delay_ms);
void led_off(int delay_ms);
void led_advertise(int peer_id);
void led_delete(int peer_id);

void eeconfig_set_advertising_channel(int peer_id);
int eeconfig_get_advertising_channel();

void eeconfig_set_usb_enabled(bool enabled);
bool eeconfig_get_usb_enabled();

void eeconfig_set_ble_enabled(bool enabled);
bool eeconfig_get_ble_enabled();

#endif /* NUMBERSETC_BLE_H_ */
