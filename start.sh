#!/bin/sh

socat -d -d pty,raw,echo=0 pty,raw,echo=0 &
sleep 2
serial_tool