#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo ${DIR}

# for dir in ${DIR}/source ${DIR}/include ; do
# 	echo $dir
# 	find $dir -iname '*.hpp' -o -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
# done

for f in $(find ${DIR}/src -name '*.h' -o -iname '*.cpp' -o -iname '*.hpp'); 
	do 
	clang-format -i $f; 
	echo $f;
done
