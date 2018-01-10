#!/bin/bash
./FaceMaker
while [ $? -eq 255 ]
do 
sleep 1s
./FaceMaker
done

