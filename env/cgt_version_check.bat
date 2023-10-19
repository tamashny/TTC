@echo off

rem ############################################################################
rem # arguments:
rem #   %1: holds the path to the file containing the version info
rem # 
rem # return value:
rem #   returns the error level set by findstr
rem ############################################################################

rem check against the following version
set CGT_VERSION=5.1.6

findstr /E /L "v%CGT_VERSION%" %1

if ERRORLEVEL 1 (
  echo ERROR: TI ARM Code Generation Tools version mismatch!
  echo Please use version %CGT_VERSION% of the TI ARM Code Generation Tools
)