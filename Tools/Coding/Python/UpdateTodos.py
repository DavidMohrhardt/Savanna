import mimetypes
import os
import re

def IsTextFile(filePath:str):
    return mimetypes.guess_type(filePath)[0] == 'text/plain'

def CreateFileLineLink(relativeFilePath:str, l_no:int, line:str):
    return "- [L" + str(l_no) + " :" +  line.strip().replace("\n", "") + "](" + relativeFilePath.replace('\\', '/') + "#L" + str(l_no) + ")\n"

def SearchForTodosInFiles(regex:str, directoryName:str):
    content="## " + directoryName + "\n"
    wasWritten = False
    for rootDir, dirs, files in os.walk(regex):
        for filename in files:
            if (filename == "UpdateTodos.py"):
                continue
            filePath = os.path.join(rootDir, filename)

            # if (IsTextFile(filePath)):
            with open(filePath, 'r') as file:
                print(filePath)
                isFirstWriteForFile = True
                try:
                    for l_no, line in enumerate(file):
                        # search string
                        lowerLine = line.lower()
                        if "todo" in lowerLine:
                            if (not wasWritten):
                                wasWritten = True
                            if (isFirstWriteForFile):
                                isFirstWriteForFile = False
                                content += "\n### " + filename + "\n\n"
                            content += CreateFileLineLink(filePath, l_no, line)
                except:
                    print("Error in file " + filename)
    if wasWritten:
        return content + "\n"
    else:
        return ""


content="# TODO\n\n"
content += SearchForTodosInFiles(r"./Src", "Src")
content += SearchForTodosInFiles(r"./BuildScripts", "BuildScripts")
content += SearchForTodosInFiles(r"./Documentation", "Documentation")
content += SearchForTodosInFiles(r"./Tools", "Tools")

with open("./Progress/TODOTracker.md", 'w') as todoFile:
    todoFile.write(content)