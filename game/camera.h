#define PI 3.1415926535897932f

float moduloTwo (float a, float b) {
  return a - b * floor(a/b);
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

static inline void rotate(int dpad_rot[], struct Camera *camera) {
    if (dpad_rot[0]) {
        camera->yrot -= 60;
    } else if (dpad_rot[1]) {
        camera->yrot += 60;
    }
    
    if (dpad_rot[2]) {
        camera->xrot -= 2.0f;
    } else if (dpad_rot[3]) {
        camera->xrot += 2.0f;
    }
}

static inline void step(struct Camera *camera, maple_device_t *purudev, uint32_t stepRumble, sfxhnd_t pistol_fire) {
    static float step = 0;
    static float lastStep = 0;
    step += camera->step;
    
    int crossed = moduloTwo(lastStep, 75) > moduloTwo(step, 75);
    lastStep = step;

    float stepHeight = sin(step / (75.0f * 2.0f) * 2.0f * PI);

    // camera->pos_y = fabs(stepHeight * stepHeight)*-1;
    camera->pos_y = fabs(stepHeight)*-1/4;
    camera->step = 0;
   
    if (crossed) {
        // snd_sfx_play_chn(0, pistol_fire, 200, 128);
        // if (purudev != NULL) {
        //     purupuru_rumble_raw(purudev, stepRumble);
        // }

        lastStep = 0;
        step = 0;
    } else if (camera->step == 0 && camera->pos_y < 0) {
        step += 1;
    }
}

