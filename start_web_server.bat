@echo off
cd components\web_server\www\html
echo Current directory: %cd%
echo Server Started

:: this command execute  server on port 4000
bundle exec jekyll serve --watch