# sdr-shell-v4
SDR-Shell is a graphical user interface for sdr-core (DttSP), a Software Defined Radio. It is developed under Ubuntu Linux, but should work on other flavors of Linux and Unix operating systems.

This branch (v4) can operate a SoftRock SDR for transmit and receive.
It uases two sdr-core processes, one each for receive and transmit, and
usbsoftrock for tuning the USB-controllable synthesizer on the SoftRock SDR.

Getting a radio working with sdr-shell, dttsp, and utilities is a lot like
building your own radio, but with less solder.  Instead of solder wick and
a solder sucker, you have a reset button.

For information about the SoftRock SDR, see:

*	<http://www.fivedash.com>		The designer's site
*	<http://www.wb5rvz.com/sdr>	Build instructions
*	softrock40@yahoogroups.com	Soft Rock discussion group (Very active)
*	dttsp-linux@yahoogroups.com	the dttsp discussion group

sdr-shell requires:
* dttsp (two copies: one recieve one transmit)
* usbsoftrock 1.0.1

usbsoftrock is available from the softrock40 Yahoo Groups files area,
under VK6JBL's directory (usbsoftrock-1.0.1.tar.gz).

Please see the file "README" for more general information about the program.

Please see the file "INSTALL" for build instructions.


