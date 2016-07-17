#!/bin/bash

uuid=$(sudo blkid -o value -s UUID /dev/$2)
cat $1 | sed "s/hashish/${uuid}/g"
