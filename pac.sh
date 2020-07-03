#! /bin/bash
if [[ $1 == '-c' ]]
then
	echo "scp"
	scp -r server ltalk@ltalk.co:/home/ltalk/
else
	echo "ssh"
	ssh ltalk@ltalk.co
fi
