clear
echo -----------------------------------------------
echo Installazione pacchetti necessari per compilare
echo il firmware RavenSystem/esp-homekit-devices
echo per Raspberry Pi3
echo by roncoa@gmail.com
echo -----------------------------------------------
echo
echo pip install esptool
pip install esptool
echo
echo sudo apt-get install git-core -y
sudo apt-get install git-core -y
echo
echo git clone https://github.com/RavenSystem/esp-homekit-devices.git
git clone https://github.com/RavenSystem/esp-homekit-devices.git
echo
echo cd esp-homekit-devices/
cd esp-homekit-devices/
echo
echo git submodule update --init --recursive
git submodule update --init --recursive
echo
echo sudo apt-get install make unrar-free autoconf automake libtool gcc g++ gperf flex bison texinfo gawk ncurses-dev libexpat-dev python-dev python python-serial sed git unzip bash help2man wget bzip2 -y
sudo apt-get install make unrar-free autoconf automake libtool gcc g++ gperf  flex bison texinfo gawk ncurses-dev libexpat-dev python-dev python python-serial sed git unzip bash help2man wget bzip2 -y
echo
echo sudo apt-get install libtool-bin -y
sudo apt-get install libtool-bin -y
echo
echo git clone --recursive https://github.com/pfalcon/esp-open-sdk.git
git clone --recursive https://github.com/pfalcon/esp-open-sdk.git
echo
echo cd esp-open-sdk/
cd esp-open-sdk/
echo
echo make toolchain esptool libhal STANDALONE=n
make toolchain esptool libhal STANDALONE=n
echo
echo PATH="$PATH:/home/pi/Desktop/HK/esp-homekit-devices/esp-open-sdk/xtensa-lx106-elf/bin"
PATH="$PATH:/home/pi/Desktop/HK/esp-homekit-devices/esp-open-sdk/xtensa-lx106-elf/bin"
echo
echo
echo
echo Per compilare il firmware usare il seguente comando:
echo make -C devices/RavenCore all
echo
echo Per impostare la porta usare il seguente comando:
echo export ESPPORT=/dev/ttyUSB0
echo
echo Per fare il test,flash,monitor,rebuild e erase_flash gli ESP, usare i seguenti comandi:
echo make -C devices/RavenCore test
echo make -C devices/RavenCore flash
echo make -C devices/RavenCore monitor
echo make -C devices/RavenCore rebuild
echo make -C devices/RavenCore erase_flash
echo
