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
	cp getLog $(CONFIG_DIR) 2>/dev/null || true
	[ -f $(CONFIG_DIR)/sysup.conf ] && (diff -u $(CONFIG_DIR)/sysup.conf sysup.conf | highlight -S diff -O ansi) || cp sysup.conf $(CONFIG_DIR)
	[ -f $(CONFIG_DIR)/phase.conf ] && (diff -u $(CONFIG_DIR)/phase.conf phase.conf | highlight -S diff -O ansi) || cp phase.conf $(CONFIG_DIR)
	[ -f $(CONFIG_DIR)/pre.sh ] && (diff -u $(CONFIG_DIR)/pre.sh pre.sh | highlight -S diff -O ansi) || cp pre.sh $(CONFIG_DIR)
	[ -f $(CONFIG_DIR)/post.sh ] && (diff -u $(CONFIG_DIR)/post.sh post.sh | highlight -S diff -O ansi) || cp post.sh $(CONFIG_DIR)
