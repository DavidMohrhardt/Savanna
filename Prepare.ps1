Write-Host "Preparing Savanna Development Environment..."
Write-Host ""

# $7z_dir="./BuildTools/7z";
$ArtifactsJson="./Artifacts/Artifacts.json";

# Can't dl 7z from github, so we have to use the one in the repo
# if 7z directory exists skip
# if (!(Test-Path "$7z_dir" ))
# {
#     # create 7z directory

#     Write-Host ""
#     Write-Host "Acquiring 7zip..."
#     mkdir "$7z_dir"
#     Invoke-WebRequest "https://www.7-zip.org/a/7zr.exe" -OutFile "$7z_dir/7zr.exe"
# }

$Artifacts = Get-Content "$ArtifactsJson" -Raw | ConvertFrom-Json

Write-Host "Acquiring Artifacts..."

foreach ($Artifact in $Artifacts)
{
    foreach ($FileName in $($Artifact.FileNames))
    {
        $Target = "./Artifacts/$($Artifact.Name)/$($Artifact.version)"
        if ( ! (Test-Path "$Target/$FileName" ))
        {
            $DownloadUrl = "$($Artifact.url)/$FileName"
            if ( Test-Path "$Target" )
            {
                Invoke-WebRequest -Uri "$DownloadUrl" -OutFile "$Target/$FileName"
            }
            else
            {
                mkdir "$Target"
                Invoke-WebRequest -Uri "$DownloadUrl" -OutFile "$Target/$FileName"
            }
        }
    }
}

$PythonExtractionDir = "./BuildTools/Python"

if (! (Test-Path "$PythonExtractionDir" ) )
{
    # Extract python for use with build scripts
    Write-Host "Extracting python..."
    $PythonDir="./Artifacts/Python/"

    $PythonZip=Get-ChildItem -Path $PythonDir -Include *.zip -Recurse
    Expand-Archive -Path $PythonZip -DestinationPath $PythonExtractionDir
}

Write-Host "Finished preparing Savanna Development Environment"
Write-Host ""
