# Config influxdb

https://hub.docker.com/_/influxdb

## Create admin user
- Enter container shell: `docker exec -it influxdb bash`
- `influx`
- `create user "pi" with password '<password>' with all privileges`

## Create database:
- Enter container shell: `docker exec -it influxdb bash`
- `influx`
- `create database Weather`