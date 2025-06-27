/* 
   KallistiOS 2.0.0

   nehe02.c
   (c)2014 Josh Pearson
   (c)2001 Benoit Miller
   (c)2000 Jeff Molofee
*/

#include <stdio.h>
#include <math.h>

#include <arch/timer.h>

#include <dc/maple.h>
#include <dc/maple/controller.h>
#include <dc/maple/purupuru.h>

#include <unistd.h>
#include <dc/video.h>

#include <dc/video.h>

#include <kos.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glkos.h>

#include <string.h>
#include <kos/init.h>
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

#include "camera.h"
#include "control.h"
#include "objects.h"
#include "gl.h"

#include "3dutils.c"

#define PI 3.1415926535897932f
#define LENGTH 10

static inline void word2hexbytes(uint32_t word, uint8_t *bytes) {
  for (int i = 0; i < 8; i++) {
    bytes[i] = (word >> (28 - (i * 4))) & 0xf;
  }
}

void wait_for_dev_attach(maple_device_t **dev_ptr, unsigned int func, int port) {
    maple_device_t *dev = *dev_ptr;

    if((dev != NULL) && (dev->valid != 0)) return;
    
    int attempts = 0;
    while(((dev == NULL) || (dev->valid == 0)) && attempts < 20) {
        *dev_ptr = maple_enum_type(port, func);
        dev = *dev_ptr;
        usleep(50);
        attempts++;
    }
}

static inline void crosshair_pos_init(int crosshair_pos[][2]) {
    for (int i = 0; i < LENGTH; i++) {
        crosshair_pos[i][0] = -1;
        crosshair_pos[i][1] = -1;
    }
}

static inline void crosshair_pos_update(int crosshair_pos[][2], int average[], int lightgun_x, int lightgun_y, int recoil) {
    int crosshair_x = lightgun_x - 220;
    int crosshair_y = lightgun_y * -2 + 536;

    int sums[2] = {0, 0};
    int i = 0;

    for (i = 0; i < LENGTH; i++) {
        if (crosshair_pos[i][0] == -1) {
            for (i = 0; i < LENGTH; i++) {
                crosshair_pos[i][0] = crosshair_x;
                crosshair_pos[i][1] = crosshair_y;
            }
            break;
        } else if (i == (LENGTH - 1)){
            for (i = 0; i < LENGTH - 1; i++) {
                crosshair_pos[i][0] = crosshair_pos[i+1][0];
                crosshair_pos[i][1] = crosshair_pos[i+1][1];
            }
            crosshair_pos[LENGTH - 1][0] = crosshair_x;
            crosshair_pos[LENGTH - 1][1] = crosshair_y;
        }
    }

    sums[0] = 0;
    sums[1] = 0;

    for (i = 0; i < LENGTH; i++) {
        sums[0] += crosshair_pos[i][0];
        sums[1] += crosshair_pos[i][1];
    }
    
    average[0] = sums[0] / LENGTH;
    average[1] = (sums[1] / LENGTH) + recoil;
}

static inline void shoot(int *trigger_rumble, int *rumble_delay_frames, uint32_t effect, maple_device_t *purudev, int *recoil) {
    if (*recoil > 0) {
        *recoil -= *recoil/10;
    } else if (*recoil < 0) {
        *recoil = 0;
    }

    if (*rumble_delay_frames == 1) {
        *trigger_rumble = 1;
        maple_gun_enable(1);
    } else if (*rumble_delay_frames > 0) {
        (*rumble_delay_frames)--;
    }

    if (*trigger_rumble) {
        if (purudev != NULL) {
            purupuru_rumble_raw(purudev, effect);
        }
        *trigger_rumble = 0;
    }
}

int main(int argc, char **argv) {
    maple_device_t *cont, *light, *lightpuru = NULL, *contpuru = NULL;
    cont_state_t *state;

    pvr_init_defaults();

    int brake = 0;    

    // snd_init();
    // sfxhnd_t pistol_fire = snd_sfx_load("/rd/pistol-fire.wav");

    // uint8_t n[8];
    // word2hexbytes(0x01157010, n);
    // uint32_t effect = (n[0] << 28) + (n[1] << 24) + (n[2] << 20) + (n[3] << 16) +
    //     (n[4] << 12) + (n[5] << 8) + (n[6] << 4) + (n[7] << 0);

    // uint8_t s[8];
    // word2hexbytes(0x011A7010, s);
    // uint32_t stepRumble = (s[0] << 28) + (s[1] << 24) + (s[2] << 20) + (s[3] << 16) +
    //     (s[4] << 12) + (s[5] << 8) + (s[6] << 4) + (s[7] << 0);
    
    // glStart();
    // struct Room room = {.base = -3.0f, .width = 25.0f, .depth = 100.0f, .height = 6.0f};

    struct Camera camera = {0};
    camera.sprint = 1.0f;

    // int lightgun_x = 320;
    // int lightgun_y = 240;

    // int average[2] = {0, 0};
    // int crosshair_pos[LENGTH][2];
    // crosshair_pos_init(crosshair_pos);
    
    // int dpad_rot[4] = {0, 0, 0, 0};
    // int aPress = 0;
    // int recoil = 0;

    // int trigger_rumble = 0;
    // int rumble_delay_frames = 0;
    
    // wait_for_dev_attach(&contpuru, MAPLE_FUNC_PURUPURU, 0);
    // wait_for_dev_attach(&lightpuru, MAPLE_FUNC_PURUPURU, 1);

    while(1) {
        vid_waitvbl();
        pvr_wait_ready();
        pvr_scene_begin();
        pvr_list_begin(PVR_LIST_OP_POLY);


        cont = maple_enum_dev(0, 0);
        std_controller_inputs(state, cont, &camera, &brake);

        // step(&camera, contpuru, stepRumble, pistol_fire);
        
        // // shoot(&trigger_rumble, &rumble_delay_frames, effect, purudev[1], &recoil);
        // shoot(&trigger_rumble, &rumble_delay_frames, effect, lightpuru, &recoil);
        // light = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN);
        // lightgun_inputs(state, light, &camera, dpad_rot, &aPress, &rumble_delay_frames, pistol_fire, &brake, &recoil);
        
        // if (brake) 
        //     break;

        // rotate(dpad_rot, &camera);
        // // step(&camera, contpuru, stepRumble, pistol_fire);

        // /* Draw the "scene" */
        // draw_room(camera.xrot, camera.yrot, camera.pos_x, camera.pos_y, camera.pos_z, room.base, room.width, room.depth, room.height);
        // draw_cube(-20.0f, room.base, -19.0f, 1.0f, 1.0f, 1.0f);
        // draw_cube(10.0f, room.base, -25.0f, 1.0f, 1.0f, 1.0f);
        // draw_cube(2.0f, room.base, -35.0f, 3.0f, 2.0f, 1.0f);

        // maple_gun_read_pos(&lightgun_x, &lightgun_y);
        // crosshair_pos_update(crosshair_pos, average, lightgun_x, lightgun_y, recoil);

        // glHUDMode();
        // glDrawHUD(average);
        // glResume();
        // glKosSwapBuffers();

        draw_poly_box(0.0f, 10.0f, 640.0f, 20.0f + (24.0f * 2.0f) + 10.0f, 90.0f,
                      0.3f, 0.2f, 0.5f, 0.0f, 0.5f, 0.1f, 0.8f, 0.2f);
        draw_poly_strf(5.0f, 20.0f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                       "     GhettoPlay  (C)2000-2002 by Megan Potter     ");
        draw_poly_strf(5.0f, 48.0f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                       "      sndoggvorbis (C)2001 by Thorsten Titze      ");

        pvr_list_finish();

        /* Finish the frame *******************************/
        pvr_scene_finish();
    }

    snd_sfx_unload_all();	
    snd_shutdown();
    return 0;
}

