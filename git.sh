#! /bin/bash
comment="new"
cancel="server/build/data/user/ "
cancel=$cancel"server/build/data/group/ "
cancel=$cancel"server/build/data/log/ "

echo $#
if (($# < 2)); then
	echo set comment
	exit
fi

if [[ $1 ]]; then
	comment=$1
fi

# clean obj
cd server
make clean
cd ..

# add src

git add .
git reset HEAD $cancel
git commit -m "$comment"
git status
git push


#echo $cancel
#echo $comment
