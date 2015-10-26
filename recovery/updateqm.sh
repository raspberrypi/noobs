#!/bin/sh

lupdate -no-obsolete recovery.pro || true
lrelease recovery.pro
