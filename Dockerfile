FROM ubuntu:latest

MAINTAINER Rafael palomar (rafael.palomar@rr-research.no)

RUN apt update

RUN apt install libinsighttoolkit4-dev -y

RUN apt install cmake git -y