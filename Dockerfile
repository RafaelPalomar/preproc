FROM ubuntu:16.04

MAINTAINER Rafael palomar (rafael.palomar@rr-research.no)

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update

RUN apt install libinsighttoolkit4-dev \
    		libvtkgdcm-cil \
		libvtkgdcm-java \
		libvtkgdcm-tools \
		libvtkgdcm2-dev \		
    		cmake \
		git \
		apt-file -y

RUN mkdir /src
RUN git clone https://github.com/RafaelPalomar/preproc /src/preproc

RUN mkdir /build
RUN cd /build && cmake ../src/preproc
