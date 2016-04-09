if not exist "C:\PLED" mkdir C:\PLED
xcopy /s .\install\* C:\PLED\

@echo off
set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"

echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SCRIPT%
echo sLinkFile = "%USERPROFILE%\Desktop\PLED.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.TargetPath = "C:\PLED\PLED_GUI.exe" >> %SCRIPT%
echo oLink.IconLocation = "C:\PLED\src\PLED-icon.ico" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%


cscript /nologo %SCRIPT%
del %SCRIPT%