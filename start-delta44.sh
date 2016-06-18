#!/bin/sh

# Start-Up for sdr-shell, dttsp, and usbsoftrock.  Oriented towards a Softrock
# RXTX board.

set -x
export SDR_DEFRATE=96000

pasuspender -- /usr/bin/jackd -R -P66 -p128 -t2000 -dalsa -r96000 -D -Chw:2 -Phw:2 -s -p 2048&
JPID="$!"

sleep 2

#
#  Start usbsoftrock.  Set your calibration valeu on the -x line.
#  If you have it internally calibrated using advanced mode, put a '-a'
#  here instead of the -x.
#
./usbsoftrock ptt off
#./usbsoftrock -vvvv -x 114.230181 -d &
./usbsoftrock -vvv -x 114.2630 -d &
PIDS="$PIDS $!"

export SDR_NAME=sdr
./sdr-core -s -m&
PIDS="$PIDS $!"
sleep 1
jack_connect sdr:ol alsa_pcm:playback_1 
jack_connect sdr:or alsa_pcm:playback_2
jack_connect alsa_pcm:capture_1 sdr:il        
jack_connect alsa_pcm:capture_2 sdr:ir     
jack_lsp -c

export SDR_NAME=sdr-tx
export SDR_PARMPORT=19005
export SDR_SPECPORT=19006
export SDR_METERPORT=19007
./sdr-core -s -m -v &
PIDS="$PIDS $!"
sleep 1
jack_lsp
jack_connect sdr-tx:ol alsa_pcm:playback_3
jack_connect sdr-tx:or alsa_pcm:playback_4
case $1 in
metronome )
	jack_metro -b 60 &
	PIDS="$PIDS $!"
	sleep 1
	jack_connect metro:60_bpm sdr-tx:il        
	jack_connect metro:60_bpm sdr-tx:ir
;;
* )
	jack_connect alsa_pcm:capture_3 sdr-tx:il
	jack_connect alsa_pcm:capture_4 sdr-tx:ir
;;
esac

echo $PIDS

export SDR_MODE=$PWD/hook-mode
#export SDR_BAND=$PWD/hook-band
./sdr-shell

# kill jack clients before killing jack himself.
# this seems to be a cleaner shutdown than everything at once

kill $PIDS
sleep 2
kill $JPID
wait

