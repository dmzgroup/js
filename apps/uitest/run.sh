#!/bin/sh

. ../../../scripts/envsetup.sh

$RUN_DEBUG$BIN_HOME/dmzAppQt -f config/js.xml config/console.xml config/input.xml $*
