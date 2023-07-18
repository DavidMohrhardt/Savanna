# Set variables
$pythonInstallPath = Get-ChildItem -Path "./BuildTools/Python" -Include python.exe -Recurse | Resolve-Path
$scriptPath = "./Tools/Coding/Python/UpdateTodos.py" | Resolve-Path

# Invoke Python
& $pythonInstallPath $scriptPath $args
