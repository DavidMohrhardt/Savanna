import os, subprocess
import SavannaLogging, SavannaSubprocess

from SavannaPlatform import SavannaBuildPlatform
from SavannaPathUtils import FindPathToFiles

def SelectProjectPath():
    return "./Projects/VisualStudio/"

def InvokeCMakeForBuildTarget(buildPlatform: SavannaBuildPlatform, execExt: str, args):
    pathList = FindPathToFiles("./External", "(cmake\." + execExt + ")")
    if (len(pathList) == 0):
        raise

    cmakeExecutablePath = os.path.abspath(pathList[0])

    if len(cmakeExecutablePath) == 0:
        raise

    cmakeListPath = os.path.abspath('./Src')
    rootPath = os.path.abspath('./')

    # TODO @david.mohrhardt: Update to handle arguments like build vs project generation.
    # Generate Projects
    # Currently just generates VS Projects

    # if (args.shouldBuild):
    if (False):
        SavannaLogging.Log(cmakeExecutablePath)
        SavannaLogging.Log(cmakeListPath)

        cachePath = os.path.abspath('./Builds/cache')

        subprocessArgs = [cmakeExecutablePath, "--build", cachePath]

        os.chdir(cmakeListPath)
        SavannaLogging.Log(subprocessArgs)
        SavannaSubprocess.RunSubprocess(subprocessArgs)
        os.chdir(rootPath)
    else:
        SavannaLogging.LogUI("Generating Projects...")

        projectPath = SelectProjectPath()
        SavannaLogging.WriteLog(cmakeExecutablePath)
        SavannaLogging.WriteLog(projectPath)
        SavannaLogging.WriteLog(cmakeListPath)

        os.chdir(projectPath)
        SavannaLogging.Log([cmakeExecutablePath, cmakeListPath])
        SavannaSubprocess.RunSubprocess([cmakeExecutablePath, cmakeListPath])
        os.chdir(rootPath)
