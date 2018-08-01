# Ambient room temperature indicator for Baby

This is simple Arduino Pro Micro powered Room temperature monitor with coloured indicator LED.

![Photos](/Photos/Collage.png)

## Features

* LED is GREEN when the temperature in the room is within the ideal range (by default 18°C -> 23°C)
* LED is RED when the temperature in the room rises above the ideal range
* LED is BLUE when the temperature in the room falls below the ideal range
* LED flashes when 2°C on either side of the ideal range to draw the eye
* LED auto dims depending on the ambient room brightness

## Parts List

* 1x Arduino Pro Micro
* 1x LDR (Light Dependent Resistor)
* 1x LM335z (Temperature sensor)
* 1x RGB LED (Common Anode)
* Resistors
    * 2x 100Ω
    * 1x 150Ω
    * 1x 2.2kΩ
    * 1x 10kΩ

## How to build it

![Breadboard](/Schematics/Baby%20Ambient%20Room%20Temperature%20Monitor%20(Breadboard).png)

![Schematic](/Schematics/Baby%20Ambient%20Room%20Temperature%20Monitor%20(Schematic).png)

## Notes

* The schematic incorrectly shows the LM35 temperature sensor because I was too lazy to find a fritzing part for the LM335z

## Future capability

This is designed to be quite a simple circuit using parts I had lying around so my desires to extend its capabilities are minimal, but there are a few things I may do.

* **TODO:**
    * 3d Printed Enclosure
    
* **Maybe:**
    * Leverage the _Sleep_ capability of the Pro Micro and attach it to a small 3.7v LiPo battery
    * Add a Potentiometer to calibrate the ideal 5°C temperature range
    * Use an ESP32 or ESP8266 with WiFi instead of the Pro Micro to enable some cool push notification or IoT functionality
    
