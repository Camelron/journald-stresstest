# journald-stresstest

## steps to activate:

- clone this repo in home directory
- cd journald-stresstest
- gcc stresstest_basic.c -pthread $(pkg-config --libs libsystemd) -o stresstest_basic
- sudo ln -s "$(pwd)/stresstest.service" /etc/systemd/system/stresstest.service
- sudo systemctl daemon-reload
- sudo systemctl start stresstest.service
- sudo systemctl status stresstest.service
