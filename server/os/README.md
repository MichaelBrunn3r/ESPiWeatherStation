# Install and setup OS

## Flash the SD Card
I recommend using Raspberry Pi OS Lite, as we won't need the desktop environment

- Download the Raspberry Pi Imager from [here](https://www.raspberrypi.org/software/) and start it
- Choose OS > Raspberry Pi OS (other) > Raspberry Pi OS Lite (32-bit)
- Choose your SD Card
- Write

## Setup WiFi and SSH before boot
Since we are lazy, we don't want to make the effort to attach a keyboard and monitor to the Pi. Instead we configure the Pis WiFi connection and enable SSH before ever booting it.<br>
For that we need the two files 'ssh', which enables SSH, and 'wpa_supplicant.conf', which configures the WiFi connection.

- Create empty file `ssh` on the SD Cards boot partition
- Copy the file `wpa_supplicant.conf` on the SD Cards boot partition
- Configure the settings in `wpa_supplicant.conf`

Now, after inserting the SD Card and booting, we can setup the Pi via SSH.

## SSH into the Pi server
- `ssh pi@<PI-IP-ADDRESS>`
- usr=`pi`, pwd=`raspberry`

## Change the default password
- Enter the command `passwd` and follow the steps

## Install software

Essential:
- Update packages: `sudo apt update && sudo apt upgrade -y`
- `sudo apt install git python3-pip curl -y`

Optional:
- `tmux`
- `vim` or `neovim`

All recommended in one cmd:
- `sudo apt update && sudo apt upgrade -y && sudo apt install git tmux python3-pip curl neovim -y`

# Other tips and tricks

## German Qwertz to English Qwerty map:
If the Pis keyboard layout is configured as EN but your keyboard has a QWERTZ layout.

|Key EN|Key DE|
|-|-|
|-|ß|
|/|#|

## Change keyboard to german QWERTZ layout
* `sudo raspi-config`
* *Localisation Options* > *Change Keyboard Layout* > *Generic 105-key PC*
* *Other* > *German* > *German (QWERTZ)*
* Press Enter for all other options

## Disable WiFi
If you want to use your Pi with ethernet instead, you can disable WiFi entirely.
- `rfkill list` (e.g. `rfkill 0`)