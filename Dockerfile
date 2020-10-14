FROM debian:bullseye
RUN  apt update && apt install -y libserial-dev \ 
								  cmake \
								  build-essential \ 
								  libtool \
								  pkg-config \
								  libboost-program-options-dev
WORKDIR /searialtool
COPY . /searialtool/.
RUN mkdir /searialtool/build
RUN cd /searialtool/build && cmake .. && make && make install
ENTRYPOINT ["serial_tool"]
