CONFIG_DIR = /etc/sysup

.PHONY: help install
.SILENT: help

all: help

help:
	echo "Type \"make install\" to install sysup"

install:
	cp sysup /usr/sbin
	[ -d $(CONFIG_DIR) ] || mkdir $(CONFIG_DIR)
	cp functions.inc $(CONFIG_DIR)
	[ -f $(CONFIG_DIR)/sysup.conf ] || cp sysup.conf $(CONFIG_DIR)
	[ -f $(CONFIG_DIR)/phase.conf ] || cp phase.conf $(CONFIG_DIR)
	[ -f $(CONFIG_DIR)/post.sh ] || cp post.sh $(CONFIG_DIR)
