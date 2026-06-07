## Datatypes

- M-024 currently supports 4 datatypes namely
    - NUMBER
    - STRING
    - BOOLEAN
    - NONE
- Any integer regardless of the sign or decimals come under the
```NUMBER``` type and a separate data type for floats is now
on going.
- Any String of any length (single or multiple characters) come under
the ```STRING``` type. M-024 does not have a dedicated type for 
characters like C or Java.
- ```True```, ```False``` values come under the ```BOOLEAN``` type and ```True``` is usually interpreted as 1 and ```False``` as 0. 
- ```None``` is a special datatype that denotes nothing, it is similar to Python's None datatype. 
- Datatypes are usually set during the evaluating stage and you can see them represented as
c enums in ```include/value.h```.
