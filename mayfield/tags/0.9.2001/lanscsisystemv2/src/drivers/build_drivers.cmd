@echo off
setlocal enableextensions
set BUILDER=call ..\..\bin\ddkbuild

:argcheck
if /I "%1-" equ "/?-" (
	goto usage
)

if /I "%1-" equ "/h-" (
	goto usage
)

if /I "%1-" equ "/help-" (
	goto usage
)

if /I "%1-" equ "/nolfs-" (
	set nolfs=1
	shift
	goto argcheck
)

if /I "%1-" equ "/k-" (
	set _noerrorstop=1
	shift
	goto argcheck
)

set CONF=%1
set PARAM=%2 %3 %4 %5 %6 %7 %8 %9

if "%CONF%-" == "-" (
	goto usage;
)

echo BUILDMSG: %BUILDER% -WNET2K %CONF% . %PARAM%
%BUILDER% -WNET2K %CONF% . %PARAM%

if not defined _noerrorstop (
	if errorlevel 1 goto builderr
)

if not defined nolfs (
	if defined _noerrorstop (
		call .\build_lfs.cmd /k %CONF% %PARAM%
	) else (
		call .\build_lfs.cmd %CONF% %PARAM%
	)
)

endlocal
goto :EOF

:builderr
echo ERROR: Error on build %ERRORLEVEL%
goto :EOF

:usage
echo Builds all driver projects.
echo.
echo build_drivers [/nolfs] [/k] ^<chk^|fre^> [build_options]
echo.
echo   /nolfs         do not call build_lfs on build
echo   /k             stop on first error (nmake stop only)
echo   ^<chk^|fre^>      checked or free build
echo   build_options  options for build.exe (e.g. -cegZ)
goto :EOF