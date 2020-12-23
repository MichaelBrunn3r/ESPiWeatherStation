# Setup IOTStack

## Install IOTStack
- `curl -fsSL https://raw.githubusercontent.com/SensorsIot/IOTstack/master/install.sh | bash`
- `sudo ./menu.sh --run-env-setup`
- Install any require dependecies
- Install docker
- `sudo ./menu.sh --run-env-setup`

# Create stack
Select containers:
  - Heimdall
    - Fix env file: `mkdir services/heimdall && touch services/heimdall/heimdall.env`
  - Grafana
  - InfluxDB
  - Node-RED
    - Build addon list
  - (Eclipse) Mosquitto

Start/Build stack:
- `sudo docker-compose up -d`

# Configure containers

## InfluxDB
Create database:
- Enter container shell: `docker exec -it influxdb bash`
- `influx`
- `CREATE DATABASE Weather`
- `USE Weather`

## Grafana
- Connect to `<pi-ip>:3000`
- Login: usr `admin`, pwd `admin`

## NodeRed
- Connect to `<pi-ip>:1880`

## Heimdall
- Change ports to `80` and `443`
- Add links to local apps:
    - e.g. `http://<pi-ip>:3000`
    - Tip: Some routers can act as a local DNS: e.g. `<pi-hostname>.fritz.box:3000`

# Other tips

## Disable swap (if available) for better SD-Card life
- `sudo ./menu.sh`
- *Miscellaneous Commands*
  - *Uninstall swapfile*
  - *Install log2ram*