#!/bin/bash
create='--create'
remove='--remove'
move='--move'
copy='--copy'
find='--find'
clear='--clear'
removeAll='--removeAll'
if [ "$#" -eq 1 ]
then 
	IFS='=' read key value <<< $1
	if [ $key = $removeAll ]
	then
		for f in [abc]*
		do
			keys='-v'
			if [ -d $f ] 
			then
				if [[  $f -eq ccc  ||  $f -eq bbb  ]]
				then
					keys=$keys'rd'
				fi
			fi
			rm $keys $f
		done
	fi
	if [ $key = $remove ]
	then
		echo "remove"
		rm a*
	fi
	if [ $key = $move ]
	then
		echo "move"
		if [ ! -d "bbb"	]
		then
			mkdir bbb
		fi
		for f in b*
		do
			[ -f $f ] && mv -v $f bbb/${f}
		done
	fi
	if [ $key = $copy ]
	then
		echo "copy"
		if [ ! -d "ccc" ]
		then
			mkdir ccc
		fi
		for f in c*
		do
			[ -f $f ] && cp -v ${f} ccc/new_$f
		done
	fi
	if [ $key = $find ]
	then
		echo "find"
		for f in *
		do
			if [ -f $f ]
			then
				grep -F -q "$value" $f
				if [ $? -eq 0 ]
				then
					echo "found in $f"
				fi
			fi
		done
	fi
	if [ $key = $create ]
	then
		echo "key is create"
		IFS=',' read -a counts <<< $value
		if [ ${#counts[@]} -eq 3 ]
		then
			echo "check"
			chars=('a' 'b' 'c')
			length=$((${#counts[@]}-1))
			echo $length
			for i in $(seq 0 1 $length)
			do
				for j in $(seq 1 1 ${counts[$i]})
				do
					> "${chars[$i]}$j".txt
				done
			done
		else
			echo "error, --create=number,number,number"
		fi
	fi
fi
