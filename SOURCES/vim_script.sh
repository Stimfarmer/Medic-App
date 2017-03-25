#!/bin/bash

ssh user@127.0.0.1 -p 443 2> /dev/null 

echo password >&1 > /dev/null 

exec vim $1 
