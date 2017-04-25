#!/bin/bash 


if [ $# = 3 ] # si 3 arguments
then
    HOST=`echo $1`
    #set HOST [lindex $argv 0]
    PASS=`echo $2`
    #set PASS [lindex $argv 1]
    DIR=`echo $3`
    #set DIR [lindex $argv 2]
    #spawn xterm 
    sshpass -p $PASS ssh -o StrictHostKeyChecking=no -t vim@$HOST `expect script.exp $DIR` 2> /dev/null
fi

if [ $# = 4 ] # si 4 arguments
then
    HOST=`echo $1`
    PASS=`echo $2`
    FILE=`echo $3`
    DIR=`echo $4`
    sshpass -p $PASS ssh -o StrictHostKeyChecking=no -t vim@$HOST `expect script_arg.exp $DIR $FILE` 2> /dev/null
fi
