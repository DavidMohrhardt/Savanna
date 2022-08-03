# Set variables
$pythonInstallPath = "./Tools/python-3.10.0-amd64/python.exe" | Resolve-Path
$buildScriptPath = "./BuildMain.py" | Resolve-Path

# # Invoke Python
& $pythonInstallPath $buildScriptPath $args