@echo off
color 0A
echo.
echo            ================================
echo             ��ѡ��Ҫ���еĲ�����Ȼ�󰴻س�
echo            ================================
echo.
echo              1.����ȫ��������
echo.     
echo.             2.�ر�ȫ��������
echo.
echo.             3.�˳������
echo.
:cho
set choice=
set /p choice=          ��ѡ��:
IF NOT "%choice%"=="" SET choice=%choice:~0,1%
if /i "%choice%"=="1" start /MIN ./CenterServer.exe & start /MIN ./LoginServer.exe & start /MIN ./ProxyServer.exe & start /MIN ./DBServer.exe & start /MIN ./GameServer.exe
if /i "%choice%"=="2" taskkill /im CenterServer.exe & taskkill /im LoginServer.exe & taskkill /im ProxyServer.exe & taskkill /im DBServer.exe & taskkill /im GameServer.exe
if /i "%choice%"=="3" exit
goto cho


