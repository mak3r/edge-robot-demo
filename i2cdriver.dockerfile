FROM python:3.10

RUN apt update & apt -y upgrade & \
apt install -y git curl

RUN git clone https://github.com/jamesbowman/i2cdriver.git; \
    cd i2cdriver/c; \
    make -f linux/Makefile; \
    make -f linux/Makefile install 

ENTRYPOINT ["/bin/bash"]
#ENTRYPOINT ["i2ccl"]
