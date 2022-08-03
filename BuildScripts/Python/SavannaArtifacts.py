import subprocess
from SavannaLogging import SavannaLoggingParameters
import SavannaLogging, SavannaPlatform, SavannaPathUtils, SavannaSubprocess

def CreateFuzzyRegex(input):
    res = '('
    for char in list(input):
        res += '(?=.*' + char + ')'
    res += ')'
    return res

#TODO
def PrepareArtifactsIfNeeded(buildPlatform: SavannaPlatform.SavannaBuildPlatform):
    osTypeFuzzyRegex = CreateFuzzyRegex(buildPlatform.system)

    # TODO change to read from artifact json
    # artifactsRoot = pathlib.Path('./Artifacts/')
    artifactsRoot = './Artifacts/'
    toolsRoot = './Tools/'
    # r'.+((osTypeFuzzyRegex)|(machineArch)){2}'
    artifactsRegex = '(.+)(\.7z|\.zip)'
    osSpecificArtifactsRegex = '(.+)((' + osTypeFuzzyRegex + ')|(' + buildPlatform.architecture + '))(.+)(\.7z|\.zip)'
    unzipperRegex = '(' + buildPlatform.architecture + ')?(.+)(7za)(.+)(' + "exe" + ')'

    artifactFiles = [f for f in SavannaPathUtils.FindPathToFiles(artifactsRoot, artifactsRegex) if ("mac" not in f.lower()) and ("linux" not in f.lower() and ("~" not in f.lower()))]
    SavannaLogging.LogDebug("All artifacts: \n" + str(artifactFiles) + "\n")

    archiveTools = SavannaPathUtils.FindPathToFiles(toolsRoot, unzipperRegex)

    for artifact in artifactFiles:
        name = artifact.split('/')[-1].split('.zip')[0].split('.7z')[0]
        # TODO @david.mohrhardt: For now we just skip existing folders but We may want to look into doing something else
        SavannaLogging.Log([str(archiveTools), "x", artifact, "-o./External/" + name])
        SavannaSubprocess.RunSubprocessWithLogging([str(archiveTools[0]), "x", artifact, "-o./External/" + name, "-aos"])

