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
    print("==========> Bin files are copied to the /release folder.")


def convert_single_bin():
    os.makedirs("release", exist_ok=True)
    os.system(
        f'esptool.py --chip esp32 merge_bin -o release/esp32nat_Router+_full_v{version}_0x0.bin --flash_freq 40m --flash_size 4MB 0x1000 release/bootloader.bin 0x10000 release/esp32nat_Router+_v{version}.bin 0x8000 release/partitions.bin')
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

    print("==========> zip files created successfully")


def after_build_program(source, target, env):
    print("\n==========> Building Successful\n")
    copy_files()
    if merge_to_single_bin == 1:
        convert_single_bin()


env.AddPostAction("$BUILD_DIR/firmware.bin", after_build_program)
