# serialtool

A tool which can read and write on HEX or DEC mode to serial port.

## Requirements
```
docker
```

## Building *nix
```
git clone https://github.com/moldyreed/serialtool.git
cd serialtool
docker build -t serial .
```

## Usage
Run tool in docker 
```
docker run -it serial
```
and then run picocom in other terminal 
```
docker exec -it $(docker ps | grep serial | awk '{print $1}') picocom -b 115200 --echo --imap 8bithex /dev/pts/2
```
now you can type a number in specific mode to send it to picocom and back.
