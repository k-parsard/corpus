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

#define PI 3.1415926535897932f

/*
   The simplest OpenGL example ever!

   Essentially the same thing as NeHe's lesson02 code.
   To learn more, go to http://nehe.gamedev.net/.
*/

static inline void word2hexbytes(uint32_t word, uint8_t *bytes) {
  for (int i = 0; i < 8; i++) {
    bytes[i] = (word >> (28 - (i * 4))) & 0xf;
  }
}

void wait_for_dev_attach(maple_device_t **dev_ptr, unsigned int func, int port) {
    maple_device_t *dev = *dev_ptr;

    /* If we already have it, and it's still valid, leave */
    /* dev->valid is set to 0 by the driver if the device
       is detached, but dev will stay not-null */
    if((dev != NULL) && (dev->valid != 0)) return;

    // /* Repeatedly check until we find one and it's valid */
    while((dev == NULL) || (dev->valid == 0)) {
        *dev_ptr = maple_enum_type(port, func);
        dev = *dev_ptr;
        usleep(50);
    }
}

void draw_gl(float xrot, float yrot, float pos_x, float pos_y, float pos_z, float step_side) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // glPushMatrix();

    glRotatef(yrot, 1.0f, 0.0f, 0.0f);
    glRotatef(xrot*57.1666666667, 0.0f, 1.0f, 0.0f);
    glTranslatef(pos_x + step_side, pos_y, pos_z);

    //floor
    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);

    glVertex3f(-25.0f, -1.0f, -50.0f);
    glVertex3f(25.0f, -1.0f, -50.0f);
    glVertex3f(25.0f, -1.0f, 0.0f);
    glVertex3f(-25.0f, -1.0f, 0.0f);
    glEnd();

    //far wall
    glBegin(GL_QUADS);

    glColor3f(0.65882352941f, 1.0f, 1.0f);

    glVertex3f(-25.0f, -1.0f, -50.0f);
    glVertex3f(25.0f, -1.0f, -50.0f);
    glVertex3f(25.0f, 3.0f, -50.0f);
    glVertex3f(-25.0f, 3.0f, -50.0f);

    glEnd();

    //ceiling
    glBegin(GL_QUADS);

    glColor3f(0.82745098039f, 0.82745098039f, 0.82745098039f);

    glVertex3f(-25.0f, 3.0f, -50.0f);
    glVertex3f(25.0f, 3.0f, -50.0f);
    glVertex3f(25.0f, 3.0f, 0.0f);
    glVertex3f(-25.0f, 3.0f, 0.0f);
    
    glEnd();

    //left wall
    glBegin(GL_QUADS);

    glColor3f(0.88235294117f, 1.0f, 0.88235294117f);

    glVertex3f(-25.0f, -1.0f, 0.0f);
    glVertex3f(-25.0f, -1.0f, -50.0f);
    glVertex3f(-25.0f, 3.0f, -50.0f);
    glVertex3f(-25.0f, 3.0f, 0.0f);

    glEnd();

    //right wall
    glBegin(GL_QUADS);

    glColor3f(0.88235294117f, 1.0f, 0.88235294117f);

    glVertex3f(25.0f, -1.0f, 0.0f);
    glVertex3f(25.0f, -1.0f, -50.0f);
    glVertex3f(25.0f, 3.0f, -50.0f);
    glVertex3f(25.0f, 3.0f, 0.0f);

    glEnd();

    //near wall    
    glBegin(GL_QUADS);

    glColor3f(0.65882352941f, 1.0f, 1.0f);


    glVertex3f(-25.0f, -1.0f, 0.0f);
    glVertex3f(-25.0f, 3.0f, 0.0f);
    glVertex3f(25.0f, 3.0f, 0.0f);
    glVertex3f(25.0f, -1.0f, 0.0f);

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

int main(int argc, char **argv) {
    maple_device_t *cont, *light, *purudev[2] = {NULL, NULL};
    cont_state_t *state;

    uint64 last = 0, now;

    uint8_t n[8];
    // word2hexbytes(0x31071011, n);
    word2hexbytes(0x0237F010, n);
    uint32_t effect = (n[0] << 28) + (n[1] << 24) + (n[2] << 20) + (n[3] << 16) +
        (n[4] << 12) + (n[5] << 8) + (n[6] << 4) + (n[7] << 0);
    
    glKosInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float xrot = 0.0f;
    float yrot = 0.0f;
    float pos_x = 0.0f;
    float pos_y = 0.0f;
    float pos_z = 0.0f;
    
    int step_pos = 0;
    float step_side = 0.0f;

    float sprint = 1.0f;

    int point_x = 320;
    int point_y = 240;
    
    //

    int average[2] = {0, 0};
    int sums[2] = {0, 0};

    int length = 10;
    int points[length][2];
    int i;

    for (i = 0; i < length; i++) {
        points[i][0] = -1;
        points[i][1] = -1;
    }

    //

    int dpad_rot[4] = {0, 0, 0, 0};
    int aPress = 0;
    int shooting = 0;
    
    // wait_for_dev_attach(&purudev[0], MAPLE_FUNC_PURUPURU, 0);
    // wait_for_dev_attach(&purudev[1], MAPLE_FUNC_PURUPURU, 1);

    while(1) {
        now = timer_ms_gettime64();
        vid_waitvbl();

        for (int i = 0; i < 2; i++) {
            if (purudev[i] == NULL) {
                purudev[i] = maple_enum_type(i, MAPLE_FUNC_PURUPURU);
            }
            printf("%d\n", purudev[i] != NULL);
            printf("\n");
        }

        cont = maple_enum_dev(0, 0);
        // cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        if ((state = (cont_state_t *)maple_dev_status(cont))) {
            if(state->buttons & CONT_START)
                break;

            int ltrig = state->ltrig;
            if(ltrig > 200) {
                sprint = 2.0f;
            } else if (ltrig > 10) {
                sprint = 1.5f;
            } else {
                sprint = 1.0f;
            }

            if(state->buttons & CONT_X) {
                xrot -= 1.5f;
            }

            if(state->buttons & CONT_B) {
                xrot += 1.5f;
            }

            if(state->buttons & CONT_A) {
                yrot += 60;
            }

            if(state->buttons & CONT_Y) {
                yrot -= 60;
            }

            if(state->buttons & CONT_DPAD_UP) {
                pos_z += .25f * sprint * cos(xrot * PI / 180);
                pos_x -= .25f * sprint * sin(xrot * PI / 180);

                step_pos += 1;
            }

            if(state->buttons & CONT_DPAD_DOWN) {
                pos_z -= .25f * cos(xrot * PI / 180);
                pos_x += .25f * sin(xrot * PI / 180);
                
                step_pos -= 1;
            }

            if(state->buttons & CONT_DPAD_LEFT) {
                pos_z += .25f * sin(xrot * PI / 180);
                pos_x += .25f * cos(xrot * PI / 180);
            }

            if(state->buttons & CONT_DPAD_RIGHT) {
                pos_z -= .25f * sin(xrot * PI / 180);
                pos_x -= .25f * cos(xrot * PI / 180);
            }

            float joyx = state->joyx;
            xrot += 1.5f * joyx / 128;

            float joyy = state->joyy;
            pos_z += -0.25f * sprint * joyy / 128 * cos(xrot * PI / 180);
            pos_x -= -0.25f * sprint * joyy / 128 * sin(xrot * PI / 180);
            step_pos -= 1 * joyy / 128;
        }

        // light = maple_enum_type(1, MAPLE_FUNC_CONTROLLER);
        light = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN);
        maple_gun_enable(1);
    
        if ((state = (cont_state_t *)maple_dev_status(light))) {
            if(state->buttons & CONT_START) {
                break;
            }

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

            if(state->buttons & CONT_A) {
                aPress = 1;
            } else {
                aPress = 0;
            }
        }

        if (dpad_rot[0]) {
            yrot -= 60;
        } else if (dpad_rot[1]) {
            yrot += 60;
        } else if (dpad_rot[2]) {
            xrot -= 1.5f;
        } else if (dpad_rot[3]) {
            xrot += 1.5f;
        }

        if (aPress && !shooting) {
            if (purudev[0] != NULL) {
                purupuru_rumble_raw(purudev[0], effect);
            }
            if (purudev[1] != NULL) {
                // printf("%d", purudev[1] == NULL);
                purupuru_rumble_raw(purudev[1], effect);
            }

            last = now;
            shooting = 1;
        }

        if (last + 1000 < now && shooting) {
            shooting = 0;
        }


        pos_y = fabs(sin(step_pos / 75.0f * 2.0f * PI) * sin(step_pos / 75.0f * 2.0f * PI))/4;
        // step_side = fabs(sin(step_pos / 75.0f * 2.0f * PI) * sin(step_pos / 75.0f * 2.0f * PI))*2;

        /* Draw the "scene" */
        draw_gl(xrot, yrot, pos_x, pos_y, pos_z, step_side);

        draw_cube(-20.0f, -1.0f, -19.0f, 1.0f, 1.0f, 1.0f);
        draw_cube(10.0f, -1.0f, -25.0f, 1.0f, 1.0f, 1.0f);
        draw_cube(2.0f, -1.0f, -35.0f, 3.0f, 2.0f, 1.0f);

        vid_waitvbl();
        maple_gun_enable(1);
        maple_gun_read_pos(&point_x, &point_y);

        int pointer_x = point_x - 220;
        int pointer_y = point_y * -2 + 536;

        //

        for (i = 0; i < length; i++) {
            if (points[i][0] == -1) {
                for (i = 0; i < length; i++) {
                    points[i][0] = pointer_x;
                    points[i][1] = pointer_y;
                }
                break;
            } else if (i == length - 1){
                for (i = 0; i < length; i++) {
                    points[i][0] = points[i+1][0];
                    points[i][1] = points[i+1][1];
                }
                points[length - 1][0] = pointer_x;
                points[length - 1][1] = pointer_y;
            }
        }

        sums[0] = 0;
        sums[1] = 0;

        for (i = 0; i < length; i++) {
            sums[0] += points[i][0];
            sums[1] += points[i][1];
        }
        
        average[0] = sums[0] / length;
        average[1] = sums[1] / length;

        //

        //HUD
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE); // Optionally disable depth writes
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 640, 0, 480);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw HUD (using 2D coordinates)
        glColor3f(0.0, 1.0, 0.0);

        // glBegin(GL_QUADS);
        // glVertex2f(pointer_x - 10, pointer_y - 10);
        // glVertex2f(pointer_x + 10, pointer_y - 10);
        // glVertex2f(pointer_x + 10, pointer_y + 10);
        // glVertex2f(pointer_x - 10, pointer_y + 10);
        // glEnd();

        glBegin(GL_QUADS);
        glVertex2f(average[0] - 10, average[1] - 10);
        glVertex2f(average[0] + 10, average[1] - 10);
        glVertex2f(average[0] + 10, average[1] + 10);
        glVertex2f(average[0] - 10, average[1] + 10);
        glEnd();
        
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* Finish the frame - Notice there is no glKosBegin/FinshFrame */
        glKosSwapBuffers();
    }

    return 0;
}

