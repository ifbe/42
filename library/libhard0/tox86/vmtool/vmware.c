#include "libhard.h"
u8* getoemid();




//https://wiki.osdev.org/VMware_tools
//https://github.com/vmware/open-vm-tools
#define   BDOOR_CMD_GETMHZ                    1
/*
 * BDOOR_CMD_APMFUNCTION is used by:
 *
 * o The FrobOS code, which instead should either program the virtual chipset
 *   (like the new BIOS code does, Matthias Hausner offered to implement that),
 *   or not use any VM-specific code (which requires that we correctly
 *   implement "power off on CLI HLT" for SMP VMs, Boris Weissman offered to
 *   implement that)
 *
 * o The old BIOS code, which will soon be jettisoned
 */
#define   BDOOR_CMD_APMFUNCTION               2 /* CPL0 only. */
#define   BDOOR_CMD_GETDISKGEO                3
#define   BDOOR_CMD_GETPTRLOCATION            4
#define   BDOOR_CMD_SETPTRLOCATION            5
#define   BDOOR_CMD_GETSELLENGTH              6
#define   BDOOR_CMD_GETNEXTPIECE              7
#define   BDOOR_CMD_SETSELLENGTH              8
#define   BDOOR_CMD_SETNEXTPIECE              9
#define   BDOOR_CMD_GETVERSION               10
#define   BDOOR_CMD_GETDEVICELISTELEMENT     11
#define   BDOOR_CMD_TOGGLEDEVICE             12
#define   BDOOR_CMD_GETGUIOPTIONS            13
#define   BDOOR_CMD_SETGUIOPTIONS            14
#define   BDOOR_CMD_GETSCREENSIZE            15
#define   BDOOR_CMD_MONITOR_CONTROL          16 /* Disabled by default. */
#define   BDOOR_CMD_GETHWVERSION             17
#define   BDOOR_CMD_OSNOTFOUND               18 /* CPL0 only. */
#define   BDOOR_CMD_GETUUID                  19
#define   BDOOR_CMD_GETMEMSIZE               20
//#define BDOOR_CMD_HOSTCOPY                 21 /* Not in use. Was devel only. */
//#define BDOOR_CMD_SERVICE_VM               22 /* Not in use. Never shipped. */
#define   BDOOR_CMD_GETTIME                  23 /* Deprecated -> GETTIMEFULL. */
#define   BDOOR_CMD_STOPCATCHUP              24
#define   BDOOR_CMD_PUTCHR                   25 /* Disabled by default. */
#define   BDOOR_CMD_ENABLE_MSG               26 /* Devel only. */
//#define BDOOR_CMD_GOTO_TCL                 27 /* Not in use. Was devel only */
#define   BDOOR_CMD_INITPCIOPROM             28 /* CPL 0 only. */
//#define BDOOR_CMD_INT13                    29 /* Not in use. */
#define   BDOOR_CMD_MESSAGE                  30
#define   BDOOR_CMD_SIDT                     31
#define   BDOOR_CMD_SGDT                     32
#define   BDOOR_CMD_SLDT_STR                 33
#define   BDOOR_CMD_ISACPIDISABLED           34
//#define BDOOR_CMD_TOE                      35 /* Not in use. */
#define   BDOOR_CMD_ISMOUSEABSOLUTE          36
#define   BDOOR_CMD_PATCH_SMBIOS_STRUCTS     37 /* CPL 0 only. */
#define   BDOOR_CMD_MAPMEM                   38 /* Devel only */
#define   BDOOR_CMD_ABSPOINTER_DATA          39
#define   BDOOR_CMD_ABSPOINTER_STATUS        40
#define   BDOOR_CMD_ABSPOINTER_COMMAND       41
//#define BDOOR_CMD_TIMER_SPONGE             42 /* Not in use. */
//#define BDOOR_CMD_PATCH_ACPI_TABLES        43 /* Not in use. */
//#define BDOOR_CMD_DEVEL_FAKEHARDWARE       44 /* Not in use. */
#define   BDOOR_CMD_GETHZ                    45
#define   BDOOR_CMD_GETTIMEFULL              46
//#define BDOOR_CMD_STATELOGGER              47 /* Not in use. */
#define   BDOOR_CMD_CHECKFORCEBIOSSETUP      48 /* CPL 0 only. */
#define   BDOOR_CMD_LAZYTIMEREMULATION       49 /* CPL 0 only. */
#define   BDOOR_CMD_BIOSBBS                  50 /* CPL 0 only. */
//#define BDOOR_CMD_VASSERT                  51 /* Not in use. */
#define   BDOOR_CMD_ISGOSDARWIN              52
#define   BDOOR_CMD_DEBUGEVENT               53
#define   BDOOR_CMD_OSNOTMACOSXSERVER        54 /* CPL 0 only. */
#define   BDOOR_CMD_GETTIMEFULL_WITH_LAG     55
#define   BDOOR_CMD_ACPI_HOTPLUG_DEVICE      56 /* Devel only. */
#define   BDOOR_CMD_ACPI_HOTPLUG_MEMORY      57 /* Devel only. */
#define   BDOOR_CMD_ACPI_HOTPLUG_CBRET       58 /* Devel only. */
//#define BDOOR_CMD_GET_HOST_VIDEO_MODES     59 /* Not in use. */
#define   BDOOR_CMD_ACPI_HOTPLUG_CPU         60 /* Devel only. */
//#define BDOOR_CMD_USB_HOTPLUG_MOUSE        61 /* Not in use. Never shipped. */
#define   BDOOR_CMD_XPMODE                   62 /* CPL 0 only. */
#define   BDOOR_CMD_NESTING_CONTROL          63
#define   BDOOR_CMD_FIRMWARE_INIT            64 /* CPL 0 only. */
#define   BDOOR_CMD_FIRMWARE_ACPI_SERVICES   65 /* CPL 0 only. */
typedef struct {
	union {
		u32 ax;
		u32 magic;
	};
	union {
		u32 bx;
		u32 size;	//size_t: 32bit or 64bit
	};
	union {
		u32 cx;
		u16 command;
	};
	union {
		u32 dx;
		u16 port;
	};
	u32 si;
	u32 di;
} vmware_cmd;
#define VMWARE_MAGIC  0x564D5868
#define VMWARE_PORT   0x5658
#define VMWARE_PORTHB 0x5659
void vmware_send(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORT;
	asm volatile("in %%dx, %0" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}
static void vmware_send_hb(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORTHB;
	asm volatile("cld; rep; outsb" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}
static void vmware_get_hb(vmware_cmd * cmd) {
	cmd->magic = VMWARE_MAGIC;
	cmd->port = VMWARE_PORTHB;
	asm volatile("cld; rep; insb" : "+a"(cmd->ax), "+b"(cmd->bx), "+c"(cmd->cx), "+d"(cmd->dx), "+S"(cmd->si), "+D"(cmd->di));
}
#define CMD_ABSPOINTER_DATA    39
#define CMD_ABSPOINTER_STATUS  40
#define CMD_ABSPOINTER_COMMAND 41
#define ABSPOINTER_ENABLE   0x45414552
#define ABSPOINTER_RELATIVE 0xF5
#define ABSPOINTER_ABSOLUTE 0x53424152
void vmware_backdoor_mouse_absolute(void) {
	vmware_cmd cmd;

	//Enable
	cmd.bx = ABSPOINTER_ENABLE;
	cmd.command = CMD_ABSPOINTER_COMMAND;
	vmware_send(&cmd);

	//Status
	cmd.bx = 0;
	cmd.command = CMD_ABSPOINTER_STATUS;
	vmware_send(&cmd);

	///Read data
	cmd.bx = 1;
	cmd.command = CMD_ABSPOINTER_DATA;
	vmware_send(&cmd);

	//Enable absolute
	cmd.bx = ABSPOINTER_ABSOLUTE;
	cmd.command = CMD_ABSPOINTER_COMMAND;
	vmware_send(&cmd);
}
void vmware_backdoor_mouse_relative(void) {
	vmware_cmd cmd;
	cmd.bx = ABSPOINTER_RELATIVE;
	cmd.command = CMD_ABSPOINTER_COMMAND;
	vmware_send(&cmd);
}
void vmware_tool()
{
	vmware_cmd cmd;
	cmd.bx = ~VMWARE_MAGIC;
	cmd.command = BDOOR_CMD_GETVERSION;
	vmware_send(&cmd);
	if (cmd.bx != VMWARE_MAGIC || cmd.ax == 0xFFFFFFFF) {
		say("vmtool: magic mismatch\n");
		return;
	}

	say("set vmware mouse mode: absolute\n");
	vmware_backdoor_mouse_absolute();
	//vmware_backdoor_mouse_relative();
}




void initvmtool()
{
	u8* oemid = getoemid();
	if(0 == oemid)return;
	else if(0 == ncmp(oemid, "VMWARE", 6)){
		vmware_tool();
	}
	else if(0 == ncmp(oemid, "BOCHS ",6)){
		say("qemu,ovmf?\n");
	}
	else{
		say("oemid=%p\n",oemid);
	}
}