set echo off
set USRBIN=C:\Program Files\Git\usr\bin\
set OPENSSL_DIR=openssl-1.0.1t
set OPENSSL_ARCHIVE=%OPENSSL_DIR%.tar.gz
set OPENSSL_URL=https://www.openssl.org/source/%OPENSSL_ARCHIVE%
set VC_DIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\x86_amd64\

@rem ------------------------------------------------------------
@rem check
@rem ------------------------------------------------------------
perl -v
if ERRORLEVEL 1 goto NO_PERL
if not exist "C:\Perl64\bin" goto NO_PERL
if not exist "%USRBIN%curl.exe" goto NO_CURL
if not exist "%VC_DIR%" goto NO_VC


@rem ------------------------------------------------------------
@rem openssl download
@rem ------------------------------------------------------------
if not exist %OPENSSL_ARCHIVE% "%USRBIN%sh.exe" -c "/bin/curl -O %OPENSSL_URL%"

@rem ------------------------------------------------------------
@rem openssl extract
@rem ------------------------------------------------------------
if exist %OPENSSL_DIR% "%USRBIN%sh.exe" -c "/bin/rm -rf %OPENSSL_DIR%"
"%USRBIN%sh.exe" -c "PATH=/usr/bin:$PATH /bin/tar xf %OPENSSL_ARCHIVE%"

@rem ------------------------------------------------------------
@rem openssl build
@rem ------------------------------------------------------------
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
pushd %OPENSSL_DIR%
perl Configure --prefix=..\openssl64 no-asm VC-WIN64A
call ms\do_win64a.bat
nmake -f ms\nt.mak install
popd

@rem ------------------------------------------------------------
@rem OK
@rem ------------------------------------------------------------
:SUCCESS
echo
echo success !!
echo
start %OPENSSL_DIR%
exit /B 0

@rem ------------------------------------------------------------
@rem Errors
@rem ------------------------------------------------------------
:NO_CURL
echo no curl
exit /B 1

:NO_VC
echo no vc. install visual studio 2015(c++ option)
exit /B 2

:NO_PERL
echo no perl. install ActivePerl
exit /B 4

