# Driver for controlling motors and ultrasonic sensor

## Build and flash the Pico
1. Fetch the sdk 
```sh
git submodule update --init
```

2. Create a build folder
```sh
mkdir build
cd build
```

3. Build the project
```sh
# inside the build folder
cmake ..
# this was build on the raspberry pi 5
# to utilize the cores for build speed -j4 flag is use
make -j4 
```

4. Flash the project onto the board (highly recommend using picotool)
- Using picotool
```sh
# inside the build folder
picotool load main.uf2
picotool reboot
```

- Manually
```sh
# check which sd port the pico is on
# if it is sdb then the port will be /dev/sdb1 instead
dmesg | tail
  [371.973555] sd 0:0:0:0: [sda] Attached SCSI removable disk

# Create a mount
sudo mkdir -p /mnt/pico
# mount the port
sudo mount /dev/sda1 /mnt/pico

# Now we should see the content of the pico
ls /mnt/pico/
  INDEX.HTM INFO_UF2.TXT

# Copy the uf2 into the pico
sudo cp blink.uf2 /mnt/pico
sudo sync

# Unmount the pico
# (the pico should have done this automatically after the copy above)
sudo umount /mnt/pico
```
