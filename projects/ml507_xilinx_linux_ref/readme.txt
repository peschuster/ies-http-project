This tar file contains a baseline system for the MicroBlaze Linux kernel running on the
Xilinx ML507 board.  The following files are contained in this package.

1. kernelimage, a prebuilt Linux kernel image that can be loaded into memory using XMD
2. system.bit, a prebuilt bitstream for the ML507, the kernel runs on this bitstream
3. ml507_mb_v1, an EDK project that has been cleaned, the bitstream was built from it
4. ml507_mb_v1.dts, a device tree file to build the Linux kernel for this hardware

The wiki site, http://xilinx.wikidot.com, gives details on the MicroBlaze Linux page.

This package is provided as a reference system in the spirit of open source to help
developers get MicroBlaze Linux running on the ML507 easily.  It is considered as is and
is not supported directly by Xilinx.  Please refer any questions to the Xilinx Embedded 
Linux forum.