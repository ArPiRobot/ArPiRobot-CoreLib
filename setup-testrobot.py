#!/usr/bin/env python3

import shutil
import os
import glob

if __name__ == "__main__":
    relto = os.path.dirname(__file__).replace("\\", "/")
    
    # Delete old testrobot projects
    cpp_files = glob.glob(f"{relto}/cpp_library/testrobot/.*")
    cpp_files.extend(glob.glob(f"{relto}/cpp_library/testrobot/*"))
    py_files = glob.glob(f"{relto}/python_bindings/testrobot-py/.*")
    py_files.extend(glob.glob(f"{relto}/python_bindings/testrobot-py/*"))
    all_files = cpp_files
    all_files.extend(py_files)
    for file in all_files:
        basename = os.path.basename(file)
        if basename == ".gitignore" or basename == "README.txt":
            continue
        if os.path.isdir(file):
            shutil.rmtree(file)
        else:
            os.remove(file)
    

    # Copy project templates to correct locations
    template_map = {
        "cpp": "cpp_library/testrobot",
        "py": "python_bindings/testrobot-py"
    }
    for template, dest in template_map.items():
        all_files = glob.glob(f"{relto}/testrobot_templates/{template}/*")
        all_files.extend(glob.glob(f"{relto}/testrobot_templates/{template}/.*"))
        for file in all_files:
            basename = os.path.basename(file)
            if os.path.isdir(file):
                shutil.copytree(file, f"{relto}/{dest}/{basename}")
            else:
                shutil.copy(file, f"{relto}/{dest}/{basename}")
    