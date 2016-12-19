# webservise
Steps to generate project:
1. Install last version of the CMake - https://cmake.org/download/
2. Install MSVC2015.
3. Install boost libraries
https://sourceforge.net/projects/boost/files/boost-binaries/
4. Set environment variable $ENV{BOOST_ROOT}
5. Run script:
cmakegen-vc14-x86.cmd
6. Build WebService and WebService-test projects.
7. Find binaries into build\vc14-x86\bin\

Test examples run:
Browse WebService/demo and follow the instractions.