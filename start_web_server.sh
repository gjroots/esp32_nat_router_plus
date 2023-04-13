#!/bin/bash
cd components\\web_server\\www\\html
echo server started

# this command execute  server on port 4000
bundle exec jekyll serve --watch

