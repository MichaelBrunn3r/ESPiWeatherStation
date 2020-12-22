# Setup IOTStack

## Install IOTStack
-  Add user to 'docker' group: `sudo usermod -aG docker <user>`
- `cd ~ && git clone https://github.com/SensorsIot/IOTstack.git IOTstack && cd ~/IOTstack`
- `sudo ./menu.sh`
- Install any require dependecies
- Install docker
- `sudo ./menu.sh --run-env-setup`

## Build stack
Select containers:
  - Grafana
  - InfluxDB
  - Node-RED
    - Build addon list

Start/Build stack:
- `sudo docker-compose up -d`

# Other tips

## Disable swap (if available) for better SD-Card life
- `sudo ./menu.sh`
- *Miscellaneous Commands*
  - *Uninstall swapfile*
  - *Install log2ram*