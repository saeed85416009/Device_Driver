#!/bin/sh
cd /root/embedded/test/device-driver
git add -A *
echo "enter commit"
read commit
git commit -a -m "$commit"
git push --force origin master
