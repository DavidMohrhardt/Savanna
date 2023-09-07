Write-Host "Preparing Savanna Development Environment..."
Write-Host ""

# Run from the directory of the script
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $scriptDir

# $7z_dir="$scriptDir/BuildTools/7z";
$ArtifactsJson="$scriptDir/Artifacts/Artifacts.json";

$Artifacts = Get-Content "$ArtifactsJson" -Raw | ConvertFrom-Json

Write-Host "Acquiring Artifacts..."

$files = @()

foreach ($Artifact in $Artifacts)
{
    foreach ($FileName in $($Artifact.FileNames))
    {
        $Target = "$scriptDir/Artifacts/$($Artifact.Name)/$($Artifact.version)"
        if ( ! (Test-Path "$Target/$FileName" ))
        {
            $DownloadUrl = "$($Artifact.url)/$FileName"
            if ( -not(Test-Path "$Target") )
            {
                mkdir "$Target"
            }
            $files += @{
                Uri = "$DownloadUrl"
                OutFile = "$Target/$FileName"
            }
        }
    }
}

$jobs = @()

foreach ($file in $files) {
    # for each file invoke a web request to download
    Write-Host "Downloading $($file.Uri)"
    # Download asynchronously
    $jobs += Start-Job -ScriptBlock { param($file) Invoke-WebRequest @file } -ArgumentList $file
}

if ($jobs.Count -gt 0)
{
    Write-Host "Downloading Artifacts..."
    Wait-Job -Job $jobs | Out-Null
}

foreach ($job in $jobs) {
    # if a job failed, throw an error
    if ($job.State -eq "Failed") {
        $job | Receive-Job -ErrorVariable error
        throw $error
    }
    Receive-Job -Job $job | Out-Null
}

Write-Host "Finished Acquiring Artifacts..."

$PythonExtractionDir = "$scriptDir/BuildTools/Python"

if (! (Test-Path "$PythonExtractionDir" ) )
{
    # Extract python for use with build scripts
    Write-Host "Extracting python..."
    $PythonDir="$scriptDir/Artifacts/Python/"

    $PythonZip=Get-ChildItem -Path $PythonDir -Include *.zip -Recurse
    Expand-Archive -Path $PythonZip -DestinationPath $PythonExtractionDir
}

Write-Host "Extracting Artifacts..."

$pythonInstallPath = Get-ChildItem -Path $PythonExtractionDir -Include python.exe -Recurse | Resolve-Path
$buildScriptPath = "$scriptDir/BuildMain.py" | Resolve-Path
& $pythonInstallPath $buildScriptPath --u

Write-Host "Finished preparing Savanna Development Environment"
Write-Host ""

# TODO @DavidMohrhardt - Move shader compilation to a runtime step
# TODO @DavidMohrhardt - Make this its own script
$vulkanSdkPath = $env:VULKAN_SDK
if ($null -ne $vulkanSdkPath)
{
    Write-Host "Compiling Vulkan Shaders..."
    $vulkanSdkPath = $vulkanSdkPath | Resolve-Path
    $vulkanSdkBinPath = "$vulkanSdkPath\Bin"
    $vulkanSdkGlslcPath = "$vulkanSdkBinPath\glslc.exe"
    $vulkanSdkGlslcPath = $vulkanSdkGlslcPath | Resolve-Path
    # If glslc.exe is not found, then fail
    if (! (Test-Path $vulkanSdkGlslcPath))
    {
        Write-Host "glslc.exe not found in $vulkanSdkBinPath" -ForegroundColor Red
        Write-Host "Please make sure the environment variable VULKAN_SDK is set to the path of the Vulkan SDK" -ForegroundColor Red
        Write-Host "If you do not have the Vulkan SDK installed, you can download it from https://vulkan.lunarg.com/sdk/home" -ForegroundColor Red
        return
    }
    # run glslc.exe to compile shaders to spirv
    # Get the Src directory in the project
    $srcDir = Get-ChildItem -Path "$scriptDir/Src" -Include Assets -Recurse

    # Get all the hlsl and glsl files in the Src directory
    $hlslFiles = Get-ChildItem -Path $srcDir -Include *.hlsl -Recurse
    # $glslFiles = Get-ChildItem -Path $srcDir -Include *.glsl -Recurse
    # Get vert and frag files
    $vertFiles = Get-ChildItem -Path $srcDir -Include *.vert -Recurse
    $fragFiles = Get-ChildItem -Path $srcDir -Include *.frag -Recurse

    # Create a list of all the files to compile
    $filesToCompile = @()
    $filesToCompile += $hlslFiles
    # $filesToCompile += $glslFiles
    $filesToCompile += $vertFiles
    $filesToCompile += $fragFiles

    # Compile each file and output the spirv to the Src/Assets/Shaders/*/spirv directories
    $shaderCompilationJobs = @()
    foreach ($file in $filesToCompile)
    {
        $fileName = Split-Path -Leaf $file
        $fileDir = Split-Path $file
        $spirvDir = "$fileDir\SPIRV"
        if (! (Test-Path $spirvDir))
        {
            mkdir $spirvDir
        }
        $spirvFile = "$spirvDir\$fileName.spv"

        # Log the file being compiled
        Write-Host "Compiling $fileName" -ForegroundColor Green

        # Start the compilation job and add it to the list of jobs
        $shaderCompilationJobs += Start-Job -ScriptBlock { param($vulkanSdkGlslcPath, $file, $spirvFile) & $vulkanSdkGlslcPath $file -o $spirvFile } -ArgumentList $vulkanSdkGlslcPath, $file, $spirvFile
    }

    # Wait for all the jobs to finish
    Wait-Job -Job $shaderCompilationJobs | Out-Null
    # Print output and errors for each job
    foreach ($job in $shaderCompilationJobs)
    {
        Receive-Job -Job $job | Out-Null
    }
}
