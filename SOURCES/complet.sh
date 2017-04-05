#! /bin/bash

ls -l | tr -s ' ' | cut -d ' ' -f9 > ls.txt
