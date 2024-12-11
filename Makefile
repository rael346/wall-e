.PHONY: brain/build brain/run brain/clean 
brain/build:
	$(MAKE) -C ./brain/

brain/run:
	$(MAKE) -C ./brain/ run

brain/clean:
	$(MAKE) -C ./brain/ clean

.PHONY: driver/build driver/reboot
driver/build:
	cd ./driver/build/ && cmake ..
	$(MAKE) -C ./driver/build/ -j4
	picotool load ./driver/build/driver.uf2 -f

driver/reboot:
	picotool reboot -f
