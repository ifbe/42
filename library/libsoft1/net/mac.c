/*
<------------------------------------------------------------>
STA（Station，工作站）：
	其英文定义是“A logical entity that is a singly addressable instance of a MAC and PHY interface to the WM”。通俗点说，STA就是指携带无线网络接口卡（即无线网卡）的设备，例如笔记本、智能手机等。另外，无线网卡和有线网卡的MAC地址均分配自同一个地址池以确保其唯一性。

AP（Access Point，接入点）：
	其原文定义是“An entity that contains one STA and provides access to the distribution services，via the WM for associated STAs”。由其定义可知，AP本身也是一个STA，只不过它还能为那些已经关联的（associated）STA提供分布式服务（Distribution Service，DS）

DS（Distribution System，分布式系统）：
	其英文定义为“A system used to interconnect a set of basic service sets（BSSs）and integrated local area networks（LANs）to create an extended service set（ESS）”。DS的定义涉及BSS、ESS等无线网络架构
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

.cts/ack=		//听他说话，大家安静一下
	[ 2.frame control ][ 2.duration ][     6.ra    ][ 4.fcs ]

.ack=
	[][][][]

.ps-poll=		//一次给我一堆，我不会一直听着
	[ 2.frame control ][    2.aid   ][ 6.bssid(ra) ][ 6.ta ][ 4.fcs ]




wifi管理帧:
	[ 2.frame control ][ 2.duration ][ 6.address1 ][ 6.address2 ][ 6.address3 ]
	[ 2.sequence control ][ 0~2304.frame body ][ 4.fcs ]
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
	[ 2.frame control ][ 2.duration ][ 6.address1 ][ 6.address2 ][ 6.address3 ]
	[ 2.sequence control ][ 6.address4 ][ 2.qos control ][ 4.ht control ]
	[ 0~7951.framebody ][ 4.fcs ]

802.11 rfc 1042=
	[ 802.11 mac headers ]
++++	[ snap dsap 0xaa ][ snap ssap 0xaa ][ control 0x03(UI) ][ rfc1042封装 0x00-00-00 ]
	[ type ][ ip packet ][ fcs ]

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

比如：00,11,22,33,44,55

byte0.bit0:	0=单播，1=组播
byte0.bit1:	0=全球唯一，1=本地唯一

剩余byte0:	厂商代码	00
byte1:		厂商代码	11
byte2:		厂商代码	22

byte3:		网卡编号	33
byte4:		网卡编号	44
byte5:		网卡编号	55

<------------------------------------------------------------>




<------------------------------------------------------------>
1
<------------------------------------------------------------>
*/
