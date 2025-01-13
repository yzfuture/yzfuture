
@echo off
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0
bcdedit >nul
if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
:UACPrompt
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
:UACAdmin
cd /d "%~dp0"

sc delete yzwlReadCard
sc delete yzwlServiceAlive
sc create yzwlReadCard start= auto binpath= "%~dp0yzwlReadCardServer.exe" DisplayName= "yzwlReadCard¶Á¿¨Çý¶¯"
sc create yzwlServiceAlive start= auto binpath= "%~dp0yzwlServiceAlive.exe" DisplayName= "yzwlServiceAlive¶Á¿¨Çý¶¯¿´ÊØ"
sc start yzwlReadCard
sc start yzwlServiceAlive



