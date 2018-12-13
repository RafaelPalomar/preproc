FROM ubuntu:16.04

MAINTAINER Rafael palomar (rafael.palomar@rr-research.no)

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update

RUN apt install libinsighttoolkit4-dev \
    		cmake \
		git \
		libtclap-dev \
		build-essential -y

RUN mkdir /src
ADD https://api.github.com/repos/RafaelPalomar/preproc/git/refs/heads/master version.json
RUN  git clone -b master https://github.com/RafaelPalomar/preproc /src/preproc

RUN mkdir /build
WORKDIR /build

RUN cmake ../src/preproc/source
RUN make

ENV EXECUTABLE=/build/preproc
ENV INPUT_DATA_DIR=/input_data
ENV OUTPUT_DATA_DIR=/output_data

CMD ["/src/preproc/scripts/run.sh"]