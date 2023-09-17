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

        generatorArgs = []
        if (args.generator is not None):
            generatorArgs = ["-G", "\"" + args.generator + "\""]

        projectPath = SelectProjectPath()
        SavannaLogging.LogDebug("Cmake Executable: " + cmakeExecutablePath)
        SavannaLogging.LogDebug("Project Path: " + projectPath)
        SavannaLogging.LogDebug("Cmake List Path: " + cmakeListPath)
        SavannaLogging.LogDebug("Cmake Generator: " + str(generatorArgs))
        
        if(not os.path.exists(projectPath)):
            os.makedirs(projectPath)
        
        os.chdir(projectPath)
        SavannaSubprocess.RunSubprocess([cmakeExecutablePath, cmakeListPath, ])
        os.chdir(rootPath)
