# dest_sensor_01

Andrew Pullin, 2016

This firmware will read pulses from an SM-PWM-01A particulate matter sensor and report the reading at regular intervals via the Particle publishing system. \
Both the Low Pulse Occupancy (LPO) and the total pulse count are reported for both channels. The LPO is simply the effective duty cycle of the low-state over the total reporting interval.

For more information, see the sensor datasheet here: http://amphenol-sensors.com/en/component/edocman/3-co2/4-co2-modules/194-sm-pwm-01c

The output of an example sensor located in an indoor environment in Berkeley, CA can be found here: http://andrewpull.in/sensor.html

(Note: Plotting is accomiplished by using a webhook with the Particle Cloud services to push data to Librato, www.librato.com)

This firmware has been tested on both a [Particle Photon](https://docs.particle.io/datasheets/kits/#photon) board and the [Sparkfun Photon Redboard](https://www.sparkfun.com/products/13321), which uses a Particle module. No code changes are needed between the platforms.

## Wiring

The code expects the PM2.5 sensor line (LOW = particle present) to be connected to pin D2, and the PM10 sensor to be connected to D3. These signals needs to be level shifted to 3.3v, to match the core voltage of the Particle board that you are using.

Other input pins can be used, but the pin must support GPIO interrupt; see the Particle documentation.

## Building

Unfortunately, the Particle Build IDE does not seem to have github integration or a "share" button, so you will have to copy and past the code into correctly-named files inside the IDE.

The publishing frequency can be changed with the PUBLISH_INTERVAL_MS macro. Default is a new publish every 20 seconds.

## TODO

* Add JSON file for webhook creation.
* Add wiring diagram.
* Add example setup picture.

## Wishlist

* Change to a more general MQTT publishing scheme.
* Static queue of timestamped readings, use rate-limited MQTT to report latest samples and clear backlog.