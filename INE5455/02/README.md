## Exercise 01

To run the tests, compile the project using gradle:

```
gradle build
```

Then, use JUnit's Console Launcher:

```
java -jar junit-platform-console-standalone-1.7.2-all.jar -cp build/classes/java/test/ -cp build/libs/02.jar --scan-classpath
```

All tests from `/src/test/java/` will run.

