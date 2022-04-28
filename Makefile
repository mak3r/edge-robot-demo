
build-i2cdriver:
	docker build -t mak3r/i2cdriver:local -f ./i2cdriver.dockerfile .

exec-i2cdriver:
	docker run -it --rm -v /dev:/dev --privileged mak3r/i2cdriver:local

exec-gui-i2cdriver:
	docker run -it --rm \
	-v /dev:/dev \
	-v /tmp/.X11-unix:/tmp/.X11-unix:rw \
	-e DISPLAY=$DISPLAY \
	--privileged \
	mak3r/i2cdriver:local

build-ft232h:
	docker build -t mak3r/ft232h:local -f ./ft232h.dockerfile .

exec-ft232h:
	docker run -it --rm -v /etc/udev/:/etc/udev/ --privileged mak3r/ft232h:local

