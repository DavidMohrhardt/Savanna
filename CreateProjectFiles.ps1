# Ensure Prepare has run
$prepareScript = "./Prepare.ps1" | Resolve-Path;
& $prepareScript

# Set variables
$pythonInstallPath = Get-ChildItem -Path "./BuildTools/Python" -Include python.exe -Recurse | Resolve-Path
$buildScriptPath = "./BuildMain.py" | Resolve-Path

# Ensure that the submodules have been cloned
git submodule update --init --recursive

# Invoke Python
& $pythonInstallPath $buildScriptPath $args
