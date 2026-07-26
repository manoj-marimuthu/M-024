## Commands

A new feature with a lot more to come. Inspired from shells like ```bash```,
what if we could write commands inside source code and they behave similarly
to how UNIX shells return answers. Not all commands are unix inspired and this
version has limited commands. In the upcoming version, Iam planning to add
commands such as chmod or file handling commands like grep (to make them the
standard for file handling in M-024 and not built-in functions).

### list of available commands

<table>
    <tr>
        <td>Command</td>
        <td>what it does ?</td>
        <td>Usage example</td>    
    </tr>
    <tr>
        <td>pop</td>
        <td>pops an element from the list and returns it</td>
        <td>popped = pop myList</td>
    </tr>
    <tr>
        <td>kill</td>
        <td>
            Inspired from pkill, kill deletes variables
            and functions from memory.        
        </td>
        <td>
            kill myVariable
        </td>
    </tr>
    <tr>
        <td>chmod</td>
        <td>Inspired from UNIX file permissions, M-024 uses chmod
        to handle permissions of a variable (soon functions too).
        Used as - chmod <variable_name> abc where a,b,c are integers
        and will take any values but 0 for permission denial and anything
        else for permission acceptance.    
        a - read permission, b - write permission, c - kill permission.
        Only supports current file permissions, soon will also allow permissions
        for files that import variables / functions.        
        </td>
        <td>
            x = 10
            chmod x 110
            # read - allowed, write - allowed, kill - denied
        </td>
    </tr>
</table>
