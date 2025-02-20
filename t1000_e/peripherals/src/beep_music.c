#include "beep_music.h"

// C: 262Hz, #C: 277 Hz, D: 294Hz, #D: 311Hz, E: 330Hz, F: 349Hz, #F: 370Hz, G: 392Hz, #G: 415Hz, A: 440Hz, #A: 466Hz, B: 494Hz
const uint16_t freq_tab[12]  = {262, 277, 294, 311, 330, 349, 369, 392, 415, 440, 466, 494}; // CDEFGAB
const uint8_t sign_tab[7]  = {0, 2, 4, 5, 7, 9, 11}; // 1~7
const uint8_t length_tab[7] = {1, 2, 4, 8, 16, 32, 64}; // 2^0,2^1,2^2,2^3...
uint16_t freq_tab_new[12];

const struct beep_song boot_up = 
{
    // .name = "boot_up",
    .data = {
        0x1F,0x02, 0x19,0x02, 0x15,0x02, 0x16,0x00, 
        0x00,0x00,
    },
    .Double_speed = 4,    
};

const struct beep_song power_off = 
{
    // .name = "power_off",
    .data = {
        0x0B,0x02, 0x15,0x02, 0x1F,0x01,0x00,0x00
    },
    .Double_speed = 4,    
};

const struct beep_song joined = 
{
    // .name = "joined",
    .data = {
        0x17,0x03, 0x17,0x03, 0x17,0x03, 0x15,0x04, 0x17,0x04,
        0x19,0x03, 0x0F,0x03, 0x00,0x00

    },
    .Double_speed = 1.5,
};

const struct beep_song lora_downlink = 
{
    // .name = "lora_downlink",
    .data = {
    0x25,0x03, 0x25,0x03, 0x25,0x03, 0x25,0x02, 0x00,0x00     
    },
    .Double_speed = 1.8,
};

const struct beep_song low_power = 
{
    // .name = "low_power",
    .data = {
            0x19,0x01, 0x16,0x02, 0x15,0x02, 0x23,0x02, 0x19,0x02,
            0x16,0x00, 0x00,0x00     
    },
    .Double_speed = 3,    
};

const struct beep_song sos = 
{
    // .name = "sos",
    .data = {
        0x25,0x03, 0x25,0x03, 0x25,0x03, 0x25,0x02, 0x00,0x00 
    },
    .Double_speed = 1.8,    
};

const struct beep_song pos_s = 
{
    // .name = "pos_s",
    .data = {
        0x19,0x01, 0x00,0x00   
    },
    .Double_speed = 2,    
};

static int beep_song_decode_new_freq(uint8_t signature, int8_t octachord)
{
    uint8_t i, j;
    for (i = 0; i < 12; i++)
    {
        j = i + signature;

        if (j > 11)
        {
            j = j - 12;
            freq_tab_new[i] = freq_tab[j] * 2;
        }
        else
        {
            freq_tab_new[i] = freq_tab[j];
        }

        if (octachord < 0)
        {
            freq_tab_new[i] >>= (- octachord);
        }
        else if (octachord > 0)
        {
            freq_tab_new[i] <<= octachord;
        }
    }
    return 0;
}

static int beep_song_decode(uint16_t tone, uint16_t length, uint16_t *freq, uint16_t *sound_len, uint16_t *nosound_len)
{
    static const uint16_t div0_len = SEMIBREVE_LEN;
    uint16_t note_len, note_sound_len, current_freq;
    uint8_t note, sharp, range, note_div, effect, dotted;
    
    note = tone % 10;
    range = tone / 10 % 10;
    sharp = tone / 100;

    current_freq = freq_tab_new[sign_tab[note - 1] + sharp];

    if (note != 0)
    {
        if (range == 1) current_freq >>= 1;
        if (range == 3) current_freq <<= 1;
        *freq = current_freq;
    }
    else
    {
        *freq = 0;
    }
    note_div = length_tab[length % 10];

    effect = length / 10 % 10;
    dotted = length / 100;

    note_len = div0_len / note_div;

    if (dotted == 1)
        note_len = note_len + note_len / 2;

    if (effect != 1)
    {
        if (effect == 0)
        {
            note_sound_len = note_len * SOUND_SPACE;
        }
        else
        {
            note_sound_len = note_len / 2;
        }
    }
    else
    {
        note_sound_len = note_len;
    }
    if (note == 0)
    {
        note_sound_len = 0;
    }
    *sound_len = note_sound_len;

    *nosound_len = note_len - note_sound_len;

    return 0;
}

uint16_t beep_song_get_len(const struct beep_song *song)
{
    uint16_t cnt = 0;

    while (song->data[cnt])
    {
        cnt += 2;
    }
    return cnt / 2;
}

uint16_t beep_song_get_data(const struct beep_song *song, uint16_t index, struct beep_song_data *data)
{
    beep_song_decode(song->data[index * 2], song->data[index * 2 + 1], &data->freq, &data->sound_len, &data->nosound_len);
    if(song->Double_speed != 0)
    {
        data->sound_len = data->sound_len/song->Double_speed;
        data->nosound_len = data->nosound_len/song->Double_speed;
    }
    return 2;
}

int beep_song_decode_init(void)
{
    beep_song_decode_new_freq(SOUND_SIGNATURE, SOUND_OCTACHORD);

    return 0;
}
