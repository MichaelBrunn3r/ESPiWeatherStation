# Setup IOTStack

## Install IOTStack
- `curl -fsSL https://raw.githubusercontent.com/SensorsIot/IOTstack/master/install.sh | bash`
- `sudo ./menu.sh --run-env-setup`
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

## Setup grafana
- Connect to `<pi-ip>:3000`
- Login: usr `admin`, pwd `admin`

## Setup nodered
- Connect to `<pi-ip>:1880`


# Other tips

## Disable swap (if available) for better SD-Card life
- `sudo ./menu.sh`
- *Miscellaneous Commands*
  - *Uninstall swapfile*
  - *Install log2ram*