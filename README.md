mtctest
=======

A simple Apache APR multicast bind address:port test. Aimed to facilitate RHEL / HP-UX APR incompatibilities investigation.

usage
-----
It's an Eclipse CDT project...consisting of a one source file exactly :-)

 # Linux

    $ cd Fedora
    $ make
    $ ./mtctest

 # HP-UX

    $ cd HP-UX
    $ gmake
    $ ./mtctest

(yes, gmake FTW, HP-UX 11.31's make doesn't parse it...)