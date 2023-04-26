#ifndef _EVENT_H
#define _EVENT_H

//keyboard
#define kbd_esc 0x1b
#define kbd_up 0x48
#define kbd_down 0x50
#define kbd_left 0x4b
#define kbd_right 0x4d
#define kbd_f1 0xf1
#define kbd_f2 0xf2
#define kbd_f3 0xf3
#define kbd_f4 0xf4


//mouse(p @ c +)
//#define mouse_eventtype 'p'        //what kind of event
//#define mouse_eventsubtype '@'     //what kind of mouse
//#define mouse_eventpart 'c'         //what part(connection, state, wheel)
//#define mouse_eventsubpart '+'      //what detail(plugin, click, scrollup)
//
#define point_mask 0xff
#define point_type 'p'
#define point_onto hex16('p','+')
#define point_away hex16('p','-')
#define point_shut hex16('p','?')
//
#define point_abs hex16('p','@')   //absolute position(windows)
#define point_per hex16('p','%')   //percentage position(qemu mouse)
#define point_dlt hex16('p','~')   //delta position(usb mouse)


//touch
#define touch_mask 0xff
#define touch_type 't'
#define touch_onto hex16('t','+')
#define touch_away hex16('t','-')
#define touch_shut hex16('t','?')
//
#define touch_abs hex16('t','@')
#define touch_per hex16('t','%')
#define touch_dlt hex16('t','~')


//gamepad
#define joy_mask  0xff00ff
#define joy_type 'j'
#define joy_left  hex32('j', 0 ,'l', 0 )
#define joy_right hex32('j', 0 ,'r', 0 )
//
#define joyl_left    0x0001
#define joyl_right   0x0002
#define joyl_down    0x0004
#define joyl_up      0x0008
#define joyl_trigger 0x0010
#define joyl_bumper  0x0020
#define joyl_thumb   0x0040
#define joyl_select  0x0080
//
#define joyr_left    0x0001
#define joyr_right   0x0002
#define joyr_down    0x0004
#define joyr_up      0x0008
#define joyr_trigger 0x0010
#define joyr_bumper  0x0020
#define joyr_thumb   0x0040
#define joyr_start   0x0080

#endif