#include "const/def.h"

#ifndef _NAMING_H
#define _NAMING_H
//
#define _ok_ hex16('o','k')
#define _no_ hex16('n','o')
#define _good_ hex32('g','o','o','d')
#define _fail_ hex32('f','a','i','l')
//
#define _ent_ hex32('e','n','t',0)
#define _sup_ hex32('s','u','p',0)
#define _art_ hex32('a','r','t',0)
#define _sys_ hex32('s','y','s',0)
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)
#define _wrk_ hex32('w','r','k',0)
#define _ori_ hex32('o','r','i',0)
//bare metal
#define _start_ hex64('s','t','a','r','t', 0, 0, 0)
#define _efimain_ hex64('e','f','i','m','a','i','n', 0)
//module
#define _lib42_ hex64('l','i','b','4','2', 0, 0, 0)
#define _kext_ hex32('k','e','x','t')
#define _ko_ hex32('k','o', 0, 0)
//application
#define _main_ hex32('m','a','i','n')
#define _ndkmain_ hex64('n','d','k','m','a','i','n', 0)
#define _win32_ hex64('w','i','n','3','2', 0, 0, 0)
#define _winmain_ hex64('w','i','n','m','a','i','n', 0)
//
#define _thread_ hex64('t','h','r','e','a','d', 0, 0)
#define _coroutine_ hex64('c','o','r','o','u','t','i','n')
//worker
#define _setup_ hex64('s','e','t','u','p', 0, 0, 0)
#define _loop_ hex32('l','o','o','p')
//
#define _subcmd_ hex64('s','u','b','c','m','d', 0, 0)
#define _compiler_ hex64('c','o','m','p','i','l','e','r')
#define _kernel_ hex64('k','e','r','n','e','l', 0, 0)
#define _myml_ hex32('m','y','m','l')
#define _mython_ hex64('m','y','t','h','o','n', 0, 0)
#define _term_ hex32('t','e','r','m')
#define _guiapp_ hex64('g','u','i','a','p','p', 0, 0)
//
#define _exiter_ hex64('e','x','i','t','e','r', 0, 0)
#define _poller_ hex64('p','o','l','l','e','r', 0, 0)
#define _pulser_ hex64('p','u','l','s','e','r', 0, 0)
#define _waiter_ hex64('w','a','i','t','e','r', 0, 0)
//
#define _bios_ hex32('b','i','o','s')
#define _efi_ hex32('e','f','i', 0)
#define _coreboot_ hex64('c','o','r','e','b','o','o','t')
#define _iboot_ hex64('i','b','o','o','t', 0, 0, 0)
//
#define _memmap_ hex64('m','e','m','m','a','p', 0, 0)
#define _devmap_ hex64('d','e','v','m','a','p', 0, 0)
//
#define _acpi_ hex32('a','c','p','i')
#define _dtb_ hex32('d','t','b', 0)
//
#define _cpu_ hex32('c','p','u',0)
#define _irq_ hex32('i','r','q',0)
#define _tmr_ hex32('t','m','r',0)
#define _bus_ hex32('b','u','s',0)
#define _mbox_ hex32('m','b','o','x')
#define _pinmgr_ hex64('p','i','n','m','g','r', 0, 0)
//
#define _wifi_ hex32('w','i','f','i')
#define _bt_ hex32('b','t',0,0)
//
#define _eth_ hex32('e','t','h',0)
#define _e1000_ hex64('e','1','0','0','0', 0, 0, 0)
//
#define _stor_ hex32('s','t','o','r')
#define _mmc_ hex32('m','m','c',0)
#define _ide_ hex32('i','d','e',0)
#define _ahci_ hex32('a','h','c','i')
#define _nvme_ hex32('n','v','m','e')
//
#define _uhci_ hex32('u','h','c','i')
#define _ohci_ hex32('o','h','c','i')
#define _ehci_ hex32('e','h','c','i')
#define _xhci_ hex32('x','h','c','i')
#define _dwc2_ hex32('d','w','c','2')
//
#define _usb_ hex32('u','s','b',0)
#define _usb2_ hex32('u','s','b','2')
#define _usb3_ hex32('u','s','b','3')
#define _usb4_ hex32('u','s','b','4')
//
#define _pci_ hex32('p','c','i',0)
#define _pcie_ hex32('p','c','i','e')
//
#define _dbg_ hex32('d','b','g',0)
#define _i2c_ hex32('i','2','c',0)
#define _spi_ hex32('s','p','i',0)
#define _gpio_ hex32('g','p','i','o')
//
#define _ptmx_ hex32('p','t','m','x')
#define _uart_ hex32('u','a','r','t')
//
#define _vmware_ hex64('v','m','w','a','r','e', 0, 0)
#define _vbox_ hex32('v','b','o','x')
#define _qemu_ hex32('q','e','m','u')
//
#define _xbox_ hex32('x','b','o','x')
#define _sony_ hex32('s','o','n','y')
//
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define _clk_ hex32('c','l','k',0)
#define _buf_ hex32('b','u','f',0)
//
#define _evby_ hex32('e','v','b','y')
#define _evto_ hex32('e','v','t','o')
#define _ioby_ hex32('i','o','b','y')
#define _ioto_ hex32('i','o','t','o')
//
#define _FILE_ hex32('F','I','L','E')
#define _file_ hex32('f','i','l','e')
#define _filetype_ hex64('f','i','l','e','t','y','p','e')
#define _fileauto_ hex64('f','i','l','e','a','u','t','o')
//
#define _part_ hex32('p','a','r','t')
#define _path_ hex32('p','a','t','h')
#define _info_ hex32('i','n','f','o')
#define _pos_ hex32('p','o','s', 0)
//
#define _vhd_ hex32('v','h','d',0)
#define _vmdk_ hex32('v','m','d','k')
//
#define _mbr_ hex32('m','b','r',0)
#define _gpt_ hex32('g','p','t',0)
//
#define _bbp_ hex32('b','b','p',0)
#define _esp_ hex32('e','s','p',0)
#define _fat_ hex32('f','a','t',0)
#define _ntfs_ hex32('n','t','f','s')
#define _rsvd_ hex32('r','s','v','d')
#define _hfs_ hex32('h','f','s',0)
#define _apfs_ hex32('a','p','f','s')
#define _ext_ hex32('e','x','t',0)
#define _zfs_ hex32('z','f','s',0)
//raw family
#define _tap_ hex32('t','a','p',0)
#define _tun_ hex32('t','u','n',0)
#define _RAW_ hex32('R','A','W',0)
#define _Raw_ hex32('R','a','w',0)
#define _raw_ hex32('r','a','w',0)
//
#define _ICMP_ hex32('I','C','M','P')
#define _icmp_ hex32('i','c','m','p')
#define _IGMP_ hex32('I','G','M','P')
#define _igmp_ hex32('i','g','m','p')
#define _ARP_ hex32('A','R','P',0)
#define _arp_ hex32('a','r','p',0)
#define _EAP_ hex32('E','A','P',0)
#define _eap_ hex32('e','a','p',0)
#define _IPX_ hex32('I','P','X',0)
#define _ipx_ hex32('i','p','x',0)
#define _PPPOE_ hex64('P','P','P','O','E',0,0,0)
#define _pppoe_ hex64('p','p','p','o','e',0,0,0)
#define _WOL_ hex32('W','O','L',0)
#define _wol_ hex32('w','o','l',0)
//
#define _UDP_ hex32('U','D','P',0)
#define _Udp_ hex32('U','d','p',0)
#define _udp_ hex32('u','d','p',0)
#define _TCP_ hex32('T','C','P',0)
#define _Tcp_ hex32('T','c','p',0)
#define _tcp_ hex32('t','c','p',0)
//
#define _dns_ hex32('d','n','s',0)
#define _Dns_ hex32('D','n','s',0)
#define _DNS_ hex32('D','N','S',0)
#define _ntp_ hex32('n','t','p',0)
#define _Ntp_ hex32('N','t','p',0)
#define _NTP_ hex32('N','T','P',0)
//
#define _bootp_ hex64('b','o','o','t','p',0,0,0)
#define _Bootp_ hex64('B','o','o','t','p',0,0,0)
#define _BOOTP_ hex64('B','O','O','T','P',0,0,0)
#define _dhcp_ hex32('d','h','c','p')
#define _Dhcp_ hex32('D','h','c','p')
#define _DHCP_ hex32('D','H','C','P')
#define _tftp_ hex32('t','f','t','p')
#define _Tftp_ hex32('T','f','t','p')
#define _TFTP_ hex32('T','F','T','P')
//
#define _webrtc_ hex64('w','e','b','r','t','c',0,0)
#define _Webrtc_ hex64('W','e','b','r','t','c',0,0)
#define _WEBRTC_ hex64('W','E','B','R','T','C',0,0)
#define _quic_ hex32('q','u','i','c')
#define _Quic_ hex32('Q','u','i','c')
#define _QUIC_ hex32('Q','U','I','C')
//
#define _HTTP_ hex32('H','T','T','P')
#define _Http_ hex32('H','t','t','p')
#define _http_ hex32('h','t','t','p')
#define _HTTP2_ hex64('H','T','T','P','2',0,0,0)
#define _Http2_ hex64('H','t','t','p','2',0,0,0)
#define _http2_ hex64('h','t','t','p','2',0,0,0)
#define _HTTP3_ hex64('H','T','T','P','3',0,0,0)
#define _Http3_ hex64('H','t','t','p','3',0,0,0)
#define _http3_ hex64('h','t','t','p','3',0,0,0)
//
#define _WS_   hex32('W','S',0,0)
#define _Ws_   hex32('W','s',0,0)
#define _ws_   hex32('w','s',0,0)
#define _TELNET_ hex64('T','E','L','N','E','T', 0, 0)
#define _Telnet_ hex64('T','e','l','n','e','t', 0, 0)
#define _telnet_ hex64('t','e','l','n','e','t', 0, 0)
#define _SSH_ hex32('S','S','H',0)
#define _Ssh_ hex32('S','s','h',0)
#define _ssh_ hex32('s','s','h',0)
#define _TLS1_2_  hex64('T','L','S','1','_','2', 0, 0)
#define _Tls1_2_  hex64('T','l','s','1','_','2', 0, 0)
#define _tls1_2_  hex64('t','l','s','1','_','2', 0, 0)
#define _TLS1_3_  hex64('T','L','S','1','_','3', 0, 0)
#define _Tls1_3_  hex64('T','l','s','1','_','3', 0, 0)
#define _tls1_3_  hex64('t','l','s','1','_','3', 0, 0)
//proxy
#define _PROXY_ hex64('P','R','O','X','Y', 0, 0, 0)
#define _Proxy_ hex64('P','r','o','x','y', 0, 0, 0)
#define _proxy_ hex64('p','r','o','x','y', 0, 0, 0)
#define _SOCKS_ hex64('S','O','C','K','S', 0, 0, 0)
#define _Socks_ hex64('S','o','c','k','s', 0, 0, 0)
#define _socks_ hex64('s','o','c','k','s', 0, 0, 0)
//
#define _PARTY_ hex64('P','A','R','T','Y', 0, 0, 0)
#define _Party_ hex64('P','a','r','t','y', 0, 0, 0)
#define _party_ hex64('p','a','r','t','y', 0, 0, 0)
//trav
#define _UDPTRAV_ hex64('U','D','P','T','R','A','V', 0)
#define _Udptrav_ hex64('U','d','p','t','r','a','v', 0)
#define _udptrav_ hex64('u','d','p','t','r','a','v', 0)
#define _TCPTRAV_ hex64('T','C','P','T','R','A','V', 0)
#define _Tcptrav_ hex64('T','c','p','t','r','a','v', 0)
#define _tcptrav_ hex64('t','c','p','t','r','a','v', 0)
//http family
#define _HTTPS_ hex64('H','T','T','P','S', 0, 0, 0)
#define _Https_ hex64('H','t','t','p','s', 0, 0, 0)
#define _https_ hex64('h','t','t','p','s', 0, 0, 0)
#define _WSS_ hex32('W','S','S', 0)
#define _Wss_ hex32('W','s','s', 0)
#define _wss_ hex32('w','s','s', 0)
//live stream
#define _FLV_ hex32('F','L','V',0)
#define _Flv_ hex32('F','l','v',0)
#define _flv_ hex32('f','l','v',0)
#define _RTMP_ hex32('R','T','M','P')
#define _Rtmp_ hex32('R','t','m','p')
#define _rtmp_ hex32('r','t','m','p')
//remote desktop
#define _RDP_ hex32('R','D','P',0)
#define _Rdp_ hex32('R','d','p',0)
#define _rdp_ hex32('r','d','p',0)
#define _VNC_ hex32('V','N','C',0)
#define _Vnc_ hex32('V','n','c',0)
#define _vnc_ hex32('v','n','c',0)
#define _SPICE_ hex64('S','P','I','C','E', 0, 0, 0)
#define _Spice_ hex64('S','p','i','c','e', 0, 0, 0)
#define _spice_ hex64('s','p','i','c','e', 0, 0, 0)
//download
#define _ED2K_ hex32('E','D','2','K')
#define _Ed2k_ hex32('E','d','2','k')
#define _ed2k_ hex32('e','d','2','k')
#define _MAGNET_ hex64('M','A','G','N','E','T', 0, 0)
#define _Magnet_ hex64('M','a','g','n','e','t', 0, 0)
#define _magnet_ hex64('m','a','g','n','e','t', 0, 0)
#define _TORRENT_ hex64('T','O','R','R','E','N','T', 0)
#define _Torrent_ hex64('T','o','r','r','e','n','t', 0)
#define _torrent_ hex64('t','o','r','r','e','n','t', 0)
//
#define _SQL_ hex32('S','Q','L',0)
#define _Sql_ hex32('S','q','l',0)
#define _sql_ hex32('s','q','l',0)

//
#define _analog_ hex64('a','n','a','l','o','g', 0, 0)
#define _digital_ hex64('d','i','g','i','t','a','l', 0)
#define _eeworld_ hex64('e','e','w','o','r','l','d', 0)
//
#define _field_ hex64('f','i','e','l','d', 0, 0, 0)
#define _fluid_ hex64('f','l','u','i','d', 0, 0, 0)
#define _force_ hex64('f','o','r','c','e', 0, 0, 0)
//
#define _axis3d_   hex64('a','x','i','s','3','d', 0, 0)
#define _guide3d_  hex64('g','u','i','d','e','3','d', 0)
#define _border2d_ hex64('b','o','r','d','e','r','2','d')
#define _border3d_ hex64('b','o','r','d','e','r','3','d')
#define _scene3d_  hex64('s','c','e','n','e','3','d', 0)
#define _wndmgr_  hex64('w','n','d','m','g','r', 0, 0)
//
#define _reality_ hex64('r','e','a','l','i','t','y', 0)
#define _virtual_ hex64('v','i','r','t','u','a','l', 0)
#define _mmio_ hex32('m','m','i','o')
#define _port_ hex32('p','o','r','t')
//
#define _htmlroot_ hex64('h','t','m','l','r','o','o','t')
#define _jsonroot_ hex64('j','s','o','n','r','o','o','t')
#define _xamlroot_ hex64('x','a','m','l','r','o','o','t')
#define _uiuxroot_ hex64('u','i','u','x','r','o','o','t')
//event
#define _cam1rd_ hex64('c','a','m','1','r','d', 0, 0)
#define _cam3rd_ hex64('c','a','m','3','r','d', 0, 0)
#define _camrts_ hex64('c','a','m','r','t','s', 0, 0)
#define _follow_ hex64('f','o','l','l','o','w', 0, 0)
#define _lookat_ hex64('l','o','o','k','a','t', 0, 0)
#define _wander_ hex64('w','a','n','d','e','r', 0, 0)
#define _clickray_ hex64('c','l','i','c','k','r','a','y')
#define _touchcam_ hex64('t','o','u','c','h','c','a','m')
#define _touchobj_ hex64('t','o','u','c','h','o','b','j')
//physic
#define _phys_ hex32('p','h','y','s')
#define _collide_ hex64('c','o','l','l','i','d','e',0)
#define _rigidsim_ hex64('r','i','g','i','d','s','i','m')
//
#define _graveasy_ hex64('g','r','a','v','e','a','s','y')
#define _gravtest_ hex64('g','r','a','v','t','e','s','t')
#define _gravity_ hex64('g','r','a','v','i','t','y',0)
//control
#define _carcon_ hex64('c','a','r','c','o','n', 0, 0)
#define _balancer_ hex64('b','a','l','a','n','c','e','r')
#define _dronecontrol_ hex64('d','r','o','n','e','c','o','n')
#define _planecontrol_ hex64('p','l','a','n','e','c','o','n')
#define _rocketcontrol_ hex64('r','o','c','k','e','t','c','o')
#define _satellitecontrol_ hex64('s','a','t','e','c','o','n', 0)
//sensor
#define _virtimu_ hex64('v','i','r','t','i','m','u', 0)
#define _sensor_ hex64('s','e','n','s','o','r',0,0)
#define _lidar_ hex64('l','i','d','a','r',0,0,0)
#define _radar_ hex64('r','a','d','a','r',0,0,0)
#define _ahrs_ hex32('a','h','r','s')
#define _slam_ hex32('s','l','a','m')
//motor
#define _bdc_ hex32('b','d','c',0)
#define _step_ hex32('s','t','e','p')
//
#define _audio_ hex64('a','u','d','i','o',0,0,0)
#define _video_ hex64('v','i','d','e','o',0,0,0)
#define _codec_ hex64('c','o','d','e','c',0,0,0)
//sound
#define _mic_ hex32('m','i','c',0)
#define _spk_ hex32('s','p','k',0)
//light
#define _wndcap_ hex64('w','n','d','c','a','p',0,0)
#define _cam_ hex32('c','a','m',0)
#define _wnd_ hex32('w','n','d',0)
//
#define _cap_ hex32('c','a','p',0)
#define _holo_ hex32('h','o','l','o')
#define _gpu_ hex32('g','p','u',0)
#define _render_ hex64('r','e','n','d','e','r',0,0)
//
#define _tray_ hex32('t','r','a','y')	//tray
#define _std_ hex32('s','t','d', 0)		//cmdline
//
#define _w2w_ hex32('w','2','w', 0)
#define _khala_ hex64('k','h','a','l','a', 0, 0, 0)
#define _loopback_ hex64('l','o','o','p','b','a','c','k')
//tier=_sup_, type=_wnd_, rfmt:
#define _dx11none_ hex64('d','x','1','1','n','o','n','e')
#define _dx11easy_ hex64('d','x','1','1','e','a','s','y')
#define _dx11list_ hex64('d','x','1','1','l','i','s','t')
#define _dx11cmdq_ hex64('d','x','1','1','c','m','d','q')
//
#define _dx11data_ hex64('d','x','1','1','d','a','t','a')
#define _dx11fbo6_ hex64('d','x','1','1','f','b','o','6')
#define _dx11fboc_ hex64('d','x','1','1','f','b','o','c')
#define _dx11fbod_ hex64('d','x','1','1','f','b','o','d')
#define _dx11fbog_ hex64('d','x','1','1','f','b','o','g')
//
#define _mt20none_ hex64('m','t','2','0','n','o','n','e')
#define _mt20easy_ hex64('m','t','2','0','e','a','s','y')
#define _mt20list_ hex64('m','t','2','0','l','i','s','t')
#define _mt20cmdq_ hex64('m','t','2','0','c','m','d','q')
//
#define _mt20data_ hex64('m','t','2','0','d','a','t','a')
#define _mt20fbo6_ hex64('m','t','2','0','f','b','o','6')
#define _mt20fboc_ hex64('m','t','2','0','f','b','o','c')
#define _mt20fbod_ hex64('m','t','2','0','f','b','o','d')
#define _mt20fbog_ hex64('m','t','2','0','f','b','o','g')
//
#define _gl41none_ hex64('g','l','4','1','n','o','n','e')
#define _gl41easy_ hex64('g','l','4','1','e','a','s','y')
#define _gl41list_ hex64('g','l','4','1','l','i','s','t')
#define _gl41cmdq_ hex64('g','l','4','1','c','m','d','q')
//
#define _vk12none_ hex64('v','k','1','2','n','o','n','e')
#define _vk12easy_ hex64('v','k','1','2','e','a','s','y')
#define _vk12list_ hex64('v','k','1','2','l','i','s','t')
#define _vk12cmdq_ hex64('v','k','1','2','c','m','d','q')
//
#define _vk12data_ hex64('v','k','1','2','d','a','t','a')
#define _vk12fbo6_ hex64('v','k','1','2','f','b','o','6')
#define _vk12fboc_ hex64('v','k','1','2','f','b','o','c')
#define _vk12fbod_ hex64('v','k','1','2','f','b','o','d')
#define _vk12fbog_ hex64('v','k','1','2','f','b','o','g')
//tier=_sup_, type=_ctx_, rfmt:
#define _gbuf_ hex32('g','b','u','f')
#define _ppll_ hex32('p','p','l','l')
//
#define _cli_ hex32('c','l','i',0)
#define _tui_ hex32('t','u','i',0)
#define _tui256_ hex64('t','u','i','2','5','6',0,0)
//
#define _func_ hex32('f','u','n','c')
#define _sch_  hex32('s','c','h',0)
#define _pcb_  hex32('p','c','b',0)
#define _xml_  hex32('x','m','l',0)
//tier=_ent_, type:
#define _baby_ hex32('b','a','b','y')
#define _test_ hex32('t','e','s','t')
#define _pcm_ hex32('p','c','m',0)
#define _ctx_ hex32('c','t','x',0)
#define _8bit_ hex32('8','b','i','t')
//
#define _game_ hex32('g','a','m','e')
//
#define _orig_ hex32('o','r','i','g')
#define _ORIG_ hex32('O','R','I','G')
#define _copy_ hex32('c','o','p','y')
#define _COPY_ hex32('C','O','P','Y')
//
#define _char_ hex32('c','h','a','r')
#define _joy_ hex32('j','o','y',0)
//
#define _hub_ hex32('h','u','b',0)
#define _kbd_ hex32('k','b','d', 0)
#define _mouse_ hex64('m','o','u','s','e', 0, 0, 0)
#define _gpad_ hex32('g','p','a','d')
//
#define _drag_ hex32('d','r','a','g')
#define _drag_str_ hex64('d','r','a','g','s','t','r',0)
#define _drag_ptrtbl_ hex64('d','r','a','g','c','h','*','*')
//
#define _rgb_ hex32('r','g','b',0)
#define _bgr_ hex32('b','g','r',0)
#define _rgbx_ hex32('r','g','b','x')
#define _rgba_ hex32('r','g','b','a')
//
#define _bgra8880_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '0')
#define _rgba8880_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '0')
#define _bgra8888_ hex64('b', 'g', 'r', 'a', '8', '8', '8', '8')
#define _rgba8888_ hex64('r', 'g', 'b', 'a', '8', '8', '8', '8')
//
#define _bggr_ hex32('b','g','g','r')
#define _rggb_ hex32('r','g','g','b')
//
#define _bg10_ hex32('b','g','1','0')
#define _rg10_ hex32('r','g','1','0')
//
#define _pBAA_ hex32('p','B','A','A')
#define _pGAA_ hex32('p','G','A','A')
//
#define _yuv_ hex32('y','u','v',0)
#define _yuvx_ hex32('y','u','v','x')
#define _yuyv_ hex32('y','u','y','v')
#define _uyvy_ hex32('u','y','v','y')
#define _y4_uv_ hex64 ('y','4',',','u','v', 0 , 0, 0)
#define _y4_vu_ hex64 ('y','4',',','v','u', 0 , 0, 0)
#define _y4_u_v_ hex64('y','4',',','u',',','v', 0, 0)
#define _y4_v_u_ hex64('y','4',',','v',',','u', 0, 0)
//
#define _vdec_ hex32('v','d','e','c')
#define _venc_ hex32('v','e','n','c')
//
#define _adec_ hex32('a','d','e','c')
#define _aenc_ hex32('a','e','n','c')
//
#define _mjpg_ hex32('m','j','p','g')
#define _h264_ hex32('h','2','6','4')
//event handle ret val
#define _thru_ hex32('t','h','r','u')
#define _done_ hex32('d','o','n','e')
//camera type
#define _01_ hex32('0','1', 0, 0)
#define _wh_ hex32('w','h', 0, 0)
//
#define _kv44_ hex32('k','v','4','4')
#define _kv88_ hex32('k','v','8','8')

//
#define _ev_ hex16('e','v')
//
#define _stride_ hex64('s','t','r','i','d','e',0,0)


#endif
