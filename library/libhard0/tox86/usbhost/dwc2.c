//https://github.com/Chadderz121/csud
#define RegOtgControl               0x0
#define RegOtgInterrupt             0x4
#define RegAhb                      0x8
#define RegUsb                      0xc
#define RegReset                    0x10
#define RegInterrupt                0x14
#define RegInterruptMask            0x18
#define RegReceivePeek              0x1c
#define RegReceivePop               0x20
#define RegReceiveSize              0x24
#define RegNonPeriodicFifoSize      0x28
#define RegNonPeriodicFifoStatus    0x2c
#define RegI2cControl               0x30
#define RegPhyVendorControl         0x34
#define RegGpio                     0x38
#define RegUserId                   0x3c
#define RegVendorId                 0x40
#define RegHardware                 0x44
#define RegLowPowerModeConfiguation 0x48

#define RegPeriodicFifoSize         0x100
#define RegPeriodicFifoBase         0x104

#define RegHostConfig        0x400
#define RegHostFrameInterval 0x404
#define RegHostFrameNumber   0x408
#define RegHostFifoStatus    0x410
#define RegHostInterrupt     0x414
#define RegHostInterruptMask 0x418
#define RegHostFrameList     0x41c
#define RegHostPort          0x440

#define RegHostChannelBase   0x500
#define RegHostChannelCharactristicBase (RegHostChannelBase + 0x0)
#define RegHostChannelSplitControl      (RegHostChannelBase + 0x4)
#define RegHostChannelInterrupt         (RegHostChannelBase + 0x8)
#define RegHostChannelInterruptMask     (RegHostChannelBase + 0xc)
#define RegHostChannelTransferSize      (RegHostChannelBase + 0x10)
#define RegHostChannelDmaAddress        (RegHostChannelBase + 0x14)
#define RegHostChannelDmaBuffer         (RegHostChannelBase + 0x1c)

#define RegPower 0xe00