/*
 * File:   logs.c
 * Author: VINAY
 *
 * Created on 21 November, 2024, 4:32 PM
 */


#include "adc.h"
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "i2c.h"
#include "ds1307.h"
#include "ext_eeprom.h"


unsigned static int delay = 0;
unsigned char time_data_hour, time_data_min, time_data_sec, set_time[10];
unsigned static int hour = 0, min = 0, sec = 0;
unsigned static int one_time_flag = 1, exit_flag = 1;

void view_log() {
    unsigned static int once = 1;
    if (store_event_flag == 0) {
        CLEAR_DISP_SCREEN;
        clcd_print("NO LOGS TO ", LINE1(4));
        clcd_print("DISPLAY...", LINE2(6));
        for (unsigned long int wait = 500000; wait--;);
        log_flag = 0;
        CLEAR_DISP_SCREEN;
    } else {
        char str[100];
        unsigned static int start, end;
        if (store_event_flag && once) {
            once = 0;
            start = 0;
            end = 10;
        }
        int k = 0;
        for (int i = start; i < end; i++) {
            if ((i % 10) == 2 || (i % 10) == 4) {
                str[k++] = ':';
                str[k++] = read_external_eeprom(i);
            } else if ((i % 10) == 6 || (i % 10) == 8) {
                str[k++] = ' ';
                str[k++] = read_external_eeprom(i);
            } else
                str[k++] = read_external_eeprom(i);
        }
        str[k] = '\0';

        static int count = 0;



        if (key == MK_SW1) {
            if (start > 0) {
                start -= 10;
                end -= 10;
                if (count > 0)
                    count--;
            }
        } else if (key == MK_SW2) {
            if (start < ((switch_count - 1) * 10)) {
                start += 10;
                end += 10;
                if (count < 9)
                    count++;
            }
        }

        clcd_print("#   TIME   EV SP", LINE1(0));
        clcd_putch(count + 48, LINE2(0));
        clcd_print(str, LINE2(2));
    }
}

void clear_log() {
    clcd_print("CLEAR LOG:", LINE1(0));
    clcd_print("LOGS CLEARING...", LINE2(0));
    store_event_flag = 0;
    switch_count = 0;
    for (unsigned long int block = 1000000; block--;);
    log_flag = 0;
    CLEAR_DISP_SCREEN;
}

void download_log() {
    if (store_event_flag == 0) {
        clcd_print("NO LOGS TO", LINE1(0));
        clcd_print("DOWNLOAD", LINE2(5));
        for (unsigned long int block = 500000; block--;);
        log_flag = 0;
        CLEAR_DISP_SCREEN;
    } else {
        unsigned int start = 0, end = 10;
        for (int j = 0; j < switch_count; j++) {
            int k = 0;
            for (int i = start; i < end; i++) {
                if ((i % 10) == 2 || (i % 10) == 4) {
                    uart_display[j][k++] = ':';
                    uart_display[j][k++] = read_external_eeprom(i);
                } else if ((i % 10) == 6 || (i % 10) == 8) {
                    uart_display[j][k++] = ' ';
                    uart_display[j][k++] = read_external_eeprom(i);
                } else
                    uart_display[j][k++] = read_external_eeprom(i);
            }
            uart_display[j][k] = '\0';
            start += 10;
            end += 10;
        }
        for (int i = 0; i < switch_count; i++) {
            puts(uart_display[i]);
            puts("\n\r");
        }
        log_flag = 0;
    }
}

void set_time_log() {
    unsigned int set_back_flag = 0;
    if (one_time_flag) {
        one_time_flag = 0;
        int k = 0;
        for (int i = 0; i <= 6; i++) {
            if (i == 2 || i == 4) {
                set_time[k++] = ':';
                set_time[k++] = store_time[i];
            } else
                set_time[k++] = store_time[i];
        }
        set_time[8] = '\0';
        hour = set_time[0] - 48;
        hour = (hour * 10) + (set_time[1] - 48);
        min = set_time[3] - 48;
        min = (min * 10) + (set_time[4] - 48);
        sec = set_time[6] - 48;
        sec = (sec * 10) + (set_time[7] - 48);
    }
    if (time_event_flag == 1) {
        if (delay++ <= 200) {
            set_time[0] = ' ';
            set_time[1] = ' ';
        } else if (delay <= 400) {
            set_time[0] = (hour / 10) + 48;
            set_time[1] = (hour % 10) + 48;
        } else
            delay = 0;
        if (key == MK_SW1) {
            if (hour < 23)
                hour++;
            else
                hour = 0;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW2) {
            time_event_flag = 2;
            set_time[0] = (hour / 10) + 48;
            set_time[1] = (hour % 10) + 48;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW11) {
            save_time_to_rtc();
            exit_flag = 0;
            set_back_flag = 1;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW12) {
            exit_flag = 0;
            CLEAR_DISP_SCREEN;
        }
    } else if (time_event_flag == 2) {
        if (delay++ <= 200) {
            set_time[3] = ' ';
            set_time[4] = ' ';
        } else if (delay <= 400) {
            set_time[3] = (min / 10) + 48;
            set_time[4] = (min % 10) + 48;
        } else
            delay = 0;
        if (key == MK_SW1) {
            if (min < 59)
                min++;
            else
                min = 0;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW2) {
            set_time[3] = (min / 10) + 48;
            set_time[4] = (min % 10) + 48;
            time_event_flag = 3;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW11) {
            save_time_to_rtc();
            exit_flag = 0;
            set_back_flag = 1;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW12) {
            exit_flag = 0;
            CLEAR_DISP_SCREEN;
        }
    } else if (time_event_flag == 3) {
        if (delay++ <= 200) {
            set_time[6] = ' ';
            set_time[7] = ' ';
        } else if (delay <= 400) {
            set_time[6] = (sec / 10) + 48;
            set_time[7] = (sec % 10) + 48;
        } else
            delay = 0;
        if (key == MK_SW1) {
            if (sec < 59)
                sec++;
            else
                sec = 0;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW2) {
            time_event_flag = 1;
            set_time[6] = (sec / 10) + 48;
            set_time[7] = (sec % 10) + 48;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW11) {
            save_time_to_rtc();
            exit_flag = 0;
            set_back_flag = 1;
            CLEAR_DISP_SCREEN;
        } else if (key == MK_SW12) {
            exit_flag = 0;
            CLEAR_DISP_SCREEN;
        }
    }
    if (exit_flag) {
        clcd_print("SET TIME:", LINE1(0));
        clcd_print(set_time, LINE2(0));
        clcd_print("  ", LINE2(8));
        clcd_print(gear[count], LINE2(10));
        clcd_print("  ", LINE2(12));
        clcd_putch((reg_val / 10) + 48, LINE2(14));
        clcd_putch((reg_val % 10) + 48, LINE2(15));
    } else {
        hour = 0;
        min = 0;
        sec = 0;
        one_time_flag = 1;
        exit_flag = 1;
        time_data_hour = 0;
        time_data_min = 0;
        time_data_sec = 0;
        log_flag = 0;
        if(set_back_flag)
        {
            menu_flag = 0;
        }
    }
}

void save_time_to_rtc() {
    time_data_hour = time_data_hour | ((hour / 10) << 4);
    time_data_hour = time_data_hour | (hour % 10);
    write_ds1307(HOUR_ADDR, time_data_hour);
    time_data_min = time_data_min | ((min / 10) << 4);
    time_data_min = time_data_min | (min % 10);
    write_ds1307(MIN_ADDR, time_data_min);
    time_data_sec = time_data_sec | ((sec / 10) << 4);
    time_data_sec = time_data_sec | (sec % 10);
    write_ds1307(SEC_ADDR, time_data_sec);
}
