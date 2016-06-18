#!/bin/sh

set -x
SDR_DEFRATE=${SDR_DEFRATE-48000}
export SDR_DEFRATE
echo Rate: $SDR_DEFRATE

export SDR_MODE=$PWD/hook_mode

./usbsoftrock -vvv ptt off
./usbsoftrock -vvv status
#./usbsoftrock -vvv -x 114.230181 -d &		# SR6.2
./usbsoftrock -vvv -x 114.2633 -d &		# SR6.3
PIDS="$PIDS $!"

#
# Receive
#
export SDR_NAME=sdr
#./sdr-core -v -s -m &
./sdr-core -s -m &
PIDS="$PIDS $!"
sleep 1
jack_lsp
jack_connect sdr:ol system:playback_1 
jack_connect sdr:or system:playback_2
jack_connect system:capture_1 sdr:il        
jack_connect system:capture_2 sdr:ir     
jack_lsp -c

#
# Transmit
#
SDR_DEFRATE=${SDR_DEFRATE-48000}
export SDR_DEFRATE
export SDR_PARMPORT=19005
export SDR_SPECPORT=19006
export SDR_METERPORT=19007
export SDR_NAME=sdr-tx
./sdr-core -v -s -m &
PIDS="$PIDS $!"
sleep 1
#jack_lsp
jack_connect sdr-tx:ol system:playback_3
jack_connect sdr-tx:or system:playback_4
jack_connect system:capture_3 sdr-tx:il        
jack_connect system:capture_4 sdr-tx:ir
echo system:capture_3 system:capture_4 >.phinput
echo $PIDS
./sdr-shell
kill $PIDS
wait

