# imx6ull Kernel启动日志分析

## 1、Raw log

![image-20230217001940272](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230217001940272.png)

```shell
Starting kernel ...

[    0.000000] Booting Linux on physical CPU 0x0
[    0.000000] Linux version 4.9.88 (book@100ask) (gcc version 7.5.0 (Buildroot 2020.02-gee85cab) ) #1 SMP PREEMPT Tue Sep 27 03:24:15 EDT 2022
[    0.000000] CPU: ARMv7 Processor [410fc075] revision 5 (ARMv7), cr=10c53c7d
[    0.000000] CPU: div instructions available: patching division code
[    0.000000] CPU: PIPT / VIPT nonaliasing data cache, VIPT aliasing instruction cache
[    0.000000] OF: fdt:Machine model: Freescale i.MX6 ULL 14x14 EVK Board
[    0.000000] Memory policy: Data cache writealloc
[    0.000000] percpu: Embedded 15 pages/cpu @8bbcf000 s30156 r8192 d23092 u61440
[    0.000000] Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 130048
[    0.000000] Kernel command line: console=ttymxc0,115200 root=/dev/mmcblk1p2 rootwait rw
[    0.000000] PID hash table entries: 2048 (order: 1, 8192 bytes)
[    0.000000] Dentry cache hash table entries: 65536 (order: 6, 262144 bytes)
[    0.000000] Inode-cache hash table entries: 32768 (order: 5, 131072 bytes)
[    0.000000] Memory: 172880K/524288K available (11264K kernel code, 1103K rwdata, 4040K rodata, 1024K init, 477K bss, 351408K reserved, 0K cma-reserved, 0K highmem)
[    0.000000] Virtual kernel memory layout:
[    0.000000]     vector  : 0xffff0000 - 0xffff1000   (   4 kB)
[    0.000000]     fixmap  : 0xffc00000 - 0xfff00000   (3072 kB)
[    0.000000]     vmalloc : 0xa0800000 - 0xff800000   (1520 MB)
[    0.000000]     lowmem  : 0x80000000 - 0xa0000000   ( 512 MB)
[    0.000000]     pkmap   : 0x7fe00000 - 0x80000000   (   2 MB)
[    0.000000]     modules : 0x7f000000 - 0x7fe00000   (  14 MB)
[    0.000000]       .text : 0x80008000 - 0x80c00000   (12256 kB)
[    0.000000]       .init : 0x81100000 - 0x81200000   (1024 kB)
[    0.000000]       .data : 0x81200000 - 0x81313c70   (1104 kB)
[    0.000000]        .bss : 0x81315000 - 0x8138c6d8   ( 478 kB)
[    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
[    0.000000] Preemptible hierarchical RCU implementation.
[    0.000000]  Build-time adjustment of leaf fanout to 32.
[    0.000000]  RCU restricting CPUs from NR_CPUS=4 to nr_cpu_ids=1.
[    0.000000] RCU: Adjusting geometry for rcu_fanout_leaf=32, nr_cpu_ids=1
[    0.000000] NR_IRQS:16 nr_irqs:16 16
[    0.000000] Switching to timer-based delay loop, resolution 333ns
[    0.000019] sched_clock: 32 bits at 3000kHz, resolution 333ns, wraps every 715827882841ns
[    0.000069] clocksource: mxc_timer1: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 637086815595 ns
[    0.004310] Console: colour dummy device 80x30
[    0.004365] Calibrating delay loop (skipped), value calculated using timer frequency.. 6.00 BogoMIPS (lpj=30000)
[    0.004407] pid_max: default: 32768 minimum: 301
[    0.004664] Mount-cache hash table entries: 1024 (order: 0, 4096 bytes)
[    0.004699] Mountpoint-cache hash table entries: 1024 (order: 0, 4096 bytes)
[    0.006388] CPU: Testing write buffer coherency: ok
[    0.006490] ftrace: allocating 35227 entries in 104 pages
[    0.140485] /cpus/cpu@0 missing clock-frequency property
[    0.140538] CPU0: thread -1, cpu 0, socket 0, mpidr 80000000
[    0.140610] Setting up static identity map for 0x80100000 - 0x80100058
[    0.229277] Brought up 1 CPUs
[    0.229316] SMP: Total of 1 processors activated (6.00 BogoMIPS).
[    0.229340] CPU: All CPU(s) started in SVC mode.
[    0.230938] devtmpfs: initialized
[    0.248864] OF: Duplicate name in lcdif@021c8000, renamed to "display#1"
[    0.255283] VFP support v0.3: implementor 41 architecture 2 part 30 variant 7 rev 5
[    0.256140] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns
[    0.256198] futex hash table entries: 256 (order: 2, 16384 bytes)
[    0.256594] pinctrl core: initialized pinctrl subsystem
[    0.259183] NET: Registered protocol family 16
[    0.259930] DMA: preallocated 256 KiB pool for atomic coherent allocations
[    0.291034] cpuidle: using governor menu
[    0.350690] hw-breakpoint: found 5 (+1 reserved) breakpoint and 4 watchpoint registers.
[    0.350728] hw-breakpoint: maximum watchpoint size is 8 bytes.
[    0.355258] imx6ul-pinctrl 20e0000.iomuxc: Invalid fsl,pins property in node /soc/aips-bus@02000000/iomuxc@020e0000/imx6ul-evk/enet1grp
[    0.356102] imx6ul-pinctrl 20e0000.iomuxc: initialized IMX pinctrl driver
[    0.356655] imx6ul-pinctrl 2290000.iomuxc-snvs: no fsl,pins property in node /soc/aips-bus@02200000/iomuxc-snvs@02290000/imx6ul-evk
[    0.356759] imx6ul-pinctrl 2290000.iomuxc-snvs: unable to find group for node hoggrp-2
[    0.356944] imx6ul-pinctrl 2290000.iomuxc-snvs: initialized IMX pinctrl driver
[    0.505276] mxs-dma 1804000.dma-apbh: initialized
[    0.513433] SCSI subsystem initialized
[    0.523988] usbcore: registered new interface driver usbfs
[    0.524197] usbcore: registered new interface driver hub
[    0.524470] usbcore: registered new device driver usb
[    0.527065] i2c i2c-0: IMX I2C adapter registered
[    0.527122] i2c i2c-0: can't use DMA, using PIO instead.
[    0.529522] i2c i2c-1: IMX I2C adapter registered
[    0.529577] i2c i2c-1: can't use DMA, using PIO instead.
[    0.530872] Linux video capture interface: v2.00
[    0.531063] pps_core: LinuxPPS API ver. 1 registered
[    0.531089] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giometti <giometti@linux.it>
[    0.531159] PTP clock support registered
[    0.534345] MIPI CSI2 driver module loaded
[    0.535592] imx rpmsg driver is registered.
[    0.536107] Advanced Linux Sound Architecture Driver Initialized.
[    0.538392] Bluetooth: Core ver 2.22
[    0.538527] NET: Registered protocol family 31
[    0.538552] Bluetooth: HCI device and connection manager initialized
[    0.538597] Bluetooth: HCI socket layer initialized
[    0.538635] Bluetooth: L2CAP socket layer initialized
[    0.538734] Bluetooth: SCO socket layer initialized
[    0.553858] clocksource: Switched to clocksource mxc_timer1
[    0.742829] VFS: Disk quotas dquot_6.6.0
[    0.743014] VFS: Dquot-cache hash table entries: 1024 (order 0, 4096 bytes)
[    0.772633] NET: Registered protocol family 2
[    0.774399] TCP established hash table entries: 4096 (order: 2, 16384 bytes)
[    0.774531] TCP bind hash table entries: 4096 (order: 3, 32768 bytes)
[    0.774694] TCP: Hash tables configured (established 4096 bind 4096)
[    0.774840] UDP hash table entries: 256 (order: 1, 8192 bytes)
[    0.774904] UDP-Lite hash table entries: 256 (order: 1, 8192 bytes)
[    0.775329] NET: Registered protocol family 1
[    0.796676] RPC: Registered named UNIX socket transport module.
[    0.796712] RPC: Registered udp transport module.
[    0.796733] RPC: Registered tcp transport module.
[    0.796754] RPC: Registered tcp NFSv4.1 backchannel transport module.
[    0.803261] Bus freq driver module loaded
[    0.818408] workingset: timestamp_bits=30 max_order=16 bucket_order=0
[    0.849381] NFS: Registering the id_resolver key type
[    0.849457] Key type id_resolver registered
[    0.849481] Key type id_legacy registered
[    0.849603] jffs2: version 2.2. (NAND) © 2001-2006 Red Hat, Inc.
[    0.851552] fuse init (API version 7.26)
[    0.870744] io scheduler noop registered
[    0.870783] io scheduler deadline registered
[    0.871270] io scheduler cfq registered (default)
[    0.872648] imx-weim 21b8000.weim: Driver registered.
[    0.882009] backlight supply power not found, using dummy regulator
[    0.890577] sii902x 1-0039: No reset pin found
[    0.894474] 21c8000.lcdif supply lcd not found, using dummy regulator
[    1.011348] Console: switching to colour frame buffer device 128x37
[    1.049514] swapper/0: page allocation failure: order:13, mode:0x24000c1(GFP_KERNEL|GFP_DMA)
[    1.050904] mxsfb 21c8000.lcdif: Unable to allocate overlay fb memory
[    1.050933] mxsfb 21c8000.lcdif: failed to map video mem for overlay
[    1.073956] mxsfb 21c8000.lcdif: Success seset LCDIF
[    1.073995] mxsfb 21c8000.lcdif: initialized
[    1.077591] imx-sdma 20ec000.sdma: loaded firmware 3.3
[    1.089191] 2020000.serial: ttymxc0 at MMIO 0x2020000 (irq = 20, base_baud = 5000000) is a IMX
[    1.817337] console [ttymxc0] enabled
[    1.822710] imx6ul-pinctrl 2290000.iomuxc-snvs: unable to find group for node uart3_rs485
[    1.831223] imx-uart: probe of 21ec000.serial failed with error -22
[    1.838482] 21fc000.serial: ttymxc5 at MMIO 0x21fc000 (irq = 74, base_baud = 5000000) is a IMX
[    1.885604] imx-rng 2284000.rngb: iMX RNG Registered.
[    1.891508] imx sema4 driver is registered.
[    1.896055] [drm] Initialized
[    1.900433] [drm] Initialized vivante 1.0.0 20120216 on minor 0
[    2.103282] brd: module loaded
[    2.186033] loop: module loaded
[    2.199432] imx6ul-pinctrl 2290000.iomuxc-snvs: unable to find group for node spi4grp
[    2.207669] spi_gpio: probe of spi4 failed with error -22
[    2.215792] spi_imx 2008000.ecspi: probed
[    2.222634] spi_imx 2010000.ecspi: probed
[    2.230060] libphy: Fixed MDIO Bus: probed
[    2.235599] CAN device driver interface
[    2.242575] flexcan 2090000.can: device registered (reg_base=a2a38000, irq=29)
[    2.254055] 20b4000.ethernet supply phy not found, using dummy regulator
[    2.305234] pps pps0: new PPS source ptp0
[    2.310626] libphy: fec_enet_mii_bus: probed
[    2.329438] fec 20b4000.ethernet eth0: registered PHC device 0
[    2.336967] 2188000.ethernet supply phy not found, using dummy regulator
[    2.395138] pps pps1: new PPS source ptp1
[    2.401890] fec 2188000.ethernet eth1: registered PHC device 1
[    2.410111] PPP generic driver version 2.4.2
[    2.415164] PPP BSD Compression module registered
[    2.419925] PPP Deflate Compression module registered
[    2.425151] PPP MPPE Compression module registered
[    2.429989] NET: Registered protocol family 24
[    2.434599] SLIP: version 0.8.4-NET3.019-NEWTTY (dynamic channels, max=256) (6 bit encapsulation enabled).
[    2.444474] CSLIP: code copyright 1989 Regents of the University of California.
[    2.451827] SLIP linefill/keepalive option.
[    2.456429] usbcore: registered new interface driver asix
[    2.462065] usbcore: registered new interface driver ax88179_178a
[    2.468459] usbcore: registered new interface driver cdc_ether
[    2.474551] usbcore: registered new interface driver cdc_eem
[    2.480385] usbcore: registered new interface driver net1080
[    2.486299] usbcore: registered new interface driver cdc_subset
[    2.492386] usbcore: registered new interface driver zaurus
[    2.498268] usbcore: registered new interface driver cdc_ncm
[    2.504038] ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
[    2.510647] ehci-platform: EHCI generic platform driver
[    2.516986] ehci-mxc: Freescale On-Chip EHCI Host driver
[    2.522644] ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
[    2.529033] ohci-platform: OHCI generic platform driver
[    2.535215] usbcore: registered new interface driver cdc_acm
[    2.540930] cdc_acm: USB Abstract Control Model driver for USB modems and ISDN adapters
[    2.549289] usbcore: registered new interface driver usblp
[    2.555053] usbcore: registered new interface driver cdc_wdm
[    2.560942] usbcore: registered new interface driver usb-storage
[    2.567375] usbcore: registered new interface driver usbserial
[    2.573367] usbcore: registered new interface driver usbserial_generic
[    2.580166] usbserial: USB Serial support registered for generic
[    2.586438] usbcore: registered new interface driver option
[    2.592172] usbserial: USB Serial support registered for GSM modem (1-port)
[    2.599401] usbcore: registered new interface driver usb_ehset_test
[    2.605767] random: fast init done
[    2.612403] 2184800.usbmisc supply vbus-wakeup not found, using dummy regulator
[    2.622441] 2184000.usb supply vbus not found, using dummy regulator
[    3.163909] ci_hdrc ci_hdrc.0: there is a vbus glitch
[    3.171369] 2184200.usb supply vbus not found, using dummy regulator
[    3.182124] ci_hdrc ci_hdrc.1: EHCI Host Controller
[    3.187268] ci_hdrc ci_hdrc.1: new USB bus registered, assigned bus number 1
[    3.223967] ci_hdrc ci_hdrc.1: USB 2.0 started, EHCI 1.00
[    3.232021] hub 1-0:1.0: USB hub found
[    3.236179] hub 1-0:1.0: 1 port detected
[    3.243493] mousedev: PS/2 mouse device common for all mice
[    3.251805] input: 20cc000.snvs:snvs-powerkey as /devices/soc0/soc/2000000.aips-bus/20cc000.snvs/20cc000.snvs:snvs-powerkey/input/input0
[    3.266823] imx6ul-pinctrl 20e0000.iomuxc: pin MX6UL_PAD_GPIO1_IO01 already requested by 2010000.ecspi; cannot claim for 2040000.tsc
[    3.279078] imx6ul-pinctrl 20e0000.iomuxc: pin-24 (2040000.tsc) status -22
[    3.286098] imx6ul-pinctrl 20e0000.iomuxc: could not request pin 24 (MX6UL_PAD_GPIO1_IO01) from group tscgrp  on device 20e0000.iomuxc
[    3.298376] imx6ul-tsc 2040000.tsc: Error applying setting, reverse things back
[    3.305847] imx6ul-tsc: probe of 2040000.tsc failed with error -22
[    3.313371] usbcore: registered new interface driver usbtouchscreen
[    3.320394] Gt9xx driver installing..
[    3.324430] imx6ul-pinctrl 2290000.iomuxc-snvs: unable to find group for node tscresetgrp
[    3.332678] goodix-ts: probe of 1-005d failed with error -22
[    3.342437] snvs_rtc 20cc000.snvs:snvs-rtc-lp: rtc core: registered 20cc000.snvs:snvs-r as rtc0
[    3.353669] i2c /dev entries driver
[    3.358853] IR NEC protocol handler initialized
[    3.363445] IR RC5(x/sz) protocol handler initialized
[    3.368658] IR RC6 protocol handler initialized
[    3.373232] IR JVC protocol handler initialized
[    3.377859] IR Sony protocol handler initialized
[    3.382519] IR SANYO protocol handler initialized
[    3.387306] IR Sharp protocol handler initialized
[    3.392045] IR MCE Keyboard/mouse protocol handler initialized
[    3.397957] IR XMP protocol handler initialized
[    3.406197] pxp-v4l2 pxp_v4l2: initialized
[    3.416219] imx2-wdt 20bc000.wdog: timeout 60 sec (nowayout=0)
[    3.423121] Bluetooth: HCI UART driver ver 2.3
[    3.427773] Bluetooth: HCI UART protocol H4 registered
[    3.432957] Bluetooth: HCI UART protocol BCSP registered
[    3.438368] Bluetooth: HCI UART protocol ATH3K registered
[    3.444106] usbcore: registered new interface driver bcm203x
[    3.450039] usbcore: registered new interface driver btusb
[    3.458076] sdhci: Secure Digital Host Controller Interface driver
[    3.464449] sdhci: Copyright(c) Pierre Ossman
[    3.468851] sdhci-pltfm: SDHCI platform and OF driver helper
[    3.476935] sdhci-esdhc-imx 2190000.usdhc: could not get ultra high speed state, work on normal mode
[    3.486331] sdhci-esdhc-imx 2190000.usdhc: Got CD GPIO
[    3.555560] mmc0: SDHCI controller on 2190000.usdhc [2190000.usdhc] using ADMA
[    3.564400] sdhci-esdhc-imx 2194000.usdhc: could not get ultra high speed state, work on normal mode
[    3.634042] mmc1: SDHCI controller on 2194000.usdhc [2194000.usdhc] using ADMA
[    3.652757] ledtrig-cpu: registered to indicate activity on CPUs
[    3.676618] usbcore: registered new interface driver usbhid
[    3.682256] usbhid: USB HID core driver
[    3.721920] wm8960 1-001a: No platform data supplied
[    3.769676] mmc1: new DDR MMC card at address 0001
[    3.786407] mmcblk1: mmc1:0001 S40004 3.64 GiB
[    3.801951] mmcblk1boot0: mmc1:0001 S40004 partition 1 4.00 MiB
[    3.818982] mmcblk1boot1: mmc1:0001 S40004 partition 2 4.00 MiB
[    3.836104] mmcblk1rpmb: mmc1:0001 S40004 partition 3 4.00 MiB
[    3.851725]  mmcblk1: p1 p2 p3
[    3.866449] imx-wm8960 sound: wm8960-hifi <-> 202c000.sai mapping ok
[    3.877105] imx-wm8960 sound: snd-soc-dummy-dai <-> 2034000.asrc mapping ok
[    3.884720] imx-wm8960 sound: wm8960-hifi <-> 202c000.sai mapping ok
[    4.028494] NET: Registered protocol family 26
[    4.035024] NET: Registered protocol family 10
[    4.052683] sit: IPv6, IPv4 and MPLS over IPv4 tunneling driver
[    4.060730] NET: Registered protocol family 17
[    4.065463] can: controller area network core (rev 20120528 abi 9)
[    4.071903] NET: Registered protocol family 29
[    4.076529] can: raw protocol (rev 20120528)
[    4.080848] can: broadcast manager protocol (rev 20161123 t)
[    4.086636] can: netlink gateway (rev 20130117) max_hops=1
[    4.092880] Bluetooth: RFCOMM TTY layer initialized
[    4.098032] Bluetooth: RFCOMM socket layer initialized
[    4.103266] Bluetooth: RFCOMM ver 1.11
[    4.107272] Bluetooth: BNEP (Ethernet Emulation) ver 1.3
[    4.112632] Bluetooth: BNEP filters: protocol multicast
[    4.117993] Bluetooth: BNEP socket layer initialized
[    4.122999] Bluetooth: HIDP (Human Interface Emulation) ver 1.2
[    4.129035] Bluetooth: HIDP socket layer initialized
[    4.134384] 8021q: 802.1Q VLAN Support v1.8
[    4.138671] lib80211: common routines for IEEE802.11 drivers
[    4.144554] Key type dns_resolver registered
[    4.152908] cpu cpu0: Registered imx6q-cpufreq
[    4.214355] sii902x 1-0039: No reset pin found
[    4.220953] imx_thermal 2000000.aips-bus:tempmon: Industrial CPU temperature grade - max:105C critical:100C passive:95C
[    4.233410] sii902x 1-0039: No reset pin found
[    4.239838] input: gpio-keys as /devices/soc0/gpio-keys/input/input1
[    4.248098] sii902x 1-0039: No reset pin found
[    4.252987] snvs_rtc 20cc000.snvs:snvs-rtc-lp: setting system clock to 1970-01-01 00:00:00 UTC (0)
[    4.263152] ltemodule-pwr: disabling
[    4.267009] wifi-pwr: disabling
[    4.270186] ALSA device list:
[    4.273175]   #0: wm8960-audio
[    4.288722] EXT4-fs (mmcblk1p2): couldn't mount as ext3 due to feature incompatibilities
[    4.400227] EXT4-fs (mmcblk1p2): recovery complete
[    4.407102] EXT4-fs (mmcblk1p2): mounted filesystem with ordered data mode. Opts: (null)
[    4.415397] VFS: Mounted root (ext4 filesystem) on device 179:2.
[    4.422493] devtmpfs: mounted
[    4.427979] Freeing unused kernel memory: 1024K
[    5.730315] EXT4-fs (mmcblk1p2): re-mounted. Opts: data=ordered
Starting syslogd: OK
Starting klogd: OK
Running sysctl: OK
Starting 100ask lvgl: OK
[    6.242151] ds18b20_drv_init OK.
[    6.277199] dht11_drv_init OK.
[    6.326815] irda_drv_init OK.
Populating /dev using udev: [    6.468713] udevd[172]: starting version 3.2.9
[    6.626306] udevd[173]: starting eudev-3.2.9
[    7.765264] inv-mpu6000-spi spi2.0: mounting matrix not found: using identity...
[    7.957363] sii902x 1-0039: No reset pin found
done

```

## 2、Analysis

```C
Starting kernel ...

[ 0.000000] Booting Linux on physical CPU 0x0 //Shows that the system has started booting
[ 0.000000] Linux version 4.9.88 (book@100ask) (gcc version 7.5.0 (Buildroot 2020.02-gee85cab) ) #1 SMP PREEMPT Tue Sep 27 03:24:15 EDT 2022 //Displays the version of the Linux Kernel and date
[ 0.000000] CPU: ARMv7 Processor [410fc075] revision 5 (ARMv7), cr=10c53c7d //Identifies the Processor
[ 0.000000] CPU: div instructions available: patching division code //Shows that division code is patched
[ 0.000000] CPU: PIPT / VIPT nonaliasing data cache, VIPT aliasing instruction cache //Shows the CPU data and instruction cache specifications
[ 0.000000] OF: fdt:Machine model: Freescale i.MX6 ULL 14x14 EVK Board //Describes the Machine Model
[ 0.000000] Memory policy: Data cache writealloc //Shows that Data Cache write allocation is used
[ 0.000000] percpu: Embedded 15 pages/cpu @8bbcf000 s30156 r8192 d23092 u61440 //Shows per-CPU statistics
[ 0.000000] Built 1 zonelists in Zone order, mobility grouping on. Total pages: 130048 //Describes the page allocation strategy
[ 0.000000] Kernel command line: console=ttymxc0,115200 root=/dev/mmcblk1p2 rootwait rw //Displays kernel boot parameters
[ 0.000000] PID hash table entries: 2048 (order: 1, 8192 bytes) //Shows the process ID
[ 0.000000] Dentry cache hash table entries: 65536 (order: 6, 262144 bytes) //Shows Dentry cache hash statistics
[ 0.000000] Inode-cache hash table entries: 32768 (order: 5, 131072 bytes) //Shows Inode cache hash statistics
[ 0.000000] Memory: 172880K/524288K available (11264K kernel code, 1103K rwdata, 4040K rodata, 1024K init, 477K bss, 351408K reserved, 0K cma-reserved, 0K highmem) //Describes Memory availability and usage
[ 0.000000] Virtual kernel memory layout: //Displays Virtual memory details
[ 0.000000] vector : 0xffff0000 - 0xffff1000 ( 4 kB) //Specifies memory layout of vector
[ 0.000000] fixmap : 0xffc00000 - 0xfff00000 (3072 kB) //Specifies memory layout of fixmap
[ 0.000000] vmalloc : 0xa0800000 - 0xff800000 (1520 MB) //Specifies memory layout of vmalloc
[ 0.000000] lowmem : 0x80000000 - 0xa0000000 ( 512 MB) //Specifies memory layout of lowmem
```

