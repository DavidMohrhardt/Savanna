import subprocess
import SavannaLogging

shouldRunSubprocesses = True

def SetGlobalSubprocessExecutionSettings(isDryRun: bool):
    global shouldRunSubprocesses
    shouldRunSubprocesses = not isDryRun

def RunSubprocessWithLogging(processArgs: list):
    global shouldRunSubprocesses
    if (not shouldRunSubprocesses):
        return

    loggingParams = SavannaLogging.GetLoggingParameters()

    if (loggingParams.verbose):
        subprocess.run(processArgs)
    elif (loggingParams.error):
        subprocess.run(processArgs, stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
    # TODO @david.mohrhardt: Add logging for other levels
    # elif (loggingParams.output):
    #     subprocess.run(processArgs)
    else:
        subprocess.run(processArgs, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def RunSubprocess(processArgs: list):
    global shouldRunSubprocesses
    if (not shouldRunSubprocesses):
        return

    SavannaLogging.LogDebug("Running Subprocess: " + str(processArgs))

    loggingParams = SavannaLogging.GetLoggingParameters()
    if (not loggingParams.error): # Implicitly means silent
        subprocess.run(processArgs, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    else:
        subprocess.run(processArgs)
