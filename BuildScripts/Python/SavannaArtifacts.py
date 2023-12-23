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
    artifactsRoot = './Artifacts/'
    toolsRoot = './BuildTools/'
    artifactsRegex = '(.+)(\.7z|\.zip|\.tar.gz)'
    osSpecificArtifactsRegex = '(.+)((' + osTypeFuzzyRegex + ')|(' + buildPlatform.architecture + '))(.+)(\.7z|\.zip)'
    unzipperRegex = '(' + buildPlatform.architecture + ')?(.+)(7z)(.+)(' + "exe" + ')'

    artifactFiles = [f for f in SavannaPathUtils.FindPathToFiles(artifactsRoot, artifactsRegex) if ("mac" not in f.lower()) and ("linux" not in f.lower() and ("~" not in f.lower()))]
    for artifactFile in artifactFiles:
        SavannaLogging.LogDebug("Found artifact file: " + artifactFile)
    SavannaLogging.LogDebug("All artifacts: \n" + str(artifactFiles) + "\n")

    archiveTools = SavannaPathUtils.FindPathToFiles(toolsRoot, unzipperRegex)

    for artifact in artifactFiles:
        name = artifact.split('/')[-1].split('.zip')[0].split('.7z')[0].split('.tar')[0]
        # TODO @david.mohrhardt: For now we just skip existing folders but We may want to look into doing something else
        SavannaLogging.Log([str(archiveTools), "x", artifact, "-o./External/" + name])

        # TODO @david.mohrhardt: Argument parsing to specify the output folder
        outputFolder = "./External/" + name
        # outputFolder = "./External/Artifacts/" + name
        if artifact.endswith(".tar.gz"):
            SavannaSubprocess.RunSubprocessWithLogging(["tar", "-xzvf", artifact, "-C", outputFolder])
        else:
            SavannaSubprocess.RunSubprocessWithLogging([str(archiveTools[0]), "x", artifact, "-o" + outputFolder, "-aos"])

