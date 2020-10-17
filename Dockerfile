FROM debian:bullseye
RUN  apt update && apt install -y libserial-dev \ 
								  cmake \
								  build-essential \ 
								  libtool \
								  pkg-config \
								  libboost-program-options-dev \
								  picocom \ 
								  socat
WORKDIR /searialtool
COPY . /searialtool/.
RUN mkdir /searialtool/build
RUN cd /searialtool/build && cmake .. && make && make install
ENTRYPOINT ["sh","/searialtool/start.sh"]