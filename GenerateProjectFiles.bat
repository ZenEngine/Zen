@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass "& {& '%~dp0BuildTools/BuildEngine.ps1' %*}"