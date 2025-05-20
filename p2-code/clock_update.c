#include <stdio.h>
#include "clock.h"

int set_tod_from_ports(tod_t *tod){
    if (CLOCK_TIME_PORT < 0 || CLOCK_TIME_PORT > (86400 * 16)){
        return 1;
    }
    //get total secs passed
    if (CLOCK_TIME_PORT & (1 << 3)){
        tod->day_secs = (CLOCK_TIME_PORT >> 4) + 1;
    } else {
        tod->day_secs = CLOCK_TIME_PORT >> 4;
    }
    //set hours to correct time
    if ((tod->day_secs/3600) == 0){
        tod->time_hours = 12;
    } else if ((tod->day_secs/3600) > 12) {
        tod->time_hours = (tod->day_secs/3600) - 12;
    } else {
        tod->time_hours = (tod->day_secs/3600);
    }
    //min and sec
    tod->time_mins = (tod->day_secs % 3600) / 60;
    tod->time_secs = ((tod->day_secs % 3600) % 60);
    //am and pm
    if (tod->day_secs >= 43200){
        tod->ampm = 2;
    } else {
        tod->ampm = 1;
    }
    return 0;
}

int set_display_from_tod(tod_t tod, int *display){
    //set nums
    char displayNums[] = {0b1110111, 0b0100100, 0b1011101, 0b1101101, 0b0101110,
    0b1101011, 0b1111011, 0b0100101, 0b1111111, 0b1101111, 0b0000000, 0b0001000,
    0b1011011, 0b0111111};
    //check valid time
    if (tod.time_hours < 0 || tod.time_mins < 0 || tod.time_secs < 0){
        return 1;
    } else if (tod.time_hours > 12 || tod.time_mins > 59 || tod.time_secs > 59){
        return 1;
    } else if (tod.ampm != 1 && tod.ampm != 2){
        return 1;
    }
    *display = 0;
    //set am or pm
    if (tod.ampm == 1){
        *display = *display | (1 << 28);
    } else {
        *display = *display | (1 << 29);
    }
    //set hours
    if (tod.time_hours < 10){
        *display = *display | (displayNums[tod.time_hours] << 14);
    } else {
        *display = *display | (displayNums[1] << 21);
        *display = *display | (displayNums[tod.time_hours - 10] << 14);
    }
    //set minutes
    if (tod.time_mins < 10){
        *display = *display | (displayNums[0] << 7);
        *display = *display | (displayNums[tod.time_mins]);
    } else {
        *display = *display | (displayNums[tod.time_mins/10] << 7);
        *display = *display | (displayNums[tod.time_mins % 10]);
    }
    return 0;
}

int clock_update(){
    //set tod
    tod_t tod = {0, 0, 0, 0, 0};
    tod_t *tod_ptr = &tod;
    //set display
    int *display = &CLOCK_DISPLAY_PORT;
    int ret = set_tod_from_ports(tod_ptr); 
    set_display_from_tod(tod, display);
    //check if worked
    if (ret == 1){
        return 1;
    }
    return 0;
}