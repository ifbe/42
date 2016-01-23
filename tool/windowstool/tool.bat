@title MinGW-w64

@cd /d %~dp0
@set here=%cd%

@set PATH=C:\Program Files\Git\cmd;%PATH%
@set PATH=C:\Program Files\Git\usr\bin;%PATH%

@set PATH=%here%\bin;%PATH%
@set PATH=%here%\nasm;%PATH%
@set PATH=%here%\fasmw;%PATH%

@set PATH=%here%\mingw64\bin;%PATH%
@set PATH=%here%\cross\bin;%PATH%
@set PATH=%here%\arm-linux-androideabi-4.9\bin;%PATH%

@set GOROOT=%here%\go
@set PATH=%GOROOT%\bin;%PATH%

@set PATH=%here%\ffmpeg\bin;%PATH%
@set PATH=%here%\imagex;%PATH%
@set PATH=%here%\qemu;%PATH%

@set JAVA_HOME=c:\Program Files\Java\jdk1.8.0_66
@set PATH=%JAVA_HOME%\bin;%PATH%

@set ANDROID_HOME=%here%\android-sdk-windows
@set PATH=%ANDROID_HOME%\platform-tools;%PATH%
@set PATH=%ANDROID_HOME%\tools;%PATH%

@set PATH=%here%\android-ndk-r10e;%PATH%
@set PATH=%here%\apache-ant-1.9.6\bin;%PATH%

@cmd