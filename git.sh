#! /bin/bash
comment="new"
cancel="server/build/data/user/ "
cancel=$cancel"server/build/data/group/ "
cancel=$cancel"server/build/data/log/ "

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
echo 'I0gan' | git push


#echo $cancel
#echo $comment
