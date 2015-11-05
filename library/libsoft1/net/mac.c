/*
<------------------------------------------------------------>
da:		destination address
sa:		source address
ra:		reciver address
ta:		transmiter address

ibss(adhoc):	1=接收者,2=发送者,3=bssid
sta->ap:	1=bssid,2=发送者,3=最终接收者
ap->sta:	1=sta地址,2=bssid,3=发送者
<------------------------------------------------------------>





<------------------------------帧------------------------------>
<------------------------call1：framecontrol-------------------->
<------------------------call2：mac addr----------------------->

802.11 mac帧:
[0,1]:		frame control
[2,3]:		duration
[4,9]:		address 1(dest)
[a,f]:		address 2(source)
[0x10,0x15]:	address 3
[0x16,0x17]:	sequence control
[0x18,0x1d]:	address 4
[30,?-5]:	frame body
[?-4,?]:	fcs




wifi控制帧:
.rts    =		//我要说话，你听好了
	[ 2.frame control ][ 2.duration ][     6.ra    ][ 6.ta ][ 4.fcs ]

.cts=			//听他说话，大家安静一下
	[ 2.frame control ][ 2.duration ][     6.ra    ][ 4.fcs ]

.ack=			//已收到
	[ 2.frame control ][ 2.duration ][     6.ra    ][ 4.fcs ]

.ps-poll=		//一次给我一堆，我不会一直听着
	[ 2.frame control ][    2.aid   ][ 6.bssid(ra) ][ 6.ta ][ 4.fcs ]




wifi管理帧:
	[ 2.frame control ][ 2.duration ][ 6.address1 ][ 6.address2 ]
	[ 6.address3 ][ 2.sequence control ][ 0~2304.frame body ][ 4.fcs ]
.beacon=
	timestamp(8字节,微秒),interval,capatibility,ssid

.probe request/response=
	ssid,supported rates,extended supported rates

.association request=
	capability,listen interval,ssid,supported rates

.association response=
	capability,status code,aid,supported rates

.authentication=
	算法，帧编号，状态，挑战文本




wifi数据帧:
	[ 2.frame control ][ 2.duration ][ 6.address1 ][ 6.address2 ]
	[ 6.address3 ][ 2.sequence control ][ 6.address4 ]

?	[ 2.qos control ][ 4.ht control ]

	[ 0~7951.framebody ]
	[ 4.fcs ]

ethernet帧在wifi里传输(802.11 rfc 1042)=
	[ 802.11 mac headers ]
++	[ 1.snap dsap 0xaa ][ 1.snap ssap 0xaa ][ 1.control 0x03(UI) ]
++	[ 3.rfc1042封装 0x00-00-00 ]
	[ 2.type ][ ?.ip packet ]
-+	[ 4.fcs ]

<------------------------------------------------------------>




<---------------------framecontrol:------------------>

[.1,.0]:	protocol version

[.3,.2]:	type
[.7,.4]:	subtype
	00,0000:association request
	00,0001:association response
	00,0010:reassociation request
	00,0011:reassociation response
	00,0100:probe request
	00,0101:probe response
	0110~0111:reserved
	00,1000:beacon帧
	00,1001:announcement traffic indication message(atim)
	00,1010:disassociation
	00,1011:authentication
	00,1100:deauthentication
	00,1101:action帧

	01,1010:power save(po)-poll
	01,1011:rts
	01,1100:cts
	01,1101:ack

	10,0000:data

[.8]:		to ds
[.9]:		from ds
[.10]:		more fragments		是否分片
[.11]:		retry		重传包
[.12]:		power management
[.13]:		more data
[.14]:		protected frame
[.15]:		order

<------------------------------------------------------------>




<------------------------mac addr(48bit)------------------->
举例：
	00 : 11 : 22 : 33 : 44 : 55
字节顺序：
	a[0]=00 , a[1]=11 , a[2]=22 , a[3]=33 , a[4]=44 , a[5]=55
传送顺序：
	bit0,bit1,bit2.....bit7,bit0,bit1......bit7,bit0....bit7......

a[0].bit0:	0=单播，1=组播
a[0].bit1:	0=全球唯一，1=本地唯一

a[0]:		厂商代码第1字节(地址第1字节)
a[1]:		厂商代码第2字节(地址第2字节)
a[2]:		厂商代码第3字节(地址第3字节)

a[3]:		网卡编号第1字节(地址第4字节)
a[4]:		网卡编号第2字节(地址第5字节)
a[5]:		网卡编号第3字节(地址第6字节)

<------------------------------------------------------------>




<------------------------------------------------------------>
capability information:
bit0:	ess
bit1:	ibss
	bit0=0,bit1=0:		mesh bss
	bit0=0,bit1=1:		ibss
	bit0=1,bit1=0:		基础结构型网络

bit2:	cf pollable
bit3:	cf-poll request
bit4:	privacy
bit5:	shortpreamble
bit6:	pbcc
bit7:	channel aglity
bit8:	spectrum migmt		1:mib.dot11spectrummanagementrequired=true
bit9:	qos
bit10:	short slot time
bit11:	apsd
bit12:	radio measurement	1:mib.dot11spectrummanagementactivated=true
bit13:	dsss-ofdm
bit14:	delayed block ack
bit15:	immediate block ack
<------------------------------------------------------------>




<------------------------------------------------------------>
ie:
	[ 1.elementid ][ 1.length ][ information ]
<------------------------------------------------------------>
*/
