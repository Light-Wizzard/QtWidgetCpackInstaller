# QtWidgetCpackInstaller

This is an example application of a normal Qt Widget Application that can be to deployed using [cmake](https://cmake.org/), [cpack](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html), [WindeployQT](https://doc.qt.io/qt-6/windows-deployment.html) and [NSIS](https://nsis.sourceforge.io/Download).

I could not find a good example of a Deployable Qt Widget using C++, and cmake, 

```
QtWidgetCpackInstaller
F:
cd F:\workspace\QtWidgetCpackInstaller\QtWidgetCpackInstaller\build-MyQtWidgetApp-Desktop_Qt_6_4_2_MinGW_64_bit-Release
or 
F:
cd F:\workspace\QtWidgetCpackInstaller\QtWidgetCpackInstaller\build-MyQtWidgetApp-Desktop_Qt_6_4_2_MSVC2019_64bit-Release

E:\Qt\Tools\CMake_64\bin\cpack.exe -C Release -G NSIS

cd F:\workspace\QtWidgetCpackInstaller\QtWidgetCpackInstaller
"C:\Program Files\doxygen\bin\doxygen" Doxyfile
```

dir /b *.dll > dll.log
