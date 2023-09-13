# Define parameters for determining which files to generate
param(
    # Capture any subsequent arguments to pass to the python script
    [Parameter(Mandatory=$false)]
    [string[]]$args
)

# Ensure Prepare has run
$prepareScript = "./Prepare.ps1" | Resolve-Path;
& $prepareScript

# Set variables
$pythonInstallPath = Get-ChildItem -Path "./BuildTools/Python" -Include python.exe -Recurse | Resolve-Path
$buildScriptPath = "./BuildMain.py" | Resolve-Path

# Ensure that the submodules have been cloned
git submodule update --init --recursive

# Invoke Python
Write-Host "Invoking Python with command line arguments: $args"
& $pythonInstallPath $buildScriptPath $args
