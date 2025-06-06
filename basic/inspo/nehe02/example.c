int main(int argc, char **argv) {
    maple_device_t *cont, *light, *purudev[2] = {NULL, NULL};
    cont_state_t *state;

    uint8_t n[8];
    word2hexbytes(0x0237F010, n);
    uint32_t effect = (n[0] << 28) + (n[1] << 24) + (n[2] << 20) + (n[3] << 16) +
        (n[4] << 12) + (n[5] << 8) + (n[6] << 4) + (n[7] << 0);
    
    // ...

    while(1) {
        now = timer_ms_gettime64();
        vid_waitvbl();

        for (int i = 0; i < 2; i++) {
            if (purudev[i] == NULL) {
                purudev[i] = maple_enum_type(i, MAPLE_FUNC_PURUPURU);
            }
            // printf("%d\n", purudev[i] != NULL);
            // printf("\n");
        }

        light = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN);
        maple_gun_enable(1);

        if ((state = (cont_state_t *)maple_dev_status(light))) {
            //...
            if(state->buttons & CONT_A) {
                aPress = 1;
            } else {
                aPress = 0;
            }
        }

        if (aPress && !shooting) {
            if (purudev[0] != NULL) {
                purupuru_rumble_raw(purudev[0], effect);
            }
            if (purudev[1] != NULL) {
                purupuru_rumble_raw(purudev[1], effect);
            }

            last = now;
            shooting = 1;
        }

        if (last + 1000 < now && shooting) {
            shooting = 0;
        }
        // ...
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

int main(int argc, char *argv[]) {
    //...
    wait_for_dev_attach(&purudev[0], MAPLE_FUNC_PURUPURU, 0);
    wait_for_dev_attach(&purudev[1], MAPLE_FUNC_PURUPURU, 1);

    while(1) {
        now = timer_ms_gettime64();
        vid_waitvbl();
 
        //...

    }
}


int main() {
    uint8_t n[8];
    word2hexbytes(0x011A7010, n);
    uint32_t effect = (n[0] << 28) + (n[1] << 24) + (n[2] << 20) + (n[3] << 16) +
        (n[4] << 12) + (n[5] << 8) + (n[6] << 4) + (n[7] << 0);

    int trigger_rumble = 0;

    //...

    while (1) {
        //...
         if (trigger_rumble) {
            purupuru_rumble_raw(purudev[1], effect);
            trigger_rumble = 0;
        }
    }
}
