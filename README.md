# PrimeSieve
To run the Prime Sieve example we first have to generate the build solution.

## Installing ZPM
On Linux, run (this installation does not require privileges):

```
wget -O - https://raw.githubusercontent.com/Zefiros-Software/ZPM/master/script/install-zpm.sh | bash
```

On OSX:

```
wget -O - https://raw.githubusercontent.com/Zefiros-Software/ZPM/master/script/install-zpm-osx.sh | bash
```

On Windows:

```
powershell -command "Invoke-WebRequest -Uri https://raw.githubusercontent.com/Zefiros-Software/ZPM/master/script/install-zpm.bat -OutFile %TEMP%/install-zpm.bat" && %TEMP%/install-zpm.bat && set PATH=%PATH%;%ALLUSERSPROFILE%/zpm
```

## Generating the Makefile
By default there is no makefile available, so we generate this using:

```
zpm gmake
```

## Building

A release build:

```
make config=release_x86_64
```

A debug build:

```
make config=debug_x86_64
```