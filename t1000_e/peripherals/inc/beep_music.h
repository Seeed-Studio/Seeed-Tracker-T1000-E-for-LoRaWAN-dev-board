#ifndef _BEEP_MUSIC_H_
#define _BEEP_MUSIC_H_

#include <stdint.h>

#define SEMIBREVE_LEN       1600
#define SOUND_SIGNATURE     0
#define SOUND_OCTACHORD     1
#define SOUND_SPACE         4/5

#define SONG_DATA_LENGTH_MAX  50

struct beep_song
{
    const uint8_t data[SONG_DATA_LENGTH_MAX];
    double Double_speed;
};

struct beep_song_data
{
    uint16_t freq;
    uint16_t sound_len;
    uint16_t nosound_len;
};

extern const struct beep_song boot_up;
extern const struct beep_song power_off;
extern const struct beep_song joined;
extern const struct beep_song lora_downlink;
extern const struct beep_song low_power;
extern const struct beep_song sos;
extern const struct beep_song pos_s;
    
extern const uint16_t freq_tab[12];
extern const uint8_t sign_tab[7];
extern const uint8_t length_tab[7];
extern uint16_t freq_tab_new[12];

/*!
 * @brief Init song decode
 */
int beep_song_decode_init(void);

/*!
 * @brief Get song data length
 * 
 * @param [in] song Pointer to buffer to be used
 * 
 * @return Song data length
 */
uint16_t beep_song_get_len(const struct beep_song *song);

/*!
 * @brief Get song data
 * 
 * @param [in] song Pointer to song to be used
 * @param [in] index Index of song data
 * @param [in] data Pointer to buffer to save song data
 * 
 * @return Song data length, not use on here
 */
uint16_t beep_song_get_data(const struct beep_song *song, uint16_t index, struct beep_song_data *data);

#endif
