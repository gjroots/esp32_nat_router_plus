
import json
import os
import gzip
import re

print("\033[94m╔═╗┌─┐┌─┐   ╔╗╔┌─┐┌┬┐ ╦═╗┌─┐┬ ┬┌┬┐┌─┐┬─┐ _|_\n║╣ └─┐├─┘32─║║║├─┤ │  ╠╦╝│ ││ │ │ ├┤ ├┬┘  |  \n╚═╝└─┘┴     ╝╚╝┴ ┴ ┴  ╩╚═└─┘└─┘ ┴ └─┘┴└─\033[0m\n")

Import("env")


os.environ['PYTHONDONTWRITEBYTECODE'] = '1'
config = env.GetProjectConfig()
version = config.get("extra_script_settings", "version")

jekyll_install_status = False

if os.system('jekyll -v') == 0:
    # print('Jekyll is installed on your system.')
    jekyll_install_status = True
else:
    print(
        '\033[91mJekyll is not installed on your system. Please install Jekyll.\033[0m')


# uncomment for linux and git bash users
# def build_site():
#     env.Execute("build_web_pages.sh")

# for windows user
def build_site():
    env.Execute("build_web_pages.bat")


base_folder_path = './components/web_server/www/'

# check version in function.js file and update new version
def version_check():
    pattern = r'version\s*=\s*"([\d.]+(?:_\w+)?)"'
    # Define the new version number
    new_version = version
    filename = base_folder_path + 'html/js/functions.js'
  
    # Open the JavaScript file
    with open(filename, 'r+', encoding='utf-8') as js_file:
        content = js_file.read()
        if re.search(pattern, content):
            old_version = re.search(pattern, content).group(1)
            if old_version == new_version:
                # print('Version already up to date.')
                return
            else:
                new_content = re.sub(pattern, 'version="' + new_version + '"', content)
                js_file.seek(0)
                js_file.write(new_content)
                js_file.truncate()
                print(f'New version released: {new_version}')
        else: 
            print('Version not found.')
            return
    # Try to remove the temporary file
    try:
        os.remove(filename + '.temp')
    except FileNotFoundError:
        pass
      
# gzip web pages and move to output/gzip folder
def gzip_web_pages():
    dirpath = base_folder_path + 'output/html/'
    output_dir = base_folder_path + 'output/gzip'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    for subdir, dirs, files in os.walk(dirpath):
        for filename in files:
            file_path = subdir + os.sep + filename
            if file_path.endswith(".html") or file_path.endswith(".css") or file_path.endswith(".js"):
                new_filename = filename
                if filename == "404.html":
                    new_filename = "error_404.html"
                with open(file_path, 'rb') as f_in:
                    with gzip.open(os.path.join(output_dir, new_filename + '.gz'), 'wb') as f_out:
                        f_out.writelines(f_in)


def files_modtime(folder_path):
    # Create a dictionary to store the modification times of each file
    file_modtimes = {}
    try:
        for root, dirs, files in os.walk(folder_path):
            for file in files:
                file_path = os.path.join(root, file)
                file_modtime = os.path.getmtime(file_path)
                file_modtimes[file_path] = file_modtime
        return file_modtimes
    except FileNotFoundError:
        return file_modtimes


def read_json_file(file_path):
    try:
        with open(file_path, 'r') as file:
            data = json.load(file)
        return data
    except FileNotFoundError:
        return {"modtimes": ""}


def write_json_file(file_path, data):
    with open(file_path, 'w') as file:
        json.dump(data, file)


# check files modified or not
def file_changes_check():
    # Call the function once to get the initial modification times
    original_modtimes = files_modtime(
        base_folder_path + 'output/html/')
    modtime_json_file_path = '.pio/modtime_data.json'

    data = read_json_file(modtime_json_file_path)

    if data['modtimes'] != original_modtimes:
        print("\033[91mFiles have been changed.\033[0m\n")
        gzip_web_pages()
        data['modtimes'] = original_modtimes
        write_json_file(modtime_json_file_path, data)
    # else:
    #     print("The files have not been changed.\n")

#checks website in development or production
def is_website_production_build():
    try:
        file_path = base_folder_path + "output/html/js/functions.js"
        folder_path = base_folder_path + "output/gzip/"
        word = "data/restart.json"
        with open(file_path, 'r', encoding="utf-8") as file:
            data = file.read()
            if (not word in data):
                print("\033[91mWebsite is in development\033[0m")
                build_site()
            # else:
            #     print("Web site is in production")

        if not os.listdir(folder_path):
            gzip_web_pages()
    except FileNotFoundError:
        build_site()


version_check()
if jekyll_install_status:
    is_website_production_build()
    file_changes_check()
