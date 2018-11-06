FROM ubuntu:16.04

MAINTAINER Rafael palomar (rafael.palomar@rr-research.no)

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update

RUN apt install libinsighttoolkit4-dev \
    		cmake \
		git \
		build-essential -y

RUN mkdir /src
RUN  git clone https://github.com/RafaelPalomar/preproc /src/preproc

RUN mkdir /build
WORKDIR /build

RUN cmake ../src/preproc
make