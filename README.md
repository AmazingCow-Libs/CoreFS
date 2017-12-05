# CoreFS

**Made with <3 by [Amazing Cow](http://www.amazingcow.com).**

<!-- ----------------------------------------------------------------------- -->
<!-- Websites                                                                -->
<!-- ----------------------------------------------------------------------- -->

## Websites:

General:
* [Amazing Cow's page](http://www.amazingcow.com).
* [Amazing Cow's Github](https://github.com/AmazingCow).

Libs:
* [Amazing Cow's Libs page](https://github.com/AmazingCow-Libs).
* [Amazing Cow's Libs Github](http://www.amazingcow.com/libs).


<!-- ----------------------------------------------------------------------- -->
<!-- Description                                                             -->
<!-- ----------------------------------------------------------------------- -->

## Description:

__CoreFS__ A C++, cross-platform mashup and implementation of the parts of
[C# Environement](https://msdn.microsoft.com/en-us/library/system.environment.aspx)
class and
[python os.path](https://docs.python.org/2/library/os.path.html) module.


### Motivation:

While C/C++ (and the underlying platforms) provides everything that we need
to access the filesystem, the current way is far from optimal. There's no way
to make the filesystem calls multiplatform by default and the resulting code 
ends to be a mess of ```#ifdef``` and/or a custom wrapper everytime.

```CoreFS``` takes this opportunity to make a straightforward API based both 
on the python's os.path module and C# Environment class. This way we can use
a nice API to access the filesystem no matter which OS we're currently in.


<!-- ----------------------------------------------------------------------- -->
<!-- Examples                                                                -->
<!-- ----------------------------------------------------------------------- -->

## Examples:



<!-- ----------------------------------------------------------------------- -->
<!-- Implementation                                                          -->
<!-- ----------------------------------------------------------------------- -->
## Implementation:

| Function              | GNU/Linux     | W32   |
| --------------------- |:-------------:| -----:|
| GetPathSeparator      | Done          | Done  |
|                       |               |       |
| CurrentDirectory      | Done          | Done  |
| NewLine               | Done          | Done  |
| SystemDirectory       | Done          | Done  |
| GetFolderPath         | Done          | Done  |
| GetLogicalDrives      |               |       |
|                       |               |       |
| AbsPath               | Done          | Done  |
| Basename              | Done          | Done  |
| CommonPrefix          | Done          | Done  |
| Dirname               | Done          | Done  |
| Exists                | Done          | Done  |
| ExpandUser            | Done          |       |
| GetATime              | Done          | Done  |
| GetCTime              | Done          | Done  |
| GetMTime              | Done          | Done  |
| GetSize               | Done          | Done  |
| IsAbs                 | Done          | Done  |
| IsDir                 | Done          | Done  |
| IsFile                | Done          | Done  |
| IsLink                |               |       |
| IsMount               |               |       |
| Join                  | Done          | Done  |
| LExists               |               |       |
| NormCase              | Done          | Done  |
| NormPath              | Done          | Done  |
| RelPath               | Done          |       |
| SameFile              | Done          | Done  |
| Split                 | Done          | Done  |
| SplitAll              | Done          | Done  |
| SplitExt              | Done          | Done  |


<!-- ----------------------------------------------------------------------- -->
<!-- Building                                                                -->
<!-- ----------------------------------------------------------------------- -->

## Building:

```CoreFS``` is intended to be include as source in your project.  
So to use it, just grab the whole CoreFS directory, put it in your 
project and setup the include paths in the compiler.

As example, let's imagine that you are copying it to a directory called ```libs```.
In your build script you need to do just the following:

``` bash
g++ -I /path/to/libs/CoreFS \
    ... Other stuff ...     \
    /path/to/libs/CoreFS/CoreFS/src/CoreFS.cpp           \ 
    /path/to/libs/CoreFS/CoreFS/src/CoreFS_GNU_Linux.cpp \
    /path/to/libs/CoreFS/CoreFS/src/CoreFS_Win32.cpp 
```

Please notice that there's no harm to compile both ```src/CoreFS_GNU_Linux.cpp```
and ```src/CoreFS_Win32.cpp```, but you might only build the platform specific
one.


<!-- ----------------------------------------------------------------------- -->
<!-- Documentation                                                           -->
<!-- ----------------------------------------------------------------------- -->

## Documentation:

We strive to make our source code fully documented.   
While there are a myriad of comments, one might find useful take a look at:

* [Doxygen Docs](http://www.amazingcow.com/docs/CoreFS/).

Anyway if you didn't understand something let us know sending a mail to  
[help_floss@amazingcow.com]() with the subject filled with the name of this repo.



<!-- ----------------------------------------------------------------------- -->
<!-- Dependencies                                                            -->
<!-- ----------------------------------------------------------------------- -->

## Dependencies:

There is no dependency for ```CoreFS```.


<!-- ----------------------------------------------------------------------- -->
<!-- License                                                                 -->
<!-- ----------------------------------------------------------------------- -->

## License:

This software is **proudly** released under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html).


<!-- ----------------------------------------------------------------------- -->
<!-- BUGS                                                                    -->
<!-- ----------------------------------------------------------------------- -->

## BUGS:

We strive to make all our code the most bug-free as possible - But we know 
that few of them can pass without we notice ;).

Please if you find any bug report to [bugs_floss@amazingcow.com]() 
with the name of this project and/or create an issue here in Github.


<!-- ----------------------------------------------------------------------- -->
<!-- TODO                                                                    -->
<!-- ----------------------------------------------------------------------- -->

## TODO:

Check the TODO file for general things.

This projects uses the COWTODO tags.   
So install [cowtodo](http://www.github.com/AmazingCow-Tools/COWTODO) and run:

``` bash
$ cd path/to/the/project
$ cowtodo 
```

That's gonna give you all things to do :D.


<!-- ----------------------------------------------------------------------- -->
<!-- Others                                                                  -->
<!-- ----------------------------------------------------------------------- -->

## Others:

Check our other Free Software repos and take a look at our 
[free software site](http://www.amazingcow.com/floss).

