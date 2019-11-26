#ifndef ITE_WIEGAND_WIEGAND_H_
#define ITE_WIEGAND_WIEGAND_H_

#ifdef __cplusplus
extern "C" {
#endif

void wg0_begin(int gpiod0, int gpiod1);
void wg1_begin(int gpiod0, int gpiod1);
int wg0_DoWiegandConversion(unsigned long *card_id);
int wg1_DoWiegandConversion(unsigned long *card_id);

#ifdef __cplusplus
}
#endif

#endif