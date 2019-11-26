@echo off

set CFG_PROJECT=%~n0
rem set CODEC=1
rem set CODEC_EX=1
rem set AUTOBUILD=1

call build.cmd

@if not defined NO_PAUSE pause
