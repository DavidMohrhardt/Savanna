import platform, json
import SavannaLogging

from dataclasses import dataclass
from SavannaPathUtils import FindPathToFiles

#TODO Update Build Platform class to handle generation of the required arguments and dependencies based on input meta files
@dataclass
class SavannaBuildPlatform:
    system : str
    architecture : str
    supportedArchitecture : str

#TODO
@dataclass
class SavannaBuildTarget:
    system : str
    architecture : str
    options : list

#TODO
def CreateSavannaBuildTargetListFromDict(buildListDict: dict):
    buildTargetList = []
    for tuple in buildListDict:
        le = []

#TODO
def GetCurrentBuildPlatform():
    if platform.system() == 'Windows':
        osType = 'Win'
    elif platform.system() == 'Darwin':
        osType = 'MacOS'
    elif platform.system() == "Linux":
        osType = 'Linux'
    else:
        SavannaLogging.Log(platform.system() + " is not supported.")
        exit(1)

    if platform.machine() == 'arm64':
        machineArch = 'arm64'
    elif platform.machine() == 'x86':
        machineArch = 'x86'
    elif platform.machine() == 'x64' or platform.machine() == 'AMD64':
        machineArch = 'x64'
    else:
        SavannaLogging.Log(platform.machine() + " is not supported.")
        exit(1)

    return SavannaBuildPlatform(osType, platform.machine(), machineArch)

#TODO
def GetAvailableBuildPlatforms():
    currentPlatform = GetCurrentBuildPlatform()
    buildListPath = FindPathToFiles('../', 'BuildTargets\.json')
    # with open(buildListPath) as fReader:
        # conve = json.loads(fReader)

