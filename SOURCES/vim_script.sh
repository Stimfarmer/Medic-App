#!/bin/bash

if [ $# = 3 ] # si 3 arguments
then
    HOST=`echo $1`
    PASS=`echo $2`
    DIR=`echo $3`
    ssh romain@$HOST -p 443 2> /dev/null
    echo $PASS >&1 > /dev/null
    cd $DIR
    exec vim
fi

if [ $# = 4 ] # si 4 arguments
then
    HOST=`echo $1`
    PASS=`echo $2`
    FILE=`echo $3`
    DIR=`echo $4`
    ssh romain@$HOST -p 443 2> /dev/null 
    echo $PASS >&1 > /dev/null
    cd $DIR
    exec vim $FILE
fi
