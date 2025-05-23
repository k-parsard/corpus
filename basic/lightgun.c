#include <stdio.h>

#include <kos/dbgio.h>
#include <arch/timer.h>

#include <dc/biosfont.h>
#include <dc/maple.h>
#include <dc/maple/controller.h>
#include <dc/pvr.h>
#include <dc/video.h>

// rumble flags
#include <stdint.h>
#include <kos/init.h>
#include <dc/maple/purupuru.h>
#include <plx/font.h>

#define PACK_PIXEL(r, g, b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3) )

typedef union rumble_fields {
  uint32_t raw;
  struct {
    /* Special Effects and motor select. The normal purupuru packs will
only have one motor. Selecting MOTOR2 for these is probably not
a good idea. The PULSE setting here supposably creates a sharp
pulse effect, when ORed with the special field. */

    /** \brief  Yet another pulse effect.
        This supposedly creates a sharp pulse effect.
    */
    uint32_t special_pulse : 1;
    uint32_t : 3; // unused

    /** \brief  Select motor #1.

        Most jump packs only have one motor, but on things that do have more
       than one motor (like PS1->Dreamcast controller adapters that support
       rumble), this selects the first motor.
    */
    uint32_t special_motor1 : 1;
    uint32_t : 2; // unused

    /** \brief  Select motor #2.

        Most jump packs only have one motor, but on things that do have more
       than one motor (like PS1->Dreamcast controller adapters that support
       rumble), this selects the second motor.
    */
    uint32_t special_motor2 : 1;

    /** \brief  Ignore this command.

        Valid value 15 (0xF).

        Most jump packs will ignore commands with this set in effect1,
       apparently.
    */
    uint32_t fx1_powersave : 4;

    /** \brief  Upper nibble of effect1.

        This value works with the lower nibble of the effect2 field to
        increase the intensity of the rumble effect.
        Valid values are 0-7.

        \see    rumble_fields_t.fx2_lintensity
    */
    uint32_t fx1_intensity : 3;

    /** \brief  Give a pulse effect to the rumble.

        This probably should be used with rumble_fields_t.fx1_pulse as well.

        \see    rumble_fields_t.fx2_pulse
    */
    uint32_t fx1_pulse : 1;

    /** \brief  Lower-nibble of effect2.

        This value works with the upper nibble of the effect1
        field to increase the intensity of the rumble effect.
        Valid values are 0-7.

        \see    rumble_fields_t.fx1_intensity
    */
    uint32_t fx2_lintensity : 3;

    /** \brief  Give a pulse effect to the rumble.

        This probably should be used with rumble_fields_t.fx1_pulse as well.

        \see    rumble_fields_t.fx1_intensity
    */
    uint32_t fx2_pulse : 1;

    /** \brief  Upper-nibble of effect2.

        This apparently lowers the rumble's intensity somewhat.
        Valid values are 0-7.
    */
    uint32_t fx2_uintensity : 3;

    /* OR these in with your effect2 value if you feel so inclined.
       if you or the PULSE effect in here, you probably should also
       do so with the effect1 one below. */

    /** \brief  Give a decay effect to the rumble on some packs. */
    uint32_t fx2_decay : 1;

    /** \brief  The duration of the effect. No idea on units... */
    uint32_t duration : 8;
  };
} rumble_fields_t;

void print_rumble_fields(uint32_t raw) {
  rumble_fields_t fields = {.raw = raw};
  printf("Rumble Fields:\n");
  printf("  .special_pulse   =  %u,\n", fields.special_pulse);
  printf("  .special_motor1  =  %u,\n", fields.special_motor1);
  printf("  .special_motor2  =  %u,\n", fields.special_motor2);

  printf("  .fx1_pulse       =  %u,\n", fields.fx1_pulse);
  printf("  .fx1_powersave   =  %u,\n", fields.fx1_powersave);
  printf("  .fx1_intensity   =  %u,\n", fields.fx1_intensity);

  printf("  .fx2_lintensity  =  %u,\n", fields.fx2_lintensity);
  printf("  .fx2_pulse       =  %u,\n", fields.fx2_pulse);
  printf("  .fx2_uintensity  =  %u,\n", fields.fx2_uintensity);
  printf("  .fx2_decay       =  %u,\n", fields.fx2_decay);

  printf("  .duration        =  %u,\n", fields.duration);
}

void wait_for_dev_attach(maple_device_t **dev_ptr, unsigned int func) {
    maple_device_t *dev = *dev_ptr;

    /* If we already have it, and it's still valid, leave */
    /* dev->valid is set to 0 by the driver if the device
       is detached, but dev will stay not-null */
    if((dev != NULL) && (dev->valid != 0)) return;

    /* Repeatedly check until we find one and it's valid */
    while((dev == NULL) || (dev->valid == 0)) {
        *dev_ptr = maple_enum_type(0, func);
        dev = *dev_ptr;
        usleep(50);
    }
}

typedef struct {
  uint32_t pattern;
  const char *description;
} baked_pattern_t;

static inline void word2hexbytes(uint32_t word, uint8_t *bytes) {
  for (int i = 0; i < 8; i++) {
    bytes[i] = (word >> (28 - (i * 4))) & 0xf;
  }
}

void drawpointer(int x, int y, int xx, int yy, int c) {
    for(yy = 0; yy < 20; ++yy) {
        for(xx = 0; xx < 20; ++xx) {
            vram_s[(((yy + y*2 - 10) - 36) * 640 + (xx + x - 10 - 220))] = c;
            // printf("%d %d", xx, yy);
        }
    }
}

#define DRAW_POINTER(x, y, xx, yy, c) do { \
    for(yy = 0; yy < 20; ++yy) { \
        for(xx = 0; xx < 20; ++xx) { \
            vram_s[(((yy + y*2 - 10) - 36) * 640 + (xx + x - 10 - 220))] = c; \
        } \
    } \
} while (0)

#define CONTROLS(dev, state, effect, break_flag) do { \
    if((dev = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN))) { \
        if((state = (cont_state_t *)maple_dev_status(dev))) { \
            if((state->buttons & CONT_START)) \
                break_flag = 1; \
            \
            if((state->buttons & CONT_A)) { \
                purupuru_rumble_raw(purudev, effect); \
                printf("Rumble: 0x%lx!\n", effect); \
            } \
        } \
    } \
} while (0)

int main(int argc, char *argv[]) {
    vid_set_mode(DM_640x480, PM_RGB565);

    int x, y, xx, yy = 0;

    uint8_t n[8];
    word2hexbytes(0x011A7010, n);
    uint32_t effect = (n[0] << 28) + (n[1] << 24) + (n[2] << 20) + (n[3] << 16) +
        (n[4] << 12) + (n[5] << 8) + (n[6] << 4) + (n[7] << 0);

    maple_device_t *dev, *purudev = NULL;
    cont_state_t *state;

    for(y = 0; y < 480; ++y) {
        for(x = 0; x < 640; ++x) {
            vram_s[y * 640 + x] = 0xFFFF;
        }
    }

    for(;;) {
        vid_waitvbl();        
        wait_for_dev_attach(&purudev, MAPLE_FUNC_PURUPURU);

        DRAW_POINTER(x, y, xx, yy, 0xFFFF);

        maple_gun_enable(0);
        maple_gun_read_pos(&x, &y);

        DRAW_POINTER(x, y, xx, yy, PACK_PIXEL(255, 0, 0));









        int break_flag = 0;
        CONTROLS(dev, state, effect, break_flag);
        
        if (break_flag) 
            break;
    }

    return 0;
}

