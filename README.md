# Edge Robot Demo
Cloud native methodologies for the edge using a humanoid robotic arm as the target. 

*Note: this project uses i2c technology which is a robust, long standing control technology and which also suffers from the linguistic problem of using the terms master and slave. Going forward, in this project, the terms Control or Controller and Agent will be used in place of the legacy terminology.*

## Preparation

1. Build the inmoov robotic arm http://inmoov.fr/hand-and-forarm/
1. Arduino Uno
1. [Adafruit servo board for use with Arduino](https://www.adafruit.com/product/1411)
1. Power supply for Arduino and Servo board
1. Computer with i2c or usb capability and Kubernetes

    1. Linux
    1. k3s
    1. pyftdi
    1. i2c

        * For aarch64

            * Use i2c-tools or ftdi usb-i2c bridge like FT232H 

        * For x86

            * Use usb-i2c bridge

## Setup

1. Install the arduino program on the Uno

    1. Also depends on the following libraries
    
        * [Make the Arduino an i2c agent device](https://github.com/mak3r/i2c-arduino-slave)
        * [Adafruit servo board api](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)
        
1. Install [pyftdi](https://github.com/eblot/pyftdi) on the linux host
1. Connect your kubernetes host to the Arduino
1. Connect the robot arm to the arduino

## Wiring diagrams

## Operating the demo

1. Insure the Arduino has the basic robot arm driver program.
2. Deploy the robot-arm-update container

## Cloud Native Features Utilized

* Operational consistency
* Identification of available devices via Node Feature Discovery or Akri
* Modification of a running application via system devices (udev)
* Rollback of a running application 
* Data collection and monitoring of a remote device 
* Gitops process for managing application deployments
* Policy/process gates for holding/releasing updated software


## Demo Content

* `arduino/` - the directory for arduino sketches
* `src/` - the directory with source code / content for execution in containers
* `scripts/` - various utility scripts
* `config/` - k8s yaml configurations
* `fleet-live/` - a directory for connecting with Rancher / Fleet GitOps functionality
* `cri/` - dockerfiles for building container runtime images
* `Makefile` - to build images and other makey stuff
