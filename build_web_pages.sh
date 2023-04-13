#!/bin/bash
cd components\\web_server\\www\\html
pwd
echo Site Building Started

# this command enables to minified version site files
JEKYLL_ENV=production # comment out if not require 

# this command execute  Building Site
bundle exec jekyll build

echo Site Building completed
