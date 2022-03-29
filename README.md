# journald-stresstest

## steps to activate:

- clone this repo in home directory
- cd journald-stresstest
- sudo ln -s "$(pwd)/stresstest.service" /etc/systemd/system/stresstest.service
- sudo systemctl daemon-reload
- sudo systemctl start stresstest.service
- sudo systemctl status stresstest.service
