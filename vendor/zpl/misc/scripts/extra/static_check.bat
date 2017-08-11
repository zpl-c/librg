@echo off

echo -------
echo -------

set Wildcard=*.h *.cpp *.inl *.c

echo STATICS FOUND:
findstr -s -n -i -l "static" %Wildcard%

echo -------
echo -------

@echo GLOBALS FOUND:
findstr -s -n -i -l "local_persist" %Wildcard%
findstr -s -n -i -l "global_variable" %Wildcard%

echo -------
echo -------
