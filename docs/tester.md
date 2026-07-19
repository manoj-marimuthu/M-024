## tester.mscf

- The standard testing library used primarily in the test suite
for this repository and is built for anyone who instantly needs a library
to write tests. It contains the following functions:

<table>
    <tr>
        <td>Function name</td>
        <td>What it does ?</td>
    </tr>
    <tr>
        <td>testEqual(a,b,test_name)</td>
        <td>prints success message if a == b else prints the failure message</td>
    </tr>
    <tr>
        <td>testAlmostEqual(a,b,test_name)</td>
        <td>prints the success message if a == b or if |a - b| < 0.01 else prints the failure message</td>
    </tr>
    <tr>
        <td>testNotEqual</td>
        <td>prints the success message if a != b else prints the failure message</td>
    </tr>
    <tr>
        <td>testTrue(a,test_name)</td>
        <td>prints the success message if a is True else prints the failure message</td>
    </tr>
    <tr>
        <td>testFalse(a,test_name)</td>
        <td>prints the success message if a is False else prints the failure message</td>
    </tr>
</table>

- All test functions use the test_name parameter to inform the user of the test if it has
failed.

- A success / pass message looks like:
```bash
[OK] MyCustomTest
```

- and a failed test looks like:

```bash
[FAILED] MyCustomTest | Expected: 1, Received: 2 
```

- the ```log_success()``` and ```log_failure()``` functions inside the module are
used by these test functions and handle the pretty printing.