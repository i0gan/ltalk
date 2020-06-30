#! /bin/bash
if [[ $1 == '-c' ]]
then
	echo "scp"
	scp -r server ltalk@39.97.119.22:/home/ltalk/
else
	echo "ssh"
	ssh ltalk@39.97.119.22
fi
