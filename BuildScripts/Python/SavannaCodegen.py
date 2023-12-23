import re
import os
import SavannaLogging, SavannaPathUtils

def SeekScriptBindingDeclarationsInFile(filePath:str):
    with open(filePath, 'r') as f:
        contents = f.read()
    pattern = r'DECLARE_SCRIPTING_FUNCTION\(\s*([^)]+)\s*\)\s*(\w+)\s*\(([^)]*)\)'
    regexResults = re.findall(pattern, contents)
    # functionNames = { str(x[1]) for x in regexResults }
    return regexResults

def CreateScriptBindingHeader(outputFilePath:str, regexResults):
    functionNames = { str(x[1]) for x in regexResults }
    functionSignatures = { '{} {}({})'.format(str(x[0]), str(x[1]), str(x[2])) for x in regexResults }
    with open(outputFilePath, 'w') as f:
        f.write('#ifndef SAVANNA_SCRIPTING_API_GEN_H\n')
        f.write('#define SAVANNA_SCRIPTING_API_GEN_H\n')
        f.write('#include "SavannaScripting.h"\n')
        f.write('// DO NOT MODIFY BY HAND. WILL BE OVERWRITTEN.\n')

        f.write('#undef APPLY_MACRO_SCRIPT_NAMES\n')
        f.write('#define APPLY_MACRO_SCRIPT_NAMES(MACRO) \\\n')
        for functionName in functionNames:
            f.write('    MACRO({}) \\\n'.format(functionName))
        f.write('\n')

        f.write('// Forward Declarations of all bindings\n')
        for functionDecl in functionSignatures:
            f.write('{};\n'.format(functionDecl))

        f.write("#endif // SAVANNA_SCRIPTING_API_GEN_H\n")

def GenerateScriptingBindings(srcCodePath:str):
    srcRegex = r'(.+)(bindings\.)(c|cpp|h|hpp)'
    files = [f for f in SavannaPathUtils.FindPathToFiles(srcCodePath, srcRegex)]
    functionNames = []
    for file in files:
        functionNames += SeekScriptBindingDeclarationsInFile(file)
    outputFile = os.path.join(os.path.abspath('./Src/Runtime/Scripting'), "SavannaScriptingApi.gen.h")
    CreateScriptBindingHeader(outputFile, functionNames)
