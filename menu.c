/*
 * File:   menu.c
 * Author: VINAY
 *
 * Created on 19 November, 2024, 3:41 PM
 */


#include "adc.h"
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"

void dashboard_menu() {

    if (star_flag == 0)
        clcd_putch('*', LINE1(0));
    else if (star_flag == 1)
        clcd_putch('*', LINE2(0));
    clcd_print(menu[index], LINE1(3));
    clcd_print(menu[index + 1], LINE2(3));

}

void main_menu() {
    if (log_flag) 
    {
        if(star_flag == 0)
        {
            if(index == 0)
                view_log();
            else if(index == 1)
                clear_log();
            else if(index == 2)
                download_log();
            else if(index == 3)
                set_time_log();
        }else if(star_flag)
        {
            if(index == 0)
                clear_log();
            else if(index == 1)
                download_log();
            else if(index == 2)
                set_time_log();     
        }
        
        if (key == MK_SW12){
            log_flag = 0;
            CLEAR_DISP_SCREEN;
        }
    } else {
        dashboard_menu();
        
            
        if ((key == MK_SW1) && (star_flag == 0) && (index > 0)) {
            index--;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW1) {
            star_flag = 0;
            CLEAR_DISP_SCREEN;
        }
        if ((key == MK_SW2) && (star_flag == 1) && (index < 2)) {
            index++;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW2) {
            star_flag = 1;
            CLEAR_DISP_SCREEN;
        }

        
        //exit or enter of the functions using this switches
        if (key == MK_SW11) {
            CLEAR_DISP_SCREEN;
            log_flag = 1;
        } else if (key == MK_SW12) {
            CLEAR_DISP_SCREEN;
            menu_flag = 0;
        }
    }
}
