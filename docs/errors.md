## Errors

M-024 has a set of errors that it raises on various occasions and this section will cover why these errors
occur, what type they are and what to do in order to debug them. The below table consists of all error types,
what causes them and hints to debug:


<table>
    <tr>
        <td>Error type</td>
        <td>Cause</td>
        <td>Hints</td>
    </tr>
    <tr>
        <td>SYNTAX_ERROR</td>
        <td>Usage of incorrect syntax</td>
        <td>Check the other documentation files to understand the grammar</td>
    </tr>
    <tr>
        <td>RUNTIME_ERROR</td>
        <td>many scenarios (run grep to see all) such as doing invalid operations etc</td>
        <td>Check error line, refer docs to know the feature limitations.</td>
    </tr>
    <tr>
        <td>COMPILE_TIME_ERROR</td>
        <td>Due to memory allocation failure from one of the C-API functions</td>
        <td>Rerun the program, Open issue/PR in the repo so that I can work it out.</td>
    </tr>
    <tr>
        <td>DIVISION_BY_ZERO_ERROR</td>
        <td>When you divide a number by 0</td>
        <td>Validate inputs.</td>
    </tr>
    <tr>
        <td>FILE_NOT_FOUND_ERROR</td>
        <td>Source file for interpretation is not found</td>
        <td>Check paths (relative to the .exe)</td>
    </tr>
    <tr>
        <td>FILE_ARG_ERROR</td>
        <td>Invalid unknown arguments are passed through the command line to the exe</td>
        <td>Check for typos, Read documentation for imports and only meta command supported is --version</td>
    </tr>
    <tr>
        <td>FILE_EXTENSION_ERROR</td>
        <td>Source file extension is incorrect</td>
        <td>Use .mscf for all files including main program and imports</td>
    </tr>
    <tr>
        <td>INDENTATION_ERROR</td>
        <td>Indentation is not matching correctly or when not in order correctly</td>
        <td>Check line number, adjust sentences correctly (same as python, refer for better understanding)</td>
    </tr>
    <tr>
        <td>FUNCTION_ERROR</td>
        <td>Function related errors such as incorrect parameters, unknown function calls etc</td>
        <td>Check line number and if function is declared. Read function definition and check if parameters are passed correctly in order (since no keyword or default arguments are supported)</td>
    </tr>
    <tr>
        <td>INDEXING_ERROR</td>
        <td>Incorrect indexing of a string or a list object.</td>
        <td>Check if index is a valid number, in valid range (for negative indices too).</td>
    </tr>
    <tr>
        <td>IMPORT_ERROR</td>
        <td>mount related errors such as deadlocks</td>
        <td>Check documentation for imports, difference between library and other program imports.</td>
    </tr>
</table>

If any of these do not help, Feel free to open an issue.
