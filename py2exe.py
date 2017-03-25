

#The script is invoked as following command in cmd dos window: python py2exe.py build

import sys
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need fine tuning.
build_exe_options = {"packages": ["os"], "excludes": ["tkinter"]}

# GUI applications require a different base on Windows (the default is for a
# console application).
base = None
if sys.platform == "win64":
    base = "Win64GUI"

setup(  name = "Eytmology",
        version = "0.1",
        description = "Eytmology Dictionary!",
        options = {"build_exe": build_exe_options},
        executables = [Executable("htmlParser.py", base=base)])
