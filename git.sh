#! /bin/bash
comment="new"
cancel="server/build/data/user/ "
cancel=$cancel"server/build/data/group/ "
cancel=$cancel"server/build/data/log/ "

if [[ $1 ]]; then
	comment=$1
fi

git add .
git reset HEAD $cancel
git commit -m "$comment"
git status
git push

#echo $cancel
#echo $comment
