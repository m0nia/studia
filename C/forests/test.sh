#!/bin/bash

# Skrypt wypisuje tylko błędne testy. Na koniec wypisuje podsuwanie, ile z nich zakończyło się błędem wykonania, a ile ma błędny wynik

PROGRAM_NAME=forests
TESTS=0
ERROR=0
WRONG_OUTPUT=0

TMP_DIR=$(mktemp -d -t $PROGRAM_NAME-tests-XXXXXX)

for f in "$2/"*.in
do
	STDOUT_DIFF=1
	STDERR_DIFF=1

	#wykonuję testowany program dla testu: $f

	file_name=$(basename $f)

	out=$TMP_DIR/test.out

	err=$TMP_DIR/test.err

	<$f ./$PROGRAM_NAME 1>$out 2>$err

	if [ $? != 0 ]; then
	 	echo "Program zakończył się błędem dla testu $file_name."
	 	((ERROR++))
	else
		#sprawdzam czy standardowe wyjście jest takie samo
		diff $out ${f%in}out
		
		if [ $? != 0 ]; then
			STDOUT_DIFF=0;
			echo "Niepoprawne standardowe wyjście dla testu $file_name."
		fi

		#sprawdzam czy standardowe wyjście błędów takie samo
		diff $err ${f%in}err

		if [ $? != 0 ]; then
			STDERR_DIFF=0;
			echo "Niepoprawne standardowe wyjście diagnostyczne dla testu $file_name."
		fi

		if [ $STDOUT_DIFF == 0 ] || [ $STDERR_DIFF == 0 ]; then
			((WRONG_OUTPUT++))
		fi

	fi
	((TESTS++))
		
done

echo "Wykonano $TESTS testów. $ERROR zakończyło się błędem. $WRONG_OUTPUT testów ma niepoprawny wynik."


# Valgrind - test wycieków pamięci

VALGRIND_FLAGS="--error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all"
ERROR=0
MEMORY_LEAK=0
TESTS=0

echo "Valgrind testy:"

for f in "$2/"*.in
do
	((TESTS++))
	<$f valgrind $VALGRIND_FLAGS ./$PROGRAM_NAME >/dev/null 2>&1
	EXIT_CODE=$?
	if [ $EXIT_CODE == 15 ]; then
		echo "Prawdopodobny wyciek pamięci."
		((MEMORY_LEAK++))
	elif [ $EXIT_CODE != 0 ]; then
		echo "Program zakończył się błędem. Kod wyjścia: $EXIT_CODE"
		((ERROR++))
	fi
done

echo "$ERROR z $TESTS testów zakończyło się błędem. W $MEMORY_LEAK nastąpił wyciek pamięci."

#Usuwam tymczasowe pliki

rm -rf $TMP_DIR