FT232_IMAGE_TAG=v0.0.4

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
	docker tag mak3r/ft232h:local mak3r/ft232h:${FT232_IMAGE_TAG}
	docker push mak3r/ft232h:${FT232_IMAGE_TAG}

live:
	cp k8s-config/gesture-cm.yaml fleet-live/.
	cp k8s-config/gesture.yaml fleet-live/.
	git add fleet-live
	git commit -m "configmap updated $(tail -1 k8s-config/gesture-cm.yaml)"

configmap-update:
	kubectl get cm/gesture-config -oyaml | sha256sum

install-service:
	sudo cp ez-booth/service/btn-control.service /etc/systemd/system/btn-control.service
	sudo cp ez-booth/src/btn-control.py /usr/local/bin/btn-control.py
	sudo mkdir -p /etc/btn-control
	sudo cp ez-booth/btn-control.conf /etc/btn-control/btn-control.conf

run-service: install-service
	sudo systemctl enable btn-control.service
	sudo systemctl start btn-control.service

stop-service:
	sudo systemctl stop btn-control.service
	sudo systemctl disable btn-control.service

clean-service: stop-service
	sudo rm -r /etc/systemd/system/btn-control.service \
	/usr/local/bin/btn-control.py \
	/etc/btn-control/