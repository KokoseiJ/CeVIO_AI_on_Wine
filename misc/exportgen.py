import re
import pefile

dllpath = input("DLL dir: ")
dll = pefile.PE(dllpath)

if "drive_c" in dllpath:
    dllpath = re.sub(r".*?drive_c", "C:", dllpath).replace("/", "\\")

repath = re.sub(r"\.[dD][lL]{2}", "", dllpath)
repath_escaped = repath.replace("\\", "\\\\")

pragma = [
    f"""#pragma comment(linker, "/export:{symbol.name.decode()}=\\"{repath_escaped}.{symbol.name.decode()}\\"")"""
    for symbol in dll.DIRECTORY_ENTRY_EXPORT.symbols
    if symbol.name
]

defs = [
    f"""  {symbol.name.decode()}="{repath}".{symbol.name.decode()}"""
    for symbol in dll.DIRECTORY_ENTRY_EXPORT.symbols
    if symbol.name
]

pragmastr = "\n".join(pragma)
defsstr = "EXPORTS\n" + "\n".join(defs)

print(pragmastr)
print()
print(defsstr)

with open("pragma.cpp", "w") as f:
    f.write(pragmastr)

with open("export.def", "w") as f:
    f.write(defsstr)

