javah -h
pause
rem del *.class
rem javac HelloWorld.java
javah -jni -classpath ./ HelloWorld
pause