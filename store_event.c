  /*
 * File:   store_event.c
 * Author: VINAY
 *
 * Created on 21 November, 2024, 2:43 PM
 */


#include "ext_eeprom.h"
#include "main.h"

void store_event()
{
    if(first_time)
    {
        store_event_flag = 1;
    }
    
    if(switch_count++  >= 10)
    {
        switch_count = 10;
        address_overwrite();
    }
    
    for(int i = 0; i < 10; i++)
    {
        if(i < 6)
        {
            write_external_eeprom(address,store_time[i]);   
        }else if(i < 8)
        {
            if(i == 6)
                write_external_eeprom(address,gear[count][0]);
            else
                write_external_eeprom(address,gear[count][1]);    
        }else
        {
            if(i == 8)
                write_external_eeprom(address,(reg_val / 10) + 48);
            else
                write_external_eeprom(address,(reg_val % 10) + 48);
        }
        address++;
       
    }
    if(address > 99)
        address = 90;
}

void address_overwrite()
{
    unsigned char ch;
    int k = 10;
    for(unsigned char i = 0; i < 90; i++)
    {
        ch = read_external_eeprom(k);
        write_external_eeprom(i,ch);
        k++;
    }
}


/*if (store_event_flag == 0) {
        CLEAR_DISP_SCREEN;
        clcd_print("NO LOGS TO", LINE1(0));
        clcd_print("DISPLAY", LINE2(7));
        for (unsigned int wait = 1000000; wait--;);
        CLEAR_DISP_SCREEN;
        log_flag = 0;
    } else if (read_address_flag && store_event_flag) {
        store_event_flag = 0;
        start = switch_count * 10;
        end = (start + 1) * 10;
    } else if (read_address_flag == 0 && store_event_flag) {
        store_event_flag = 0;
        start = 0 * 10;
        end = 1 * 10;
    }*/

/*if (key == MK_SW2) {
        if (read_address_flag) {
            if ((start >= (switch_count * 10)) && (start <= 90) && flag) {
                start = start + 10;
                end = end + 10;
                if (count < 10)
                    count++;
                if (start > 90) {
                    start = 0;
                    end = 10;
                    flag = 0;
                    count -= 1;
                }
                
            } else if (start >= 0 && start < ((switch_count - 1) * 10)) {
                start += 10;
                end += 10; 
                if (count < 10)
                    count++;
            }
            
        } else {
            if (start < ((switch_count - 1) * 10)) {
                start += 10;
                end += 10;
                if (count < 10)
                    count++;
            }
        }
        
        CLEAR_DISP_SCREEN;
    } else if (key == MK_SW1) {
        if (read_address_flag) {
            if ((start >= (switch_count * 10)) && (start <= 90) && flag) {
                start = start - 10;
                end = end - 10;
                if (count > 0)
                    count--;
                if(start < (switch_count * 10))
                {
                    start = (switch_count - 1) * 10;
                    end =  start + 10;
                    flag = 0;
                    count -= 1;
                }
            }else if (start > 0 && start < switch_count * 10 && flag) {
                start -= 10;
                end -= 10;
                if (count > 0)
                    count--;
            }
            
        } else {
            if (start > 0) {
                start -= 10;
                end -= 10;
            }
            if (count > 0)
                count--;
        }
        
        CLEAR_DISP_SCREEN;
    }*/
