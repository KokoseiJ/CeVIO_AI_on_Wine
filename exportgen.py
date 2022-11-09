import re
import pefile

dllpath = input("DLL dir: ")
dll = pefile.PE(dllpath)

repath = re.sub(r"\.[dD][lL]{2}", "", dllpath).replace("\\", "\\\\")

pragma = [
	f"""#pragma comment(linker, "/export:{symbol.name.decode()}=\\"{repath}.{symbol.name.decode()}\\"")"""
	for symbol in dll.DIRECTORY_ENTRY_EXPORT.symbols
	if symbol.name
]

pragmastr = "\n".join(pragma)

print(pragmastr)

with open("pragma.cpp", "w") as f:
	f.write(pragmastr)

