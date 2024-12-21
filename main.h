/* 
 * File:   main.h
 * Author: VINAY
 *
 * Created on 18 November, 2024, 12:45 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include<xc.h>

#define DASHBOARD 1

unsigned char *gear[] = {"ON","GN","GR","G1","G2","G3","G4","G5","CN"};
unsigned char key;
unsigned int reg_val = 0,count = 0,collision_flag = 0,address = 0;
unsigned int switch_count,view_flag= 1;

unsigned char *menu[] = {"VIEW_LOG","CLEAR_LOG","DOWNLOAD_LOG","SET_TIME"};
unsigned int index = 0,index_flag = 1,star_flag = 0,menu_flag = 0;

unsigned int first_store_flag = 1;
unsigned int first_time = 0;
unsigned int log_flag = 0,read_address_flag = 0,store_event_flag = 0;

unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[9];
unsigned char store_time[6];
unsigned char date[11];
//unsigned char time[] = "000000";

//download log
unsigned char uart_display[10][16];
unsigned int time_event_flag = 1;

void view_lcd_dashboard();
void edit_gear_and_speed();
void main_menu();
void dashboard_menu();

void store_event();
void address_overwrite();
void compare_and_call();


void view_log();
void clear_log();
void download_log();
void set_time_log();
void save_time_to_rtc();

static void get_time(void);
void display_time(void);

#endif	/* MAIN_H */

