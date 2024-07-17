@echo off

start "" "C:\Program Files\nodejs\node.exe" server.js

timeout /t 1 /nobreak > nul

start "" "http://localhost:3000"

echo Server and browser are running
pause
