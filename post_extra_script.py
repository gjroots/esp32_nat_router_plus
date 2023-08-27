import shutil
import os
import zipfile


Import("env")

config = env.GetProjectConfig()
version = config.get("extra_script_settings", "version")

merge_to_single_bin = int(config.get(
    "extra_script_settings", "merge_to_single_bin"))


def copy_files():
    os.makedirs("release", exist_ok=True)
    files = [
        ('.pio/build/esp32dev/firmware.bin',
         f'esp32nat_Router+_v{version}.bin'),
        ('.pio/build/esp32dev/bootloader.bin', 'bootloader.bin'),
        ('.pio/build/esp32dev/partitions.bin', 'partitions.bin')
    ]
    for src, dest in files:
        shutil.copyfile(src, f'release/{dest}')
    print("==========> Bin files are copied to the /release folder.\n")


def convert_single_bin():
    build_dir = env.subst("$BUILD_DIR")
    binary_path = os.path.join(build_dir, "firmware.bin")
    bootloader_path = os.path.join(build_dir, "bootloader.bin")
    partitions_path = os.path.join(build_dir, "partitions.bin")
    release_path = os.path.join(env.subst("$PROJECT_DIR"), "release")
    merged_file = os.path.join(release_path, f"esp32nat_Router+_full_v{version}_0x0.bin")
    esptool_dir = env.PioPlatform().get_package_dir("tool-esptoolpy")
    esptool_command = f'python {str(esptool_dir)}\\esptool.py --chip esp32 merge_bin -o {merged_file} --flash_freq 40m --flash_size 4MB 0x1000 {bootloader_path} 0x8000 {partitions_path} 0x10000 {binary_path}'
    
    os.makedirs("release", exist_ok=True)
    env.Execute(esptool_command)
    zip_all_bins()


def zip_all_bins():
    files = [
        (f'release/esp32nat_Router+_v{version}.bin',
         f'esp32nat_Router+_v{version}.bin'),
        ('release/partitions.bin', 'partitions.bin'),
        ('release/bootloader.bin', 'bootloader.bin')
    ]
    with zipfile.ZipFile(f'release/esp32nat_Router+_v{version}.zip', 'w', zipfile.ZIP_DEFLATED) as zipObj:
        for file in files:
            zipObj.write(*file)

    with zipfile.ZipFile(f'release/esp32nat_Router+_full_v{version}_0x0.zip', 'w', zipfile.ZIP_DEFLATED) as zipObj:
        zipObj.write(
            f'release/esp32nat_Router+_full_v{version}_0x0.bin', f'esp32nat_Router+_full_v{version}_0x0.bin')

    print("\n==========> zip files created successfully")


def after_build_program(source, target, env):
    print("\n==========> Building Successful\n")
    copy_files()
    if merge_to_single_bin == 1:
        convert_single_bin()


env.AddPostAction("buildprog", after_build_program)
