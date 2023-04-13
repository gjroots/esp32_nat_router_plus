@echo off
cd components\web_server\www\html
echo Current directory: %cd%
echo Site Building Started

::this command enables to minified version site files
set JEKYLL_ENV=production

::Execute Building Site
bundle exec jekyll build

echo Site Building completed