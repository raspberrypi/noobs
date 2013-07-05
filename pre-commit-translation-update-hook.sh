#!/bin/sh
# Automatically update the translation files on every commit
#
# To enable this commit hook, run the following commands in the project root:
# chmod +x pre-commit-translation-update-hook.sh
# cp pre-commit-translation-update-hook.sh .git/hooks/pre-commit

cd "$(git rev-parse --show-toplevel)"
lupdate -no-obsolete recovery/recovery.pro
git add recovery/*.ts
