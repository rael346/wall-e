.PHONY: brain/build
brain/build:
	$(MAKE) -C ./brain/ build

.PHONY: brain/run
brain/run:
	$(MAKE) -C ./brain/ run

.PHONY: driver/build
driver/build:
	cd ./driver/build/ && cmake ..
	$(MAKE) -C ./driver/build/ -j4
	picotool load driver.uf2 -f

.PHONY: driver/reboot
driver/reboot:
	picotool reboot -f
