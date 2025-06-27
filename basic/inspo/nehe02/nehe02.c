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

#include "anim_loader.h"
#include "bone_hierarchy.h"

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

void draw_room(float xrot, float yrot, float pos_x, float pos_y, float pos_z, float base, float width, float depth, float height) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();

    glRotatef(yrot, 1.0f, 0.0f, 0.0f);
    glRotatef(xrot*57.1666666667, 0.0f, 1.0f, 0.0f);
    glTranslatef(pos_x, pos_y, pos_z);

    //floor
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);

    glVertex3f(-width, base, -depth);
    glVertex3f(width, base, -depth);
    glVertex3f(width, base, 0.0f);
    glVertex3f(-width, base, 0.0f);
    glEnd();

    //far wall
    glBegin(GL_QUADS);
    glColor3f(0.65882352941f, 1.0f, 1.0f);

    glVertex3f(-width, base, -depth);
    glVertex3f(width, base, -depth);
    glVertex3f(width, base + height, -depth);
    glVertex3f(-width, base + height, -depth);
    glEnd();

    //ceiling
    glBegin(GL_QUADS);
    glColor3f(0.82745098039f, 0.82745098039f, 0.82745098039f);

    glVertex3f(-width, base + height, -depth);
    glVertex3f(width, base + height, -depth);
    glVertex3f(width, base + height, 0.0f);
    glVertex3f(-width, base + height, 0.0f);
    glEnd();

    //left wall
    glBegin(GL_QUADS);
    glColor3f(0.88235294117f, 1.0f, 0.88235294117f);

    glVertex3f(-width, base, 0.0f);
    glVertex3f(-width, base, -depth);
    glVertex3f(-width, base + height, -depth);
    glVertex3f(-width, base + height, 0.0f);
    glEnd();

    //right wall
    glBegin(GL_QUADS);
    glColor3f(0.88235294117f, 1.0f, 0.88235294117f);

    glVertex3f(width, base, 0.0f);
    glVertex3f(width, base, -depth);
    glVertex3f(width, base + height, -depth);
    glVertex3f(width, base + height, 0.0f);
    glEnd();

    //near wall    
    glBegin(GL_QUADS);
    glColor3f(0.65882352941f, 1.0f, 1.0f);

    glVertex3f(-width, base, 0.0f);
    glVertex3f(-width, base + height, 0.0f);
    glVertex3f(width, base + height, 0.0f);
    glVertex3f(width, base, 0.0f);

    glEnd();
}

void draw_cube(float x, float y, float z, float width, float height, float depth) {
    //bottom
    glBegin(GL_QUADS);
    glColor3f(0.36078431372f, 0.25098039215f, 0.2f);
    glVertex3f(x, y, z - depth);
    glVertex3f(x + width, y, z - depth);
    glVertex3f(x + width, y, z);
    glVertex3f(x, y, z);
    glEnd();

    //blue
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x, y, z);
    glVertex3f(x, y + height, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x + width, y, z);
    glEnd();

    //red
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex3f(x + width, y, z - depth);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x, y + height, z - depth);
    glVertex3f(x, y, z - depth);
    glEnd();

    //pink/purple
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(x, y, z);
    glVertex3f(x, y + height, z);
    glVertex3f(x, y + height, z - depth);
    glVertex3f(x, y, z - depth);
    glEnd();

    //blue/green
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.5f, 1.0f);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x + width, y, z - depth);
    glEnd();

    //top
    glBegin(GL_QUADS);
    glColor3f(0.36078431372f, 0.25098039215f, 0.2f);
    glVertex3f(x, y + height, z - depth);
    glVertex3f(x + width, y + height, z - depth);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x, y + height, z);
    glEnd();
}

static inline void glStart(void) {
    glKosInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

static inline void glHUDMode(void) {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE); // Optionally disable depth writes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static inline void glDrawHUD(int average[]) {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(average[0] - 10, average[1] - 10);
    glVertex2f(average[0] + 10, average[1] - 10);
    glVertex2f(average[0] + 10, average[1] + 10);
    glVertex2f(average[0] - 10, average[1] + 10);
    glEnd();
}

static inline void glResume(void) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

struct Camera {
    float xrot, yrot;
    float pos_x, pos_y, pos_z;
    int step;
    float sprint;
};

struct Room {
    float base;
    float width, depth, height;
};

static inline void std_controller_inputs(maple_device_t *cont, struct Camera *camera, int *brake) {
    cont_state_t *state;

    cont = maple_enum_dev(0, 0);
    if ((state = (cont_state_t *)maple_dev_status(cont))) {
        if(state->buttons & CONT_START)
            *brake = 1;

        int ltrig = state->ltrig;
        if(ltrig > 200) {
            camera->sprint = 2.0f;
        } else if (ltrig > 10) {
            camera->sprint = 1.5f;
        } else {
            camera->sprint = 1.0f;
        }

        if(state->buttons & CONT_X) {
            camera->xrot -= 1.5f;
        }

        if(state->buttons & CONT_B) {
            camera->xrot += 1.5f;
        }

        if(state->buttons & CONT_A) {
            camera->yrot += 60;
        }

        if(state->buttons & CONT_Y) {
            camera->yrot -= 60;
        }

        if(state->buttons & CONT_DPAD_UP) {
            camera->pos_z += .25f * camera->sprint * cos(camera->xrot * PI / 180);
            camera->pos_x -= .25f * camera->sprint * sin(camera->xrot * PI / 180);

            camera->step += 1;
        }

        if(state->buttons & CONT_DPAD_DOWN) {
            camera->pos_z -= .25f * cos(camera->xrot * PI / 180);
            camera->pos_x += .25f * sin(camera->xrot * PI / 180);
            camera->step -= 1;
        }

        if(state->buttons & CONT_DPAD_LEFT) {
            camera->pos_z += .25f * sin(camera->xrot * PI / 180);
            camera->pos_x += .25f * cos(camera->xrot * PI / 180);
        }

        if(state->buttons & CONT_DPAD_RIGHT) {
            camera->pos_z -= .25f * sin(camera->xrot * PI / 180);
            camera->pos_x -= .25f * cos(camera->xrot * PI / 180);
        }

        float joyx = state->joyx;
        camera->xrot += 1.5f * joyx / 128;

        float joyy = state->joyy;
        camera->pos_z += -0.25f * camera->sprint * joyy / 128 * cos(camera->xrot * PI / 180);
        camera->pos_x -= -0.25f * camera->sprint * joyy / 128 * sin(camera->xrot * PI / 180);
        camera->step -= 1 * joyy / 128;
    }
}

static inline void lightgun_inputs(maple_device_t *light, struct Camera *camera, int dpad_rot[], int *aPress, int *rumble_delay_frames, sfxhnd_t pistol_fire, int *brake, int *recoil) {
    cont_state_t *state;

    if ((state = (cont_state_t *)maple_dev_status(light))) {
        if(state->buttons & CONT_START)
            *brake = 1;

        if(state->buttons & CONT_DPAD_UP) {
            dpad_rot[0] = 1;
        } else {
            dpad_rot[0] = 0;
        }
        
        if (state->buttons & CONT_DPAD_DOWN) {
            dpad_rot[1] = 1;
        } else {
            dpad_rot[1] = 0;
        }

        if(state->buttons & CONT_DPAD_LEFT) {
            dpad_rot[2] = 1;
        } else {
            dpad_rot[2] = 0;
        }

        if(state->buttons & CONT_DPAD_RIGHT) {
            dpad_rot[3] = 1;
        } else {
            dpad_rot[3] = 0;
        }

        if (state->buttons & CONT_A && !*aPress) {
            *aPress = 1;
            snd_sfx_play_chn(0, pistol_fire, 200, 128);
            *recoil += 50;
            *rumble_delay_frames = 2;
        } else if (!(state->buttons & CONT_A)) {
            *aPress = 0;
        }
    }
}

static inline void rotate(int dpad_rot[], struct Camera *camera) {
    if (dpad_rot[0]) {
        camera->yrot -= 60;
    } else if (dpad_rot[1]) {
        camera->yrot += 60;
    }
    
    if (dpad_rot[2]) {
        camera->xrot -= 1.5f;
    } else if (dpad_rot[3]) {
        camera->xrot += 1.5f;
    }
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

static inline void step(struct Camera *camera) {
    camera->pos_y = fabs(sin(camera->step / 75.0f * 2.0f * PI) * sin(camera->step / 75.0f * 2.0f * PI))/4;
}

static inline void multiply_matrices(float *out, const float *a, const float *b) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            out[col * 4 + row] = 
                a[0 * 4 + row] * b[col * 4 + 0] +
                a[1 * 4 + row] * b[col * 4 + 1] +
                a[2 * 4 + row] * b[col * 4 + 2] +
                a[3 * 4 + row] * b[col * 4 + 3];
        }
    }
}

int main(int argc, char **argv) {
    maple_device_t *cont, *light, *purudev[2] = {NULL, NULL};
    cont_state_t *state;
    uint64 last = 0, now;

    int brake = 0;

    float scale = 1.0f;

       
    Animation *walk_anim = load_animation("/rd/animation.animbin");
    int b = 0;

    if (walk_anim) {
        printf("Loaded animation: %d bones, %d frames, %.1f fps\n",
               walk_anim->num_bones, walk_anim->num_frames, walk_anim->frame_rate);
    }

    if (!walk_anim) {
        printf("Failed to load animation file!\n");
        fflush(stdout);
    }

    float current_time = 0.0f;     // Time in seconds
    float current_frame = 0.0f;    // Interpolated frame index (e.g., 12.5)
    

    snd_init();
    sfxhnd_t pistol_fire = snd_sfx_load("/rd/pistol-fire.wav");

    uint8_t n[8];
    word2hexbytes(0x01157010, n);
    uint32_t effect = (n[0] << 28) + (n[1] << 24) + (n[2] << 20) + (n[3] << 16) +
        (n[4] << 12) + (n[5] << 8) + (n[6] << 4) + (n[7] << 0);
    
    glStart();
    struct Room room = {.base = -3.0f, .width = 25.0f, .depth = 100.0f, .height = 6.0f};

    struct Camera camera = {0};
    camera.sprint = 1.0f;

    int lightgun_x = 320;
    int lightgun_y = 240;

    int average[2] = {0, 0};
    int crosshair_pos[LENGTH][2];
    crosshair_pos_init(crosshair_pos);
    
    int dpad_rot[4] = {0, 0, 0, 0};
    int aPress = 0;
    int shooting = 0;
    int recoil = 0;

    int trigger_rumble = 0;
    int rumble_delay_frames = 0;
    
    wait_for_dev_attach(&purudev[0], MAPLE_FUNC_PURUPURU, 0);
    wait_for_dev_attach(&purudev[1], MAPLE_FUNC_PURUPURU, 1);

    camera.pos_x = -10.0f;
    camera.pos_y = -10.0f;
    camera.pos_z = -25.0f;

    camera.xrot = 0.0f;
    camera.yrot = 0.0f;

    while(1) {
        now = timer_ms_gettime64();
        vid_waitvbl();

        cont = maple_enum_dev(0, 0);
        std_controller_inputs(cont, &camera, &brake);
        
        shoot(&trigger_rumble, &rumble_delay_frames, effect, purudev[1], &recoil);
        light = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN);
        lightgun_inputs(light, &camera, dpad_rot, &aPress, &rumble_delay_frames, pistol_fire, &brake, &recoil);
        
        if (brake) 
            break;

        rotate(dpad_rot, &camera);
        step(&camera);

        /* Draw the "scene" */
        draw_room(camera.xrot, camera.yrot, camera.pos_x, camera.pos_y, camera.pos_z, room.base, room.width, room.depth, room.height);
        draw_cube(-20.0f, room.base, -19.0f, 1.0f, 1.0f, 1.0f);
        draw_cube(10.0f, room.base, -25.0f, 1.0f, 1.0f, 1.0f);
        draw_cube(2.0f, room.base, -35.0f, 3.0f, 2.0f, 1.0f);

        // glDisable(GL_DEPTH_TEST);
        // glLineWidth(3.0f);
        // glColor3f(1.0f, 0.0f, 1.0f);  // bright magenta

        // glBegin(GL_LINES);
        // glVertex3f(0.0f, 0.0f, -5.0f);  // From origin
        // glVertex3f(0.0f, 1.0f, -5.0f);  // Up 1 unit
        // glEnd();

        //

        float delta_time = 1.0f / 60.0f;  // or use actual timing
        current_time += delta_time;

        // Advance frame using animation frame rate
        current_frame = current_time * walk_anim->frame_rate;

        // Loop the animation
        if (current_frame >= walk_anim->num_frames)
            current_frame = fmodf(current_frame, (float)walk_anim->num_frames);

        int frame_i = (int)current_frame;

        BoneMatrix *local = &walk_anim->frames[frame_i * walk_anim->num_bones];
        BoneMatrix world[NUM_BONES];

        for (int i = 0; i < NUM_BONES; i++) {
            int parent = bone_parent[i];

            if (parent == -1) {
                world[i] = local[i];  // Copy local transform to world for root
            } else if (parent >= 0 && parent < NUM_BONES) {
                multiply_matrices(world[i].matrix, world[parent].matrix, local[i].matrix);
                for (int i = 0; i < 5; i++) {
                    int p = bone_parent[i];

                    printf(
                        "Bone %d | Parent: %d\n"
                        "  Local Pos: %.2f %.2f %.2f\n"
                        "  World Pos: %.2f %.2f %.2f\n\n",
                        i, p,
                        local[i].matrix[12], local[i].matrix[13], local[i].matrix[14],
                        world[i].matrix[12], world[i].matrix[13], world[i].matrix[14]
                    );
                }
            } else {
                printf("⚠️ Invalid parent index: %d for bone %d\n", parent, i);
            }
        }
        // glDisable(GL_DEPTH_TEST);
        // glColor3f(1.0f, 0.8f, 0.1f);  // bright yellow

        // glBegin(GL_LINES);
        // for (int i = 0; i < NUM_BONES; i++) {
        //     int p = bone_parent[i];
        //     if (p == -1) continue;  // skip root

        //     float *child_matrix = world[i].matrix;
        //     float *parent_matrix = world[p].matrix;

        //     // Translation = last column of column-major 4x4
        //     float cx = child_matrix[12];
        //     float cy = child_matrix[13];
        //     float cz = child_matrix[14];

        //     float px = parent_matrix[12];
        //     float py = parent_matrix[13];
        //     float pz = parent_matrix[14];

        //     glVertex3f(px * scale, py * scale, pz * scale);
        //     glVertex3f(cx * scale, cy * scale, cz * scale);
        // }
        // glEnd();
        

        // glEnable(GL_LINE_SMOOTH);
        // glLineWidth(2.0f);  // Thicker debug lines

        // float g = world[1].matrix[12];
        // float h = world[1].matrix[13];
        // float i = world[1].matrix[14];

        // printf("HIP: %.2f %.2f %.2f\n", g, h, i);
        // fflush(stdout);

        // glDisable(GL_DEPTH_TEST); 

        // glColor3f(1.0f, 0.0f, 0.0f);
        // glPointSize(8.0f);
        // glBegin(GL_POINTS);
        // glVertex3f(g * scale, h * scale, i * scale);
        // glEnd();

///

        // glBegin(GL_LINES);
        // glColor3f(0.0f, 1.0f, 0.0f);
        // glVertex3f(g * scale, h * scale, i * scale);
        // glVertex3f(g * scale, h * scale + 1.0f, i * scale); // vertical line
        // glEnd();

        // int parent = 1;
        // int child = 2;  // Or another bone known to be offset

        // float *pm = world[parent].matrix;
        // float *cm = world[child].matrix;

        // glBegin(GL_LINES);
        // glColor3f(1.0f, 1.0f, 0.0f);
        // glVertex3f(pm[12] * scale, pm[13] * scale, pm[14] * scale);
        // glVertex3f(cm[12] * scale, cm[13] * scale, cm[14] * scale);
        // glEnd();

        // draw_cube(g, h, i, 1.0f, 1.0f, 1.0f);

        maple_gun_read_pos(&lightgun_x, &lightgun_y);
        crosshair_pos_update(crosshair_pos, average, lightgun_x, lightgun_y, recoil);

        glHUDMode();
        glDrawHUD(average);
        glResume();
        glKosSwapBuffers();
    }

    snd_sfx_unload_all();	
    snd_shutdown();
    return 0;
}

