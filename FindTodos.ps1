# Set variables
$pythonInstallPath = "./BuildDependencies/python-3.10.0-amd64/python.exe" | Resolve-Path
$scriptPath = "./Tools/Coding/Python/UpdateTodos.py" | Resolve-Path

# # Invoke Python
& $pythonInstallPath $scriptPath $args