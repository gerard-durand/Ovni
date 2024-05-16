which find
which find
which grep
which wxrc
which gcc
exit
sed
sed 's/\\\\/\\/g' all_codeblocks.pot > test.pot
sed 's/\\\\\\\\/\\\\/g' all_codeblocks.pot > test.pot
cp all_codeblocks.pot test.pot
sed -i 's/\\\\\\\\/\\\\/g' test.pot
exit
which sed
exit
exit
echo "123." | sed -E 's/([[:digit:]]*)\./\1\
next line/'
nl=$'\n'
echo "123." | sed 's/\./'"\\${nl}"'next line/'
exit
