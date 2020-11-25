#!/bin/sh

lupdate -no-obsolete recovery.pro -qt=5 || true
lrelease recovery.pro
