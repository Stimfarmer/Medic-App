#!/bin/bash

if [ $# = 2 ] # si 2 arguments
then
    HOST=`echo $1`
    PASS=`echo $2`
    ssh vim@$HOST -p 443 2> /dev/null 
    echo $PASS >&1 > /dev/null 
    exec vim
fi

if [ $# = 3 ] # si 3 arguments
then
    HOST=`echo $1`
    PASS=`echo $2`
    FILE=`echo $3`
    ssh vim@$HOST -p 443 2> /dev/null 
    echo $PASS >&1 > /dev/null 
    exec vim $FILE
fi
