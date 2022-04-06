#!/bin/bash 
sudo tdnf install python3-systemd 
python3 /home/cameronbaird/journald-stresstest/stresstest.py & 
sudo ./home/cameronbaird/journald-stresstest/stresstest_basic
