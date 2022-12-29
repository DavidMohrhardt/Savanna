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
            Write-Host "Downloading $DownloadUrl"
            $files += @{
                Uri = "$DownloadUrl"
                OutFile = "$Target/$FileName"
            }
        }
    }
}

$jobs = @()

foreach ($file in $files) {
    $jobs += Start-ThreadJob -Name $file.OutFile -ScriptBlock {
        $params = $using:file
        Invoke-WebRequest @params
    }
}

if ($jobs.Count -gt 0)
{
    Write-Host "Downloading Artifacts..."
    Wait-Job -Job $jobs | Out-Null
}

foreach ($job in $jobs) {
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
