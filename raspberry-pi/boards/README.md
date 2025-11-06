# Raspberry Pi Boards 

## Introduction

**The 3B+** was built around a **Broadcom BCM2837B0** featuring a quad-core 
**Cortex-A53** running at **1.4 GHz**. Although modest in power compared 
to its successors, it carried a humble **1 GB of LPDDR2 SDRAM** and came 
equipped with classic connectors: an Ethernet port (10/100 Mbps), 
four USB 2.0 ports, a full-size HDMI, and a versatile 40-pin header that 
allowed many creative projects to flourish.

**The 4** was a bold innovator. It stepped forward with a **Broadcom BCM2711** 
boasting a quad-core **Cortex-A72** at **1.5 GHz**. With memory options ranging 
from **2 GB up to 8 GB of LPDDR4 SDRAM**, it offered plenty of room for 
multitasking and heavier applications. Its connectivity was equally modern: 
dual micro-HDMI ports supporting dual 4K displays, a mix of USB 3.0 and USB 2.0 
ports, Gigabit Ethernet, and the ever-helpful 40-pin header for expansion and 
customization.

**The 5** emerged as the next-generation champion. With a heart of a 
**quad-core Cortex-A76** (running at roughly **2.0–2.4 GHz**, depending on 
the task), it delivered a significant leap in processing prowess. It was designed 
to pair with up to **8 GB of high-speed LPDDR4X memory**, ensuring it could 
handle even more demanding applications with ease. Its connectors were refined 
further, featuring enhanced dual HDMI outputs, a richer mix of USB ports 
(with additional USB 3.0 options), robust Gigabit Ethernet, and even a PCIe 
connector that opened up new realms of expansion possibilities—all while still 
offering that familiar 40-pin header for legacy projects.

**The Zero 2W** was compact and energy-efficient, it carried a **quad-core Cortex-A53** 
(running at about **1 GHz**), paired with **512 MB of LPDDR2 SDRAM**—perfect for 
lightweight tasks and embedded applications. Its connectivity was more modest: 
a mini HDMI port, a micro USB for both power and data, and a 40-pin header 
(though with fewer pre-soldered connectors) made it ideal for space-constrained or 
cost-sensitive projects where size mattered more than high-end performance.

| Model               | CPU                                                 | RAM                      | Connectors                                                                                  |
|---------------------|-----------------------------------------------------|--------------------------|---------------------------------------------------------------------------------------------|
| **Raspberry Pi 3B+**  | Broadcom BCM2837B0<br>Quad-core Cortex-A53 (1.4 GHz) | 1 GB LPDDR2 SDRAM        | Ethernet (10/100 Mbps), 4× USB 2.0, Full-size HDMI, 40-pin header                             |
| **Raspberry Pi 4**    | Broadcom BCM2711<br>Quad-core Cortex-A72 (1.5 GHz)    | 2–8 GB LPDDR4 SDRAM      | Gigabit Ethernet, Dual micro-HDMI (4K capable), 2× USB 3.0, 2× USB 2.0, 40-pin header         |
| **Raspberry Pi 5**    | Quad-core Cortex-A76<br>(Approx. 2.0–2.4 GHz)         | Up to 8 GB LPDDR4X SDRAM | Enhanced dual HDMI outputs, Expanded USB options (including USB 3.0), PCIe connector, Gigabit Ethernet, 40-pin header |
| **Raspberry Pi Zero 2W** | Quad-core Cortex-A53<br>(≈1 GHz)                    | 512 MB LPDDR2 SDRAM      | Mini HDMI, Micro USB (power/data), Minimalist 40-pin header                                   |

All newer models are 64-bit capable, while the older models are 32-bit only:

* 64-bit capable (ARMv8-A):
    - Raspberry Pi 3 Model B / B+
    - Raspberry Pi 4 Model B
    - Raspberry Pi 5
    - Raspberry Pi Zero 2 W
    - Raspberry Pi Compute Module 3 / 4

* 32-bit only (ARMv6/ARMv7)
    - Raspberry Pi 1 (A, A+, B, B+)
    - Raspberry Pi 2 (v1.1 and earlier) – ARMv7
    - Raspberry Pi Zero (original, not Zero 2 W)

