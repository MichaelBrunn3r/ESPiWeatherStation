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

## Continuous queries and retention policy
To compress older data, we use Continuous Queries to compute and store averages.

Create Retention Policy that deletes raw data older than 1 week:
```
CREATE RETENTION POLICY "1week" ON "Weather" DURATION 1w REPLICATION 1
```

Create CQ that downsamples raw data into 1h timeframes:
```
CREATE CONTINUOUS QUERY downsample_1h ON Weather RESAMPLE EVERY 1d FOR 1d6h BEGIN SELECT mean(temp) AS temp, mean(hum) AS hum, mean(press) AS press, mean(batv) AS batv INTO stations_1h FROM "1week".stations GROUP BY time(1h), mac END
```

# Tips

## InfluxQL commands
- Insert measurement: `INSERT <measurement_name>,<tag>=<value>,... <field>=<value>,... <timestamp>`
- Remove Retention Policy: `DROP RETENTION POLICY "<policy_name>" ON <db_name>`
- Remove CQ: `DROP CONTINUOUS QUERY "<cq_name>" ON <db_name>`

