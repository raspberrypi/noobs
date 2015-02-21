#!/bin/sh

lupdate -qt=4 -no-obsolete recovery.pro
lrelease -qt=4 recovery.pro
