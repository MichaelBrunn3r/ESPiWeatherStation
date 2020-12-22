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

## Portainer
- Connect to your portainer instance `<PI-IP-ADDRESS>:9000`
- Create new user
- Select *Docker* and connect to your docker agent

# Other tips

## Disable swap (if available) for better SD-Card life
- `sudo ./menu.sh`
- *Miscellaneous Commands*
  - *Uninstall swapfile*
  - *Install log2ram*