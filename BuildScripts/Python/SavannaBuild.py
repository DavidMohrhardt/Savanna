import argparse
import SavannaLogging, SavannaPlatform, SavannaArtifacts, SavannaCMake, SavannaSubprocess, SavannaCodegen

def ProcessArguments():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    # TODO Add size limit
    # parser.add_argument('--platform', '--p', type=str, action="store", required=True, help="The platform to build for.")
    parser.add_argument('--ow', '--overwrite', action="store_true", help="Overwrites all build related files including extracted artifacts.", dest='overwriteFiles')
    parser.add_argument('--f', '--force', action="store_true", help="Forces the build script to update all related files", dest='force')
    parser.add_argument('--dr', '--dryrun', action="store_true", help="Runs all the checks but doesn't build anything.", dest='dryrun')

    # TODO
    parser.add_argument('--b', '--batchbuild', action="store_true", help="TODO Replace with build target.", dest='shouldBuild')

    # TODO
    parser.add_argument('--t', '--target', type=list, action="store", help="Specifies what builds you wish to compile. Use the '--bh' or '--buildhelp' command to see the set of accepted arguments", dest='requestedBuildTargetNames')
    # parser.add_argument('--bh', '--buildhelp', type=bool, action="store_true", help="Prints the list of Targets you may build against.", dest='printBuildHelp')
    # TODO
    parser.add_argument('--i', '--interactive', action="store_true", help="Enables the interactive build process and allows you to select build targets and specific projects to compile. [IMPORTANT] Overrides other commands.", dest='interactive')

    # Only Unpack artifacts.
    parser.add_argument('--u', '--unpack', action="store_true", help="Unpacks artifacts only.", dest='unpackOnly')

    parser.add_argument('--g', '--generator', action="store", help="Specifies the generator to use for CMake. If not specified, the default generator for the platform will be used.", dest='generator')

    # TODO Logging
    parser.add_argument('--v', '--verbose', action="store_true", help="Enable Verbose Logging. Implicitly enables Warning, and Error logging", dest="verbose")
    parser.add_argument('--w', '--warning', action="store_true", help="Enable Warning Logging. Implicitly enables Error logging", dest="warning")
    parser.add_argument('--db', '--debug', action="store_true", help="Enable Debug Logging. Implicitly enables Verbose, Warning, and Error logging", dest="debug")
    parser.add_argument('--dbo', '--debug_only', action="store_true", help="Enable Debug Only Logging. Disables all other logging", dest="debugonly")
    parser.add_argument('--s', '--silent', action="store_true", help="Disables all Logging.", dest="silent")
    parser.add_argument('--cg', '--scriptCodeGen', action="store_true", help="Executes scripting code generation.", dest="scriptCodegen")
    return parser.parse_args()

#TODO @david.mohrhardt: Update this to be in submodules instead of one monster python script
def main():
    args = ProcessArguments()
    SavannaLogging.SetLoggingLimiter(args)
    SavannaSubprocess.SetGlobalSubprocessExecutionSettings(args.dryrun)
    SavannaLogging.LogUI("Savanna Engine Build Tool")

    # Build Steps:
    #
    # 1) Set Platform Variables
    # 2) Extract Artifacts if needed (TODO)
    # 3) Invoke CMake For Project Generation or Build (TODO)
    # if (args.interactive):
    #     BuildArgumentsInteractive(args)

    SavannaLogging.LogUI("Acquiring Build Platform...")
    currentBuildPlatform = SavannaPlatform.GetCurrentBuildPlatform()
    SavannaLogging.LogUI("Done\n")

    # Foreach build target when available
    SavannaLogging.LogUI("Preparing Artifacts...")
    SavannaArtifacts.PrepareArtifactsIfNeeded(currentBuildPlatform)
    SavannaLogging.LogUI("Done\n")

    # Run Basic Codegen
    if (args.scriptCodegen):
        SavannaLogging.LogUI("Generating Scripting API List...")
        SavannaCodegen.GenerateScriptingBindings('./Src/Runtime')
        SavannaLogging.LogUI("Done\n")

    # Cease Cmake invocation here.
    if (args.dryrun):
        SavannaLogging.LogUI("Dry Run Complete\n")
        return
    elif(args.unpackOnly):
        SavannaLogging.LogUI("Unpack Complete\n")
        return

    SavannaCMake.InvokeCMakeForBuildTarget(currentBuildPlatform, "exe", args)
    SavannaLogging.LogUI("Done\n")
