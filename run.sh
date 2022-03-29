#!/bin/bash 
sudo tdnf install python3-systemd 
python3 /home/cameronbaird/journald-stresstest/stresstest.py & 




while true
do
	sleep 3
done
