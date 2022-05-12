FROM python:3.10

RUN apt update && apt -y upgrade && \
apt install -y git curl vim

RUN git clone https://github.com/jamesbowman/i2cdriver.git; \
    cd i2cdriver/c; \
    make -f linux/Makefile; \
    make -f linux/Makefile install 

COPY src /root/src
WORKDIR /root

ENTRYPOINT ["/bin/bash"]
#ENTRYPOINT ["i2ccl"]
