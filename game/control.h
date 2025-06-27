static inline void std_controller_inputs(cont_state_t *state,maple_device_t *cont, struct Camera *camera, int *brake) {
    // cont_state_t *state;

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
            camera->xrot -= 2.0f;
        }

        if(state->buttons & CONT_B) {
            camera->xrot += 2.0f;
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
            camera->step = 1 * camera->sprint;
        }

        if(state->buttons & CONT_DPAD_DOWN) {
            camera->pos_z -= .25f * cos(camera->xrot * PI / 180);
            camera->pos_x += .25f * sin(camera->xrot * PI / 180);
            camera->step = 1 * camera->sprint;
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
        camera->xrot += 2.0f * joyx / 128;

        float joyy = state->joyy;
        camera->pos_z += -0.25f * camera->sprint * joyy / 128 * cos(camera->xrot * PI / 180);
        camera->pos_x -= -0.25f * camera->sprint * joyy / 128 * sin(camera->xrot * PI / 180);

        if (camera->step == 0 && fabs(joyy) > 50) {
            camera->step = 1 * camera->sprint;
        }
    }
}

static inline void lightgun_inputs(cont_state_t *state, maple_device_t *light, struct Camera *camera, int dpad_rot[], int *aPress, int *rumble_delay_frames, sfxhnd_t pistol_fire, int *brake, int *recoil) {
    // cont_state_t *state;

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
