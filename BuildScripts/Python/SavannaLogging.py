import argparse
from dataclasses import dataclass

# verbose = False
# warning = False
# error = True
# UI = True

@dataclass
class SavannaLoggingParameters:
    debug : bool
    verbose : bool
    warning : bool
    error : bool
    UI : bool
    output : bool
    outputLocation : str

limiters = SavannaLoggingParameters(False, False, False, False, False, False, "")

def GetLoggingParameters():
    global limiters
    return limiters

def SetLoggingLimiter(args: argparse.ArgumentParser):
    global limiters

    limiters.UI = limiters.error = True
    if (args.debugonly):
        limiters.debug = True
    if (args.debug):
        limiters.debug = limiters.verbose = limiters.warning = True
    if (args.verbose):
        limiters.verbose = limiters.warning = True
    elif (args.warning):
        limiters.warning = True
    elif (args.silent):
        limiters.verbose = limiters.warning = limiters.error = limiters.UI = False

def SetOutputLogging(shouldLog: bool, path: str):
    global limiters
    limiters.output = shouldLog
    if (shouldLog):
        limiters.outputLocation = path

# TODO
def WriteLog(msg: str):
    global limiters
    # if (limiters.output):

def LogDebug(msg: str):
    global limiters
    if (limiters.debug):
        print("DEBUG: " + msg)
        WriteLog(msg)

def Log(msg: str):
    global limiters
    if (limiters.verbose):
        print(msg)
        WriteLog(msg)

def LogWarning(msg: str):
    global limiters
    if (limiters.warning):
        print("WARNING: " + msg)
        WriteLog(msg)

def LogError(msg: str):
    global limiters
    if (limiters.error):
        print("ERROR: " + msg)
        WriteLog(msg)

def LogUI(msg: str):
    global limiters
    if (limiters.UI):
        print(msg)
        WriteLog(msg)