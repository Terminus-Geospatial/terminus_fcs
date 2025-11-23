#  Sample Usage of the FCS in a C++ Application

This directory contains a sample application that demonstrates how to use the FCS in a C++ application.

## Run the command

```bash
./demo_app_01 -c config.toml \
              -p app.database.host=localhost \
              -p app.database.port=5432 \
              -p app.database.ssl=true \
              -p app.server.workers=4 \
              -p app.server.memory_limit=1024 \
              --debug
```

Where:

*  `-c CONFIG_FILE` is the path to the configuration file
*  `-p KEY=VALUE` is a property to set
    * Note:  `KEY` may be nested such as `key1.key2.key3`.
*  Optional Log Severity
    * `--trace` is `TRACE` level
    * `--debug` is `DEBUG` level
    * `--info` is `INFO` level
    * `--warning` is `WARNING` level
    * `--error` is `ERROR` level
* `--help` prints usage instructions.
    * Note:  Detailed flags require app to have a schema.
* `-g|--gen-config` generates a default configuration file.
    * Note:  Detailed flags require app to have a schema.

## Demo Schema

A schema is created in the `demo_app_01` application to validate the configuration file.  The schema checks values each of the settings in the configuration file and overriding command-line parameters.