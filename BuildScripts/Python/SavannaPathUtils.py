import os, re
import SavannaLogging

def FindPathToFiles(root, regex):
    SavannaLogging.LogDebug("Searching for files in " + str(root) + " using regex " + regex)

    fileList=[]

    for walkedRoot, dirs, files in os.walk(root):
        for file in files:
            filePath = os.path.join(walkedRoot, file)
            fileList.append(filePath)

    res = [f for f in fileList if re.search(regex, f, re.IGNORECASE)]
    for i, f in enumerate(res):
        f = f.replace('\\', '/')
        res[i] = f

    SavannaLogging.LogDebug("File Set for Regex " + regex + ":\n" + str(res) + "\n")

    if (len(res) == 0):
        raise ValueError("No Files Found for regex " + regex + " in " + root)

    return res


def CheckEmptyDir(directory, errMessage):
    if len(os.listdir(directory)) == 0:
        SavannaLogging.Log(errMessage)
        return False

# TODO: Potentially useful for Unit file generation?
def CreateFileHierarchyStrings(outFileHierarchy):
    outFileHierarchy = ['./']