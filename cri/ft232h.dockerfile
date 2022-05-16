FROM python:3.10

# install libusb and python imaging library
RUN apt update && apt -y upgrade && \
apt install -y git curl vim libusb-1.0-0 python3-pil

WORKDIR /root

RUN pip install pyftdi

ENTRYPOINT ["/bin/bash"]