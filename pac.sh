#! /bin/bash

if [ $1 == '1' ]
then
	echo "scp"
	scp -r server root@39.97.119.22:/root/
else
	echo "ssh"
	ssh root@39.97.119.22
fi
