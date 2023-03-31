# DLTPowerPlugin

DLTPowerPlugin is used to control Power Supplies for Test Automation.
Different Power supplies are supported.
It provides an interface to DLT for Logging and Test Automation.

![Image of DLTPowerPlugin](https://github.com/alexmucde/dlt-sim/blob/main/plugins/DLTPowerPlugin/doc/images/DLTPower.jpg)

For further information about DLT visit the DLT Viewer project:

https://github.com/GENIVI/dlt-viewer

## Supported Hardware

The following Hardware is currently supported:

* Power supply: Manson HCS-3302 or Voltcraft PPS-11810 or compatible [Amazon Germany Manson](https://amzn.to/2QUWQ4D) [Amazon Germany Voltcraft](https://amzn.to/3e33EWJ)
  * Interface: USB Serial
  * Protocol: See user manual

## DLT injection commands

The commands are interpreted independent of the service id.

* \<Power name\> on/off
* VOLTAGE \<value\>

## Contributing

Contibutions are always welcome! Please provide a Pull Request on Github.

https://github.com/alexmucde/dlt-sim

## Donation

If you find this SW useful and you want to donate my work please select one of the following donations:

Paypal Donation:

[![Donations](https://www.paypalobjects.com/en_US/DK/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=YBWSNXYWJJP2Q)

Github Sponsors:

[:heart: Sponsor](https://github.com/sponsors/alexmucde)

## Changes

v0.0.1:

* Initial version

## Copyright

Alexander Wenzel <alex@eli2.de>

This code is licensed under Mozilla Public license.
