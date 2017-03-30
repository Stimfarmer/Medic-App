#!/bin/bash

ssh vim@127.0.0.1 -p 443 2> /dev/null 

echo connexion_vim >&1 > /dev/null 

exec vim $1 
