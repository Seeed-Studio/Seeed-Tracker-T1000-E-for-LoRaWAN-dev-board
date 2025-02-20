
#ifndef _SMTC_HAL_CONFIG_H
#define _SMTC_HAL_CONFIG_H

#define LR1110_IRQ_PIN      33  // P1.01
#define LR1110_NRESER_PIN	42  // P1.10  
#define LR1110_BUSY_PIN		7   // P0.07
#define LR1110_SPI_NSS_PIN 	12  // P0.12
#define LR1110_SPI_SCK_PIN	11  // P0.11
#define LR1110_SPI_MOSI_PIN	41  // P1.09
#define LR1110_SPI_MISO_PIN	40  // P1.08

#define DEBUG_RX_PIN    17  // P0.17
#define DEBUG_TX_PIN    16  // P0.16

#define USER_BUTTON     6   // P0.06
#define USER_LED_G      24  // P0.24
#define USER_LED_R      3   // P0.3

#define SENSE_POWER_EN  38  // P1.06
#define SENSE_ADC_VCC   4   // P0.04, AIN2
#define SENSE_ADC_NTC   31  // P0.31, AIN7
#define SENSE_ADC_PHOTO 29  // P0.29, AIN5
#define SENSE_ADC_BAT   2   // P0.02, AIN0

#define CHARGER_ADC_DET 5   // P0.05, AIN3
#define CHARGER_DONE    36  // P1.04
#define CHARGER_CHRG    35  // P1.03

#define BUZZER_POWER_EN 37  // P1.05
#define BUZZER_PWM      25  // P0.25

#define ACC_POWER       39  // P1.07
#define ACC_I2C_SCL     27  // P0.27
#define ACC_I2C_SDA     26  // P0.26
#define ACC_INT1        34  // P1.02

#define AG3335_POWER_EN     43  // P1.11
#define AG3335_VRTC_EN      8   // P0.08
#define AG3335_RESET        47  // P1.15
#define AG3335_RESETB_OUT   46  // P1.14
#define AG3335_SLEEP_INT    44  // P1.12
#define AG3335_RTC_INT      15  // P0.15
#define AG3335_UART_TX      13  // P0.13
#define AG3335_UART_RX      14  // P0.14

#define FLASH_POWER_EN  45  // P1.13
#define FLASH_QSPI_CSN  20  // P0.20
#define FLASH_QSPI_SCK  19  // P0.19
#define FLASH_QSPI_IO0  21  // P0.21
#define FLASH_QSPI_IO1  22  // P0.22
#define FLASH_QSPI_IO2  23  // P0.23
#define FLASH_QSPI_IO3  32  // P1.00

#endif
