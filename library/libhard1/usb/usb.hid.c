#include "libhard.h"
#define NoKeyPress 0
#define InvalidScanCode 1
#define SelfTestFail 2
#define UndefinedError 3
//[A,B,C...Z]: [4,29]
//[1,2,3...0]: [30, 39]
#define USBKEY_Enter 40
#define USBKEY_Esc   41
#define USBKEY_BSp   42
#define USBKEY_Tab   43
#define USBKEY_Space 44
//45: -_
//46: =+
//47: [{
//48: ]}
//49: \|
//50: ...
//51: ;:
//52: '"
//53: `~
//54: ,<
//55: .>
//56: /?
//57: CapsLock
//[f1,f12]: [58,69]
//70: PrtScr
//71: ScrollLock
//72: Pause
//73: Insert
//74: Home
//75: PgUp
//76: Delete
//77: End
//78: PgDn
//79: Right
//80: Left
//81: Down
//82: Up
//83: NumLock
//84: KP /
//85: KP *
//86: KP -
//87: KP +
//88: KP Enter
//89: KP 1End
//90: KP 2Down
//91: KP 3PgDn
//92: KP 4Left
//93: KP 5
//94: KP 6Right
//95: KP 7Home
//96: KP 8Up
//97: KP 9PgUp
//98: KP 0Ins
//99: KP .Del
//100: ...
//101: Applic
//102: Power
//103: KP =
//[F13,F24]: [104,115]
//116: Execute
//117: Help
//118: Menu
//119: Select
//120: Stop
//121: Again
//122: Undo
//123: Cut
//124: Copy
//125: Paste
//126: Find
//127: Mute
//128: VolumeUp
//129: VolumeDown
//130: Locking CapsLock
//131: LockingNumLock
//132: LockingScrollLock
//133: KP ,
//134: KP =
//[135,143]: Internat
//[144,152]: LANG
//153: AltErase
//154: SysRq
//155: Cancel
//156: Clear
//157: Prior
//158: Return
//159: Separ
//160: Out
//161: Oper
//162: Clear / Again
//163: CrSel / Props
//164: ExSel
//224: LCtrl
//225: LShift
//226: LAlt
//227: LSuper
//228: RCtrl
//229: RShift
//230: RAlt
//231: RSuper
//
#define LEDLAMP_NUMLOCK    0
#define LEDLAMP_CAPSLOCK   1
#define LEDLAMP_SCROLLLOCK 2
#define LEDLAMP_COMPOSE    3
#define LEDLAMP_KANA       4
//
#define MODIFIER_LEFTCTRL   0
#define MODIFIER_LEFTSHIFT  1
#define MODIFIER_LEFTALT    2
#define MODIFIER_LEFTSUPER  3
#define MODIFIER_RIGHTCTRL  4
#define MODIFIER_RIGHTSHIFT 5
#define MODIFIER_RIGHTALT   6
#define MODIFIER_RIGHTSUPER 7
struct report_keyboard{
	u8 modifier;
	u8 reserved;
	u8 keys[6];
}__attribute__((packed));
//
#define MOUSE_LEFTBUTTON
#define MOUSE_RIGHTBUTTON
#define MOUSE_MIDDLEBUTTON
struct report_mouse{
	u8 btn;
	u8 x;
	u8 y;
}__attribute__((packed));