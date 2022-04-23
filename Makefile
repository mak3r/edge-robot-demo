
build:
	docker build -t mak3r/i2cdriver:local -f ./i2cdriver.dockerfile .

exec:
	docker run -it --rm -v /dev:/dev --privileged mak3r/i2cdriver:local

exec-gui:
	docker run -it --rm \
	-v /dev:/dev \
	-v /tmp/.X11-unix:/tmp/.X11-unix:rw \
	-e DISPLAY=$DISPLAY \
	--privileged \
	mak3r/i2cdriver:local
