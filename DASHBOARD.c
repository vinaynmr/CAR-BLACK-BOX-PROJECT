/*
 * File:   DASHBOARD.c
 * Author: VINAY
 *
 * Created on 18 November, 2024, 12:57 PM
 */


#include "adc.h"
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"

void view_lcd_dashboard(void) {
    clcd_print("  TIME   GEAR SP", LINE1(0));
    //clcd_print("00:00:00", LINE2(0));
    clcd_print(time,LINE2(0));
    clcd_print("  ", LINE2(8));
    clcd_print(gear[count], LINE2(10));
    clcd_print("  ", LINE2(12));
    clcd_putch((reg_val / 10) + 48, LINE2(14));
    clcd_putch((reg_val % 10) + 48, LINE2(15));
}

void edit_gear_and_speed() {
    //static int once_gear_flag = 1;
    reg_val = read_adc(CHANNEL4) / 10.24;

//    if (once_gear_flag) {
//        once_gear_flag = 0;
//        store_event();
//    }
    if (key == MK_SW1) {
        if (collision_flag) {
            collision_flag = 0;
            count = 0;
        }
        count++;
        if (count > 7)
            count = 7;
        store_event();
    } else if (key == MK_SW2) {
        if (collision_flag) {
            collision_flag = 0;
            count = 2;
        }
        if (count > 1)
            count--;
        store_event();
    } else if (key == MK_SW3) {
        count = 8;
        collision_flag = 1;
        store_event();
    }

}