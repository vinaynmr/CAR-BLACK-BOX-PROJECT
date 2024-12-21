/*
 * File:   main.c
 * Author: VINAY
 *
 * Created on 18 November, 2024, 12:45 PM
 */


#include "adc.h"
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "uart.h"

void init_config() {
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_i2c();
    init_ds1307();
    init_uart();
}

void main(void) {
    init_config();
    while (1) {
        get_time();
        key = read_switches(STATE_CHANGE);

        if (menu_flag == 1) {
            main_menu();
        } else {
            edit_gear_and_speed();
            view_lcd_dashboard();
            if (first_store_flag) {
                first_store_flag = 0;
                store_event();
                first_time = 1;
            }
            if (key == MK_SW11) {
                CLEAR_DISP_SCREEN;
                menu_flag = 1;
            } else if (key == MK_SW12)
                menu_flag = 0;
        }

    }
    return;
}

static void get_time(void) {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40) {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        store_time[0] = time[0];
        time[1] = '0' + (clock_reg[0] & 0x0F);
        store_time[1] = time[1];
    } else {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
        store_time[0] = time[0];
        store_time[1] = time[1];
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
    store_time[2] = time[3];
    store_time[3] = time[4];
    store_time[4] = time[6];
    store_time[5] = time[7];
    store_time[6] = '\0';
}
