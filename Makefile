.PHONY: help install
.SILENT: help

all: help

help:
	echo "Type \"make install\" to install sysup"

install:
	cp sysup /usr/sbin
	[ -d /etc/sysup ] || mkdir /etc/sysup
	[ -f /etc/sysup/sysup.conf ] || cp sysup.conf /etc/sysup
	[ -f /etc/sysup/phase.conf ] || cp phase.conf /etc/sysup
	[ -f /etc/sysup/post.sh ] || cp post.sh /etc/sysup
