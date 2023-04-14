# <u>ESP32-NAT Router +</u>

## What is Jekyll?

[Jekyll](https://jekyllrb.com/) is a pre-compiler for static content generation that simplifies website maintenance. It uses Ruby to generate static HTML pages, which means that the outputted HTML is the same as if it were written by hand.

To start using Jekyll, you need to follow these steps:

- Install Ruby, Gem package manager & Jekyll ( check [here](https://www.ruby-lang.org/ ) )
- Run `bundle install` in the `/html` folder to install dependencies

Note :

Jekyll is a static site generator that uses a specific format to separate the metadata of a web page from its content. At the top of each web file, it's important to include a specific syntax in the form of three hyphens 

```
---
---
```

This is known as the front matter and it specifies the metadata of the web page, such as the author and the layout styles etc.

It's important to include these three hyphen lines at the top of each file in order for Jekyll to properly parse the content and avoid errors while running and building the site. Therefore, always make sure to position these lines in the correct place in your web files to ensure a smooth development and build process.

## How to run a local server

Once Jekyll is installed, you can start a local server by following these steps:

- Run `bundle exec jekyll serve --watch` in the `/html` folder
- Navigate to `http://127.0.0.1:4000` to see the website
- Changes made to files will be reflected in real-time, so you can test and make adjustments to your site's appearance and functionality. (make sure to refresh the page though)
- Alternatively, You can run the server, navigate to the root folder of the project and use `./start_web_server.sh` for Linux terminal or `./start_web_server.bat` for Windows.

## How to update web-server files?

### Build

- Building is a mandatory step after making changes. you can run the command `JEKYLL_ENV=production bundle exec jekyll build` for Linux Terminal  OR
  
   `set JEKYLL_ENV=production bundle exec jekyll build`  for Windows CMD  in the `/html` folder to generate and return to production mode minified files.

- Alternatively, navigate to the root folder of the project and use `./build_web_pages.sh` in Linux terminal or `./build_web_pages.bat` in Windows CMD.

### Auto Mode (Windows only)

- Make sure you have Jekyll & Gems installed (see above)

- Launch `auto_generate.exe`

- Wait for it to finish

- That's it **¯\_(ツ)_/¯**
  
  ### How to Build auto_generate.exe
  
  The  `auto_generate.exe`  code is written in JavaScript and converted into an executable file using the  `pkg`  package for Node.js.
  
  To build the  `auto_generate.exe`  file, which automatically generates the website, follow these simple steps:
1. Install [Node.js](https://nodejs.org)
2. Open a terminal and run  `npm i -g pkg` .
3. Extract the ZIP file provided in the **additional** folder
4. Click on  `build_auto_generate.bat`  or simply run  `pkg --compress GZip index.js -o auto_generate.exe` 
5. Wait for some time to download the required files.
6. That's it! You have successfully built the  `auto_generate.exe`  file which automatically generates the website.