# Set variables
pythonInstallPath = "./Tools/python-3.10.0-amd64/python"
buildScriptPath = "./BuildMain.py"

git submodule update --init --recursive

# Invoke Python
$pythonInstallPath $buildScriptPath $args
