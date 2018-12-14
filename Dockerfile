FROM ubuntu:16.04

MAINTAINER Rafael palomar (rafael.palomar@rr-research.no)

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update

RUN apt install libinsighttoolkit4-dev \
    		cmake \
		git \
		libtclap-dev \
		build-essential -y

RUN apt install parallel -y

ADD ./source /source

RUN mkdir /build
WORKDIR /build

RUN cmake ../source
RUN make

ADD ./scripts /scripts


ENV EXECUTABLE=/build/preproc
ENV INPUT_DATA_DIR=/input_data
ENV OUTPUT_DATA_DIR=/output_data

CMD ["/scripts/run.sh"]