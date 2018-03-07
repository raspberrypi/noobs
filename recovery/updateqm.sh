#!/bin/sh

"${HOST_DIR}"/usr/bin/lupdate -no-obsolete recovery.pro || true
"${HOST_DIR}"/usr/bin/lrelease recovery.pro
