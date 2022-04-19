
build:
	docker build -t mak3r/i2cdriver:local -f /home/mak3r/dev/edge-robot-demo/i2cdriver.dockerfile .

exec:
	docker run -it --rm -v /dev:/dev --privileged mak3r/i2cdriver:local
