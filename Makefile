
build-i2cdriver:
	docker build -t mak3r/i2cdriver:local -f ./cri/i2cdriver.dockerfile .

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
	docker build -t mak3r/ft232h:local -f ./cri/ft232h.dockerfile .

exec-ft232h:
	docker run -it --rm -v /etc/udev/:/etc/udev/ --privileged mak3r/ft232h:local

publish-ft232h: build-ft232h
	docker tag mak3r/ft232h:local mak3r/ft232h:latest
	docker push mak3r/ft232h:latest

live:
	cp src/k8s-config/gesture-cm.yaml live/.
	cp src/k8s-config/gesture.yaml live/.
	git add src/k8s-config
	git commit -m "configmap updated $(tail -1 k8s-config/gesture-cm.yaml)"
