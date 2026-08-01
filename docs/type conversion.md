## Type conversion

- Since M-024 is dynamically typed, type conversion is
an default feature in its design.

<table>
    <tr>
        <td>function</td>
        <td>number</td>
        <td>string</td>
        <td>bool</td>
        <td>None</td>
        <td>list</td>
    </tr>
    <tr>
        <td>str()</td>
        <td>number to string conversion (1 -> '1')</td>
        <td>returns the same object</td>
        <td>"True" or "False"</td>
        <td>"None"</td>
        <td>under work</td>
    </tr>
    <tr>
        <td>int()</td>
        <td>returns number</td>
        <td>error</td>
        <td>1 for True, 0 for False</td>
        <td>error</td>
        <td>under work</td>
    </tr>
    <tr>
        <td>bool()</td>
        <td>True if n != 0, else False</td>
        <td>error</td>
        <td>returns the same object</td>
        <td>error</td>
        <td>under work</td>
    </tr>
</table>