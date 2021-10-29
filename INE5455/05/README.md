## Exercise 01

To build, test and make the covarage verification, use gradle:

```
gradle build
gradle test
gradle build jacocoTestReport
gradle jacocoTestCoverageVerification
```

You can see the covarege test result with a browser:

```
firefox build/reports/jacoco/test/html/index.html
```
